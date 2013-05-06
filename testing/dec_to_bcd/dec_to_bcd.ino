//dec-to-bcd
//this outputs a decimal number 0-9 as bc on channels a-d
// d is most significant bit, a is least

int led0 = 13; //led0 is the onboard LED
int bcd_a = 7; //pin 7 is bcd-a
int bcd_b = 6; //pin 6 id bcd-b
int bcd_c = 5; //pin 5 is bcd-c
int bcd_d = 4; //pin 4 id bcd-d

void setup() { //setup function, runs once
    pinMode(led0, OUTPUT);
    pinMode(bcd_a, OUTPUT);
    pinMode(bcd_b, OUTPUT);
    pinMode(bcd_c, OUTPUT);
    pinMode(bcd_d, OUTPUT); //initialize all LED pins as output
}

void dec_to_bcd(int decimal) { //takes an int 0-9 and puts BCD out on corresponding pins

	byte bcdbyte;
	bcdbyte = (byte) decimal; //converts the decimal to a byte

	digitalWrite(bcd_a, bitRead(bcdbyte, 0));
	digitalWrite(bcd_b, bitRead(bcdbyte, 1));
	digitalWrite(bcd_c, bitRead(bcdbyte, 2));
	digitalWrite(bcd_d, bitRead(bcdbyte, 3)); 
	//the above block sets pins bcd a-d as the corresponding value of bcdbyte
}	

void bcd_blank() { //this turns off all bcd output
	digitalWrite(bcd_a, 0);
	digitalWrite(bcd_b, 0);
	digitalWrite(bcd_c, 0);
	digitalWrite(bcd_d, 0);
}

void loop() { //loops inf times

	digitalWrite(led0, 1); //turn on led0
	delay(1000); //wait 1s
	digitalWrite(led0, 0); //turn led0 back off

	int x = 0;
	for( ; x < 10; x++) { //loop through numbers up to 9
		dec_to_bcd(x);
		delay(500); //delay 500ms
	}

	bcd_blank(); // make sure that none of the BCD is outputting

}
