#include <FastLED.h>
#include "driver/adc.h"
#include "esp_adc_cal.h"


#define M1PWM 4
#define M2PWM 5
#define M3PWM 6
#define M4PWM 7

#define SDA 8
#define SCL 9

#define M1PH 11
#define M2PH 12
#define M3PH 13
#define M4PH 14

// the number of the LED pin
const int ledPin = M3PWM;  // 16 corresponds to GPIO16

String command;

// setting PWM properties
const int freq = 28000;
const int ledChannel = 0;
const int resolution = 8;


void setup() {
  Serial.begin(115200);
  
  pinMode(M3PH, OUTPUT);
  digitalWrite(M3PH, HIGH);
  
  // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPin, ledChannel);
  ledcAttachPin(M1PWM, ledChannel);
}

void loop() {
  if (Serial.available()) {
    command = Serial.readStringUntil('\n');
    Serial.println(command);
    //ledcWrite(ledChannel,command.toInt());


    int cmd = command.toInt();
    
    double duty    = cmd * 100 / 255;
    double periode = 1/double(freq/10000);
    double delta = periode * (duty/100);
    Serial.println(duty);
    Serial.println(periode);
    Serial.println(delta);
    for(int i = 255 ; i>=cmd ; i--){
      
      ledcWrite(ledChannel,i);
    }
  }
//  ledcWrite(ledChannel, 255);
//  for(int dutyCycle = 15; dutyCycle <= 255; dutyCycle++){   
//    // changing the LED brightness with PWM
//    ledcWrite(ledChannel, dutyCycle);
//    delay(50);
//  }
//
//   
//  // decrease the LED brightness
//  for(int dutyCycle = 255; dutyCycle >= 15; dutyCycle--){
//    // changing the LED brightness with PWM
//    ledcWrite(ledChannel, dutyCycle);   
//    delay(50);
//  }
}
