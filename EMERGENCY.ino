/////////////////////VARIABLES DE CONFIGURACION MANIOBRA DE EMERGENCIA:
float GAP_Temperatura = 0.5;
float GAP_Voltage_Cutoff = 0.3;
float GAP_PEAK_current = 1.0;
int Delay_Temperatura = 500;
int Delay_Corriente = 50;
int Delay_Voltaje = 50;
extern struct emergency EMERGENCY;
////////////////////ESTRUCTURA DE HYSTERESYS:
struct histeresys {
  unsigned long time_emergency;
  unsigned long last_change;
};
///////////////////////////////////////////////// FUNCION DE ESTADO DE EMERGENCIA /////////////////////////////////////////
bool emergency_state(void)
{
  static bool state;
  SENSORS_READ(FLOAT);
  static struct histeresys hys_current, hys_voltage, hys_temperature_positive, hys_temperature_negative, hys_temperature_body, hys_temperature_down;
  //////////////////////////////////////////Deteccion temperature_positive:
  if (EMERGENCY.temperature_positive > EMERGENCY.MAX_temperature - GAP_Temperatura) {
    if ((EMERGENCY.temperature_positive <= EMERGENCY.MAX_temperature))hys_temperature_positive.last_change = millis();
  } else hys_temperature_positive.last_change = millis();
  //////////////////////////////////////////Deteccion temperature_negative:
  if (EMERGENCY.temperature_negative > EMERGENCY.MAX_temperature - GAP_Temperatura) {
    if ((EMERGENCY.temperature_negative <= EMERGENCY.MAX_temperature))hys_temperature_negative.last_change = millis();
  } else hys_temperature_negative.last_change = millis();
  /*//////////////////////////////////////////Deteccion temperature_body:
    if (EMERGENCY.temperature_body > EMERGENCY.MAX_temperature - GAP_Temperatura) {
    if ((EMERGENCY.temperature_body <= EMERGENCY.MAX_temperature))hys_temperature_body.last_change = millis();
    } else hys_temperature_body.last_change = millis();
    //////////////////////////////////////////Deteccion temperature_down:
    if (EMERGENCY.temperature_down > EMERGENCY.MAX_temperature - GAP_Temperatura) {
    if ((EMERGENCY.temperature_down <= EMERGENCY.MAX_temperature))hys_temperature_down.last_change = millis();
    } else hys_temperature_down.last_change = millis();*/
  ///////////////////////////////////////////Deteccion voltaje:
  if (EMERGENCY.voltage < EMERGENCY.voltage_cutoff + GAP_Voltage_Cutoff) {
    if ((EMERGENCY.voltage >= EMERGENCY.voltage_cutoff))hys_voltage.last_change = millis();
  } else hys_voltage.last_change = millis();
  //////////////////////////////////////////Deteccion corriente:
  if (EMERGENCY.current > EMERGENCY.PEAK_current - GAP_PEAK_current) {
    if ((EMERGENCY.current <= EMERGENCY.PEAK_current))hys_current.last_change = millis();
  } else hys_current.last_change = millis();
  //TIEMPOS DE EMERGENCIA:
  hys_temperature_positive.time_emergency = millis() - hys_temperature_positive.last_change;
  hys_temperature_negative.time_emergency = millis() - hys_temperature_negative.last_change;
  /*hys_temperature_body.time_emergency = millis() - hys_temperature_body.last_change;
    hys_temperature_down.time_emergency = millis() - hys_temperature_down.last_change;*/
  hys_voltage.time_emergency = millis() - hys_voltage.last_change;
  hys_current.time_emergency = millis() - hys_current.last_change;
  //CONDICIONES DE DISPARO:
  if ((hys_temperature_positive.time_emergency > Delay_Temperatura) || (hys_temperature_negative.time_emergency > Delay_Temperatura) || (hys_voltage.time_emergency > Delay_Voltaje) || (hys_current.time_emergency > Delay_Corriente)) {
    state = true;
  }
  else {
    state = false;
  }
  return state;
  /*|| (hys_temperature_body.time_emergency > Delay_Temperatura) || (hys_temperature_down.time_emergency > Delay_Temperatura)*/
}
