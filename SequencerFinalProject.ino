//waveform stuff
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

//drum stuff
#include <synth_simple_drum.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

//Mixer Stuff
AudioSynthSimpleDrum     drum2;          //xy=327,337
AudioSynthSimpleDrum     drum1;          //xy=329,271
AudioSynthWaveform       waveform1;      //xy=336,217
AudioSynthSimpleDrum     drum3;          //xy=352,411
AudioMixer4              mixer1;         //xy=497,306
AudioOutputAnalog        dac1;           //xy=658,285
AudioConnection          patchCord1(drum2, 0, mixer1, 2);
AudioConnection          patchCord2(drum1, 0, mixer1, 1);
AudioConnection          patchCord3(waveform1, 0, mixer1, 0);
AudioConnection          patchCord4(drum3, 0, mixer1, 3);
AudioConnection          patchCord5(mixer1, dac1);

//neopixelstuff
//#include "BetterButton.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

//LEDS
#define PIN 18
#define NUM_LEDS 8
#define BRIGHTNESS 15
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

byte neopix_gamma[] =
{
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
  2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
  5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
  10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
  17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
  25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
  37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
  51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
  69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
  90, 92, 93, 95, 96, 98, 99, 101, 102, 104, 105, 107, 109, 110, 112, 114,
  115, 117, 119, 120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142,
  144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175,
  177, 180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,
  215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255
};

//COLORS
int color[3] = {0, 0, 0};

//POTS
int potPin[8] = {17, 16, 15, 14, 39, 38, 37, 36};
int tempoPot = A13;
int tempo = 0;
int potPitch[4] = {0, 0, 0, 0};

//BUTTONS
int buttonPin[8] = { 24, 25, 26, 27, 28, 29, 30, 31};
int nextChannelButtonPin = 12;
int prevChannelButtonPin = 11;
int nextWavButton = 10;
int prevWavButton = 9;

int channelDisplayed = 0;
int previousChan = 0;
int currentChan = 0;

//MIDI
int midiNotes[4] = {60, 62, 64, 66};
int midiNotesOct[4] = {72, 74, 76, 78};
int keyMidi[8] = {60, 62, 64, 65, 67, 69, 71, 72};
int keyMidiOct[8] = {72, 74, 76, 77, 79, 81, 83, 84};
int wavPitch[8] = {261.63, 293.67, 329.63, 349.23, 392, 440, 493.88, 523.25};
int wavPitchOct[8] = {523.25, 587.33, 659.26, 698.46, 783.99, 880, 987.77, 1046.5};
int loNote = 261.63;
int hiNote = 523.25;
int loMidi = 60;
int hiMidi = 72;

//SWITCHES
int octaveSwitch = 35;
//(A16)
int backSwitch = 34;
//(A15)
int midiSwitch = 22;
//(A8)

//STATE STUFF
int currentStep = 0;
unsigned long lastStepTime = 0;

boolean lastButtonState[8] = { LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};
boolean buttonState[8] = { LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW };
boolean lastButtonStateK[8] = { LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};
boolean buttonStateK[8] = { LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW };
boolean lastButtonStateWav[2] = { LOW, LOW};
boolean buttonStateWav[2] = { LOW, LOW};

boolean on[4][8] =
{
  { LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW },
  { LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW },
  { LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW },
  { LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW },
};

boolean lastBSUp = LOW;
boolean lastBSDown = LOW;
boolean buttonStateUp = LOW;
boolean buttonStateDown = LOW;

//for WAV select
boolean lastBSUpWav = LOW;
boolean lastBSDownWav = LOW;
boolean buttonStateUpWav = LOW;
boolean buttonStateDownWav = LOW;

//WAVEFORM STUFF
int wavNum = 0;
char wavForm [4][25] = { WAVEFORM_SINE, WAVEFORM_SAWTOOTH, WAVEFORM_SQUARE, WAVEFORM_TRIANGLE};

