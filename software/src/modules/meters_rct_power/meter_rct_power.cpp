/* esp32-firmware
 * Copyright (C) 2024 Matthias Bolte <matthias@tinkerforge.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "meter_rct_power.h"

#include <lwip/sockets.h>

#include "event_log_prefix.h"
#include "module_dependencies.h"
#include "tools.h"

#if 0
#define debugfln(...) logger.printfln(__VA_ARGS__)
#else
#define debugfln(...) do {} while (0)
#endif

static const MeterValueID grid_value_ids[] = {
    MeterValueID::PowerActiveLSumImExDiff,
    MeterValueID::EnergyActiveLSumExport,
    MeterValueID::EnergyActiveLSumImport,
};

static const RCTValueSpec grid_rct_value_specs[] = {
    {0x91617C58, 1.0f},   // Total grid power [W]
    {0x44D4C533, 0.001f}, // Total energy grid feed-in [Wh]
    {0x62FBE7DC, 0.001f}, // Total energy grid load [Wh]
};

static const MeterValueID battery_value_ids[] = {
    MeterValueID::PowerDCChaDisDiff,
    MeterValueID::EnergyDCCharge,
    MeterValueID::Temperature,
    MeterValueID::StateOfCharge,
    MeterValueID::VoltageDC,
    MeterValueID::EnergyDCDischarge,
};

static const RCTValueSpec battery_rct_value_specs[] = {
    {0x400F015B,  -1.0f},   // Battery power [W]
    {0x5570401B,   0.001f}, // Total energy flow into battery [Wh]
    {0x902AFAFB,   1.0f},   // Battery temperature [°C]
    {0x959930BF, 100.0f},   // Battery SOC [0.01 %]
    {0xA7FA5C5D,   1.0f},   // Battery voltage [V]
    {0xA9033880,   0.001f}, // Total energy flow from battery [Wh]
};

static uint16_t crc16ccitt(uint8_t *buffer, size_t length)
{
    uint32_t checksum = 0xFFFF;

    for (size_t i = 0; i < length; ++i) {
        for (size_t k = 0; k < 8; ++k) {
            bool bit = (buffer[i] >> (7 - k) & 1) == 1;
            bool c15 = ((checksum >> 15) & 1) == 1;

            checksum <<= 1;

            if (c15 ^ bit) {
                checksum ^= 0x1021;
            }
        }

        checksum &= 0xFFFF;
    }

    return checksum & 0xFFFF;
}

void RCTPowerClient::setup(const Config &ephemeral_config)
{
    String host = ephemeral_config.get("host")->asString();
    uint16_t port = static_cast<uint16_t>(ephemeral_config.get("port")->asUint());
    virtual_meter = ephemeral_config.get("virtual_meter")->asEnum<VirtualMeter>();

    task_scheduler.scheduleWithFixedDelay([this, host, port]() {
        TFGenericTCPClientConnectionStatus connection_status = get_connection_status();

        if (connection_status == TFGenericTCPClientConnectionStatus::Disconnected) {
            connect(host.c_str(), port,
            [host, port](TFGenericTCPClientConnectResult result, int error_number) {
                logger.printfln("Connected to %s:%u: %s / %s (%d)",
                                host.c_str(),
                                port,
                                get_tf_generic_tcp_client_connect_result_name(result),
                                strerror(error_number),
                                error_number);
            },
            [host, port](TFGenericTCPClientDisconnectReason reason, int error_number) {
                logger.printfln("Disconnected from %s:%u: %s / %s (%d)",
                                host.c_str(),
                                port,
                                get_tf_generic_tcp_client_disconnect_reason_name(reason),
                                strerror(error_number),
                                error_number);
            });
        }
    }, 1_s, 5_s);

    task_scheduler.scheduleWithFixedDelay([this]() {
        TFGenericTCPClientConnectionStatus connection_status = get_connection_status();

        if (connection_status == TFGenericTCPClientConnectionStatus::Connected) {
            read_next_value();
        }
    }, 1_s, 250_ms);

    task_scheduler.scheduleWithFixedDelay([this]() {
        tick();
    }, 1_s, 10_ms);

    switch (virtual_meter) {
    case VirtualMeter::None:
        logger.printfln("No Virtual Meter selected");
        return;

    case VirtualMeter::Grid:
        value_specs = grid_rct_value_specs;
        value_specs_length = ARRAY_SIZE(grid_rct_value_specs);
        meters.declare_value_ids(slot, grid_value_ids, ARRAY_SIZE(grid_value_ids));
        break;

    case VirtualMeter::Battery:
        value_specs = battery_rct_value_specs;
        value_specs_length = ARRAY_SIZE(battery_rct_value_specs);
        meters.declare_value_ids(slot, battery_value_ids, ARRAY_SIZE(battery_value_ids));
        break;

    default:
        logger.printfln("Unknown Virtual Meter: %u", static_cast<uint8_t>(virtual_meter));
        return;
    }
}

void RCTPowerClient::read_next_value()
{
    debugfln("read_next_value socket_fd=%d value_specs_length=%zu value_specs_index=%zu", socket_fd, value_specs_length, value_specs_index);

    if (socket_fd < 0) {
        return;
    }

    if (value_specs_length == 0) {
        return;
    }

    if (value_specs_index >= value_specs_length) {
        value_specs_index = 0;
    }

    uint8_t request[8];
    uint32_t id = value_specs[value_specs_index].id;

    request[0] = 1; // command: read
    request[1] = 4; // length
    request[2] = (uint8_t)((id >> 24) & 0xFF);
    request[3] = (uint8_t)((id >> 16) & 0xFF);
    request[4] = (uint8_t)((id >>  8) & 0xFF);
    request[5] = (uint8_t)((id >>  0) & 0xFF);

    uint32_t checksum = crc16ccitt(request, 6);

    request[6] = (checksum >> 8) & 0xFF;
    request[7] = (checksum >> 0) & 0xFF;

    uint8_t escaped_request[1 + 8 * 2] = "+";
    size_t escaped_request_length = 1;

    for (size_t i = 0; i < sizeof(request); ++i) {
        if (request[i] == '+' || request[i] == '-') {
            escaped_request[escaped_request_length++] = '-';
        }

        escaped_request[escaped_request_length++] = request[i];
    }

    if (!send(escaped_request, escaped_request_length)) {
        int saved_errno = errno;
        logger.printfln("Failed to send request: %s (%d)", strerror(saved_errno), saved_errno);
        disconnect(TFGenericTCPClientDisconnectReason::SocketSendFailed, saved_errno);
        return;
    }

    ++value_specs_index;

    debugfln("Send request for ID 0x%08x", id);
}

void RCTPowerClient::close_hook()
{
    wait_for_start = true;
    last_received_byte = 0;
    pending_response_used = 0;
}

void RCTPowerClient::tick_hook()
{
}

bool RCTPowerClient::receive_hook()
{
    micros_t deadline = calculate_deadline(10_ms);

    while (sizeof(pending_response) - pending_response_used > 0) {
        if (deadline_elapsed(deadline)) {
            return true;
        }

        uint8_t received_byte;
        ssize_t result = recv(socket_fd, &received_byte, 1, 0);

        if (result < 0) {
            if (errno != EAGAIN && errno != EWOULDBLOCK) {
                disconnect(TFGenericTCPClientDisconnectReason::SocketReceiveFailed, errno);
            }

            return false;
        }

        if (result == 0) {
            disconnect(TFGenericTCPClientDisconnectReason::DisconnectedByPeer, -1);
            return false;
        }

        debugfln("received_byte %u 0x%02x %s| last_received_byte %u 0x%02x %s",
                 received_byte, received_byte, received_byte == '+' ? "+ " : (received_byte == '-' ? "- " : ""),
                 last_received_byte, last_received_byte, last_received_byte == '+' ? "+ " : (last_received_byte == '-' ? "- " : ""));

        if (wait_for_start) {
            if (received_byte == '+' && last_received_byte != '-') {
                debugfln("Received expected start byte");
                wait_for_start = false;
            }
        }
        else if (received_byte == '+') {
            if (last_received_byte == '-') {
                pending_response[pending_response_used++] = received_byte;
            }
            else {
                debugfln("Received unexpected start byte, starting new response");
                pending_response_used = 0;
            }
        }
        else if (received_byte == '-') {
            if (last_received_byte == '-') {
                pending_response[pending_response_used++] = received_byte;
            }
        }
        else {
            pending_response[pending_response_used++] = received_byte;
        }

        last_received_byte = received_byte;

        if (pending_response_used == 1 && pending_response[0] != 5) {
            debugfln("Received response with unexpected command %u, ignoring response", pending_response[0]);
            wait_for_start = true;
            pending_response_used = 0;
        }
        else if (pending_response_used == 2 && pending_response[1] != 8) {
            debugfln("Received response with unexpected length %u, ignoring response", pending_response[1]);
            wait_for_start = true;
            pending_response_used = 0;
        }
    }

    uint16_t actual_checksum = crc16ccitt(pending_response, pending_response_used - 2);
    uint16_t expected_checksum = ((uint16_t)pending_response[pending_response_used - 2] << 8) | pending_response[pending_response_used - 1];

    if (actual_checksum != expected_checksum) {
        logger.printfln("%s", "Received response with checksum mismatch, ignoring response");
        wait_for_start = true;
        pending_response_used = 0;
        return true;
    }

    uint32_t id = ((uint32_t)pending_response[2] << 24) |
                  ((uint32_t)pending_response[3] << 16) |
                  ((uint32_t)pending_response[4] <<  8) |
                  ((uint32_t)pending_response[5] <<  0);

    union {
        float value;
        uint8_t bytes[4];
    } u;

    u.bytes[0] = pending_response[6 + 3];
    u.bytes[1] = pending_response[6 + 2];
    u.bytes[2] = pending_response[6 + 1];
    u.bytes[3] = pending_response[6 + 0];

    debugfln("Received response for ID 0x%08x and value %f", id, u.value);

    for (size_t i = 0; i < value_specs_length; ++i) {
        if (value_specs[i].id == id) {
            meters.update_value(slot, i, u.value * value_specs[i].scale_factor);
            meters.finish_update(slot);
            break;
        }
    }

    wait_for_start = true;
    pending_response_used = 0;

    return true;
}

MeterClassID MeterRCTPower::get_class() const
{
    return MeterClassID::RCTPower;
}

void MeterRCTPower::setup(const Config &ephemeral_config)
{
    client.setup(ephemeral_config);
}
