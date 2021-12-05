#include <Arduino.h>

#define WATER_OFF 0
#define WATER_ON 1

class Waterpump {
  private:
    int pin1;
    int pin2;
    byte state;

  public:
    Waterpump(int pin1,int pin2);
    void init();
    void on();
    void off();
    byte getState();
};
