#include "sem20.h"
#include "obis.h"

#include <WiFiUdp.h>

#include "api.h"
#include "event_log.h"
#include "task_scheduler.h"

extern API api;
extern EventLog logger;
extern TaskScheduler task_scheduler;

WiFiUDP      udp;
IPAddress    mc_groupIP(239, 12, 255, 254);
unsigned int mc_Port = 9522;


obis kennzahlen[] = {
  obis(0,  1, 4, 0,      10.0, 4),
  obis(0,  1, 8, 0, 3600000.0, 8),
  obis(0,  2, 4, 0,      10.0, 4),
  obis(0,  2, 8, 0, 3600000.0, 8),
  obis(0,  3, 4, 0,      10.0, 4),
  obis(0,  3, 8, 0, 3600000.0, 8),
  obis(0,  4, 4, 0,      10.0, 4),
  obis(0,  4, 8, 0, 3600000.0, 8),
  obis(0,  9, 4, 0,      10.0, 4),
  obis(0,  9, 8, 0, 3600000.0, 8),
  obis(0, 10, 4, 0,      10.0, 4),
  obis(0, 10, 8, 0, 3600000.0, 8),
  obis(0, 13, 4, 0,    1000.0, 4),
  obis(0, 14, 4, 0,    1000.0, 4),

  obis(0, 21, 4, 0,      10.0, 4),
  obis(0, 21, 8, 0, 3600000.0, 8),
  obis(0, 22, 4, 0,      10.0, 4),
  obis(0, 22, 8, 0, 3600000.0, 8),
  obis(0, 23, 4, 0,      10.0, 4),
  obis(0, 23, 8, 0, 3600000.0, 8),
  obis(0, 24, 4, 0,      10.0, 4),
  obis(0, 24, 8, 0, 3600000.0, 8),
  obis(0, 29, 4, 0,      10.0, 4),
  obis(0, 29, 8, 0, 3600000.0, 8),
  obis(0, 30, 4, 0,      10.0, 4),
  obis(0, 30, 8, 0, 3600000.0, 8),
  obis(0, 31, 4, 0,    1000.0, 4),
  obis(0, 32, 4, 0,    1000.0, 4),
  obis(0, 33, 4, 0,    1000.0, 4),

  obis(0, 41, 4, 0,      10.0, 4),
  obis(0, 41, 8, 0, 3600000.0, 8),
  obis(0, 42, 4, 0,      10.0, 4),
  obis(0, 42, 8, 0, 3600000.0, 8),
  obis(0, 43, 4, 0,      10.0, 4),
  obis(0, 43, 8, 0, 3600000.0, 8),
  obis(0, 44, 4, 0,      10.0, 4),
  obis(0, 44, 8, 0, 3600000.0, 8),
  obis(0, 49, 4, 0,      10.0, 4),
  obis(0, 49, 8, 0, 3600000.0, 8),
  obis(0, 50, 4, 0,      10.0, 4),
  obis(0, 50, 8, 0, 3600000.0, 8),
  obis(0, 51, 4, 0,    1000.0, 4),
  obis(0, 52, 4, 0,    1000.0, 4),
  obis(0, 53, 4, 0,    1000.0, 4),

  obis(0, 61, 4, 0,      10.0, 4),
  obis(0, 61, 8, 0, 3600000.0, 8),
  obis(0, 62, 4, 0,      10.0, 4),
  obis(0, 62, 8, 0, 3600000.0, 8),
  obis(0, 63, 4, 0,      10.0, 4),
  obis(0, 63, 8, 0, 3600000.0, 8),
  obis(0, 64, 4, 0,      10.0, 4),
  obis(0, 64, 8, 0, 3600000.0, 8),
  obis(0, 69, 4, 0,      10.0, 4),
  obis(0, 69, 8, 0, 3600000.0, 8),
  obis(0, 70, 4, 0,      10.0, 4),
  obis(0, 70, 8, 0, 3600000.0, 8),
  obis(0, 71, 4, 0,    1000.0, 4),
  obis(0, 72, 4, 0,    1000.0, 4),
  obis(0, 73, 4, 0,    1000.0, 4),
};


