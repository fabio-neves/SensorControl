#include <Arduino.h>

#define Vin 5.0 
#define T0 298.15 // define constante igual a 298.15 Kelvin 
#define Rt 10000 // Resistor do divisor de tensao 
#define R0 10000 // Valor da resistencia inicial do NTC 
#define T1 273.15 // [K] in datasheet 0º C 
#define T2 373.15 // [K] in datasheet 100° C 
#define RT1 35563 // [ohms] resistencia in T1 
#define RT2 549 // [ohms] resistencia in T2 
float beta = 0.0; // parametros iniciais [K]
float Rinf = 0.0; // parametros iniciais [ohm]
float Vout;

void WaitForData();
float ReadVoltageFromSensor(int pin);
float CalculateRout(float voltage);
float CalculateAbsoluteTemperature(float voltage);
float CalculateCelsius(float voltage);
void ReadData(String &mensagem);

void setup() {
  Serial.begin(115200);
  beta = (log(RT1 / RT2)) / ((1 / T1) - (1 / T2)); // calculo de beta
  Rinf = R0 * exp(-beta / T0); // calculo de Rinf
  delay(100); 
}

void loop() {
  WaitForData();

  String cmd;
  
  ReadData(cmd);

  //Serial.println(cmd);

  cmd.trim();

  if(cmd == "TMP") {
    Vout = ReadVoltageFromSensor(0); 
    Serial.println(CalculateCelsius(Vout)); 
  } else if(cmd == "LUZ") {
    Vout = ReadVoltageFromSensor(1); 
    Serial.println(Vout); 
  } else {
    Serial.println("ERR:" + cmd);
  }

  delay(500); 
}

void WaitForData() {
  while (!(Serial.available() > 0)) {}
}

void ReadData(String &mensagem) {
  mensagem = Serial.readStringUntil('\n');
}

float ReadVoltageFromSensor(int pin) {
  return Vin * ((float)(analogRead(pin)) / 1024.0);
}

float CalculateRout(float voltage) {
  return (Rt * voltage / (Vin - voltage));
}

float CalculateAbsoluteTemperature(float voltage) {
  float Rout = CalculateRout(voltage);
  return (beta / log(Rout / Rinf));
}

float CalculateCelsius(float voltage) {
  float k = CalculateAbsoluteTemperature(voltage);
  return k - 273.15;
}