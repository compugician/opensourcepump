int motorPin = 5;
int solenoid = 6;
int pPump = A0;
int pBreast = A1;

const int T_HOLD_PRESSURE = 1000;
const int T_HOLD_SOLENOID_OPEN = 600;
const int T_MAX_PUMP_ON = 2000;
const int P_THRESHOLD = 200;

void setup() 
{ 
  pinMode(motorPin, OUTPUT);
  pinMode(solenoid, OUTPUT);
  Serial.begin(9600);
  while (! Serial);
  //Serial.println("Speed 0 to 255");

} 

int digitalReadOutputPin(uint8_t pin)
{
 uint8_t bit = digitalPinToBitMask(pin);
 uint8_t port = digitalPinToPort(pin);
 if (port == NOT_A_PIN) 
   return LOW;

 return (*portOutputRegister(port) & bit) ? HIGH : LOW;
}

void printStatus() {
  Serial.print(millis());
  Serial.print(" ");  
  Serial.print(digitalReadOutputPin(motorPin));
  Serial.print(" ");
  Serial.print(digitalReadOutputPin(solenoid));
  Serial.print(" ");
  Serial.print(analogRead(pPump));
  Serial.print(" ");
  Serial.println(analogRead(pBreast));  

}

void loop() 
{ //digitalWrite(solenoid, LOW);

  digitalWrite(motorPin, HIGH);

  //Serial.print(analogRead(pPump));
  //Serial.print(" ");

  int minPressure = 1023;

  long startTime = millis();
  int runTime = 0;
  while((runTime = (millis()-startTime))<T_MAX_PUMP_ON && analogRead(pPump)>P_THRESHOLD)
  {
//    int p = analogRead(pPump);
//    if (p<minPressure) { 
//      minPressure = p; 
//    }
    printStatus();
  }
  analogWrite(motorPin, LOW); //turn off motor

  //hold the pressure
  startTime = millis();
  while(millis()-startTime<T_HOLD_PRESSURE) {
    printStatus();
  }
 
  //open the solenoid
  digitalWrite(solenoid, HIGH);
  // hold solenoid open
  startTime = millis();
  while(millis()-startTime<T_HOLD_SOLENOID_OPEN) {
    printStatus();
  }
  digitalWrite(solenoid, LOW);




}

