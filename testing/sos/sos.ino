// super SOS - this repeats an SOS indefinitely

int LED  = 13; //initiates pin 13 as "LED"

void setup() { //setup function
	pinMode(LED, OUTPUT); //set LED as output pin
}

void s_blink() { //blink morse "s"
	for(byte x = 0; x < 3; x++ ) { //loop three times
		digitalWrite(LED, HIGH); 
		delay(200); //on for 200ms
		digitalWrite(LED, LOW); 
		delay(100); //off for 100ms
	}
}

void o_blink() { //blink morse "o"
	for (byte x = 0; x < 3; x++) { //loop three times
		digitalWrite(LED, 1); 
		delay(400); //on for 400ms
		digitalWrite(LED, LOW); 
		delay(100); //off for 100ms
	}
}

void loop() { //main loop function
s_blink(); //blink morse "s"
delay(800); //wait 800ms between characters
o_blink(); //blink morse "o"
delay(800); //wait 800ms between characters
s_blink(); //blink morse "s" again
delay(1500); //wait 1.5s before SOSing again

}
