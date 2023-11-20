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

String command;

// setting PWM properties
#define PWM_BASE_Freq 30000
#define PWM_LOW_Freq 5000
#define M1PWM_CH 1
#define PWM_Bits 8

float freq = PWM_BASE_Freq;

void setup() {
  Serial.begin(115200);

  pinMode(M1PH, OUTPUT);
  digitalWrite(M1PH, HIGH);

  // configure LED PWM functionalitites
  ledcSetup(M1PWM_CH, freq, PWM_Bits);
  ledcAttachPin(M1PWM, M1PWM_CH);
}

void loop() {

  // Réception commande uart
  if (Serial.available()) {
    command = Serial.readStringUntil('\n');
    Serial.println(command);
    int cmd = command.toInt();

    setMotorSpeed(cmd, M1PWM_CH);
  }
}

void setMotorSpeed(int cmd, int no_motor) {
  if (cmd > pow(2, PWM_Bits)){ 
    cmd = pow(2, PWM_Bits);
  }
  if (cmd != 0) {
    // Calcul tu temps haut de la PWM
    float duty = cmd / (pow(2, PWM_Bits) - 1);  //duty cycle
    float periode = 1 / (freq / 1000000);       //période en us
    float delta = periode * duty;               // temps haut en us

    if (delta <= 5) { // Détection des moments haut inférieurs à 5 us  
      periode = 5 / duty;
      float freq_low = (1 / periode) * 1000000;   // Calcul de la nouvelle fréquence

      if (freq_low <= 500) //limite de fréquances basses
        freq_low = 500;

      ledcSetup(no_motor, freq_low, PWM_Bits); // réglage fréquences basses

    } else {
      ledcSetup(no_motor, freq, PWM_Bits); // réglage fréquences hautes
    }

    for (int i = 255; i >= cmd; i--) {
      ledcWrite(no_motor, i);  // rampe décroissante pour que le controleur moteur acroche la commande
    }
  } else {
    ledcWrite(no_motor, 0); // rampe décroissante pour que le controleur moteur acroche la commande
  }
}
