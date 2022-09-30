#include <Adafruit_MAX31856.h>
Adafruit_MAX31856 max0 = Adafruit_MAX31856(10);
Adafruit_MAX31856 max1 = Adafruit_MAX31856(9);
float SENSIBILITY = 0.066; // Modelo 30A
int SAMPLESNUMBER = 100;
//////////////DECLARACION DE PINES:
#define sensor_temp_positive A0
#define sensor_temp_negative A1
#define sensor_temp_body A2
#define sensor_temp_down A3
#define voltage A5
#define SWITCH 2
#define SECURE_RELE 3
////////////DECLARACION DE CONTROL TO READ SENSOR:
#define CODE 1
#define FLOAT 2
////////////SENSOR DE CORRIENTE:
#include <Wire.h>
//#include <MCP342X.h>
//OBjeto MCP342x:
//MCP342X myADC;
/////////////ESTRUCTURAS DE DATOS:
struct outputs {
  int current;
  int voltage;
  int temperature_positive;
  int temperature_negative;
  int temperature_body;
  int temperature_down;
  bool emergency;
  //bool rele_state;
} OUT_TO_LABVIEW;
struct emergency {
  float PEAK_current = 500;
  float voltage_cutoff = 3.2;
  float MAX_temperature = 60;
  float current;
  float voltage;
  float temperature_positive;
  float temperature_negative;
  float temperature_body;
  float temperature_down;
} EMERGENCY;
///////////////////////////ADQUISION DE DATOS:
boolean UP_DATE = false;
int CH[11];//ARRAY DE CANALES PARA ENVIAR DATOS
int REFRESH_DATES(void)//FUNCION PARA ACTUALIZAR LOS DATOS
{
  if (UP_DATE == false) {
    SENSORS_READ(CODE);
    OUT_TO_LABVIEW.emergency = emergency_state(); //READ EMERGENCY STATE NO FALSE.
    CH[0] = OUT_TO_LABVIEW.temperature_positive;
    CH[1] = OUT_TO_LABVIEW.temperature_negative;
    CH[2] = OUT_TO_LABVIEW.temperature_body;
    CH[3] = OUT_TO_LABVIEW.temperature_down;
    CH[4] = OUT_TO_LABVIEW.current;
    CH[5] = OUT_TO_LABVIEW.voltage;
    CH[6] = EMERGENCY.PEAK_current;//valor entero
    CH[7] = EMERGENCY.voltage_cutoff*(1024/5); //valor codificado en entero
    CH[8] = EMERGENCY.MAX_temperature;//valor entero
    CH[9] = OUT_TO_LABVIEW.emergency;
    UP_DATE = true;
  }
}
////////////////////////////ENVIO DE DATOS A LABVIEW:
void SEND_TO_LABVIEW(void) {
  unsigned int last_send;
  if (millis() - last_send > 10) {
    REFRESH_DATES();
    last_send = millis();
  }
  if (UP_DATE == true) {
    for (int x = 0; x <= 10; x++) {
      Serial.print(CH[x]);
      Serial.print(' ');
    }
    Serial.println();
    UP_DATE = false;
  }
}
//////////////////////////////////////////////////////////////  SETUP  ////////////////////////////////////////////////////
void setup(void)
{
  //////////SETUP PARA INTERRUPTOR:
  pinMode(SWITCH, INPUT_PULLUP);
  /////////final setup interruptor.
  //////////SETUP PARA RELE DE SEGURIDAD:
  pinMode(SECURE_RELE, OUTPUT);
  digitalWrite(SECURE_RELE, LOW);
  /////////final setup rele de seguridad.
  //////////SETUP ENVIO DE DATOS:
  Serial.begin(9600);//250000 to labview communication
  /////////final setup envio de datos.
  /////////SETUP SENSOR DE CORRIENTE:
  Wire.begin();
  //  TWBR = 12;
  /*  myADC.configure( MCP342X_MODE_CONTINUOUS |
    /                   MCP342X_CHANNEL_1 |
                     MCP342X_SIZE_18BIT |
                     MCP342X_GAIN_1X
                   );*/
  /////////final setup sensor de corriente.
  max0.begin();
  max1.begin();
  max0.setThermocoupleType(MAX31856_TCTYPE_K);
  max1.setThermocoupleType(MAX31856_TCTYPE_K);

}
//////////////////////////////////////////////////////////////  LOOP  ////////////////////////////////////////////////////
void loop(void)
{
  //////////ENVIO DE DATOS A LABVIEW:
  SEND_TO_LABVIEW();
  //////////final envio datos a labview.
  /////////CONTROL SECURE:
  if (Serial == true) {//mediante esta condicion el rele solo se puede activar si el usb esta conectado
    if ((emergency_state() == false) && (digitalRead(SWITCH) == true)) {
      //digitalWrite(SECURE_RELE, HIGH);////////Se cierra el rele, por tanto hay corriente.
      CH[10] = HIGH;//SE CAMBIA EL CANAL 11(INFORMA DEL ESTADO DEL RELE) A 1
    }
    else {
      //digitalWrite(SECURE_RELE, LOW);///////Se habré el rele, por tanto no hay corriente.
      CH[10] = LOW;//SE CAMBIA EL CANAL 11(INFORMA DEL ESTADO DEL RELE) A 0
    }
  }
}
// FORMAT FOR LABVIEW
//%d %d %d %d %d %d %d %d %d %d %d //

