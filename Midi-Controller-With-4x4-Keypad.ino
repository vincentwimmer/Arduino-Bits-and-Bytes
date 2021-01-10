#include "Keypad.h"
#include "MIDIUSB.h"
#include "PitchToNote.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/* -------
OLED-Stuff
------- */
#define OLED_RESET 3
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

/* --------
Analog-Knob
-------- */
const uint16_t CCPot0 = 9;
uint16_t cc0;
uint16_t AnalogValue0;
uint16_t lastAnalogValue0;
uint16_t AV0;

/* -------
Func-Butts
------- */
// Func-Butts-Count
#define NUM_BUTTONS 16
// Func-Butts-Pins
const int octUp = 6;
const int octDn = 7;
const int majminBut = 5;
const int ledPin = 4;
// Func-Butts-States
int octUpState = 0;
int octDnState = 0;
int majminButState = 0;

/* ---------
KeyPad-Setup
--------- */
// Note Buttons
const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
    {'0', '1', '2', '3'},
    {'4', '5', '6', '7'},
    {'8', '9', ':', ';'},
    {'<', '=', '>', '?'}};
byte rowPins[ROWS] = {15, 14, 16, 10};
byte colPins[COLS] = {18, 19, 20, 21};
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
// Thing that translates the keypad into a value
int r;

/* --------
Notes-Setup
-------- */
byte notePitches[NUM_BUTTONS] = {C3, D3, E3, F3, G3, A3, B3, C4, D4, E4, F4, G4, A4, B4, C5, D5};
char currentScale;
char currentScaleM;
char currentOctaveAS;
char currentOctave;
int oct;

/* ---------
Intro-Switch
--------- */
int introPlayed = 0;

void setup()
{
  //   Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(octUp, INPUT_PULLUP);
  pinMode(octDn, INPUT_PULLUP);
  pinMode(majminBut, INPUT_PULLUP);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  // Clear the buffer.
  display.clearDisplay();
}

