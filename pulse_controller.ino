
unsigned long pulseTimer;
boolean pulse;
boolean startPulse;
int pulsePin;

void sendPulse(int pin) {
    startPulse = true;
    pulsePin = pin;
}


void setupPulseController() {
    pulse = false;
    startPulse = false;
    pulsePin = 0;
    
    pinMode(PIN_CENTRALIZADO, OUTPUT);
    digitalWrite(PIN_CENTRALIZADO, LOW);
}


void loopPulseController() {
    if(startPulse) {
       pulseTimer = millis() + LONGITUD_PULSO;
       startPulse = false;
    }

    if(pulseTimer > millis()) {
      if(!pulse) {
        pulse = true;
        digitalWrite(pulsePin, HIGH);
      }
    } else {
      if(pulse) {
        pulse = false;
        digitalWrite(pulsePin, LOW);
      }
    }
}
