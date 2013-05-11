int pwm = 9; //pin 9 is PWM


void setup() {
	TCCR1B = TCCR1B & 0b11111000 | 0x01 ; //set fuse for 31KHz frequency
	pinMode(pwm, OUTPUT);
  	analogWrite(pwm, 128); //second argument is duty cycle out of 255
}

void loop() {

}
