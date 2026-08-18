#include "Buzzer.h"

#define BUZZER_PIN 4


void buzzerBegin()
{
    ledcAttach(BUZZER_PIN, 1000, 8);

    ledcWriteTone(BUZZER_PIN, 0);
}


void playTone()
{
    ledcWriteTone(BUZZER_PIN, 1000);
}


void stopTone()
{
    ledcWriteTone(BUZZER_PIN, 0);
}