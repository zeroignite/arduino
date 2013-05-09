int pwm = 9; //pin 9 is PWM


void setup() {
	TCCR1B = TCCR1B & 0b11111000 | 0x01 ;
	pinMode(pwm, OUTPUT);
  	analogWrite(pwm, 192);
}

void loop() {

}
