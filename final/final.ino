/*

switch pin 23, internal pull up, set ground
LDR A0 - pin VP, 3.3v 10k, 

*/

#include <WiFi.h>
#include <PubSubClient.h>
 
const char* ssid = "Fed Geek"; // Enter your WiFi name
const char* password =  "11112222"; // Enter WiFi password
const char* mqttServer = "soldier.cloudmqtt.com";
const int mqttPort = 12729;
const char* mqttUser = "ciyajbsc";
const char* mqttPassword = "je1T5MADj3Wi";

WiFiClient espClient;
PubSubClient client(espClient);


int pushButton = 23; //bulb on button pull up
int bulbPin = 2;
int bulbOnTime = 10; // on duration in second
int boundVal=50;  // for ldr
#define ldrPin A0

// sending data
String bState;
String bDuration;


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


 // mqtt server callback
void callback(char* topic, byte* payload, unsigned int length) 
{
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) 
  {
    Serial.print((char)payload[i]);
  }
 
  Serial.println();
  Serial.println("-----------------------");
 
}
 
 
void setup() 
{
 
  Serial.begin(9600);
 
  timer = timerBegin(0, 80, true);  // timer, prescaller, count up
  timerAttachInterrupt(timer, &onTimer, true); // true for interrupt genarate by eadge type
  timerAlarmWrite(timer, 1000000, true); // one second 1000 000 , true for restart
  timerAlarmEnable(timer);
  
  pinMode(pushButton, INPUT_PULLUP); // for switch
  pinMode(bulbPin, OUTPUT); // for bulb
 
  // wifi setup
  WiFi.begin(ssid, password);
 
  if (WiFi.status() != WL_CONNECTED) 
  {
    delay(100);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
 
}
 
void loop() 
{

	// try to connect server
	if (!client.connected()) 
	{
		Serial.println("Connecting to MQTT...");
		if(client.connect("ESP8266Client", mqttUser, mqttPassword )) 
		{
			Serial.println("connected");  
		} else
		{
			Serial.print("failed with state ");
			Serial.print(client.state());
			delay(500);
 
		}
  }


 
 
  int buttonState = digitalRead(pushButton);
  // when press on switch
  if(!buttonState)
  {
	totalTimeCounter=0;
	digitalWrite(bulbPin, HIGH);
	delay(500);
	initLdrVal = getLdrValue(); // get sensor values
	Serial.print("Bulb on Init LDR value :- ");
	Serial.println(initLdrVal);
	
	bState = "On";
  
  }
  
  
  if(totalTimeCounter> bulbOnTime)
  {
	digitalWrite(bulbPin, LOW);
	bState = "Off";
  
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
  
  
  // data send
  client.loop();
  client.publish("HomeData", "hello"); //Topic name
  delay(500);
  
 // timer
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