void setup()
{
  //Serial.begin(9600);
  Serial.begin(115200);

  //waveform stuff
  AudioMemory(50); //always include this when using the Teensy Audio Library
  //AudioMemory(15);

  //pinmodes
  for (int i = 0; i < 8; i++)
  {
    pinMode(potPin[i], INPUT);
    pinMode(buttonPin[i], INPUT);
  }

  pinMode(tempoPot, INPUT);
  pinMode(backSwitch, INPUT);
  pinMode(octaveSwitch, INPUT);
  pinMode(midiSwitch, INPUT);

  pinMode(12, INPUT);
  pinMode(11, INPUT);
  pinMode(10, INPUT);
  pinMode(9, INPUT);

  //LED stuff
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  //Audio Stuff
  for (int i = 0; i < 4; i++)
  {
    mixer1.gain(i, 2);
  }
}

void loop()
{
  checkButton();
  noteSeq();

  onLed();

  checkChannel();
  chanColor();

  selectWav();

  //testMidSwitch();
}

void testMidSwitch()
{
  if (digitalRead(midiSwitch) == HIGH)
  {
    Serial.println ("ON");
  }
  if (digitalRead(midiSwitch) == LOW)
  {
    Serial.println ("OFF");
  }
}

void stepUp()
{
  if (digitalRead(midiSwitch) == LOW)
  {
    //step 0-7
    currentStep = currentStep + 1;
    if (currentStep > 7)
    {
      currentStep = 0;
    }
  }
}

void stepDown()
{
  if (digitalRead(midiSwitch) == LOW)
  {
    //step 7-0
    currentStep = currentStep - 1;
    if (currentStep < 0)
    {
      currentStep = 7;
    }
  }
}

void checkChannel()
{
  lastBSUp = buttonStateUp;
  buttonStateUp = digitalRead(12);

  if (buttonStateUp == HIGH && lastBSUp == LOW)
  {
    channelDisplayed++;

    if (channelDisplayed > 3)
    {
      channelDisplayed = 0;
    }
    Serial.print (channelDisplayed);
  }

  lastBSDown = buttonStateDown;
  buttonStateDown = digitalRead(11);

  if (buttonStateDown == HIGH && lastBSDown == LOW)
  {
    channelDisplayed--;

    if (channelDisplayed < 0)
    {
      channelDisplayed = 3;
    }
    Serial.print (channelDisplayed);
  }
}

void chanColor()
{
  if (channelDisplayed == 0)
  {
    color[0] = 255;
    color[1] = 0;
    color[2] = 0;
    strip.setBrightness(15);
  }

  if (channelDisplayed == 1)
  {
    color[0] = 153;
    color[1] = 000;
    color[2] = 204;
    strip.setBrightness(25);
  }

  if (channelDisplayed == 2)
  {
    color[0] = 0;
    color[1] = 0;
    color[2] = 255;
    strip.setBrightness(25);
  }

  if (channelDisplayed == 3)
  {
    color[0] = 0;
    color[1] = 255;
    color[2] = 0;
    strip.setBrightness(15);
  }
}

void selectWav()
{
  lastBSUpWav = buttonStateUpWav;
  buttonStateUpWav = digitalRead(10);

  if (buttonStateUpWav == HIGH && lastBSUpWav == LOW)
  {
    wavNum++;

    if (wavNum > 3)
    {
      wavNum = 0;
    }
    Serial.print (wavNum);
  }

  lastBSDownWav = buttonStateDownWav;
  buttonStateDownWav = digitalRead(9);

  if (buttonStateDownWav == HIGH && lastBSDownWav == LOW)
  {
    wavNum--;

    if (wavNum < 0)
    {
      wavNum = 3;
    }
    Serial.print (wavNum);
  }
}

void checkButton()
{
  if (digitalRead(midiSwitch) == HIGH)
  {
    keyboardMidi();
  }

  if (digitalRead(midiSwitch) == LOW)
  {
    for (int i = 0; i < 8; i++)
    {
      lastButtonState[i] = buttonState[i];
      buttonState[i] = digitalRead(buttonPin[i]);

      if (buttonState[i] == HIGH && lastButtonState[i] == LOW)
      {
        if (on[channelDisplayed][i] == false)
        {
          on[channelDisplayed][i] = true;
        }
        else if (on[channelDisplayed][i] == true)
        {
          on[channelDisplayed][i] = false;
        }
      }
    }
  }

}

