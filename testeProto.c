#include "pt.h"
#define LEDPIN 13 

static struct pt pt1, pt2;

void setup() {
  pinMode(LEDPIN, OUTPUT); // LED init
  PT_INIT(&pt1);  // inicializar dos protothreads
  PT_INIT(&pt2);
}

void toggleLED() { //cambia el estado del LED usando xor
  boolean ledstate = digitalRead(LEDPIN);
  ledstate ^= 1;
  digitalWrite(LEDPIN, ledstate);
}

static int protothread1(struct pt *pt, int interval) {  // la funcion cambia el LED despues del intervalo
  static unsigned long timestamp = 0;
  PT_BEGIN(pt);
  while(1) {
    PT_WAIT_UNTIL(pt, millis() - timestamp > interval );
    timestamp = millis();
    toggleLED();
  }
  PT_END(pt);
}

static int protothread2(struct pt *pt, int interval) {
  static unsigned long timestamp = 0;
  PT_BEGIN(pt);
  while(1) {
    PT_WAIT_UNTIL(pt, millis() - timestamp > interval );
    timestamp = millis();
    toggleLED();
  }
  PT_END(pt);
}

void loop() {
  protothread1(&pt1, 900);
  protothread2(&pt2, 1000);
}