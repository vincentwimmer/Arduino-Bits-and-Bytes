/* -------
-Vincent Wimmer 2018-

This is a POLYPHONIC MIDI controller that utilizes 16 buttons (consider arcade cabinet buttons) and an i2c OLED display.
The controller also has built in Scale and Octive switching (Default: C Major)

Expected Arduino Board: Arduino Pro Micro (or similar architecture)

Must wire in accordance to Lines 42-57.

Activate Menu by pressing the 4 corners of the 4x4 grid. (All info shown on i2c screen.)
Buttons 5 and 9 change the Scale +/-
Buttons 6 and 10 change to Octive +/-

Lowest Note is bottom right.
Highest Note is top left.
------- */

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

/* -------
Func-Butts
------- */

#define NUM_BUTTONS  16

const uint16_t button1 = 18;
const uint16_t button2 = 19;
const uint16_t button3 = 20;
const uint16_t button4 = 21;
const uint16_t button5 = 10;
const uint16_t button6 = 16;
const uint16_t button7 = 14;
const uint16_t button8 = 15;
const uint16_t button9 = 6;
const uint16_t button10 = 7;
const uint16_t button11 = 8;
const uint16_t button12 = 9;
const uint16_t button13 = 1;
const uint16_t button14 = 0;
const uint16_t button15 = 4;
const uint16_t button16 = 5;

/* --------
Notes-Setup
-------- */

const uint16_t buttons[NUM_BUTTONS] = {button1, button2, button3, button4, button5, button6, button7, button8, button9, button10, button11, button12, button13, button14, button15, button16};
byte notePitches[NUM_BUTTONS] = {48, 50, 52, 53, 55, 57, 59, 60, 62, 64, 65, 67, 69, 71, 72, 74};

uint16_t notesTime[NUM_BUTTONS];
uint16_t pressedButtons = 0x00;
uint16_t previousButtons = 0x00;
uint16_t intensity;
char currentScale;
char currentScaleM;
char currentOctaveAS;
char currentOctave;
int oct;
int noteScale;
int noteOct;

/* ---------
Intro-Switch
--------- */
int introPlayed = 0;

/* ---
Start
--- */

void setup() {
  for (int i = 0; i < NUM_BUTTONS; i++)
    pinMode(buttons[i], INPUT_PULLUP);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  // Clear the buffer.
  display.clearDisplay();

    currentScale = 'C';
  currentOctave = '0';
  oct = 0;
  noteOct = 0;
  intro();
  intro();
}


void intro()
{
  Adafruit_SSD1306 display(OLED_RESET);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  // Clear the buffer.
  display.clearDisplay();

  // draw white circles, 10 pixel radius
  display.fillCircle(64, (display.height() / 2) + 3, 10, WHITE);
  display.fillCircle(96, (display.height() / 2) + 3, 10, WHITE);
  display.fillCircle(32, (display.height() / 2) + 3, 10, WHITE);
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(31, 16);
  display.print(" PIZZA BOX ");
  display.display();
  delay(2000);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(42, 15);
  display.print("VILL");
  display.display();
  delay(2000);
  display.clearDisplay();

  // Set Things

}


void loop() {
  
  
  

  //bring up menu
  if(digitalRead(buttons[0]) == LOW && digitalRead(buttons[3]) == LOW && digitalRead(buttons[12]) == LOW && digitalRead(buttons[15]) == LOW){
  display.clearDisplay();
  display.drawRoundRect(5, 0, 118, 1, 6, WHITE);
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(18, 8);
  display.print("CHOOSE TOPPINGS");
  display.setCursor(22, 25);
  display.print("Scl:");
  display.print(currentScale);
  display.print(currentScaleM);
  display.setCursor(68, 25);
  display.print("Oct:");
  display.print(oct);
  display.display();

  //action buttons
  if(digitalRead(buttons[9]) == LOW){
    if(noteScale <= 12 && noteScale >= 0){
    noteScale++;  
    delay(250);
    }
  }

  if(digitalRead(buttons[5]) == LOW){
    if(noteScale <= 13 && noteScale >= 1){
    noteScale--; 
    delay(250);  
    }
  }

  if(digitalRead(buttons[10]) == LOW){
    if(oct <= 1 && oct >= -2){
    oct++;
    noteOct += 12;
    delay(250);  
    }
  }

  if(digitalRead(buttons[6]) == LOW){
    if(oct <= 2 && oct >= -1){
      oct--;
    noteOct -= 12; 
    delay(250);  
    }
  }

  setNotes();

  //draw default display
  } else {
  display.clearDisplay();
  display.drawRoundRect(5, 0, 118, 1, 6, WHITE);
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(16, 8);
  display.print("SPECIAL DELIVERY");
  display.setCursor(22, 25);
  display.print("Scl:");
  display.print(currentScale);
  display.print(currentScaleM);
  display.setCursor(68, 25);
  display.print("Oct:");
  display.print(oct);
  display.display();
  
  readButtons();
  readIntensity();
  playNotes();
  }

}

// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

void setNotes()
{
  if(noteScale == 0){
    // C Major
      currentScale = 'C'; 
      currentScaleM = ' '; 
      notePitches[0] = 48 + noteOct; 
      notePitches[1] = 50 + noteOct; 
      notePitches[2] = 52 + noteOct; 
      notePitches[3] = 53 + noteOct; 
      notePitches[4] = 55 + noteOct; 
      notePitches[5] = 57 + noteOct; 
      notePitches[6] = 59 + noteOct; 
      notePitches[7] = 60 + noteOct; 
      notePitches[8] = 62 + noteOct; 
      notePitches[9] = 64 + noteOct; 
      notePitches[10] = 65 + noteOct; 
      notePitches[11] = 67 + noteOct; 
      notePitches[12] = 69 + noteOct; 
      notePitches[13] = 71 + noteOct; 
      notePitches[14] = 72 + noteOct; 
      notePitches[15] = 74 + noteOct; 
  }
  if(noteScale == 1){
    // C minor
      currentScale = 'C'; 
      currentScaleM = 'm'; 
      notePitches[0] = 48 + noteOct; 
      notePitches[1] = 50 + noteOct; 
      notePitches[2] = 51 + noteOct; 
      notePitches[3] = 53 + noteOct; 
      notePitches[4] = 55 + noteOct; 
      notePitches[5] = 56 + noteOct; 
      notePitches[6] = 58 + noteOct; 
      notePitches[7] = 60 + noteOct; 
      notePitches[8] = 62 + noteOct; 
      notePitches[9] = 63 + noteOct; 
      notePitches[10] = 65 + noteOct; 
      notePitches[11] = 67 + noteOct; 
      notePitches[12] = 68 + noteOct; 
      notePitches[13] = 70 + noteOct; 
      notePitches[14] = 72 + noteOct; 
      notePitches[15] = 74 + noteOct; 
  }
if(noteScale == 2){
    // D Major
      currentScale = 'D'; 
      currentScaleM = ' '; 
      notePitches[0] = 50 + noteOct; 
      notePitches[1] = 52 + noteOct; 
      notePitches[2] = 54 + noteOct; 
      notePitches[3] = 55 + noteOct; 
      notePitches[4] = 57 + noteOct; 
      notePitches[5] = 59 + noteOct; 
      notePitches[6] = 61 + noteOct; 
      notePitches[7] = 62 + noteOct; 
      notePitches[8] = 64 + noteOct; 
      notePitches[9] = 66 + noteOct; 
      notePitches[10] = 67 + noteOct; 
      notePitches[11] = 69 + noteOct; 
      notePitches[12] = 71 + noteOct; 
      notePitches[13] = 73 + noteOct; 
      notePitches[14] = 74 + noteOct; 
      notePitches[15] = 76 + noteOct; 
  }
  if(noteScale == 3){
    // D minor
      currentScale = 'D'; 
      currentScaleM = 'm'; 
      notePitches[0] = 50 + noteOct; 
      notePitches[1] = 52 + noteOct; 
      notePitches[2] = 53 + noteOct; 
      notePitches[3] = 55 + noteOct; 
      notePitches[4] = 57 + noteOct; 
      notePitches[5] = 58 + noteOct; 
      notePitches[6] = 60 + noteOct; 
      notePitches[7] = 62 + noteOct; 
      notePitches[8] = 64 + noteOct; 
      notePitches[9] = 65 + noteOct; 
      notePitches[10] = 67 + noteOct; 
      notePitches[11] = 69 + noteOct; 
      notePitches[12] = 70 + noteOct; 
      notePitches[13] = 72 + noteOct; 
      notePitches[14] = 74 + noteOct; 
      notePitches[15] = 76 + noteOct; 
  }
  if(noteScale == 4){
    // E Major
      currentScale = 'E'; 
      currentScaleM = ' '; 
      notePitches[0] = 52 + noteOct; 
      notePitches[1] = 54 + noteOct; 
      notePitches[2] = 56 + noteOct; 
      notePitches[3] = 57 + noteOct; 
      notePitches[4] = 59 + noteOct; 
      notePitches[5] = 61 + noteOct; 
      notePitches[6] = 63 + noteOct; 
      notePitches[7] = 64 + noteOct; 
      notePitches[8] = 66 + noteOct; 
      notePitches[9] = 68 + noteOct; 
      notePitches[10] = 69 + noteOct; 
      notePitches[11] = 71 + noteOct; 
      notePitches[12] = 73 + noteOct; 
      notePitches[13] = 75 + noteOct; 
      notePitches[14] = 76 + noteOct; 
      notePitches[15] = 78 + noteOct; 
  }
  if(noteScale == 5){
    // E minor
      currentScale = 'E'; 
      currentScaleM = 'm'; 
      notePitches[0] = 52 + noteOct; 
      notePitches[1] = 54 + noteOct; 
      notePitches[2] = 55 + noteOct; 
      notePitches[3] = 57 + noteOct; 
      notePitches[4] = 59 + noteOct; 
      notePitches[5] = 60 + noteOct; 
      notePitches[6] = 62 + noteOct; 
      notePitches[7] = 64 + noteOct; 
      notePitches[8] = 66 + noteOct; 
      notePitches[9] = 67 + noteOct; 
      notePitches[10] = 69 + noteOct; 
      notePitches[11] = 71 + noteOct; 
      notePitches[12] = 72 + noteOct; 
      notePitches[13] = 74 + noteOct; 
      notePitches[14] = 76 + noteOct; 
      notePitches[15] = 78 + noteOct; 
  }
  if(noteScale == 6){
    // F Major
      currentScale = 'F'; 
      currentScaleM = ' '; 
      notePitches[0] = 53 + noteOct; 
      notePitches[1] = 55 + noteOct; 
      notePitches[2] = 57 + noteOct; 
      notePitches[3] = 58 + noteOct; 
      notePitches[4] = 60 + noteOct; 
      notePitches[5] = 62 + noteOct; 
      notePitches[6] = 64 + noteOct; 
      notePitches[7] = 65 + noteOct; 
      notePitches[8] = 67 + noteOct; 
      notePitches[9] = 69 + noteOct; 
      notePitches[10] = 70 + noteOct; 
      notePitches[11] = 72 + noteOct; 
      notePitches[12] = 74 + noteOct; 
      notePitches[13] = 76 + noteOct; 
      notePitches[14] = 77 + noteOct; 
      notePitches[15] = 79 + noteOct; 
  }
  if(noteScale == 7){
    // F minor
      currentScale = 'F'; 
      currentScaleM = 'm'; 
      notePitches[0] = 53 + noteOct; 
      notePitches[1] = 55 + noteOct; 
      notePitches[2] = 56 + noteOct; 
      notePitches[3] = 58 + noteOct; 
      notePitches[4] = 60 + noteOct; 
      notePitches[5] = 61 + noteOct; 
      notePitches[6] = 63 + noteOct; 
      notePitches[7] = 65 + noteOct; 
      notePitches[8] = 67 + noteOct; 
      notePitches[9] = 68 + noteOct; 
      notePitches[10] = 70 + noteOct; 
      notePitches[11] = 72 + noteOct; 
      notePitches[12] = 73 + noteOct; 
      notePitches[13] = 75 + noteOct; 
      notePitches[14] = 77 + noteOct; 
      notePitches[15] = 79 + noteOct; 
  }
  if(noteScale == 8){
    // G Major
      currentScale = 'G'; 
      currentScaleM = ' '; 
      notePitches[0] = 55 + noteOct; 
      notePitches[1] = 57 + noteOct; 
      notePitches[2] = 59 + noteOct; 
      notePitches[3] = 60 + noteOct; 
      notePitches[4] = 62 + noteOct; 
      notePitches[5] = 64 + noteOct; 
      notePitches[6] = 66 + noteOct; 
      notePitches[7] = 67 + noteOct; 
      notePitches[8] = 69 + noteOct; 
      notePitches[9] = 71 + noteOct; 
      notePitches[10] = 72 + noteOct; 
      notePitches[11] = 74 + noteOct; 
      notePitches[12] = 76 + noteOct; 
      notePitches[13] = 78 + noteOct; 
      notePitches[14] = 79 + noteOct; 
      notePitches[15] = 81 + noteOct; 
  }
  if(noteScale == 9){
    // G minor
      currentScale = 'G'; 
      currentScaleM = 'm'; 
      notePitches[0] = 55 + noteOct; 
      notePitches[1] = 57 + noteOct; 
      notePitches[2] = 58 + noteOct; 
      notePitches[3] = 60 + noteOct; 
      notePitches[4] = 62 + noteOct; 
      notePitches[5] = 63 + noteOct; 
      notePitches[6] = 65 + noteOct; 
      notePitches[7] = 67 + noteOct; 
      notePitches[8] = 69 + noteOct; 
      notePitches[9] = 70 + noteOct; 
      notePitches[10] = 72 + noteOct; 
      notePitches[11] = 74 + noteOct; 
      notePitches[12] = 75 + noteOct; 
      notePitches[13] = 77 + noteOct; 
      notePitches[14] = 79 + noteOct; 
      notePitches[15] = 81 + noteOct; 
  }
  if(noteScale == 10){
    // A Major
      currentScale = 'A'; 
      currentScaleM = ' '; 
      notePitches[0] = 45 + noteOct; 
      notePitches[1] = 47 + noteOct; 
      notePitches[2] = 49 + noteOct; 
      notePitches[3] = 50 + noteOct; 
      notePitches[4] = 52 + noteOct; 
      notePitches[5] = 54 + noteOct; 
      notePitches[6] = 56 + noteOct; 
      notePitches[7] = 57 + noteOct; 
      notePitches[8] = 59 + noteOct; 
      notePitches[9] = 61 + noteOct; 
      notePitches[10] = 62 + noteOct; 
      notePitches[11] = 64 + noteOct; 
      notePitches[12] = 66 + noteOct; 
      notePitches[13] = 68 + noteOct; 
      notePitches[14] = 69 + noteOct; 
      notePitches[15] = 71 + noteOct; 
  }
  if(noteScale == 11){
    // A minor
      currentScale = 'A'; 
      currentScaleM = 'm'; 
      notePitches[0] = 45 + noteOct; 
      notePitches[1] = 47 + noteOct; 
      notePitches[2] = 48 + noteOct; 
      notePitches[3] = 50 + noteOct; 
      notePitches[4] = 52 + noteOct; 
      notePitches[5] = 53 + noteOct; 
      notePitches[6] = 55 + noteOct; 
      notePitches[7] = 57 + noteOct; 
      notePitches[8] = 59 + noteOct; 
      notePitches[9] = 60 + noteOct; 
      notePitches[10] = 62 + noteOct; 
      notePitches[11] = 64 + noteOct; 
      notePitches[12] = 65 + noteOct; 
      notePitches[13] = 67 + noteOct; 
      notePitches[14] = 69 + noteOct; 
      notePitches[15] = 71 + noteOct; 
  }
  if(noteScale == 12){
    // B Major
      currentScale = 'B'; 
      currentScaleM = ' '; 
      notePitches[0] = 47 + noteOct; 
      notePitches[1] = 49 + noteOct; 
      notePitches[2] = 51 + noteOct; 
      notePitches[3] = 52 + noteOct; 
      notePitches[4] = 54 + noteOct; 
      notePitches[5] = 56 + noteOct; 
      notePitches[6] = 58 + noteOct; 
      notePitches[7] = 59 + noteOct; 
      notePitches[8] = 61 + noteOct; 
      notePitches[9] = 63 + noteOct; 
      notePitches[10] = 64 + noteOct; 
      notePitches[11] = 66 + noteOct; 
      notePitches[12] = 68 + noteOct; 
      notePitches[13] = 70 + noteOct; 
      notePitches[14] = 71 + noteOct; 
      notePitches[15] = 73 + noteOct; 
  }
  if(noteScale == 13){
    // B minor
      currentScale = 'B'; 
      currentScaleM = 'm'; 
      notePitches[0] = 47 + noteOct; 
      notePitches[1] = 49 + noteOct; 
      notePitches[2] = 50 + noteOct; 
      notePitches[3] = 52 + noteOct; 
      notePitches[4] = 54 + noteOct; 
      notePitches[5] = 55 + noteOct; 
      notePitches[6] = 57 + noteOct; 
      notePitches[7] = 59 + noteOct; 
      notePitches[8] = 61 + noteOct; 
      notePitches[9] = 62 + noteOct; 
      notePitches[10] = 64 + noteOct; 
      notePitches[11] = 66 + noteOct; 
      notePitches[12] = 67 + noteOct; 
      notePitches[13] = 69 + noteOct; 
      notePitches[14] = 71 + noteOct; 
      notePitches[15] = 73 + noteOct; 
  }
}

void readButtons()
{
  for (int i = 0; i < NUM_BUTTONS; i++)
  {
    if (digitalRead(buttons[i]) == LOW)
    {
      bitWrite(pressedButtons, i, 1);
      //delay(50);
    }
    else
      bitWrite(pressedButtons, i, 0);
  }
}

void readIntensity()
{
  int val = 1023;
  intensity = (uint16_t) (map(val, 0, 1023, 0, 127));
}

void playNotes()
{
  for (int i = 0; i < NUM_BUTTONS; i++)
  {
    if (bitRead(pressedButtons, i) != bitRead(previousButtons, i))
    {
      if (bitRead(pressedButtons, i))
      {
        bitWrite(previousButtons, i , 1);
        noteOn(0, notePitches[i], 1023);
        MidiUSB.flush();
      }
      else
      {
        bitWrite(previousButtons, i , 0);
        noteOff(0, notePitches[i], 0);
        MidiUSB.flush();
      }
    }
  }
}

// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | 0, pitch, 1023};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | 0, pitch, 0};
  MidiUSB.sendMIDI(noteOff);
}
