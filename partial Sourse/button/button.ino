


int pushButton = 23;


void setup() {
 
  Serial.begin(9600);
  
  pinMode(pushButton, INPUT_PULLUP);
}


void loop() {
  
  int buttonState = digitalRead(pushButton);
  
  Serial.println(buttonState);
  delay(10);        // delay in between reads for stability
}
