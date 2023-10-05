/* warp-charger
 * Copyright (C) 2020-2021 Erik Fleckstein <erik@tinkerforge.com>
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

#include "sun_spec_meter.h"

#include "api.h"
#include "event_log.h"
#include "task_scheduler.h"
#include "tools.h"
#include "module_dependencies.h"

#define LAST_DEVICE_ADDRESS 247

static const uint16_t base_addresses[] {
    40000,
    50000,
    0
};

static const char *get_modbus_event_name(Modbus::ResultCode event)
{
    switch (event) {
    case 0x00: return "SUCCESS";
    case 0x01: return "ILLEGAL_FUNCTION";
    case 0x02: return "ILLEGAL_ADDRESS";
    case 0x03: return "ILLEGAL_VALUE";
    case 0x04: return "SLAVE_FAILURE";
    case 0x05: return "ACKNOWLEDGE";
    case 0x06: return "SLAVE_DEVICE_BUSY";
    case 0x08: return "MEMORY_PARITY_ERROR";
    case 0x0A: return "PATH_UNAVAILABLE";
    case 0x0B: return "DEVICE_FAILED_TO_RESPOND";
    case 0xE1: return "GENERAL_FAILURE";
    case 0xE2: return "DATA_MISMACH";
    case 0xE3: return "UNEXPECTED_RESPONSE";
    case 0xE4: return "TIMEOUT";
    case 0xE5: return "CONNECTION_LOST";
    case 0xE6: return "CANCEL";
    case 0xE7: return "PASSTHROUGH";
    case 0xE8: return "FORCE_PROCESS";
    default:   return "unkown error code";
    }
}

void SunSpecMeter::pre_setup()
{
    start_discovery = ConfigRoot{Config::Object({
        {"host", Config::Str("", 0, 64)},
        {"port", Config::Uint16(0)},
        {"device_address", Config::Uint8(0)},
    })};
}

void SunSpecMeter::setup()
{
    modbus.client();

    initialized = true;
}

void SunSpecMeter::register_urls()
{
    api.addCommand("sun_spec_meter/start_discovery", &start_discovery, {}, [this](){
        discovery_new = true;
        discovery_new_host = start_discovery.get("host")->asString();
        discovery_new_port = start_discovery.get("port")->asUint();

        uint8_t device_address = start_discovery.get("device_address")->asUint();

        if (device_address == 0) {
            discovery_new_device_address = 1;
            discovery_new_device_address_next = 2;
        } else {
            discovery_new_device_address = device_address;
            discovery_new_device_address_next = device_address;
        }
    }, true);
}

void SunSpecMeter::loop()
{
    if (discovery_new && discovery_state != DiscoveryState::Idle) {
        discovery_state = DiscoveryState::Disconnect;
    }

    switch (discovery_state) {
    case DiscoveryState::Idle:
        if (!discovery_log_idle) {
            ws.pushRawStateUpdate("", "sun_spec_meter/discovery_log");

            discovery_log_idle = true;
        }

        if (discovery_new) {
            ws.pushRawStateUpdate("\"<<<clear_discovery_log>>>\"", "sun_spec_meter/discovery_log");
            discovery_printfln("Starting discovery");

            discovery_state = DiscoveryState::Connect;
            discovery_host = discovery_new_host;
            discovery_port = discovery_new_port;
            discovery_device_address = discovery_new_device_address;
            discovery_device_address_next = discovery_new_device_address_next;
            discovery_base_address_index = 0;
            ++discovery_cookie;

            discovery_new = false;
            discovery_new_host = "";
            discovery_new_port = 0;
            discovery_new_device_address = 0;
            discovery_new_device_address_next = 0;
        }

        break;

    case DiscoveryState::Connect:
        discovery_printfln("Connecting to %s:%u", discovery_host.c_str(), discovery_port);

        if (!modbus.connect(discovery_host, discovery_port)) {
            discovery_printfln("Could not connect to %s:%u", discovery_host.c_str(), discovery_port);

            discovery_state = DiscoveryState::Idle;
            ++discovery_cookie;
        }
        else {
            discovery_state = DiscoveryState::ReadSunSpecID;
        }

        break;

    case DiscoveryState::Disconnect:
        discovery_printfln("Disconnecting from %s", discovery_host.c_str());

        if (!modbus.disconnect(discovery_host)) {
            discovery_printfln("Could not disconnect from %s", discovery_host.c_str());
        }

        discovery_state = DiscoveryState::Idle;
        ++discovery_cookie;

        break;

    case DiscoveryState::NextDeviceAddress:
        discovery_base_address_index = 0;

        if (discovery_device_address == discovery_device_address_next) {
            discovery_state = DiscoveryState::Disconnect;
        }
        else {
            discovery_device_address = discovery_device_address_next;

            if (discovery_device_address_next < LAST_DEVICE_ADDRESS) {
                ++discovery_device_address_next;
            }

            discovery_state = DiscoveryState::ReadSunSpecID;
        }

        break;

    case DiscoveryState::NextBaseAddress:
        ++discovery_base_address_index;

        if (discovery_base_address_index >= ARRAY_SIZE(base_addresses)) {
            discovery_state = DiscoveryState::NextDeviceAddress;
        }
        else {
            discovery_state = DiscoveryState::ReadSunSpecID;
        }

        break;

    case DiscoveryState::ReadSunSpecID: {
            discovery_printfln("Using device address %u", discovery_device_address);
            discovery_printfln("Using base address %u", base_addresses[discovery_base_address_index]);
            discovery_printfln("Reading SunSpec ID");

            uint32_t cookie = discovery_cookie;

            discovery_state = DiscoveryState::ReadingSunSpecID;

            modbus.readHreg(discovery_host, base_addresses[discovery_base_address_index], discovery_buffer, 2,
            [this, cookie](Modbus::ResultCode event, uint16_t transactionId, void *data) -> bool {
                if (discovery_state != DiscoveryState::ReadingSunSpecID || cookie != discovery_cookie) {
                    return true;
                }

                if (event == Modbus::ResultCode::EX_SUCCESS) {
                    uint32_t sun_spec_id = ((uint32_t)discovery_buffer[0] << 16) | discovery_buffer[1];

                    discovery_printfln("Found SunSpec ID: 0x%x", sun_spec_id);

                    discovery_state = DiscoveryState::ReadCommonModel;
                }
                else {
                    discovery_printfln("Could not read SunSpec ID: %s (%d)", get_modbus_event_name(event), event);

                    if (event == Modbus::ResultCode::EX_DEVICE_FAILED_TO_RESPOND) {
                        discovery_state = DiscoveryState::NextDeviceAddress;
                    }
                    else {
                        discovery_state = DiscoveryState::NextBaseAddress;
                    }
                }

                return true;
            }, discovery_device_address);
        }

        break;

    case DiscoveryState::ReadingSunSpecID:
        break;

    case DiscoveryState::ReadCommonModel:
        discovery_printfln("Reading Common Model (FIXME)");

        discovery_state = DiscoveryState::NextDeviceAddress; // FIXME
        break;

    }

    modbus.task();
}

void SunSpecMeter::discovery_printfln(const char *fmt, ...)
{
    va_list args;
    char buf[256];
    auto buf_size = sizeof(buf) / sizeof(buf[0]);

    va_start(args, fmt);
    memset(buf, 0, buf_size);
    auto written = vsnprintf(buf + 1, buf_size - 2, fmt, args);
    va_end(args);

    written = MIN(written, buf_size - 2);

    logger.printfln("SunSpec: %s", buf + 1);

    buf[0] = '\"';
    buf[written + 1] = '\"';
    buf[written + 2] = '\0';

    discovery_log_idle = false;

    ws.pushRawStateUpdate(buf, "sun_spec_meter/discovery_log");
}