void onLed()
{
  tempo = map (analogRead(A13), 0, 1023, 50, 500);

  if (digitalRead(midiSwitch) == HIGH)
  {
    for (int i = 0; i < 8; i++)
    {
      if (buttonState[i] == HIGH)
      {
        strip.setPixelColor( (i), strip.Color(color[0], color[1], color[2]) );
        strip.show();
      }
      else if (buttonState[i] == LOW)
      {
        strip.setPixelColor( (i), strip.Color(000, 000, 000) );
        strip.show();
      }
    }
  }

  if (digitalRead(midiSwitch) == LOW)
  {
    for (int i = 0; i < 8; i++)
    {
      if (currentStep == i) {
        strip.setPixelColor( (i), strip.Color(color[0] / 4, color[1] / 4, color[2] / 4) );
        strip.show();
      }
      else if (on[channelDisplayed][i] == true || currentStep == i)
      {
        strip.setPixelColor( (i), strip.Color(color[0], color[1], color[2]) );
        strip.show();
      }
      else if (on[channelDisplayed][i] == false)
      {
        strip.setPixelColor( (i), strip.Color(000, 000, 000) );
        strip.show();
      }
    }
  }

}

void noteSeq()
{
  //SET VALUES
  tempo = map (analogRead(A13), 0, 1023, 50, 500);
  int potQuantArray[8] = { 0, 0, 0, 0, 0, 0, 0 , 0};

  if (digitalRead(octaveSwitch) == LOW)
  {
    int midiNotes[4] = {60, 62, 64, 66};
  }

  if (digitalRead(octaveSwitch) == HIGH)
  {
    int midiNotes[4] = {72, 74, 76, 78};
  }

  //DO KEYBOARD
  if (digitalRead(midiSwitch) == HIGH)
  {
    keyboardMidi();
    wavSelection();
  }

  //DO SEQ
  if (digitalRead(midiSwitch) == LOW)
  {
    //SET CLOCK
    if (millis() > lastStepTime + tempo)
    {
      //SET DIRECTION
      if (digitalRead(backSwitch) == HIGH)
      {
        stepDown();
      }
      else
      {
        stepUp();
      }

      //MIDI SEQUENCE
      for (int i = 0; i < 4; i++)
      {
        if (on[i][currentStep] == true)
        {
          if (digitalRead(octaveSwitch) == LOW)
          {
            //int midiNotes[4] = {60, 62, 64, 66};
            usbMIDI.sendNoteOff(midiNotes[i], 127, 1);
            usbMIDI.sendNoteOn(midiNotes[i], 127, 1);
            Serial.println(midiNotes[i]);
          }

          if (digitalRead(octaveSwitch) == HIGH)
          {
            //int midiNotes[4] = {72, 74, 76, 78};
            usbMIDI.sendNoteOff(midiNotesOct[i], 127, 1);
            usbMIDI.sendNoteOn(midiNotesOct[i], 127, 1);
            Serial.println(midiNotesOct[i]);
          }
        }
      }

      //TEENSY SEQUENCE
      if (on[0][currentStep] == true)
      {
        if (digitalRead(octaveSwitch) == LOW)
        {
          potQuantArray[currentStep] = map(analogRead(potPin[currentStep]), 0, 1023, 0, 12);
          potPitch[currentStep] = loNote * pow(2, potQuantArray[currentStep] / 12.0);
        }
        if (digitalRead(octaveSwitch) == HIGH)
        {
          potQuantArray[currentStep] = map(analogRead(potPin[currentStep]), 0, 1023, 0, 12);
          potPitch[currentStep] = hiNote * pow(2, potQuantArray[currentStep] / 12.0);
        }
      }

      wavSelection();

      //DRUM SEQUENCE
      AudioNoInterrupts();
      drum1.frequency(60);
      drum1.length(1500);
      drum1.secondMix(1.0);
      drum1.pitchMod(0.55);

      drum2.frequency(1000);
      drum2.length(15);
      drum2.secondMix(0.9);
      drum2.pitchMod(0.8);

      drum3.frequency(3800);
      drum3.length(10);
      drum3.secondMix(0.4);
      drum3.pitchMod(0.6);
      AudioInterrupts();

      if (on[1][currentStep] == true)
      {
        drum1.noteOn();
      }

      if (on[2][currentStep] == true)
      {
        drum2.noteOn();
      }

      if (on[3][currentStep] == true)
      {
        drum3.noteOn();
      }

      lastStepTime = millis();
    }
  }

}

