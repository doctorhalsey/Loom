#include <AddicoreRFID.h>
#include <SPI.h>
#include <FastLED.h>

#define	uchar	unsigned char
#define	uint	unsigned int
#define chipSelectPin 10
#define NRSTPD 5
#define MAX_LEN 16
#define DATA_PIN 9
#define NUM_LEDS 16

CRGB leds[NUM_LEDS];

uchar fifobytes;
uchar fifoValue;

AddicoreRFID myRFID; 

void setup() { 
   FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
   Serial.begin(9600); 
 
  // start the SPI library:
  SPI.begin();
  
  pinMode(chipSelectPin,OUTPUT);
    digitalWrite(chipSelectPin, LOW); 
  pinMode(NRSTPD,OUTPUT); 
    digitalWrite(NRSTPD, HIGH);

  myRFID.AddicoreRFID_Init();  
}

void loop()
{
  	uchar i, tmp, checksum1;
	uchar status;
        uchar str[MAX_LEN];
        uchar RC_size;
        uchar blockAddr;
        String mynum = "";
        str[1] = 0x4400;
	
	status = myRFID.AddicoreRFID_Request(PICC_REQIDL, str);	
	if (status == MI_OK)
	{
          Serial.println("RFID tag detected");
          Serial.print("Tag Type:\t\t");
          uint tagType = str[0] << 8;
          tagType = tagType + str[1];
          switch (tagType) {
            case 0x4400:
              Serial.println("Mifare UltraLight");
              break;
            case 0x400:
              Serial.println("Mifare One (S50)");
              break;
            case 0x200:
              Serial.println("Mifare One (S70)");
              break;
            case 0x800:
              Serial.println("Mifare Pro (X)");
              break;
            case 0x4403:
              Serial.println("Mifare DESFire");
              break;
            default:
              Serial.println("Unknown");
              break;
          }
	}

	status = myRFID.AddicoreRFID_Anticoll(str);
	if (status == MI_OK)
	{
          checksum1 = str[0] ^ str[1] ^ str[2] ^ str[3];
          Serial.print("The tag's number is:\t");
    	    Serial.print(str[0]);
            Serial.print(" , ");
    	    Serial.print(str[1]);
            Serial.print(" , ");
    	    Serial.print(str[2]);
            Serial.print(" , ");
    	    Serial.println(str[3]);

          Serial.print("Read Checksum:\t\t");
    	      Serial.println(str[4]);
          Serial.print("Calculated Checksum:\t");
            Serial.println(checksum1);
        
   
            if(str[0] == 177) {                     
              Serial.println("\nHello briano!\n");
              for (int i = 0; i < 16; i++) {
                leds[i] = CRGB(0, 255, 255);
              }
            } 
            else if(str[0] == 230) {           
                Serial.println("\nHello rob :3!\n");
                for (int i = 0; i < 16; i++) {
                leds[i] = CRGB(255, 0, 255);
              }
            }
            else if(str[0] == 208) {           
                Serial.println("\nHello 01!\n");
                for (int i = 0; i < 16; i++) {
                leds[i] = CRGB(255, 255, 0);
              }
            }
            else if(str[0] == 96) {           
                Serial.println("\nHello 02!\n");
                for (int i = 0; i < 16; i++) {
                leds[i] = CRGB(255, 255, 255);
              }
            }
            FastLED.show();
            Serial.println();
            delay(100);
	}

 else { 
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < NUM_LEDS; j++) {
        leds[j] = CRGB(255, 0, 0);
      }
      FastLED.show();
      delay (100);
      for (int j = 0; j < NUM_LEDS; j++) {
        leds[j] = CRGB(0, 0, 0);
      }
      FastLED.show();
      delay (100);
    }
 }		
        myRFID.AddicoreRFID_Halt();              

}

