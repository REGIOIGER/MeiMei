#include <ADCTouchSensor.h>                                  
#include "Keyboard.h"                                                                                                                                                                                   
// 
// This requires an stm32f1 board compatible with the no-grounding-pin feature of ADCTouchSensor.
//

#ifdef ADCTOUCH_INTERNAL_GROUNDING
# define GROUNDED_PIN -1
#endif

#if defined(ARDUINO_ARCH_AVR)
# define PIN1 A0
# define PIN2 A1
# define PIN1 A2
# define PIN2 A3
# define PIN1 A4
# define PIN2 A5
# define PIN1 A6
# define PIN2 A7
# define PIN1 A8
#else
# define PIN1 PA0
# define PIN2 PA1
# define PIN1 PA2
# define PIN2 PA3
# define PIN1 PA4
# define PIN2 PA5
# define PIN1 PA6
# define PIN2 PA7
# define PIN1 PB0
# ifndef ADCTOUCH_INTERNAL_GROUNDING
#  define GROUNDED_PIN PB1
# endif
#endif

#define NUM_PINS  9
unsigned pins[NUM_PINS] = {PA0,PA1,PA2,PA3,PA4,PA5,PA6,PA7,PB0};
unsigned keys[NUM_PINS] = {' ',KEY_UP_ARROW,KEY_LEFT_ARROW,KEY_DOWN_ARROW,KEY_RIGHT_ARROW,'w','a','s','d'}; // Makey-Makey also has 'g' and CLICK, but we don't have enough ADC channels
unsigned previous[NUM_PINS];

ADCTouchSensor* sensors[NUM_PINS];

void processPressKeyboard(int i) {
  Keyboard.press(keys[i]);
}

void processReleaseKeyboard(int i) {
  Keyboard.release(keys[i]);
}

void (*processPress)(int) = processPressKeyboard;
void (*processRelease)(int) = processReleaseKeyboard;


void setup() 
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, 0);     

    for (int i=0; i<NUM_PINS; i++) {
        sensors[i] = new ADCTouchSensor(pins[i], GROUNDED_PIN);
        sensors[i]->begin();
        previous[i] = 0;
    }

    digitalWrite(LED_BUILTIN, 1);
    Keyboard.begin();
} 


void loop() 
{
    uint8_t pressed = 0;
    
    processPress = processPressKeyboard;
    processRelease = processReleaseKeyboard;

    
for (int i=0; i<NUM_PINS; i++) {
      if (sensors[i]->read() > 35) {
         pressed = 1;
         if(previous[i] > 0)
           previous[i] = 10; // 
         if(previous[i] == 0) {
           previous[i] = 10;
           processPress(i);
         }
      }
      else {
         if(previous[i] > 1)
           previous[i]--;
         if(previous[i] == 1) {
           processRelease(i);
           previous[i] = 0;
         }
      }
    }
    
    digitalWrite(LED_BUILTIN, !pressed);
}
