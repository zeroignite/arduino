// nixiecode
// "Zeroignite" Tenzin Beck, 2013
// tzbeck@gmail.com
//
//To the extent possible under law, the author has dedicated all 
//copyright and related and neighboring rights to this software to the 
//public domain worldwide. This software is distributed without any warranty.
//
//Full legal information can be found at 
// http://creativecommons.org/publicdomain/zero/1.0/


//=============================

#include <EEPROM.h> //needed to write to nonvolatile memory

//=============================
// handy macros
//=============================

#define BRIGHTMAX 80 // maxiumum brightness in percent
#define BRIGHTMIN 0.625 // brightness in percent considered "off"

#define ASCII_0 48 // ascii 0 is used to convert chars to decimal ints

//=============================
// initialize pin numbers
//=============================

int led0 = 13; // pin 13 is arduino onborad LED

int pwm = 10; // PWM pin, controls boost FET

int bcd_a = 2; // bcd pins --- a is least significant bit, d is most
int bcd_b = 3;
int bcd_c = 4; 
int bcd_d = 5; 

int h10mux = 7; // mux pins go high to turn on respective tubes
int h00mux = 8;
int m10mux = 11;
int m00mux = 12;

int brightup = A0; // input pins, UI
int brightdown = A1;
int hourplus = A2;
int hourminus = A3; 

// Vin takes 12v DC
// RX and TX pins connect to GPS module, Serial1

//=============================
// initialize mux/dimming variables
//=============================

float brightness = BRIGHTMAX; // brightness in percent
// testing indicates brightness range is <100 to >0.5

float cycle = 2000; // time per numeral in microseconds

//=============================
// initialize time variables
//=============================

byte hours10 = 0; // hours-10 value
byte hours00 = 0; // hours value
byte minutes10 = 0; // minutes-10 value
byte minutes00 = 0; // minutes value

int timezone; // timezone offset

byte timezone_saved; //timezone stored in EEPROM

//==============================
// initialize nmea input/parse stuff
//==============================

char want_kind[] = "$GPRMC,"; // the sentence we want is prefixed by $GPRMC,

byte want_idx = 0; // want_idx is index of what we're checking in Serial1 buffer

byte raw_time[4]; // array of raw time string

//==============================
// setup function, runs once
//==============================
void setup(){

    pinMode(led0, OUTPUT); //onboard LED is useful for debug

    pinMode(pwm, OUTPUT); // pwm is output

    pinMode(bcd_a, OUTPUT); // all bcd pins as output
    pinMode(bcd_b, OUTPUT);
    pinMode(bcd_c, OUTPUT);
    pinMode(bcd_d, OUTPUT); 

    pinMode(h10mux, OUTPUT); // all mux pins are output
    pinMode(h00mux, OUTPUT);
    pinMode(m10mux, OUTPUT);
    pinMode(m00mux, OUTPUT); 

    pinMode(brightup, INPUT_PULLUP); // UI pins are input, pulled high
    pinMode(brightdown, INPUT_PULLUP);
    pinMode(hourplus, INPUT_PULLUP);
    pinMode(hourminus, INPUT_PULLUP); 

    TCCR1B = TCCR1B & 0b11111000 | 0x01 ; // set fuse for 31KHz frequency
    
    analogWrite(pwm, 168); // turn on the FET pwm

    Serial1.begin(9600); // opens Serial1 port, sets data rate to 9600 bps

    timezone_saved = EEPROM.read(0); //pull saved timezone out of EEPROM
    timezone = timezone_saved - 12; //converts 24 saved timezone to 12h timezone
}

//==============================
// grab_nmea looks for the desired sentence and sticks the raw time into array
//==============================
void grab_nmea() {

    while(Serial1.available()) {

        if (Serial1.read() == want_kind[want_idx]) { // if idx = 0, want_kind[idx] = $
            want_idx++; // if we get $, we go on, looking for G, etc...
        }

        else { // the sentence we are going to get is NOT what we want
            want_idx = 0; // reset index
        }
   
        if (want_idx == sizeof(want_kind)-1 && Serial1.available() > 4) { 
        // we have the sentence we want, and there' enough serial left to get the time
            want_idx = 0; // reset index

            for (byte i = 0; i < 4;) {
                raw_time[i] = Serial1.read() - ASCII_0; // fill raw_time array with next 4 chars,
                //subtracting ASCII_0 converts chars to ints
                i++;
            }
        }         
    }
}


