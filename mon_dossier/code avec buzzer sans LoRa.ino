#include <Wire.h>
#include "SHTSensor.h" // http://librarymanager/All#arduino_shtc3
#include <FastLED.h> // http://librarymanager/All#FASTLED



#define LED_PIN     4
#define NUM_LEDS    21
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
CRGBPalette16 currentPalette;
TBlendType    currentBlending;

#define UPDATES_PER_SECOND 100
SHTSensor sht;

#include "pitches.h"

#define Button 2
#define Buzzer 8


// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};
float Temperature;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  sht.init();
  sht.setAccuracy(SHTSensor::SHT_ACCURACY_MEDIUM); // only supported by SHT3x


  Serial.println("Temperature:,Humidity:");   // Plot labels

  //start serial connection
  Serial.begin(9600);
  //configure pin 2 as an input and enable the internal pull-up resistor
  pinMode(Button, INPUT_PULLUP);
  pinMode(13, OUTPUT);
 
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(Buzzer, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(Buzzer);
  }

}


void loop() {
    sht.readSample();
      int sensorVal = digitalRead(Button);
  //print out the value of the pushbutton
  Serial.println(sensorVal);
 
    Temperature = sht.getTemperature();
    Serial.print(Temperature);
    Serial.print(F(" "));
    Serial.println(sht.getHumidity());

    if(Temperature >40.00){
      Serial.print("Il fait trop chaud");
      // iterate over the notes of the melody:
      for (int thisNote = 0; thisNote < 8; thisNote++) {
   
        // to calculate the note duration, take one second divided by the note type.
        //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        int noteDuration = 1000 / noteDurations[thisNote];
        tone(Buzzer, melody[thisNote], noteDuration);
   
        // to distinguish the notes, set a minimum time between them.
        // the note's duration + 30% seems to work well:
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        // stop the tone playing:
        noTone(Buzzer);
      }
    }


  // Keep in mind the pull-up means the pushbutton's logic is inverted. It goes
  // HIGH when it's open, and LOW when it's pressed. Turn on pin 13 when the
  // button's pressed, and off when it's not:
  if (sensorVal == HIGH) {
    digitalWrite(13, LOW);
  } else {
    digitalWrite(13, HIGH);
    }

 
   


uint8_t temp = map(sht.getTemperature(), 20,35,170,0); // Map value from luminosity sensor to LED

 // FastLED's built-in rainbow generator
  fill_solid( leds, NUM_LEDS, ColorFromPalette(RainbowColors_p,temp,BRIGHTNESS, LINEARBLEND));




 
 
// send the 'leds' array out to the actual LED strip
  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
   
    delay(1000);
  }
