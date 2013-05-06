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

void loop() { //loops inf times
	delay(500);
	digitalWrite(led0, 1); //turn onboard LED on
	delay(1000); //wait 1s
	digitalWrite(led0, 0); //turn onboard LED off
	delay(500);

	digitalWrite(bcd_a, 1);
	delay(500); 
	digitalWrite(bcd_a, 0); //bcd-a on for 500ms, then off

	digitalWrite(bcd_b, 1);
	delay(500);
	digitalWrite(bcd_b, 0); //bcd-b on for 500ms

	digitalWrite(bcd_c, 1);
	delay(500);
	digitalWrite(bcd_c, 0); //bcd-c on for 500ms

	digitalWrite(bcd_d, 1);
	delay(500);
	digitalWrite(bcd_d, 0); //bcd-d on for 500ms
}
