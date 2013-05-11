int pwm = 9; //pin 9 is PWM


void setup() {
    TCCR1B = TCCR1B & 0b11111000 | 0x01 ; //set fuse for 31KHz frequency
    pinMode(pwm, OUTPUT);
}

void HVon(){ //this function turns the HV line on
    digitalWrite(pwm, 128);
}

void HVoff(){ //this functions turns the HV line off
    digitalWrite(pwm, 0);
}

void loop() {
  
int strobetime = 5000; //time of full cycle (microseconds)
int brightness = 9; //brightness steps, 10 is max

HVon(); //power on
delayMicroseconds(brightness * 500); //500 is microseconds per brightness step

HVoff(); //power off
delayMicroseconds( (brightness - 10) * 500);

  }


