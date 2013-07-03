//muxtest
//this outputs a decimal number 0-9 as bc on channels a-d
// d is most significant bit, a is least

int led0 = 13; //led0 is the onboard LED

int bcd_a = 2; //bcd-as
int bcd_b = 3; //bcd-b
int bcd_c = 4; //bcd-c
int bcd_d = 5; //bcd-d

int H10 = 7; //ten hours mux switch
int H00 = 8; //hours mux switch
int M10 = 11; //ten minutes mux switch
int M00 = 12; //minutes mux switch

int testpin = 6;

void setup() { //setup function, runs once
    pinMode(led0, OUTPUT);
    pinMode(bcd_a, OUTPUT);
    pinMode(bcd_b, OUTPUT);
    pinMode(bcd_c, OUTPUT);
    pinMode(bcd_d, OUTPUT); //initialize all LED pins as output
    
    pinMode(H10, OUTPUT);
    pinMode(H00, OUTPUT);
    pinMode(M10, OUTPUT);
    pinMode(M00, OUTPUT); //mux control pins are output
}

//
//since LEDs are common-cathode in the breadboarded circuit, 
//set an output LOW to light the LED
//

void dec_to_bcd(int decimal) { //takes an int 0-9 and puts BCD out on corresponding pins

	byte bcdbyte;
	bcdbyte = (byte) decimal; //converts the decimal to a byte

	digitalWrite(bcd_a, 1^bitRead(bcdbyte, 0));
	digitalWrite(bcd_b, 1^bitRead(bcdbyte, 1));
	digitalWrite(bcd_c, 1^bitRead(bcdbyte, 2));
	digitalWrite(bcd_d, 1^bitRead(bcdbyte, 3)); 
	//the above block sets pins bcd a-d as the corresponding value of bcdbyte
        //the 1^ inverts the bit, needed since outputs go low
}	

void bcd_blank() { //this turns off all bcd output
	digitalWrite(bcd_a, 1);
	digitalWrite(bcd_b, 1);
	digitalWrite(bcd_c, 1);
	digitalWrite(bcd_d, 1);
}

void loop() { //loops inf times

	//digitalWrite(led0, 1); //turn on led0
	//delay(1000); //wait 1s
	//digitalWrite(led0, 0); //turn led0 back off

	int hours10 = 1;
        int hours00 = 2;
        int minutes10 = 3;
        int minutes00 = 4; //this'll feed into the display
        
        //time to actually run the display.
        //display the right digit to each bank in series
        //order is H10, H00, M10, M00
        
        int ontime = 5; //time per bank in milliseconds
        
        digitalWrite(H10, 1); //turn on the H10 bank
        dec_to_bcd(hours10); //send to correct binary to the 
        delay(ontime); 
        bcd_blank(); // make sure that none of the BCD is outputting
        digitalWrite(H10, 0); //turn off the H10 bank
        
        digitalWrite(H00, 1); //turn on the H00 bank
        dec_to_bcd(hours00); //send to correct binary to the bank
        delay(ontime); 
        bcd_blank(); // make sure that none of the BCD is outputting
        digitalWrite(H00, 0); //turn off the H10 bank
        
        digitalWrite(M10, 1); //turn on the M10 bank
        dec_to_bcd(minutes10); //send to correct binary to the bank
        delay(ontime); 
        bcd_blank(); // make sure that none of the BCD is outputting
        digitalWrite(M10, 0); //turn off the M10 bank
        
        digitalWrite(M00, 1); //turn on the M00 bank
        dec_to_bcd(minutes00); //send to correct binary to the bank
        delay(ontime); 
        bcd_blank(); // make sure that none of the BCD is outputting
        digitalWrite(M00, 0); //turn off the M00 bank

        digitalWrite(testpin, 0);
}
