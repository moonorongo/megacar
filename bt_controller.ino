/* Maquina de estados para leer serial in */

void setupBtController() {
   Serial.begin(SERIAL_BITRATE);
   SSerial.begin(SS_BITRATE);
   inputString.reserve(20);
   
}


void loopBtController() {
   sserialEvent(); 
}


void sserialEvent() {
  while (SSerial.available()) {
    char inChar = (char)SSerial.read();
    if (inChar == TERMINADOR) {
        if(inputString.startsWith("c")) { // solo toma ordenes validas
            inputComplete = true;
        } else {
            inputComplete = false;
            inputString = "";
        }
    } else {
        inputString += inChar;
    }
  } // while
}


void sendIntValue(const char *key, int value) {
    if(key[0] == ',') {
      SSerial.print(',');
    } else {
      SSerial.print(value);      
    }
}