//===========================
// dec-to-bcd function - for outputting BCD directly
//===========================
void dec_to_bcd(int decimal) { // takes an int 0-9 and puts binary out on corresponding pins

    byte bcdbyte;
    bcdbyte = (byte) decimal; // converts the decimal to a byte

    digitalWrite(bcd_a, bitRead(bcdbyte, 0));
    digitalWrite(bcd_b, bitRead(bcdbyte, 1));
    digitalWrite(bcd_c, bitRead(bcdbyte, 2));
    digitalWrite(bcd_d, bitRead(bcdbyte, 3)); 
    // the above block sets pins bcd a-d as the corresponding value of bcdbyte
}


//==================================
// main loop, repeats inf times
//==================================
void loop(){

    grab_nmea(); // grab incoming nmea GPRMC sentence

    //===============================
    // pull individual times out of the raw_time array
    //===============================

    int workinghours = raw_time[0] * 10 + raw_time[1]; //adds hours from raw_time array to get 24h

    workinghours = workinghours + timezone; // adjsut for timezone

    if (workinghours < 0) { //avoid negative time, rollover to before midnight
        workinghours = workinghours + 24;
    }
    else if (workinghours > 23) { //keep hours in range 0-23
        workinghours = workinghours - 24; 
    }

    hours10 = workinghours / 10; // get the tens value out of working hours, int division
    hours00 = workinghours % 10; // get the ones value out of working hours

    minutes10 = raw_time[2]; 
    minutes00 = raw_time[3];

    // ------------------------------------
    // fallback testing -- hardcode numeral values 
    // hours10 = 1;
    // hours00 = 2;
    // minutes10 = 3;
    // minutes00 = 4;
    // end hardcode test block
    // ------------------------------------

    //================================
    // display the numerals!
    //================================

    float ontime = brightness / 100 * cycle; 
    float offtime = cycle - ontime; // calculate mux times from brightness
    // don't feed delayMicroseconds() 0 --- results in 17ms delay --- arduino bug

    for (byte x = 1; x < 20; x++) {

        if (brightness > BRIGHTMIN) { //only display if display is on

            dec_to_bcd(hours10); // send out binary of hours10 value on pins a-d
            digitalWrite(h10mux, 1); // turn on h10 tube
            delayMicroseconds(ontime); // hold tube on for ontime us
            digitalWrite(h10mux, 0); // turn off tube h10
            delayMicroseconds(offtime); // wait offtime us before moving to next tube

            dec_to_bcd(hours00); // same code as above block for each tube -- rinse, repeat
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

    //=====================================
    // poll brightness buttons for input
    //=====================================

    if (digitalRead(brightup) == 0){
        brightness = brightness * 2; //each brightness step is a factor of two duty cycle
    }

    if (digitalRead(brightdown) == 0){
        brightness = brightness / 2;
    }

    if (brightness > BRIGHTMAX){
        brightness = BRIGHTMAX; //cap upperlower bound on brightness
    }
    
    if (brightness < BRIGHTMIN){
        brightness = BRIGHTMIN; //cap lower bound on brightness
    }

    //====================================
    //poll timzeone buttons for input
    //====================================

    if (digitalRead(hourplus) == 0){ //adjust timezone via UI buttons
        timezone = timezone + 1;
    }

    if (digitalRead(hourminus) == 0){
        timezone = timezone - 1;
    }

    if (timezone == 13){ //timezone rollover
        timezone = -12;
    }

    if (timezone == -13){
        timezone = 12;
    }  

    if (timezone_saved - 12 != timezone){ //check to see if timezone has changed from save
        EEPROM.write(0, timezone + 12); //if yes, convert to 24h offset and write
    } 

}