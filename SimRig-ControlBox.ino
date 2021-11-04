#include <Joystick.h>

// Create the Joystick
Joystick_ Joystick;

#define HS_XAXIS_12        400
#define HS_XAXIS_56        500
#define HS_YAXIS_135       800
#define HS_YAXIS_246       300

int gearAct=0;
int zAxis_ = 0;

void setup() {
  // Shifter Pins
  pinMode(A0, INPUT_PULLUP);  // ShiftX axis
  pinMode(A1, INPUT_PULLUP);  // ShiftY axis
  pinMode(15, INPUT);     // Reverse Pin

  // HB Pin
  pinMode(A2, INPUT_PULLUP);

  // Button Box Pins
  pinMode(0, INPUT);
  pinMode(1, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(10, INPUT);
  pinMode(16, INPUT);
  pinMode(14, INPUT);
  
  Joystick.begin();
  
}

void loop() {
  
  // --- Begin Shifter ---
  int ShiftX=analogRead(0); // X axis
  int ShiftY=analogRead(1); // Y axis

  int reverseEngaged = digitalRead(15);
  int engagedGear = 0;
  int reverseAct = 0;

  if( reverseEngaged == 1 ){

      engagedGear = 8;
      reverseAct = 1;

  }else{ 
    if(ShiftX < HS_XAXIS_12) 
    {
      if(ShiftY > HS_YAXIS_135) engagedGear=1;
      if(ShiftY < HS_YAXIS_246) engagedGear=2;
    }
    else if(ShiftX > HS_XAXIS_56)
    {
      if(ShiftY > HS_YAXIS_135) engagedGear=5;
      if(ShiftY < HS_YAXIS_246) engagedGear=6;

    }
    else
    {
      if(ShiftY > HS_YAXIS_135) engagedGear=3;
      if(ShiftY < HS_YAXIS_246) engagedGear=4;
    }

  }

  if(gearAct != 6) reverseAct = 0;

    if (engagedGear != gearAct){
        gearAct = engagedGear;
        desactivar();
        Joystick.setButton(gearAct-1, HIGH);
    }
  // --- End Shifter -- 

  // --- Begin HB ---
  zAxis_ = analogRead(A2);
  zAxis_ = map(zAxis_,0,1023,0,255);
  Joystick.setZAxis(zAxis_);
  // --- End HB ----

  // --- Begin BB ----
  int BB1 = digitalRead(0);
  int BB2 = digitalRead(1);
  int BB3 = digitalRead(4);
  int BB4 = digitalRead(5);
  int BB5 = digitalRead(6);
  int BB6 = digitalRead(7);
  int BB7 = digitalRead(8);
  int BB8 = digitalRead(9);
  int BB9 = digitalRead(10);
  int BB10 = digitalRead(16);
  int BB11 = digitalRead(14);

  if( BB1 == 1 ){
    Joystick.setButton(8, HIGH);
  } else {
    Joystick.setButton(8, LOW);
  }

  if( BB2 == 1 ){
    Joystick.setButton(9, HIGH);
  } else {
    Joystick.setButton(9, LOW);
  }

  if( BB3 == 1 ){
    Joystick.setButton(10, HIGH);
  } else {
    Joystick.setButton(10, LOW);
  }

  if( BB4 == 1 ){
    Joystick.setButton(11, HIGH);
  } else {
    Joystick.setButton(11, LOW);
  }

  if( BB5 == 1 ){
    Joystick.setButton(12, HIGH);
  } else {
    Joystick.setButton(12, LOW);
  }

  if( BB6 == 1 ){
    Joystick.setButton(13, HIGH);
  } else {
    Joystick.setButton(13, LOW);
  }

  if( BB7 == 1 ){
    Joystick.setButton(14, HIGH);
  } else {
    Joystick.setButton(14, LOW);
  }

  if( BB8 == 1 ){
    Joystick.setButton(15, HIGH);
  } else {
    Joystick.setButton(15, LOW);
  }

  if( BB9 == 1 ){
    Joystick.setButton(16, HIGH);
  } else {
    Joystick.setButton(16, LOW);
  }

  if( BB10 == 1 ){
    Joystick.setButton(17, HIGH);
  } else {
    Joystick.setButton(17, LOW);
  }

  if( BB11 == 1 ){
    Joystick.setButton(18, HIGH);
  } else {
    Joystick.setButton(18, LOW);
  }
  // --- End BB ---

  // "Polling" Rate
  delay(50);
}

void desactivar(){
  // Depress virtual button for current gear
  for(int i = 0; i <= 7 ; i++ )  Joystick.setButton(i, LOW);
}
