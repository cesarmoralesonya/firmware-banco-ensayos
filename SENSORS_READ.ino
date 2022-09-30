void SENSORS_READ(int CASE)
{
  ////////////////////SENSOR DE CORRIENTE:
  // static int32_t  sensor_current;
  //  myADC.startConversion();
  //  myADC.getResult(&sensor_current);
  if (CASE == CODE) {
    OUT_TO_LABVIEW.temperature_positive = max0.readThermocoupleTemperature();
    OUT_TO_LABVIEW.temperature_negative = max1.readThermocoupleTemperature();
    //OUT_TO_LABVIEW.temperature_body = analogRead(sensor_temp_body);
    //OUT_TO_LABVIEW.temperature_down = analogRead(sensor_temp_down);
    OUT_TO_LABVIEW.current = getCorriente(SAMPLESNUMBER);
    OUT_TO_LABVIEW.voltage = analogRead(voltage);

  }
  if (CASE == FLOAT) {
    EMERGENCY.temperature_positive = max0.readThermocoupleTemperature();
    EMERGENCY.temperature_negative = max1.readThermocoupleTemperature();
    //EMERGENCY.temperature_body = analogRead(sensor_temp_body) * (5.0 / 1024.0) * 100;
    //EMERGENCY.temperature_down = analogRead(sensor_temp_down) * (5.0 / 1024.0) * 100;
    EMERGENCY.current = getCorriente(SAMPLESNUMBER);
    EMERGENCY.voltage = analogRead(voltage) * (5.0 / 1024.0);

  }
}
float getCorriente(int samplesNumber)
{
  float voltage;
  float corrienteSum = 0;
  for (int i = 0; i < samplesNumber; i++)
  {
    voltage = analogRead(A0) * 5.0 / 1023.0;
    corrienteSum += (voltage - 2.5) / SENSIBILITY;
  }
  return (corrienteSum / samplesNumber);
}