void controlChange(byte channel, byte control, byte value)
{
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

void readPots2()
{
      AnalogValue0 = analogRead(CCPot0);
    //  convert to a range from 0 to 127:
    cc0 = AnalogValue0 / 8;
    
  if (octDnState != HIGH)
  {
        if ((lastAnalogValue0 - cc0) > 10)
    {
    AV0 = analogRead(CCPot0);
    if (AV0 > 0 && AV0 < 73)
    {
      currentScale = 'C';
      currentScaleM = ' ';
      notePitches[0] = 48;
      notePitches[1] = 50;
      notePitches[2] = 52;
      notePitches[3] = 53;
      notePitches[4] = 55;
      notePitches[5] = 57;
      notePitches[6] = 59;
      notePitches[7] = 60;
      notePitches[8] = 62;
      notePitches[9] = 64;
      notePitches[10] = 65;
      notePitches[11] = 67;
      notePitches[12] = 69;
      notePitches[13] = 71;
      notePitches[14] = 72;
      notePitches[15] = 74;
    }
    else if ((AV0 > 74 && AV0 < 147))
    {
      currentScale = 'C';
      currentScaleM = 'm';
      notePitches[0] = 48;
      notePitches[1] = 50;
      notePitches[2] = 51;
      notePitches[3] = 53;
      notePitches[4] = 55;
      notePitches[5] = 56;
      notePitches[6] = 58;
      notePitches[7] = 60;
      notePitches[8] = 62;
      notePitches[9] = 63;
      notePitches[10] = 65;
      notePitches[11] = 67;
      notePitches[12] = 68;
      notePitches[13] = 70;
      notePitches[14] = 72;
      notePitches[15] = 74;
    }
    else if ((AV0 > 148 && AV0 < 221))
    {
      currentScale = 'D';
      currentScaleM = ' ';
      notePitches[0] = 50;
      notePitches[1] = 52;
      notePitches[2] = 54;
      notePitches[3] = 55;
      notePitches[4] = 57;
      notePitches[5] = 59;
      notePitches[6] = 61;
      notePitches[7] = 62;
      notePitches[8] = 64;
      notePitches[9] = 66;
      notePitches[10] = 67;
      notePitches[11] = 69;
      notePitches[12] = 71;
      notePitches[13] = 73;
      notePitches[14] = 74;
      notePitches[15] = 76;
    }
  }
  }
  else if (octUpState != HIGH)
  {
            if ((lastAnalogValue0 - cc0) > 10)
    {
    AV0 = analogRead(CCPot0);
    if (AV0 > 0 && AV0 < 204)
    {
      currentOctaveAS = '-';
      currentOctave = '2';
      oct = -24;
    }
    else if ((AV0 > 205 && AV0 < 409))
    {
      currentOctaveAS = '-';
      currentOctave = '1';
      oct = -12;
    }
    else if ((AV0 > 210 && AV0 < 614))
    {
      currentOctaveAS = ' ';
      currentOctave = '0';
      oct = 0;
    }
    else if ((AV0 > 615 && AV0 < 819))
    {
      currentOctaveAS = '+';
      currentOctave = '1';
      oct = 12;
    }
    else if ((AV0 > 820 && AV0 < 1023))
    {
      currentOctaveAS = '+';
      currentOctave = '2';
      oct = 24;
    }
    }
  } else
  {
    // check if analog input has changed
    if ((lastAnalogValue0 - cc0) > 1)
    {
      controlChange(0, 2, cc0);
      // update lastAnalogValue zero variable
      lastAnalogValue0 = cc0;
      MidiUSB.flush();
    }
  }
}

void intro()
{
  Adafruit_SSD1306 display(OLED_RESET);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  // Clear the buffer.
  display.clearDisplay();

  // draw white circles, 10 pixel radius
  display.fillCircle(64, display.height() / 2, 10, WHITE);
  display.fillCircle(96, display.height() / 2, 10, WHITE);
  display.fillCircle(32, display.height() / 2, 10, WHITE);
  display.display();
  delay(2000);
  display.clearDisplay();

  // Set Things
  currentScale = 'C';
  currentOctave = '0';
  oct = 0;
}

void loop()
{
  char customKey = customKeypad.getKey();
  int r = customKey - '0';
  octUpState = digitalRead(octUp);
  octDnState = digitalRead(octDn);
  majminButState = digitalRead(majminBut);

  // This plays twice because the screen doesn't fucking work right otherwise..
  if (introPlayed == 0)
  {
    intro();
  }
  if (introPlayed == 0)
  {
    intro();
    introPlayed = 1;
  }

  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0, 0);
  display.print("Scl:");
  display.print(currentScale);
  display.print(currentScaleM);
  display.setCursor(43, 0);
  display.print("Oct:");
  display.print(currentOctaveAS);
  display.print(currentOctave);
  display.setCursor(92, 0);
  display.print("Knb:");
  display.print("01");
  display.display();

  readPots2();

  if (customKey)
  {
    int o = r;
    Serial.print(customKey);
    midiEventPacket_t noteOn = {0x09, 0x90 | 0, notePitches[o]+oct, 1023};
    MidiUSB.sendMIDI(noteOn);
    MidiUSB.flush();
    //  display.clearDisplay(0x00, 0x0F);
    display.setTextSize(2);
    display.setTextColor(WHITE, BLACK);
    display.setCursor(0, 16);
    display.print("Key:");
    display.print(notePitches[o]+oct);

    delay(100);
    midiEventPacket_t noteOff = {0x08, 0x80 | 0, notePitches[o]+oct, 0};
    MidiUSB.sendMIDI(noteOff);
    MidiUSB.flush();
  }
  if (octUpState != HIGH)
  {
    digitalWrite(ledPin, HIGH);
  }
  else
  {
    digitalWrite(ledPin, LOW);
  }

  if (majminButState != HIGH)
  {
    digitalWrite(ledPin, HIGH);
  }
  else
  {
    digitalWrite(ledPin, LOW);
  }

  if (octDnState != HIGH)
  {
    digitalWrite(ledPin, HIGH);
  }
  else
  {
    digitalWrite(ledPin, LOW);
  }

  if (introPlayed == 0)
  {
    intro();
    introPlayed = 1;
  }
}

void noteOn(byte channel, byte pitch, byte velocity)
{
  midiEventPacket_t noteOn = {0x09, 0x90 | 0, notePitches[r], 1023};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity)
{
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}
