#include <Arduino.h>
#include <TM1637Display.h>

 #define debug

#define CLK 14  // Define the CLK pin for TM1637
#define DIO 13  // Define the DIO pin for TM1637
#define IR_PIN 12 // Define the IR sensor pin

TM1637Display display(CLK, DIO);

volatile int count = 0; // Initialize the count variable (volatile for ISR)
bool lastState = HIGH; // Variable to hold the last state of the IR sensor
 volatile double length ;

volatile u_int32_t temp ;
u_int32_t start ;
bool flag ;

// Interrupt Service Routine (ISR) for counting objects
void IRAM_ATTR countObject() {
        if(flag==0)
    {
        flag =1;
        start = millis();
    }
    count++; // Increment count when an object is detected
    temp = millis() ;
}

void setup() {
    Serial.begin(115200);
    display.setBrightness(0x0f); // Set maximum brightness
    pinMode(IR_PIN, INPUT); // Set the IR pin as input

    // Attach interrupt to the IR_PIN for falling edge detection
    attachInterrupt(digitalPinToInterrupt(IR_PIN), countObject, RISING);
    millis();
}
 u_int32_t tm;
void loop() {
    // Display the count on TM1637
        if(flag==1)
    {
        length = count * 4.1 ;
        length = length/100 ;
        display.showNumberDec(u_int32_t(length));
    }
    if(flag == 1)
    {
        // display.showNumberDec(count); // Update display in the main loop
    }
    if(millis()- temp >3000 && flag == 1)
    {
        #ifdef debug
        tm = temp - start ; 
        Serial.print("time is : ");
        Serial.println(tm);
        Serial.println("Count is");
        Serial.println(count);
        Serial.println(length); 
        #endif
        flag= 0 ;
        count = 0 ;  
        
    }

}
