#include <Arduino.h>
#include <Servo.h>

// Pin to read for buttons value
#define BUTTON_IN 2
// Pin to command servo (PWM)
#define SERVO 10

// Positions
#define POSITIONA 5
#define POSITIONB 175

// Delays
#define DEBOUNCE 500
#define PRELOAD 10

Servo monServo;
int angleServo;

// Boolean to show if pin value has changed
// Because this value is changed in the interrupt, must declare as volatile,
// which tells the compiler to always read this from memory, not a register
volatile int pinChange=0; // Communicates from interrupt routine
volatile bool interr= false;

// Interrupt handler, this is called via the interrupt handler every
// time a change is detected on the button pin.  Best to do very little here.
void pinChangeRoutine() {
    pinChange = !pinChange;
    interr=true;
}

void preload(){
    digitalWrite(8,LOW);
    delay(PRELOAD);
    digitalWrite(8,HIGH);
}

void setup() {
    pinMode(BUTTON_IN, INPUT_PULLUP);
    pinMode(8,OUTPUT);
    monServo.attach(SERVO);

    preload();

    Serial.begin(9600);
    attachInterrupt(digitalPinToInterrupt(BUTTON_IN), pinChangeRoutine, FALLING);
}

void loop() {
    if(interr){
        angleServo = (pinChange?POSITIONA:POSITIONB);
        monServo.write(angleServo);
        delay(DEBOUNCE);
        interr=false;
        preload();
    }
}
