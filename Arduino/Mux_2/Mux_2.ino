#include <Capacitor.h>
const int mux1SelectPins[] = {2, 3, 4, 5};
const int mux1OutPin = A0;               
const int mux2SelectPins[] = {8, 9, 10, 11}; 
const int mux2OutPin = A2;                   
Capacitor capSensorMux1(mux1OutPin, mux2OutPin);

const float Frequency = 0.5;
const unsigned long period = 1000/Frequency;
unsigned long previousM = 0;
const float calibrationFactors[] = {
  10.0 / 3.8 ,
  2.0 / 1.3,    
  5.0 / 2.5, 
  10.0 / 3.8  
};
void sendCap(float capacitance,int channel){
  Serial.print("CC:");
  Serial.print(channel);
  Serial.print(",");
  Serial.println(capacitance);
}
void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 4; i++) {
    pinMode(mux1SelectPins[i], OUTPUT);
    pinMode(mux2SelectPins[i], OUTPUT);
  }
  capSensorMux1.ShowDebug(true);
}
void loop() {
  unsigned long currentM = millis();
  if(currentM - previousM >= period){
     previousM = currentM;
  
  for (int Channel = 0; Channel < 4; Channel++) {
    for (int i = 0; i < 4; i++) {
      digitalWrite(mux1SelectPins[i], bitRead(Channel, i));
      digitalWrite(mux2SelectPins[i], bitRead(Channel, i));
    }
    float capacitanceMux1 = capSensorMux1.Measure();
    int index = Channel;
    float calibratedCapacitance = capacitanceMux1 * calibrationFactors[index];
    sendCap(capacitanceMux1,Channel);
    Serial.print("Channel ");
    Serial.print(Channel);
    Serial.print(": MeasuredCapacitance = ");
    Serial.print(capacitanceMux1, 2);
    Serial.println(" pF");
    Serial.print("| calibrationCapacitance = ");
    Serial.print(calibratedCapacitance, 2);
    Serial.println(" pF");
    Serial.print(" | Frequency = ");
    Serial.print(Frequency);
    Serial.println(" Hz");
  }
  }
}
