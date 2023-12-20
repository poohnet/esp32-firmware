/* esp32-firmware
 * Copyright (C) 2023 Thomas Hein
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

#include <WiFiUdp.h>

#include "meter_sma.h"
#include "obis.h"
#include "module_dependencies.h"

#include "modules/meters/meter_value_id.h"
#include "modules/meters/sdm_helpers.h"
#include "task_scheduler.h"
#include "tools.h"

#include "gcc_warnings.h"

WiFiUDP    udp;
IPAddress  mc_groupIP(239, 12, 255, 254);
uint16_t   mc_Port = 9522;

#define SMA_PACKET_LEN 608

obis kennzahlen[] = {
  // Wirkleistung Bezug minus Einspeisung
  obis(MeterValueID::PowerActiveLSumImExDiff,     0,  0, 0, 0,       1.0, 0),  // Summe

  // Wirkleistung Bezug (aktueller Mittelwert)
  obis(MeterValueID::PowerActiveLSumImport,       0,  1, 4, 0,      10.0, 4),  // Summe
  obis(MeterValueID::PowerActiveL1Import,         0, 21, 4, 0,      10.0, 4),  // L1
  obis(MeterValueID::PowerActiveL2Import,         0, 41, 4, 0,      10.0, 4),  // L2
  obis(MeterValueID::PowerActiveL3Import,         0, 61, 4, 0,      10.0, 4),  // L3

  // Wirkleistung Bezug (Zählerstand)
  obis(MeterValueID::EnergyActiveLSumImport,      0,  1, 8, 0, 3600000.0, 8),  // Summe
  obis(MeterValueID::EnergyActiveL1Import,        0, 21, 8, 0, 3600000.0, 8),  // L1
  obis(MeterValueID::EnergyActiveL2Import,        0, 41, 8, 0, 3600000.0, 8),  // L2
  obis(MeterValueID::EnergyActiveL3Import,        0, 61, 8, 0, 3600000.0, 8),  // L3

  // Blindleistung Bezug (aktueller Mittelwert)
  obis(MeterValueID::PowerReactiveLSumInductive,  0,  3, 4, 0,      10.0, 4),  // Summe
  obis(MeterValueID::PowerReactiveL1Inductive,    0, 23, 4, 0,      10.0, 4),  // L1
  obis(MeterValueID::PowerReactiveL2Inductive,    0, 43, 4, 0,      10.0, 4),  // L2
  obis(MeterValueID::PowerReactiveL3Inductive,    0, 63, 4, 0,      10.0, 4),  // L3

  // Blindleistung Bezug (Zählerstand)
  obis(MeterValueID::EnergyReactiveLSumInductive, 0,  3, 8, 0, 3600000.0, 8),  // Summe
  obis(MeterValueID::EnergyReactiveL1Inductive,   0, 23, 8, 0, 3600000.0, 8),  // L1
  obis(MeterValueID::EnergyReactiveL2Inductive,   0, 43, 8, 0, 3600000.0, 8),  // L2
  obis(MeterValueID::EnergyReactiveL3Inductive,   0, 63, 8, 0, 3600000.0, 8),  // L3

  // Scheinleistung Bezug (aktueller Mittelwert)
  obis(MeterValueID::PowerApparentLSumImport,     0,  9, 4, 0,      10.0, 4),  // Summe
  obis(MeterValueID::PowerApparentL1Import,       0, 29, 4, 0,      10.0, 4),  // L1
  obis(MeterValueID::PowerApparentL2Import,       0, 49, 4, 0,      10.0, 4),  // L2
  obis(MeterValueID::PowerApparentL3Import,       0, 69, 4, 0,      10.0, 4),  // L3

  // Scheinleistung Bezug (Zählerstand)
  obis(MeterValueID::EnergyApparentLSumImport,    0,  9, 8, 0, 3600000.0, 8),  // Summe
  obis(MeterValueID::EnergyApparentL1Import,      0, 29, 8, 0, 3600000.0, 8),  // L1
  obis(MeterValueID::EnergyApparentL2Import,      0, 49, 8, 0, 3600000.0, 8),  // L2
  obis(MeterValueID::EnergyApparentL3Import,      0, 69, 8, 0, 3600000.0, 8),  // L3

  // Wirkleistung Einspeisung (aktueller Mittelwert)
  obis(MeterValueID::PowerActiveLSumExport,       0,  2, 4, 0,      10.0, 4),  // Summe
  obis(MeterValueID::PowerActiveL1Export,         0, 22, 4, 0,      10.0, 4),  // L1
  obis(MeterValueID::PowerActiveL2Export,         0, 42, 4, 0,      10.0, 4),  // L2
  obis(MeterValueID::PowerActiveL3Export,         0, 62, 4, 0,      10.0, 4),  // L3

  // Wirkleistung Einspeisung (Zählerstand)
  obis(MeterValueID::EnergyActiveLSumExport,      0,  2, 8, 0, 3600000.0, 8),  // Summe
  obis(MeterValueID::EnergyActiveL1Export,        0, 22, 8, 0, 3600000.0, 8),  // L1
  obis(MeterValueID::EnergyActiveL2Export,        0, 42, 8, 0, 3600000.0, 8),  // L2
  obis(MeterValueID::EnergyActiveL3Export,        0, 62, 8, 0, 3600000.0, 8),  // L3

  // Blindleistung Einspeisung (aktueller Mittelwert)
  obis(MeterValueID::PowerReactiveLSumCapacitive, 0,  4, 4, 0,      10.0, 4),  // Summe
  obis(MeterValueID::PowerReactiveL1Capacitive,   0, 24, 4, 0,      10.0, 4),  // L1
  obis(MeterValueID::PowerReactiveL2Capacitive,   0, 44, 4, 0,      10.0, 4),  // L2
  obis(MeterValueID::PowerReactiveL3Capacitive,   0, 64, 4, 0,      10.0, 4),  // L3

  // Blindleistung Einspeisung (Zählerstand)
  obis(MeterValueID::EnergyReactiveLSumCapacitive,0,  4, 8, 0, 3600000.0, 8),  // Summe
  obis(MeterValueID::EnergyReactiveL1Capacitive,  0, 24, 8, 0, 3600000.0, 8),  // L1
  obis(MeterValueID::EnergyReactiveL2Capacitive,  0, 44, 8, 0, 3600000.0, 8),  // L2
  obis(MeterValueID::EnergyReactiveL3Capacitive,  0, 64, 8, 0, 3600000.0, 8),  // L3

  // Scheinleistung Einspeisung (aktueller Mittelwert)
  obis(MeterValueID::PowerApparentLSumExport,     0, 10, 4, 0,      10.0, 4),  // Summe
  obis(MeterValueID::PowerApparentL1Export,       0, 30, 4, 0,      10.0, 4),  // L1
  obis(MeterValueID::PowerApparentL2Export,       0, 50, 4, 0,      10.0, 4),  // L2
  obis(MeterValueID::PowerApparentL3Export,       0, 70, 4, 0,      10.0, 4),  // L3

  // Scheinleistung Einspeisung (Zählerstand)
  obis(MeterValueID::EnergyApparentLSumExport,    0, 10, 8, 0, 3600000.0, 8),  // Summe
  obis(MeterValueID::EnergyApparentL1Export,      0, 30, 8, 0, 3600000.0, 8),  // L1
  obis(MeterValueID::EnergyApparentL2Export,      0, 50, 8, 0, 3600000.0, 8),  // L2
  obis(MeterValueID::EnergyApparentL3Export,      0, 70, 8, 0, 3600000.0, 8),  // L3

  // Leistungsfaktor (cos phi)
  obis(MeterValueID::PowerFactorLSum,             0, 13, 4, 0,    1000.0, 4),  // Summe
  obis(MeterValueID::PowerFactorL1,               0, 33, 4, 0,    1000.0, 4),  // L1
  obis(MeterValueID::PowerFactorL2,               0, 53, 4, 0,    1000.0, 4),  // L2
  obis(MeterValueID::PowerFactorL3,               0, 73, 4, 0,    1000.0, 4),  // L3

  // Spannung
  obis(MeterValueID::VoltageL1N,                  0, 32, 4, 0,    1000.0, 4),  // L1
  obis(MeterValueID::VoltageL2N,                  0, 52, 4, 0,    1000.0, 4),  // L2
  obis(MeterValueID::VoltageL3N,                  0, 72, 4, 0,    1000.0, 4),  // L3

  // Strom
  obis(MeterValueID::CurrentL1ImExSum,            0, 31, 4, 0,    1000.0, 4),  // L1
  obis(MeterValueID::CurrentL2ImExSum,            0, 51, 4, 0,    1000.0, 4),  // L2
  obis(MeterValueID::CurrentL3ImExSum,            0, 71, 4, 0,    1000.0, 4),  // L3

  // Netzfrequenz
  obis(MeterValueID::FrequencyLAvg,               0, 14, 4, 0,    1000.0, 4),
};

MeterSMA::MeterSMA(uint32_t slot)
  : _slot(slot)
{
}

_ATTRIBUTE((const))
MeterClassID MeterSMA::get_class() const
{
  return MeterClassID::SMA;
}

void MeterSMA::setup(Config &ephemeral_config)
{
  if (udp.beginMulticast(mc_groupIP, mc_Port)) {
      logger.printfln("MeterSMA: Listening to Multicast at %s:%u\n", mc_groupIP.toString().c_str(), mc_Port);
  }

  MeterValueID valueIds[ARRAY_SIZE(kennzahlen)];

  for (size_t index = 0; index < ARRAY_SIZE(kennzahlen); index++) {
    valueIds[index] = kennzahlen[index].valueId();
  }

  meters.declare_value_ids(_slot, valueIds, ARRAY_SIZE(kennzahlen));

  task_scheduler.scheduleWithFixedDelay([this](){
    update_all_values();
  }, 0, 990);
}

void MeterSMA::update_all_values()
{
  auto packetSize = udp.parsePacket();

  if (packetSize > 0) {
    uint8_t buf[1024] = { 0 };
    auto len = static_cast<size_t>(udp.read(buf, 1023));

    if (len == SMA_PACKET_LEN) {
      float values[ARRAY_SIZE(kennzahlen)];
      values[0] = kennzahlen[1].value(buf, len) - kennzahlen[25].value(buf, len);

      for (size_t index = 1; index < ARRAY_SIZE(kennzahlen); index++) {
        values[index] = kennzahlen[index].value(buf, len);
      }

      //float power = kennzahlen[3].value(buf, len) - kennzahlen[27].value(buf, len);
      meters.update_all_values(_slot, values);
    }
  }
}
