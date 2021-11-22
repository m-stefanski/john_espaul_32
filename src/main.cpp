#include <Arduino.h>
#include <TFT_eSPI.h>
#include <Button2.h>

#define BUTTON_1 35
#define BUTTON_2 0

TFT_eSPI tft = TFT_eSPI();

Button2 btn1;
Button2 btn2;

hw_timer_t * timer = NULL;
volatile byte state = LOW;

char *papieskieMysli[] = {
    "Tso?",
    "Nie wiem",
    "Nie dali mi nigdy",
    "A mozna",
    "Jak najbardziej",
    "Jeszcze jak!",
    "Okrutnik"
};

void IRAM_ATTR onTimer(){
  state = !state;
  if(state){
    tft.drawCentreString("...", 70, 0, 2);
  }
  else {
    tft.drawCentreString("   ", 70, 0, 2);
  }
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Initialized");
    tft.begin();
    tft.setRotation(0);	
    tft.fillScreen(TFT_BLACK);
    tft.setSwapBytes(true);

    tft.fillEllipse(70, 280, 70, 140, TFT_WHITE);
    tft.fillEllipse(70, 140, 30, 40, TFT_ORANGE);

    btn1.begin(BUTTON_1);
    btn2.begin(BUTTON_2);

    btn1.setTapHandler([](Button2 & b) {
        tft.drawCentreString(papieskieMysli[random(7)], 70, 60, 2);
        delay(2000);
        tft.fillRect(0, 60, 160, 20, TFT_BLACK);
    });

    btn2.setTapHandler([](Button2 & b) {
        tft.fillEllipse(60, 140, 3, 3, TFT_RED);
        tft.fillEllipse(80, 140, 3, 3, TFT_RED);
        delay(1000);
        tft.fillEllipse(70, 140, 30, 40, TFT_ORANGE);
    });

    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, 1000000, true);
    timerAlarmEnable(timer);

    randomSeed(analogRead(1));
}

void loop()
{
    btn1.loop();
    btn2.loop();
}

