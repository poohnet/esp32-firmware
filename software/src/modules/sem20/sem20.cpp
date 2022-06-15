#include "sem20.h"
#include "obis.h"

#include <WiFiUdp.h>

#include "api.h"
#include "event_log.h"
#include "task_scheduler.h"

extern EventLog logger;
extern TaskScheduler task_scheduler;

WiFiUDP      udp;
IPAddress    mc_groupIP(239, 12, 255, 254);
unsigned int mc_Port = 9522;


obis kennzahlen[] = {
  obis("Wirkleistung Bezug ................. ", 0,  1, 4, 0,      10.0, 4),
  obis("Wirkleistung Lieferung ............. ", 0,  2, 4, 0,      10.0, 4),
  obis("Wirkleistung Bezug Summe ........... ", 0,  1, 8, 0, 3600000.0, 8),
  obis("Wirkleistung Lieferung Summe ....... ", 0,  2, 8, 0, 3600000.0, 8),
  obis("Leistungsfaktor (cos phi) .......... ", 0, 13, 4, 0,    1000.0, 4),
  obis("Netzfrequenz ........................", 0, 14, 4, 0,    1000.0, 4),

  obis("L1 Wirkleistung Bezug .............. ", 0, 21, 4, 0,      10.0, 4),
  obis("L1 Wirkleistung Lieferung .......... ", 0, 22, 4, 0,      10.0, 4),
  obis("L1 Wirkleistung Bezug Summe ........ ", 0, 21, 8, 0, 3600000.0, 8),
  obis("L1 Wirkleistung Lieferung Summe .... ", 0, 22, 8, 0, 3600000.0, 8),
  obis("L1 Strom ........................... ", 0, 31, 4, 0,    1000.0, 4),
  obis("L1 Spannung ........................ ", 0, 32, 4, 0,    1000.0, 4),
  obis("L1 Leistungsfaktor (cos phi) ....... ", 0, 33, 4, 0,    1000.0, 4),

  obis("L2 Wirkleistung Bezug .............. ", 0, 41, 4, 0,      10.0, 4),
  obis("L2 Wirkleistung Lieferung .......... ", 0, 42, 4, 0,      10.0, 4),
  obis("L2 Wirkleistung Bezug Summe ........ ", 0, 41, 8, 0, 3600000.0, 8),
  obis("L2 Wirkleistung Lieferung Summe .... ", 0, 42, 8, 0, 3600000.0, 8),
  obis("L2 Strom ........................... ", 0, 51, 4, 0,    1000.0, 4),
  obis("L2 Spannung ........................ ", 0, 52, 4, 0,    1000.0, 4),
  obis("L2 Leistungsfaktor (cos phi) ....... ", 0, 53, 4, 0,    1000.0, 4),

  obis("L3 Wirkleistung Bezug .............. ", 0, 61, 4, 0,      10.0, 4),
  obis("L3 Wirkleistung Lieferung .......... ", 0, 62, 4, 0,      10.0, 4),
  obis("L3 Wirkleistung Bezug Summe ........ ", 0, 61, 8, 0, 3600000.0, 8),
  obis("L3 Wirkleistung Lieferung Summe .... ", 0, 62, 8, 0, 3600000.0, 8),
  obis("L3 Strom ........................... ", 0, 71, 4, 0,    1000.0, 4),
  obis("L3 Spannung ........................ ", 0, 72, 4, 0,    1000.0, 4),
  obis("L3 Leistungsfaktor (cos phi) ....... ", 0, 73, 4, 0,    1000.0, 4),
};


SEM20::SEM20()
{
}

void SEM20::setup()
{
  if (udp.beginMulticast(mc_groupIP, mc_Port)) {
    logger.printfln("Listening to Multicast at %s:%u\n", mc_groupIP.toString().c_str(), mc_Port);
  }

  task_scheduler.scheduleWithFixedDelay([this](){
    auto packetSize = udp.parsePacket();
  
    if (packetSize > 0) {
      uint8_t buf[1024] = { NULL };
      auto len = udp.read(buf, 1023);

      for (auto index = 0; index < sizeof(kennzahlen) / sizeof(kennzahlen[0]); index++) {
        logger.printfln("%s : %.3lf\n", kennzahlen[index].name(), kennzahlen[index].value(buf, len));
      }
    }
  }, 1000, 10000);

  initialized = true;
}

void SEM20::register_urls()
{
}

void SEM20::loop()
{
}
