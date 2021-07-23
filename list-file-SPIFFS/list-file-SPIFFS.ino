#include <M5Stack.h>


void setup() {
    M5.begin();
    SPIFFS.begin();

    M5.Lcd.println("start.");
    M5.Lcd.println();

    // ルート直下のファイルを確認 (無用なファイルがアップされてないか確認)
    File root = SPIFFS.open("/");

    File file = root.openNextFile();
    while(file){
        M5.Lcd.println(file.name());
        file = root.openNextFile();
    }

    M5.Lcd.println();
    M5.Lcd.println("finish.");

    SPIFFS.end();
}

void loop() {
}