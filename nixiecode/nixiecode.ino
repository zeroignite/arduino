// nixiecode
// "Zeroignite" Tenzin Beck, 2013
// released into the public domain


//=============================
// handy macros
//=============================

#define BRIGHTMAX 80 // maxiumum brightness in percent
#define BRIGHTMIN 0.625 // brightness in percent considered "off"

#define CR ('\r')
#define LF ('\n')

#define ASCII_BOTTOM 0x20 // Lower end of ASCII printable (space)

#define ASCII_0 48 // ascii 0 is used to convert chars to decimal ints

//=============================
// initialize pin numbers
//=============================

int led0 = 13; // pin 13 is arduino onborad LED

int pwm = 9; // 9 is PWM pin, controls boost FET

int bcd_a = 2; // bcd pins --- a is least significant bit, d is most
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
int hourminus = A3; // input pins, UI

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

int hours10 = 0; // hours-10 value
int hours00 = 0; // hours value
int minutes10 = 0; // minutes-10 value
int minutes00 = 0; // minutes value

int timezone = 0; // timezone offset

//==============================
// initialize nmea input/parse stuff
//==============================

char want_kind[] = "$GPRMC,"; // the sentence we want is prefixed by $GPRMC,

int want_idx = 0; // want_idx is index of what we're checking in Serial1 buffer

int raw_time[4]; // array of raw time string

//==============================
// grab_nmea looks for the desired sentence and sticks the raw time into array
//==============================
void grab_nmea() {

    Serial.print("in grab_nmea");

    if (Serial1.available()) {
        Serial.print(Serial1.peek());

        if (Serial1.read() == want_kind[want_idx]) { // if idx = 0, want_kind[idx] = $
            want_idx++; // if we get $, we go on, looking for G, etc...
        }

        else { // the sentence we are going to get is NOT what we want

            want_idx = 0; // reset index
            while (Serial1.read() > ASCII_BOTTOM);
            // eat anything until we get less than the lower end of ASCII's printable region
        }
       
        if (want_idx == sizeof(want_kind)-1) { // we have the sentence we want
            want_idx = 0; // reset index
            Serial.print("----");

            for (int i; i < 4; i++) {
                raw_time[i] = Serial1.read() - ASCII_0; // fill array with next 4 chars, rawtime
            }            
        }
    }
}

//==============================
// setup function, runs once
//==============================
void setup(){

    pinMode(led0, OUTPUT); 

    pinMode(pwm, OUTPUT); // pwm is output

    pinMode(bcd_a, OUTPUT);
    pinMode(bcd_b, OUTPUT);
    pinMode(bcd_c, OUTPUT);
    pinMode(bcd_d, OUTPUT); // all bcd pins as output

    pinMode(h10mux, OUTPUT);
    pinMode(h00mux, OUTPUT);
    pinMode(m10mux, OUTPUT);
    pinMode(m00mux, OUTPUT); // all mux pins are output

    pinMode(brightup, INPUT_PULLUP);
    pinMode(brightdown, INPUT_PULLUP);
    pinMode(hourplus, INPUT_PULLUP);
    pinMode(hourminus, INPUT_PULLUP); // UI pins are input, pulled high


    TCCR1B = TCCR1B & 0b11111000 | 0x01 ; // set fuse for 31KHz frequency
    
    analogWrite(pwm, 168); // turn on the FET pwm

    Serial1.begin(9600); // opens Serial1 port, sets data rate to 9600 bps

    Serial.begin(9600);
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

    int workinghours = raw_time[0] * 10 + raw_time[1];
    // the above line creates an int by pulling out the hours chars from array,
    // turning them into ints, and adding them to produce 24H int. 

    workinghours = workinghours + timezone; // adjsut for timezone

    hours10 = workinghours % 10; // get the tens value out of working hours
    hours00 = workinghours / 10; // get the ones value out of working hours, int division

    minutes10 = raw_time[2]; 
    minutes00 = raw_time[3];

    // ------------------------------------
    // testing -- hardcode numeral values 
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
    // don't feed delayMicroseconds() 0 --- results in 17ms delay ---arduino bug

    for (int x = 1; x < 20; x++) {

        if (brightness > 1) { // only display if brightness >10, i.e display is on

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

    if (digitalRead(brightup) == 0)
        brightness = brightness * 2;

    if (digitalRead(brightdown) == 0)
        brightness = brightness / 2;

    if (brightness > BRIGHTMAX)
        brightness = BRIGHTMAX;

    if (brightness < BRIGHTMIN)
        brightness = BRIGHTMIN;
}