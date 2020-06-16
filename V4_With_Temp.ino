#define PWM_PIN 9
#define ON_OFF_PIN 3
#define TEMP_TARGET_PIN 2
#define TEMP_TOLERANCE 5
#define TEMP_TARGET_HIGH 85
#define TEMP_TARGET_LOW 65
#include <Servo.h>

Servo myservo;  // create servo object to control the speed controller (which changes the main output voltage)
byte pulse = 93; //The PWM pulse
//This array converts a pulse into a output voltage
//Since the pulse starts at 93, there is 93 offset to find the right value
//So if you want to find out the voltage of a pulse of 100 -> PULSETOVOLTAGE[100-93]
float const PULSETOVOLTAGE[] = {0.0f,3.1f,3.32f,3.7f,4.0f,4.3f,4.6f,4.95f,5.23f,5.52f,5.85f,6.2f,6.45f,6.77f,7.1f,7.4f,7.65f,7.95f,8.3f,8.45f,8.83f,9.1f,9.35f,9.65f,9.9f,10.2f,10.7f,10.9f,11.1f,11.35f,11.55f,11.7f,11.9f,12.0f,12.1f,12.15f,12.2f};

bool stringComplete = false;  // Is used for the Serial Comunication
String inputString = "";         // a String to hold incoming data


int TEMP_TARGET = TEMP_TARGET_LOW; //The target temp we want to reach
int sensorPin = A0;  //The pin where we have the NTC
int bitwertNTC = 0;
long widerstand1=100000;                   //Ohm
int bWert =3950;                           // B- Wert vom NTC
double widerstandNTC =0;
double kelvintemp = 273.15;                // 0°Celsius in Kelvin
double Tn=kelvintemp + 25;                 //Nenntemperatur in Kelvin
double TKelvin = 0;                        //Die errechnete Isttemperatur
//Messured Temp
double T = 0;                              
double deltaT = 0; //Delta to the target

bool isHeating = false; //True when trying to reach the target

void setup() {
  
  Serial.begin(9600);
  myservo.attach(PWM_PIN);  // attaches the servo on pin 9 to the servo object
  attachInterrupt(digitalPinToInterrupt(ON_OFF_PIN), onOFF, RISING); //Interrup onOFF
  attachInterrupt(digitalPinToInterrupt(TEMP_TARGET_PIN), targetChange, RISING); //Interrupt targetChange
  pinMode(LED_BUILTIN, OUTPUT); //Is high when TEMP_TARGET == TEMP_TARGET_HIGH
  digitalWrite(LED_BUILTIN, LOW);
}


void loop() {
  //This is mostly for debugging
  /*
  if (stringComplete) {
    pulse = 0;
    for(int j = 0;j<3;j++){
        pulse += ((inputString[j]) - 48) * switchYBoi(j);
     }
     
     inputString = "";
     stringComplete = false;
  }
  */

  
  getTemp();
  if(isHeating){
    if(deltaT > 5){
      pulse = 129;
    }else if(deltaT >= 0){
      pulse = 110;
    }else{
      pulse = 93;
    }
  }else{
    pulse = 93; //Turning off
  }
  
  myservo.write(pulse); 
  
  Serial.print("Voltage :\t");             
  Serial.print(PULSETOVOLTAGE[pulse - 93]);
  Serial.print("V\tTemp :\t");
  Serial.print(T);
  Serial.print("/");
  Serial.print(TEMP_TARGET);
  Serial.print("C°\tHeating :\t");
  Serial.println(isHeating);
  
  delay(1000);
}

double getTemp(){
  bitwertNTC = analogRead(sensorPin);      // lese Analogwert an A0 aus
  widerstandNTC = widerstand1*(((double)bitwertNTC/1024)/(1-((double)bitwertNTC/1024)));

                                           // berechne den Widerstandswert vom NTC
  TKelvin = 1/((1/Tn)+((double)1/bWert)*log((double)widerstandNTC/widerstand1));

  T = TKelvin-kelvintemp;                    // ermittle die Temperatur in °C
  deltaT = TEMP_TARGET - T;
  return T;
}

void onOFF(){
  isHeating = !isHeating;
  delay(100);
}

void targetChange(){
  if(TEMP_TARGET == TEMP_TARGET_HIGH){
    TEMP_TARGET = TEMP_TARGET_LOW;
    digitalWrite(LED_BUILTIN, LOW);
  }else{
    TEMP_TARGET = TEMP_TARGET_HIGH;
    digitalWrite(LED_BUILTIN, HIGH);
  }
  delay(100);
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

int switchYBoi(int j){
  switch (j)
  {
    case 0:
      return 100;
    case 1:
      return 10;
    case 2:
      return 1;
  }
  return 0;
}
