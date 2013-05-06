/*
  Super Blinkey
  this is a test to mess with pin 13 LED
  */


int led = 13; //this initiates pin 13 as "led"

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);     
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(led, HIGH);   // LED on
  delay(100);               // wait 100ms
  digitalWrite(led, LOW);    // LED off
  delay(100);               // wait 100ms
}
