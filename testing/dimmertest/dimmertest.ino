int pwm = 9; //pin 9 is PWM
int control= 5; //pin 5 is optoisoloator switch


void setup() {
    TCCR1B = TCCR1B & 0b11111000 | 0x01 ; //set fuse for 31KHz frequency
    
    pinMode(pwm, OUTPUT);
    pinMode(control, OUTPUT);
    
    analogWrite(pwm, 128); //turn on the HV pwm
    
    
}


void loop() {
  float brightness = 100; //brightness in percent
  //meaningful brightness is 12.5 to 100 in 2x steps
  
  float cycle = 5; //cycle time in milliseconds 
  
  float ontime = brightness / 100 * cycle;
  float offtime = cycle - ontime;
  
  if(brightness > 10) {
    digitalWrite(control, 1);
    delay(ontime);
    }
  
  digitalWrite(control, 0);
  delay(offtime);
  }


