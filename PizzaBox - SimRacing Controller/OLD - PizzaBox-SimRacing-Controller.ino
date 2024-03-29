
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

// Called at startup
// Must initialize hardware and software modules
void setup()
{
    // G25 shifter analog inputs configuration 
    pinMode(Shifter_X_AXIS_PIN, INPUT);   // X axis
    pinMode(Shifter_Y_AXIS_PIN, INPUT);   // Y axis
    pinMode(Shifter_Reverse_Pin, INPUT);
    
    pinMode(14, INPUT);

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
    int reverse=analogRead(Shifter_Reverse_Pin);
    
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
        
    if(gear!=6) reverse=0;         // Reverse gear is allowed only on 6th gear position
    //Serial.print(reverse);
    //Serial.print("\n");
    if(reverse>1000) gear=7;         // 6th gear is deactivated if reverse gear is engaged
    
    // Release virtual buttons for all gears
    Gamepad.release(1);
    Gamepad.release(2);
    Gamepad.release(3);
    Gamepad.release(4);
    Gamepad.release(5);
    Gamepad.release(6);
    Gamepad.release(7);

    
    Gamepad.release(9);
    Gamepad.release(10);
    Gamepad.release(11);
    Gamepad.release(12);
    Gamepad.release(13);
    Gamepad.release(14);
    Gamepad.release(15);
    Gamepad.release(16);
    Gamepad.release(17);
    Gamepad.release(18);

    xAxis_ = analogRead(A2);
    if(xAxis_ > 50){
        xAxis_1 = map(xAxis_,300,0,45000,-48000);            
        Gamepad.xAxis(xAxis_1);
    } 
    
    // Depress virtual button for current gear
    if(gear>0) Gamepad.press(gear);

   BB1 = digitalRead(14);
   Serial.print(BB1);
   if(BB1 > 0){
       Gamepad.press(8);
   } else {
       Gamepad.release(8);
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
