//nixiecode
//"Zeroignite" Tenzin Beck, 2013
//Licsensed under the "do whatever you want" license


//=============================
//initialize pin numbers
//=============================

int led0 = 13; //pin 13 is arduino onborad LED

int pwm = 9; //9 is PWM pin, controls boost FET

int bcd_a = 2; //bcd pins --- a is least significant bit, d is most
int bcd_b = 3;
int bcd_c = 4; 
int bcd_d = 5; 

int h10mux = 7; // mux pins go high to turn on respective tubes
int h00mux = 8;
int m10mux = 11;
int m00mux = 12;

int brightup = A0;
int brightdown = A1;
int hourplus = A2;
int hourminus = A3; //input pins, UI

//Vin takes 12v DC
//RX and TX pins connect to GPS module, serial

//=======================
//initialize mux/dimming variables
//=========================

float brightness = 80; //brightness in percent
//testing indicates brightness range is <100 to >0.5

float cycle = 4000; //time per numeral in microseconds

//==============================
//setup function, runs once
//==============================
void setup(){

	pinMode(led0, OUTPUT); 

	pinMode(pwm, OUTPUT); //pwm is output

    pinMode(bcd_a, OUTPUT);
    pinMode(bcd_b, OUTPUT);
    pinMode(bcd_c, OUTPUT);
    pinMode(bcd_d, OUTPUT); // all bcd pins as output

    pinMode(h10mux, OUTPUT);
    pinMode(h00mux, OUTPUT);
    pinMode(m10mux, OUTPUT);
    pinMode(m00mux, OUTPUT); //all mux pins are output

    pinMode(brightup, INPUT_PULLUP);
    pinMode(brightdown, INPUT_PULLUP);
    pinMode(hourplus, INPUT_PULLUP);
    pinMode(hourminus, INPUT_PULLUP); //UI pins are input, pulled high


    TCCR1B = TCCR1B & 0b11111000 | 0x01 ; //set fuse for 31KHz frequency
    
    analogWrite(pwm, 168); //turn on the FET pwm
}


//===========================
//dec-to-bcd function - for outputting BCD directly
//===========================
void dec_to_bcd(int decimal) { //takes an int 0-9 and puts binary out on corresponding pins

	byte bcdbyte;
	bcdbyte = (byte) decimal; //converts the decimal to a byte

	digitalWrite(bcd_a, bitRead(bcdbyte, 0));
	digitalWrite(bcd_b, bitRead(bcdbyte, 1));
	digitalWrite(bcd_c, bitRead(bcdbyte, 2));
	digitalWrite(bcd_d, bitRead(bcdbyte, 3)); 
	//the above block sets pins bcd a-d as the corresponding value of bcdbyte
}


//==================================
//main loop, repeats inf times
//==================================
void loop(){

	int hours10; //hours-10 value
	int hours00; //hours value
	int minutes10; //minutes-10 value
	int minutes00; //minutes value

	//testing -- hardcode numeral values 
	hours10 = 1;
	hours00 = 2;
	minutes10 = 3;
	minutes00 = 4;
	//end hardcode test block


	//================================
	//display the numerals!
	//================================

	float ontime = brightness / 100 * cycle; 
	float offtime = cycle - ontime; //calculate mux times from brightness
	//don't feed delayMicroseconds() 0 --- results in 17ms delay ---arduino bug
	
	int x;

	for( x=1; x < 100; x++){

		if(brightness > 0){ //only display if brightness >10, i.e display is on

			dec_to_bcd(hours10); //send out binary of hours10 value on pins a-d
			digitalWrite(h10mux, 1); // turn on h10 tube
			delayMicroseconds(ontime); //hold tube on for ontime us
			digitalWrite(h10mux, 0); //turn off tube h10
			delayMicroseconds(offtime); //wait offtime us before moving to next tube

			dec_to_bcd(hours00); //same code as above block for each tube -- rinse, repeat
			digitalWrite(h00mux, 1); 
			delayMicroseconds(ontime);
			digitalWrite(h00mux, 0);
			delayMicroseconds(offtime);

			dec_to_bcd(minutes10);
			digitalWrite(m10mux, 1);
			delayMicroseconds(ontime);
			digitalWrite(m10mux, 0);
			delayMicroseconds(offtime);

			dec_to_bcd(minutes00);
			digitalWrite(m00mux, 1);
			delayMicroseconds(ontime);
			digitalWrite(m00mux, 0);
			delayMicroseconds(offtime);
		}	
	}

}