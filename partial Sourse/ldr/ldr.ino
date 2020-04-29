

int initLdrVal=0;
int boundVal=50;

int getLdrValue()
{  
  int sensorValue=0;
  for(int i=0; i<10; i++)
  {
    sensorValue += analogRead(A0);
    delay(100); 
  }
  return sensorValue/10;
}


void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  //delay(1000);
  
  initLdrVal = getLdrValue();
  Serial.println(initLdrVal);

  
}
 

void loop() {

  
  
  
  int checkVal = getLdrValue();
  
  if((initLdrVal+boundVal)>checkVal and (initLdrVal-boundVal)<checkVal)
  {
    Serial.println("in range");
    Serial.println(checkVal);
  }else
  {
    Serial.println("out of range");
    Serial.println(checkVal);
  }
  
  
}
      
