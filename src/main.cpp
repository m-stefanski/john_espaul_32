#include <Arduino.h>
#include <TFT_eSPI.h>
#include <Button2.h>

#define BUTTON_1 35
#define BUTTON_2 0

TFT_eSPI tft = TFT_eSPI();

Button2 btnLeft;
Button2 btnRight;

hw_timer_t * timerEraseText = NULL;
hw_timer_t * timerEraseEyes = NULL;

char *papieskieMysli[] = {
    "Tso?",
    "Nie wiem",
    "Nie dali mi nigdy",
    "A mozna",
    "Jak najbardziej",
    "Jeszcze jak!",
    "Okrutnik",
    "Ja wam dam",
    "A po maturze"
};

void drawPapa(){
    tft.fillScreen(TFT_BLACK);
    tft.fillEllipse(70, 280, 70, 140, TFT_WHITE);
    tft.fillEllipse(70, 140, 30, 40, TFT_ORANGE);
    tft.drawCentreString("JP2 8-ball", 70, 0, 2);
}

void fillTextArea(){
    tft.drawCentreString(papieskieMysli[random(9)], 70, 60, 2);
    Serial.println("Filled text area");
}

void eraseTextArea(){
    tft.fillRect(0, 60, 160, 20, TFT_BLACK);
    Serial.println("Erased text area");
}

void IRAM_ATTR onTimerEraseText(){
    eraseTextArea();
}

void createTextTimer(){
    timerEraseText = timerBegin(0, 80, true);
    timerAttachInterrupt(timerEraseText, &onTimerEraseText, true);
    timerAlarmWrite(timerEraseText, 2000000, false);
    timerAlarmEnable(timerEraseText);
    Serial.println("Text timer created");
}

void fillEyesArea(){
    tft.fillEllipse(60, 140, 3, 3, TFT_RED);
    tft.fillEllipse(80, 140, 3, 3, TFT_RED);
    Serial.println("Filled eyes area");
}

void eraseEyesArea(){
    tft.fillEllipse(70, 140, 30, 40, TFT_ORANGE);
    Serial.println("Erased eyes area");
}

void IRAM_ATTR onTimerEraseEyes(){
    eraseEyesArea();
}

void createEyeTimer(){
    timerEraseEyes = timerBegin(1, 80, true);
    timerAttachInterrupt(timerEraseEyes, &onTimerEraseEyes, true);
    timerAlarmWrite(timerEraseEyes, 1000000, false);
    timerAlarmEnable(timerEraseEyes);
    Serial.println("Eye timer created");
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Initialized");
    tft.begin();
    tft.setRotation(0);	

    btnLeft.begin(BUTTON_1);
    btnRight.begin(BUTTON_2);

    btnLeft.setTapHandler([](Button2 & b) {
        eraseTextArea();
        fillTextArea();
        createTextTimer();
    });

    btnRight.setTapHandler([](Button2 & b) {
        fillEyesArea();
        createEyeTimer();
    });

    randomSeed(analogRead(1));

    drawPapa();
}

void loop()
{
    btnLeft.loop();
    btnRight.loop();
}

