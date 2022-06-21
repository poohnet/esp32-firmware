#pragma once

#include <Arduino.h>

class obis {
  public:
    obis(uint8_t kanal, uint8_t messgroesse, uint8_t messart, uint8_t tarifstufe, double faktor, uint8_t valueSize);

    String name() const;
    double value(uint8_t buf[], size_t bufSize);

  private:
    bool compareObis(uint8_t buf[]) const;
    static uint32_t convert_uint32(uint8_t buf[]);
    static uint64_t convert_uint64(uint8_t buf[]);

  private:
    uint8_t _obis[4];
    double  _faktor;
    uint8_t _valueSize;
    size_t  _position;
};
