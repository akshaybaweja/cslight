#include <Adafruit_NeoPixel.h>


#define PIN 11
#define NUMBER_OF_LEDS 20
#define MODE_HEADER 0
#define MODE_HOLD   1
#define MODE_DATA   2

char recvChar;
String recvString;  

uint8_t         // Globale Variablen für aktuelles Farbsetting
  rotwert,      
  gruenwert, 
  blauwert;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMBER_OF_LEDS, PIN, NEO_GRB + NEO_KHZ800);


void setup() {
        Serial.begin(57600);     // opens serial port
        pinMode(13, OUTPUT);

        strip.begin();
        
        strip.show(); // Initialize all pixels to 'off'
}


void loop() {     
  
    String recvString = "";               // String für nächsten Einlesevorgang leeren
    recvChar = 0;
    
    // Eingabe vorhanden?
    if (Serial.available() > 0) {
        while ((Serial.available() > 0) && (recvChar != '.'))
        {
          recvChar = Serial.read();    //Lese jedes Zeichen einzeln ein
          recvString.concat(recvChar);      //und konkatiniere den String damit
          delay(1);
        }     
        Serial.println(recvString);
        farbStringeingabe(recvString);      // Zerlegt übegebenen String in Farbwerte und legt diese in die globalen Farbvariablen
        setzeFarben(0);            // Setzt die LEDs auf globale Farbvariablen
       Serial.flush();          // Leeren des Buffers, sicherheitshalber
      }
              
}

void farbStringeingabe(String farbstring) {                       // Zerlegt übegebenen String anhand der Bindestriche in einzelne
  uint8_t p_rotwert=0, p_gruenwert = 0, p_blauwert = 0, p_pruef;    // Farbwerte und legt sie global ab
    
  p_rotwert = stringToInt((farbstring.substring(0,farbstring.indexOf('-'))));
  p_gruenwert = stringToInt((farbstring.substring(farbstring.indexOf('-')+1,farbstring.lastIndexOf('-'))));
  p_blauwert = stringToInt((farbstring.substring(farbstring.lastIndexOf('-')+1,farbstring.indexOf('.'))));
  //p_pruef = stringToInt((farbstring.substring(farbstring.indexOf('.')+1,farbstring.lastIndexOf('.'))));
  
//  if ((p_rotwert + p_gruenwert + p_blauwert) % 9 == p_pruef) {
    rotwert=p_rotwert;
    gruenwert=p_gruenwert;
    blauwert=p_blauwert;
  //} else { blueToothSerial.println("Modulo abweichend" + p_pruef); Serial.println("MODULO ABWEICHEN"); }
}


void setzeFarben(uint8_t refreshtempo) {
    // Serial.println(rotwert);
 //   Serial.println(gruenwert);
  //   Serial.println(blauwert);
   
  
   for (int i = 0; i < NUMBER_OF_LEDS; i++) {
    
     strip.setPixelColor(i, rotwert, gruenwert, blauwert);
     delay(refreshtempo);
   }
   strip.show();      
}



int stringToInt(String string){        // StrToInt-Funktion
  char char_string[string.length()+1];
  string.toCharArray(char_string, string.length()+1);
    return atoi(char_string);
}
 
        

