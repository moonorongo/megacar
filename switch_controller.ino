int statusPulsador;

void setupSwitchController() {
  pinMode(PIN_LED_VERDE, OUTPUT);
  pinMode(PIN_LED_ROJO, OUTPUT);
  pinMode(PIN_PULSADOR, INPUT);
  digitalWrite(PIN_PULSADOR, HIGH);
  statusPulsador = 0;
}

void updateLed() {
  switch(cebadorStatus) {
    case 1 :  digitalWrite(PIN_LED_VERDE, HIGH);
              digitalWrite(PIN_LED_ROJO, LOW);
              break;
    case 2 :  digitalWrite(PIN_LED_VERDE, HIGH);
              digitalWrite(PIN_LED_ROJO, HIGH);
              break;
    case 3 :  digitalWrite(PIN_LED_VERDE, LOW);
              digitalWrite(PIN_LED_ROJO, HIGH);
              break;
  }
}


void loopSwitchController() {
  
  
  switch (statusPulsador) {
    case 0 :  if(!digitalRead(PIN_PULSADOR)) {
                statusPulsador = 1;
                delay(100);
              }
              break;
    case 1 : if(!digitalRead(PIN_PULSADOR)) {
                statusPulsador = 2;
              } else {
                statusPulsador = 0;
              }
              break;
    case 2 : if(digitalRead(PIN_PULSADOR)) {
               // SOLO PARA TESTING
               inputString = "cstar";
               inputComplete = true;

               /* CODIGO ORIGINAL
               cebadorStatus++;
               if(cebadorStatus > 3) {
                 cebadorStatus = 1; 
               }
               moverCebadorPorStatus();
               */ 

               statusPulsador = 0;
               delay(100);
             }
    }
}