SEM20::SEM20()
{
  values = Config::Object({
    {"1.4.0", Config::Float(0.0)},
    {"1.8.0", Config::Float(0.0)},
    {"2.4.0", Config::Float(0.0)},
    {"2.8.0", Config::Float(0.0)},
    {"3.4.0", Config::Float(0.0)},
    {"3.8.0", Config::Float(0.0)},
    {"4.4.0", Config::Float(0.0)},
    {"4.8.0", Config::Float(0.0)},
    {"9.4.0", Config::Float(0.0)},
    {"9.8.0", Config::Float(0.0)},
    {"10.4.0", Config::Float(0.0)},
    {"10.8.0", Config::Float(0.0)},
    {"13.4.0", Config::Float(0.0)},
    {"14.4.0", Config::Float(0.0)},
    {"21.4.0", Config::Float(0.0)},
    {"21.8.0", Config::Float(0.0)},
    {"22.4.0", Config::Float(0.0)},
    {"22.8.0", Config::Float(0.0)},
    {"23.4.0", Config::Float(0.0)},
    {"23.8.0", Config::Float(0.0)},
    {"24.4.0", Config::Float(0.0)},
    {"24.8.0", Config::Float(0.0)},
    {"29.4.0", Config::Float(0.0)},
    {"29.8.0", Config::Float(0.0)},
    {"30.4.0", Config::Float(0.0)},
    {"30.8.0", Config::Float(0.0)},
    {"31.4.0", Config::Float(0.0)},
    {"32.4.0", Config::Float(0.0)},
    {"33.4.0", Config::Float(0.0)},
    {"41.4.0", Config::Float(0.0)},
    {"41.8.0", Config::Float(0.0)},
    {"42.4.0", Config::Float(0.0)},
    {"42.8.0", Config::Float(0.0)},
    {"43.4.0", Config::Float(0.0)},
    {"43.8.0", Config::Float(0.0)},
    {"44.4.0", Config::Float(0.0)},
    {"44.8.0", Config::Float(0.0)},
    {"49.4.0", Config::Float(0.0)},
    {"49.8.0", Config::Float(0.0)},
    {"50.4.0", Config::Float(0.0)},
    {"50.8.0", Config::Float(0.0)},
    {"51.4.0", Config::Float(0.0)},
    {"52.4.0", Config::Float(0.0)},
    {"53.4.0", Config::Float(0.0)},
    {"61.4.0", Config::Float(0.0)},
    {"61.8.0", Config::Float(0.0)},
    {"62.4.0", Config::Float(0.0)},
    {"62.8.0", Config::Float(0.0)},
    {"63.4.0", Config::Float(0.0)},
    {"63.8.0", Config::Float(0.0)},
    {"64.4.0", Config::Float(0.0)},
    {"64.8.0", Config::Float(0.0)},
    {"69.4.0", Config::Float(0.0)},
    {"69.8.0", Config::Float(0.0)},
    {"70.4.0", Config::Float(0.0)},
    {"70.8.0", Config::Float(0.0)},
    {"71.4.0", Config::Float(0.0)},
    {"72.4.0", Config::Float(0.0)},
    {"73.4.0", Config::Float(0.0)},
  });
}

void SEM20::setup()
{
  if (udp.beginMulticast(mc_groupIP, mc_Port)) {
    logger.printfln("Listening to Multicast at %s:%u\n", mc_groupIP.toString().c_str(), mc_Port);
  }

  task_scheduler.scheduleWithFixedDelay([this](){
    auto packetSize = udp.parsePacket();

    if (packetSize > 0) {
      uint8_t buf[1024] = { 0 };
      auto len = udp.read(buf, 1023);

      if (len == 608) {
        for (auto index = 0; index < sizeof(kennzahlen) / sizeof(kennzahlen[0]); index++) {
          Config* value = values.get(kennzahlen[index].name());

          if (value != nullptr) {
            value->updateFloat(kennzahlen[index].value(buf, len));
          }
        }
      }
    }
  }, 1000, 1000);

  initialized = true;
}

void SEM20::register_urls()
{
  api.addState("sem20/values", &values, {}, 1000);
}

void SEM20::loop()
{
}
