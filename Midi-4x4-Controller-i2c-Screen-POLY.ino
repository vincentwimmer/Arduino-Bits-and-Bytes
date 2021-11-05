
#include "HID.h"
#include "HID-Project.h"

// pin definitions
#define Shifter_Reverse_Pin        15
#define Shifter_X_AXIS_PIN         A0 //4 Shifter X axis
#define Shifter_Y_AXIS_PIN         A1 //8 Shifter Y axis

// H-shifter mode analog axis thresholds
#define HS_XAXIS_350        350
#define HS_XAXIS_600        600
#define HS_YAXIS_300       300
#define HS_YAXIS_600       575


// Digital inputs definitions
#define DI_REVERSE         1
#define DI_MODE            3

int yAxis_ = 0; 
int rxAxis_ = 0;                    
int ryAxis_ = 0;  
int xAxis_ = 0;          
int Throttle_ = 0; 

int yAxis_1 = -32000; 
int rxAxis_1 = -32000;                    
int ryAxis_1 = -32000;  
int xAxis_1 = -32000;          
int Throttle_1 = -32000; 

int BB1 = 0;
int BB2 = 0;
int BB3 = 0;
int BB4 = 0;
int BB5 = 0;
int BB6 = 0;
int BB7 = 0;
int BB8 = 0;
int BB9 = 0;
int BB10 = 0;
int BB11 = 0;

// Called at startup
// Must initialize hardware and software modules
void setup()
{
    // G25 shifter analog inputs configuration 
    pinMode(Shifter_X_AXIS_PIN, INPUT);   // X axis
    pinMode(Shifter_Y_AXIS_PIN, INPUT);   // Y axis
    pinMode(Shifter_Reverse_Pin, INPUT_PULLUP);
    
    pinMode(0, INPUT_PULLUP);
    pinMode(1, INPUT_PULLUP);
    pinMode(4, INPUT_PULLUP);
    pinMode(5, INPUT_PULLUP);
    pinMode(6, INPUT_PULLUP);
    pinMode(7, INPUT_PULLUP);
    pinMode(8, INPUT_PULLUP);
    pinMode(9, INPUT_PULLUP);
    pinMode(10, INPUT_PULLUP);
    pinMode(16, INPUT_PULLUP);
    pinMode(14, INPUT_PULLUP);

    // Virtual joystick configuration 
    Gamepad.begin();
    
    // Virtual serial interface configuration 
    Serial.begin(38400);
}

// Called in a loop after initialization
void loop() 
{
    // Reading of button states from G25 shift register
    int b[16];
    
    
    // Reading of shifter position
    int x=analogRead(Shifter_X_AXIS_PIN);                 // X axis
    int y=analogRead(Shifter_Y_AXIS_PIN);                 // Y axis
    int reverse=digitalRead(Shifter_Reverse_Pin);
    
    // Current gear calculation
    int gear=0;                          // Default value is neutral

        if(x<HS_XAXIS_350)                  // Shifter on the left?
        {
            if(y>HS_YAXIS_600) gear=1;       // 1st gear
            if(y<HS_YAXIS_300) gear=2;       // 2nd gear
        }
        else if(x>HS_XAXIS_600)             // Shifter on the right?
        {
            if(y>HS_YAXIS_600) gear=5;       // 5th gear
            if(y<HS_YAXIS_300) gear=6;       // 6th gear
        }
        else                               // Shifter is in the middle
        {
            if(y>HS_YAXIS_600) gear=3;       // 3rd gear
            if(y<HS_YAXIS_300) gear=4;       // 4th gear
        }
        
    //if(gear!=6) reverse=0;         // Reverse gear is allowed only on 6th gear position
    //Serial.print(reverse);
    //Serial.print("\n");
    //if(reverse>1) gear=7;         // 6th gear is deactivated if reverse gear is engaged
    
    // Release virtual buttons for all gears
    Gamepad.release(1);
    Gamepad.release(2);
    Gamepad.release(3);
    Gamepad.release(4);
    Gamepad.release(5);
    Gamepad.release(6);
    //Gamepad.release(7);

    xAxis_ = analogRead(A2);
    if(xAxis_ > 50){
        xAxis_1 = map(xAxis_,300,0,45000,-48000);            
        Gamepad.xAxis(xAxis_1);
    } 

    // Depress virtual button for current gear
    if(gear>0) Gamepad.press(gear);

   BB1 = digitalRead(0);
   //Serial.print(BB1);
   //Serial.print("\n");
   if(BB1 < 1){
       Gamepad.press(8);
   } else {
       Gamepad.release(8);
   }

   BB2 = digitalRead(1);
   if(BB2 < 1){
       Gamepad.press(9);
   } else {
       Gamepad.release(9);
   }

   BB3 = digitalRead(4);
   if(BB3 < 1){
       Gamepad.press(10);
   } else {
       Gamepad.release(10);
   }

   BB4 = digitalRead(5);
   if(BB4 < 1){
       Gamepad.press(11);
   } else {
       Gamepad.release(11);
   }

   BB5 = digitalRead(6);
   if(BB5 < 1){
       Gamepad.press(12);
   } else {
       Gamepad.release(12);
   }

   BB6 = digitalRead(7);
   if(BB6 < 1){
       Gamepad.press(13);
   } else {
       Gamepad.release(13);
   }

   BB7 = digitalRead(8);
   if(BB7 < 1){
       Gamepad.press(14);
   } else {
       Gamepad.release(14);
   }

   BB8 = digitalRead(9);
   if(BB8 < 1){
       Gamepad.press(15);
   } else {
       Gamepad.release(15);
   }

   BB9 = digitalRead(10);
   if(BB9 < 1){
       Gamepad.press(16);
   } else {
       Gamepad.release(16);
   }

   BB10 = digitalRead(16);
   if(BB10 < 1){
       Gamepad.press(17);
   } else {
       Gamepad.release(17);
   }

   BB11 = digitalRead(14);
   if(BB11 < 1){
       Gamepad.press(18);
   } else {
       Gamepad.release(18);
   }

    
    // Write new virtual joystick state
    Gamepad.write();
    
    // Write inputs and outputs (remove comments to debug)
    
      
    /*
    Serial.print(" X axis: ");
    Serial.print(ryAxis_);

    Serial.print(" Y axis: ");
    Serial.print(y);
    
    Serial.print(reverse);
    Serial.print(" Gear: ");
    Serial.print(gear);

    Serial.print(" Digital inputs: ");
    for(int i=0; i<16; i++)Serial.print(b[i]);
    Serial.print(" ");
    
    Serial.print("\n");
    */

    
    // Wait
delay(50);                                // Wait for 10ms
}