void wavSelection()
{
  //SYNTH KEYS

  if (wavNum == 0)
  {
    waveform1.begin(WAVEFORM_SINE);
    //Serial.println("YEET");
  }

  if (wavNum == 1)
  {
    waveform1.begin(WAVEFORM_SAWTOOTH);
  }

  if (wavNum == 2)
  {
    waveform1.begin(WAVEFORM_SQUARE);
  }

  if (wavNum == 3)
  {
    waveform1.begin(WAVEFORM_TRIANGLE);
  }

  for (int i = 0; i < 8; i++)
  {

    //      int loNote = 261.63;
    //      int hiNote = 523.25;

    int wavPitch[8] = {261.63, 293.67, 329.63, 349.23, 392, 440, 493.88, 523.25};
    int wavPitchOct[8] = {523.25, 587.33, 659.26, 698.46, 783.99, 880, 987.77, 1046.5};

    lastButtonStateK[i] = buttonStateK[i];
    buttonStateK[i] = digitalRead(buttonPin[i]);

    // sSerial.println(i);

    if (buttonStateK[i] == HIGH && lastButtonStateK[i] == LOW)
    {
      Serial.println("SUH");

      if (digitalRead(octaveSwitch) == LOW)
      {
        waveform1.amplitude(0.2);
        waveform1.frequency(wavPitch[i]);
      }

      if (digitalRead(octaveSwitch) == HIGH)
      {
        waveform1.amplitude(0.2);
        waveform1.frequency(wavPitchOct[i]);
      }
    }
    else if (buttonStateK[i] == LOW && lastButtonStateK[i] == HIGH)
    {
      waveform1.amplitude(0);
    }
  }


  //SYNTH SEQUENCE
  if (digitalRead(midiSwitch) == LOW)
  {
    if (wavNum == 0)
    {
      waveform1.begin(WAVEFORM_SINE);

      if (on[0][currentStep] == true)
      {
        waveform1.amplitude(0.2);
        waveform1.frequency(potPitch[currentStep]);
      }
      else
      {
        waveform1.amplitude(0);
      }
    }

    if (wavNum == 1)
    {
      waveform1.begin(WAVEFORM_SAWTOOTH);

      if (on[0][currentStep] == true)
      {
        waveform1.amplitude(0.1);
        waveform1.frequency(potPitch[currentStep]);
      }
      else
      {
        waveform1.amplitude(0);
      }
    }

    if (wavNum == 2)
    {
      waveform1.begin(WAVEFORM_SQUARE);

      if (on[0][currentStep] == true)
      {
        waveform1.amplitude(0.1);
        waveform1.frequency(potPitch[currentStep]);
      }
      else
      {
        waveform1.amplitude(0);
      }
    }

    if (wavNum == 3)
    {
      waveform1.begin(WAVEFORM_TRIANGLE);

      if (on[0][currentStep] == true)
      {
        waveform1.amplitude(0.3);
        waveform1.frequency(potPitch[currentStep]);
      }
      else
      {
        waveform1.amplitude(0);
      }
    }
  }

}

//KEYBOARD MODE
void keyboardMidi()
{
  int keyMidi[8] = {60, 62, 64, 65, 67, 69, 71, 72};
  int keyMidiOct[8] = {72, 74, 76, 77, 79, 81, 83, 84};

  for (int i = 0; i < 8; i++)
  {
    lastButtonState[i] = buttonState[i];
    buttonState[i] = digitalRead(buttonPin[i]);

    if (buttonState[i] == HIGH && lastButtonState[i] == LOW)
    {
      if (digitalRead(octaveSwitch) == HIGH)
      {
        usbMIDI.sendNoteOff(keyMidiOct[i], 127, 1);
        usbMIDI.sendNoteOn(keyMidiOct[i], 127, 1);
      }
      if (digitalRead(octaveSwitch) == LOW)
      {
        usbMIDI.sendNoteOff(keyMidi[i], 127, 1);
        usbMIDI.sendNoteOn(keyMidi[i], 127, 1);
      }
    }

    if (buttonState[i] == LOW && lastButtonState[i] == HIGH)
    {
      usbMIDI.sendNoteOff(keyMidi[i], 127, 1);
      usbMIDI.sendNoteOff(keyMidiOct[i], 127, 1);
    }
  }

}
