#include "sem20.h"
#include "obis.h"

#include <WiFiUdp.h>

#include "api.h"
#include "event_log.h"
#include "task_scheduler.h"

extern API api;
extern EventLog logger;
extern TaskScheduler task_scheduler;

WiFiUDP    udp;
IPAddress  mc_groupIP(239, 12, 255, 254);
uint16_t   mc_Port = 9522;

#define PACKET_LEN 608
#define ALL_VALUES_COUNT 59

obis kennzahlen[ALL_VALUES_COUNT] = {
  // Wirkleistung Bezug (aktueller Mittelwert)
  obis(0, 21, 4, 0,      10.0, 4),  // L1
  obis(0, 41, 4, 0,      10.0, 4),  // L2
  obis(0, 61, 4, 0,      10.0, 4),  // L3
  obis(0,  1, 4, 0,      10.0, 4),  // Summe

  // Wirkleistung Bezug (Zählerstand)
  obis(0, 21, 8, 0, 3600000.0, 8),  // L1
  obis(0, 41, 8, 0, 3600000.0, 8),  // L2
  obis(0, 61, 8, 0, 3600000.0, 8),  // L3
  obis(0,  1, 8, 0, 3600000.0, 8),  // Summe

  // Blindleistung Bezug (aktueller Mittelwert)
  obis(0, 23, 4, 0,      10.0, 4),  // L1
  obis(0, 43, 4, 0,      10.0, 4),  // L2
  obis(0, 63, 4, 0,      10.0, 4),  // L3
  obis(0,  3, 4, 0,      10.0, 4),  // Summe

  // Blindleistung Bezug (Zählerstand)
  obis(0, 23, 8, 0, 3600000.0, 8),  // L1
  obis(0, 43, 8, 0, 3600000.0, 8),  // L2
  obis(0, 63, 8, 0, 3600000.0, 8),  // L3
  obis(0,  3, 8, 0, 3600000.0, 8),  // Summe

  // Scheinleistung Bezug (aktueller Mittelwert)
  obis(0, 29, 4, 0,      10.0, 4),  // L1
  obis(0, 49, 4, 0,      10.0, 4),  // L2
  obis(0, 69, 4, 0,      10.0, 4),  // L3
  obis(0,  9, 4, 0,      10.0, 4),  // Summe

  // Scheinleistung Bezug (Zählerstand)
  obis(0, 29, 8, 0, 3600000.0, 8),  // L1
  obis(0, 49, 8, 0, 3600000.0, 8),  // L2
  obis(0, 69, 8, 0, 3600000.0, 8),  // L3
  obis(0,  9, 8, 0, 3600000.0, 8),  // Summe

  // Wirkleistung Einspeisung (aktueller Mittelwert)
  obis(0, 22, 4, 0,      10.0, 4),  // L1
  obis(0, 42, 4, 0,      10.0, 4),  // L2
  obis(0, 62, 4, 0,      10.0, 4),  // L3
  obis(0,  2, 4, 0,      10.0, 4),  // Summe

  // Wirkleistung Einspeisung (Zählerstand)
  obis(0, 22, 8, 0, 3600000.0, 8),  // L1
  obis(0, 42, 8, 0, 3600000.0, 8),  // L2
  obis(0, 62, 8, 0, 3600000.0, 8),  // L3
  obis(0,  2, 8, 0, 3600000.0, 8),  // Summe

  // Blindleistung Einspeisung (aktueller Mittelwert)
  obis(0, 24, 4, 0,      10.0, 4),  // L1
  obis(0, 44, 4, 0,      10.0, 4),  // L2
  obis(0, 64, 4, 0,      10.0, 4),  // L3
  obis(0,  4, 4, 0,      10.0, 4),  // Summe

  // Blindleistung Einspeisung (Zählerstand)
  obis(0, 24, 8, 0, 3600000.0, 8),  // L1
  obis(0, 44, 8, 0, 3600000.0, 8),  // L2
  obis(0, 64, 8, 0, 3600000.0, 8),  // L3
  obis(0,  4, 8, 0, 3600000.0, 8),  // Summe

  // Scheinleistung Einspeisung (aktueller Mittelwert)
  obis(0, 30, 4, 0,      10.0, 4),  // L1
  obis(0, 50, 4, 0,      10.0, 4),  // L2
  obis(0, 70, 4, 0,      10.0, 4),  // L3
  obis(0, 10, 4, 0,      10.0, 4),  // Summe

  // Scheinleistung Einspeisung (Zählerstand)
  obis(0, 30, 8, 0, 3600000.0, 8),  // L1
  obis(0, 50, 8, 0, 3600000.0, 8),  // L2
  obis(0, 70, 8, 0, 3600000.0, 8),  // L3
  obis(0, 10, 8, 0, 3600000.0, 8),  // Summe

  // Leistungsfaktor (cos phi)
  obis(0, 33, 4, 0,    1000.0, 4),  // L1
  obis(0, 53, 4, 0,    1000.0, 4),  // L2
  obis(0, 73, 4, 0,    1000.0, 4),  // L3
  obis(0, 13, 4, 0,    1000.0, 4),  // Summe

  // Spannung
  obis(0, 32, 4, 0,    1000.0, 4),  // L1
  obis(0, 52, 4, 0,    1000.0, 4),  // L2
  obis(0, 72, 4, 0,    1000.0, 4),  // L3

  // Strom
  obis(0, 31, 4, 0,    1000.0, 4),  // L1
  obis(0, 51, 4, 0,    1000.0, 4),  // L2
  obis(0, 71, 4, 0,    1000.0, 4),  // L3

  // Netzfrequenz
  obis(0, 14, 4, 0,    1000.0, 4),
};


SEM20::SEM20()
{
}

SEM20::~SEM20()
{
}

void SEM20::pre_setup()
{
  values = Config::Object({{"power", Config::Float(0.0)}});
  all_values = Config::Array({}, new Config{Config::Float(0.0)}, 0, ALL_VALUES_COUNT, Config::type_id<Config::ConfFloat>());

  for (auto index = all_values.count(); index < ALL_VALUES_COUNT; index++) {
    all_values.add();
  }
}

void SEM20::setup()
{
  if (udp.beginMulticast(mc_groupIP, mc_Port)) {
    logger.printfln("SEM20: Listening to Multicast at %s:%u\n", mc_groupIP.toString().c_str(), mc_Port);
  }

  power_hist.setup();

  task_scheduler.scheduleWithFixedDelay([this](){
    auto packetSize = udp.parsePacket();

    if (packetSize > 0) {
      uint8_t buf[1024] = { 0 };
      auto len = udp.read(buf, 1023);

      if (len == PACKET_LEN) {
        for (auto index = 0; index < ALL_VALUES_COUNT; index++) {
          all_values.get(index)->updateFloat(kennzahlen[index].value(buf, len));
        }

        auto power = kennzahlen[3].value(buf, len) - kennzahlen[27].value(buf, len);

        values.get("power")->updateFloat(power);
        power_hist.add_sample(power);
      }
    }
  }, 1000, 1000);

  initialized = true;
}

void SEM20::register_urls()
{
  api.addState("sem20/values", &values, {}, 1000);
  api.addState("sem20/all_values", &all_values, {}, 1000);

  power_hist.register_urls("sem20/");
}

void SEM20::loop()
{
}
