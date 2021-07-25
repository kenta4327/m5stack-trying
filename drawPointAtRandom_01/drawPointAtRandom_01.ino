#include <M5Stack.h>

void setup() {
    M5.begin();
}

void loop() {

    int x = random(320);
    int y = random(240);
    int color = random(0x10000); // Random colour

    M5.Lcd.fillEllipse(x, y, 2, 2, color);

    delay(500);
}
