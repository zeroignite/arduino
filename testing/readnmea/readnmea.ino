//For more info please visit http://www.electronicsblog.net
// Arduino GPS clock

unsigned char incomingByte = 0;

unsigned int c=0;

unsigned int hour;
unsigned int minute;
unsigned int second;
  
unsigned char RMC_id[7]={
  '$','G','P','R','M','C',','};
  
unsigned char buffer[100];  

//------

void show_time(void) { //read current time from buffer
  hour = (((int)buffer[0]&0x0F)*10)+((int)buffer[1]&0x0F); 
  minute = (((int)buffer[2]&0x0F)*10)+((int)buffer[3]&0x0F); 
  second  = (((int)buffer[4]&0x0F)*10)+((int)buffer[5]&0x0F); 
}
  
 //------

void fill_buffer(void) { 
  while (Serial.available()) {
    
    buffer[c++] = Serial.read();
    
    if (buffer [c] == 0x0D) {
      Serial.flush();
    }
  }
}

//------

void setup()  { //arduino setup
    Serial.begin(9600);
  }

//------

void loop() { //main arduino loop, repeats
  
  c = 0;

  int valid_RMC = 7;

  boolean valid_msg=false;
    
  if (Serial.available() > 20) {

    for (int i=0;i<7;i++) {    
      incomingByte =Serial.read();     

      if (incomingByte==RMC_id[i]) {
        valid_RMC--;
        valid_msg=true;           
      }
            
      if(!valid_msg) {i=7;} // break from "if" loop 
    }    
   
    if (valid_RMC==0) {
      while (Serial.available()<25) {
        delay(10);
      }           
      
      fill_buffer();     
      show_time();

    } 
  }
}