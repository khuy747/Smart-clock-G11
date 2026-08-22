#include "Buzzer.h"

#define BUZZER_PIN 41

void buzzerBegin() {
    pinMode(BUZZER_PIN, OUTPUT);
    noTone(BUZZER_PIN);
}

void playTone() {
    tone(BUZZER_PIN, 1000);
}

void stopTone() {
    noTone(BUZZER_PIN);
}