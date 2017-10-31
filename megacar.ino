#include <SoftwareSerial.h>
#include <Servo.h>
#include <EEPROM.h>


/* CONSTANTES */
/* IO Digitales */
// #define PIN_ 2 // DIGITAL
#define PIN_SERVO 3  // cable naranja (marron = gnd, rojo = vcc)
#define PIN_LED_ROJO 4
// #define PIN_ 5 // PWM
// #define PIN_ 6 // PWM
#define PIN_PULSADOR 7
#define PIN_LED_VERDE 8
// #define PIN_ 9 // PWM
#define PIN_SS_RX 10
#define PIN_SS_TX 11
#define PIN_CENTRALIZADO 12
// #define PIN_ 13 // DIGITAL

/* IO Analogicas */
// ANALOG_ A1 a A7

#define SS_BITRATE 4800
#define SERIAL_BITRATE 9600
#define LONGITUD_PULSO 500
#define TERMINADOR '\n'


/* TIPOS */
struct ConfigStructure {
    unsigned int CEBADOR_MAX;
    unsigned int CEBADOR_MIN;
};

/* VARIABLES */

// utilizadas por bt_controller
String inputString = "";         
boolean inputComplete = false;   
int cebadorMax = 180;
int cebadorMin = 1;
int cebadorPos = cebadorMin;
int cebadorStatus = 1;
unsigned long cebadorHastaMillis;

ConfigStructure configuracion; 

SoftwareSerial SSerial(PIN_SS_RX, PIN_SS_TX);
Servo ServoCebador;  

/* ---------- funciones internas ---------------- */
void getConfig() {
    EEPROM.get(0, configuracion);    
    cebadorMax = configuracion.CEBADOR_MAX;
    cebadorMin = configuracion.CEBADOR_MIN;
}

void saveConfig() {
    configuracion.CEBADOR_MAX = cebadorMax;
    configuracion.CEBADOR_MIN = cebadorMin;
    EEPROM.put(0, configuracion);    
};

void moverCebador(int posicion) {
  ServoCebador.write(posicion);  
  cebadorPos = posicion;
}

void moverCebadorPorStatus() {
  switch(cebadorStatus) {
    case 1 : moverCebador(cebadorMin);
             break;
    case 2 : moverCebador(((cebadorMax - cebadorMin) / 2) + cebadorMin);
             break;
    case 3 : moverCebador(cebadorMax);
             break;
  }
  
  updateLed();
}

void setup() {
  getConfig();

  setupBtController();
  setupPulseController();
  setupSwitchController();

  ServoCebador.attach(PIN_SERVO);
  ServoCebador.write(cebadorMin);

  cebadorHastaMillis = millis();

  moverCebadorPorStatus();
  Serial.println("START!");
}

void loop() {
  loopBtController();
  loopPulseController();
  loopSwitchController();
  
  if(inputComplete) {
    if(inputString.startsWith("copen")) { // pulso centralizado
      if(inputString.substring(5) == "ni4Diche") {
        sendPulse(PIN_CENTRALIZADO);
      }
    }  

    if(inputString.startsWith("cstar")) { // motor en contacto (optoacoplador? ver)
        /*
          - checkear temperatura del motor
          - si esta frio (A IMPLEMENTAR A FUTURO) 
            - Tomar el timer millis() (unsigned?? preguntar)
            - cebadorHasta = millis() + (1000 * 60) (1 minuto?? configurable desde app)
         */
        cebadorHastaMillis = millis() + (1000 * 2);
    }  


    if(inputString.startsWith("ccmax")) { // cebador max
        moverCebador(cebadorMax);
    }  

    if(inputString.startsWith("ccmin")) { // cebador min
        moverCebador(cebadorMin);
    }  

    if(inputString.startsWith("ccmov")) { // cebador mueve angulo
        cebadorPos = inputString.substring(5).toInt();
        moverCebador(cebadorPos);
    }  

    if(inputString.startsWith("csmin")) { // set cebador min
        cebadorMin = cebadorPos;
    }  

    if(inputString.startsWith("csmax")) { // set cebador max
        cebadorMax = cebadorPos;
    }  

    if(inputString.startsWith("cstor")) { // guarda en eeprom
        saveConfig();
    }  

    if(inputString.startsWith("cgetp")) { // obtiene de eeprom
        getConfig();
        sendIntValue("", cebadorMin);
        sendIntValue(",", 0);
        sendIntValue("", cebadorMax);
    }  

    inputString = "";
    inputComplete = false; 
  }

  /* LOGICA PARA CONTROLAR APERTURA DE CEBADOR AL DAR CONTACTO */
  if (millis() < cebadorHastaMillis) { // hay que abrir el cebador
    if(cebadorPos != cebadorMax) { // si no esta abierto lo abre
      moverCebador(cebadorMax);        
    } 
     
  } else {
    if(cebadorPos != cebadorMin) { // si esta abierto lo cierra  
      moverCebador(cebadorMin);
    }
  }

  
}


