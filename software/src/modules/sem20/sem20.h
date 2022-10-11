#pragma once

#include "config.h"

class SEM20 {
  public:
    SEM20();
    ~SEM20();

    void pre_setup();
    void setup();
    void register_urls();
    void loop();

    bool initialized = false;

  private:
    ConfigRoot values;
};
