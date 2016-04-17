#include <Adafruit_NeoPixel.h>


#define PIN 11
#define NUMBER_OF_LEDS 20
#define MODE_HEADER 0
#define MODE_HOLD   1
#define MODE_DATA   2

char recvChar;
String recvString;  

uint8_t         // values for current color
  current_red,      
  current_green, 
  current_blue;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMBER_OF_LEDS, PIN, NEO_GRB + NEO_KHZ800);


void setup() {
        Serial.begin(57600);     // opens serial port
        pinMode(13, OUTPUT);

        strip.begin();
        
        strip.show(); // Initialize all pixels to 'off'
}


void loop() {     
  
    String recvString = "";               // clear string for next reading
    recvChar = 0;
    
    // Eingabe vorhanden?
    if (Serial.available() > 0) {
        while ((Serial.available() > 0) && (recvChar != '.'))
        {
          recvChar = Serial.read();     //read every character
          recvString.concat(recvChar);  //and concat to string complete string, until dot found
          delay(1);
        }     
        Serial.println(recvString);
        setCurrentColor(recvString);
        refreshStrip(0);            
       Serial.flush();          
      }
              
}

void setCurrentColor(String colorstring) {           // seperate the rescieved string by dashes end set global current-colors
  uint8_t p_red=0, p_green = 0, p_blue = 0; 
      
  p_red = stringToInt((colorstring.substring(0,colorstring.indexOf('-'))));                                     // first value
  p_green = stringToInt((colorstring.substring(colorstring.indexOf('-')+1,colorstring.lastIndexOf('-'))));  // second value
  p_blue = stringToInt((colorstring.substring(colorstring.lastIndexOf('-')+1,colorstring.indexOf('.'))));   // third value
    
  current_red=p_red;          // set globals
  current_green=p_green;
  current_blue=p_blue;
  
}


void refreshStrip(uint8_t refreshdelay) {    // send current-colors to LED-Strip
 
    for (int i = 0; i < NUMBER_OF_LEDS; i++) {
     strip.setPixelColor(i, current_red, current_green, current_blue);
     delay(refreshdelay);
   }
   strip.show();      
}

int stringToInt(String string){        // StrToInt-Function
  char char_string[string.length()+1];
  string.toCharArray(char_string, string.length()+1);
    return atoi(char_string);
}
