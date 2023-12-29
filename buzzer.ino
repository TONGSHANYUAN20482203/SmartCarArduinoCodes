#include "pitches.h"

#define BUZZER_PIN 9

int melody[] = {
  NOTE_C4,NOTE_C4,NOTE_G4,NOTE_G4,NOTE_A5,NOTE_A5,NOTE_G4
  ,NOTE_F4,NOTE_F4,NOTE_E4,NOTE_E4,NOTE_D4,NOTE_D4,NOTE_C4

};

int durations[] = {
4,4,4,4,4,4,2,
4,4,4,4,4,4,2
};

void setup()
{
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop()
{
  int size = sizeof(durations) / sizeof(int);

  for (int note = 0; note < size; note++) {

    int duration = 1000 / durations[note];
    tone(BUZZER_PIN, melody[note], duration);


    int pauseBetweenNotes = duration * 1.30;
    delay(pauseBetweenNotes);
    

    noTone(BUZZER_PIN);
  }
}