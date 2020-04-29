

int pushButton = 23; //bulb on button pull up
int bulbPin = 2;
int bulbOnTime = 10;
int boundVal=50;  // for ldr
#define ldrPin A0


int initLdrVal=0;
volatile int interruptCounter;
int totalTimeCounter;

int getLdrValue()
{  
  int sensorValue=0;
  for(int i=0; i<10; i++)
  {
    sensorValue += analogRead(ldrPin);
    delay(100); 
  }
  return sensorValue/10;
}
 
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
 
void IRAM_ATTR onTimer() 
{
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);
 
}
 
void setup() {
 
  Serial.begin(9600);
 
  timer = timerBegin(0, 80, true);  // timer, prescaller, count up
  timerAttachInterrupt(timer, &onTimer, true); // true for interrupt genarate by eadge type
  timerAlarmWrite(timer, 1000000, true); // one second 1000 000 , true for restart
  timerAlarmEnable(timer);
  
  pinMode(pushButton, INPUT_PULLUP); // for switch
  pinMode(bulbPin, OUTPUT); // for bulb
 
}
 
void loop() {
 
 
  int buttonState = digitalRead(pushButton);
  if(!buttonState)
  {
	totalTimeCounter=0;
	digitalWrite(bulbPin, HIGH);
	delay(500);
	initLdrVal = getLdrValue();
	Serial.print("Bulb on Init LDR value :- ");
	Serial.println(initLdrVal);
  
  }
  
  
  if(totalTimeCounter> bulbOnTime)
  {
	digitalWrite(bulbPin, LOW);
  
  }
  
  
  
  
  int checkVal = getLdrValue();
  if((initLdrVal+boundVal)>checkVal and (initLdrVal-boundVal)<checkVal)
  {
    Serial.println("Not human activity");
    //Serial.println(checkVal);
  }else
  {
    Serial.print("Human activity detected, LDR val :- ");
    Serial.println(checkVal);
	totalTimeCounter=0; // zero counter
  }
  
  
  
  
 
  if (interruptCounter > 0) 
  {
 
    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);
 
    totalTimeCounter++;
 
    Serial.print("Timer Count : ");
    Serial.println(totalTimeCounter);
 
  }
}
