#include <M5Stack.h>

bool skip_flg_1_press   = false;
bool skip_flg_1         = false;
bool skip_flg_2_press   = false;
bool skip_flg_2         = false;
bool skip_flg_3_press   = false;
bool skip_flg_3         = false;
bool skip_flg_4         = false;
bool skip_flg_5_release = false;
bool skip_flg_5         = false;
int  pressedForCnt      = 0;

void correctProcessing(String next_question) {
    M5.Lcd.println();
    M5.Lcd.println("correct!!!");
    M5.Lcd.println();
    M5.Lcd.println("One moment, please.");
    delay(5000);

    // 画面クリア
    M5.Lcd.clear(BLACK);
    M5.Lcd.setCursor(0, 0);

    M5.Lcd.println(next_question);
}

void fromTheBeginning() {
    skip_flg_1_press   = false;
    skip_flg_1         = false;
    skip_flg_2_press   = false;
    skip_flg_2         = false;
    skip_flg_3_press   = false;
    skip_flg_3         = false;
    skip_flg_4         = false;
    skip_flg_5_release = false;
    skip_flg_5         = false;
    pressedForCnt      = 0;

    // 画面クリア
    M5.Lcd.clear(BLACK);
    M5.Lcd.setCursor(0, 0);

    M5.Lcd.println("Press button A.");
}

void setup() {
    M5.begin();

    M5.Lcd.println("Press button A.");
}

void loop() {

    // ボタン状況読み取り
    M5.update();

    if (!skip_flg_1_press) {
        // readの瞬間に押されていた
        if (M5.BtnA.isPressed()) {
            M5.Lcd.println("BtnA.isPressed()");
            skip_flg_1_press = true;
        }
    }

    if (skip_flg_1_press && !skip_flg_1) {
        // readの瞬間は離れていた
        if (M5.BtnA.isReleased()) {
            M5.Lcd.println("BtnA.isReleased()");
            skip_flg_1 = true;
            correctProcessing("Press button A for more than 5 seconds.");
        }
    }

    if (skip_flg_1 && !skip_flg_2_press) {
        // 前回のread時に押された (変化があった)
        if (M5.BtnA.wasPressed()) {
            M5.Lcd.println("BtnA.wasPressed()");
            skip_flg_2_press = true;
        }
    }

    if (skip_flg_1 && skip_flg_2_press && !skip_flg_2) {
        // 前回のread時に離れた (押され始めたときから 引数 ミリ秒より多かったとき)
        if (M5.BtnA.wasReleasefor(5000)) {
            M5.Lcd.println("BtnA.wasReleasefor(5000)");
            skip_flg_2 = true;
            correctProcessing("Press button A for more than 10 seconds.");
        }
    }

    if (skip_flg_1 && skip_flg_2 && !skip_flg_3) {

        if (!skip_flg_3_press) {
            // 引数 ミリ秒以上押されてる間
            if (M5.BtnA.pressedFor(5000)) {
                M5.Lcd.println("BtnA.pressedFor(5000)");
                M5.Lcd.println("A little more.");
                skip_flg_3_press = true;
            }
        } 

        // 引数 ミリ秒以上押されてる間 かつ 前回このメソッドが呼ばれてから 引数2 ミリ秒以上たった
        if (M5.BtnA.pressedFor(5000, 5000)) {
            pressedForCnt++;
            if (pressedForCnt > 1) {
                M5.Lcd.println("BtnA.pressedFor(5000, 5000)");
                skip_flg_3 = true;
                correctProcessing("Release button A in less than 5 seconds.");
            }
        }
    }

    if (skip_flg_1 && skip_flg_2 && skip_flg_3 && !skip_flg_4) { 
        // 前回のread時に離れた (上より短いタイムのとき)
        // wasReleasefor() が呼ばれない限りは -1 未満じゃないとだめなのここ通らなそう
        if (M5.BtnA.wasReleased()) {
            M5.Lcd.println("BtnA.wasReleased()");
            skip_flg_4 = true;
            correctProcessing("Please release the button and wait a moment.");
        }
    }

    if (skip_flg_1 && skip_flg_2 && skip_flg_3 && skip_flg_4 && !skip_flg_5) {

        if (!skip_flg_5_release) {
            // 引数 ミリ秒以上離れてる間
            if (M5.BtnA.releasedFor(9000)) {
                M5.Lcd.println("BtnA.releasedFor(9000)");
                skip_flg_5_release = true;
                M5.Lcd.println();
                M5.Lcd.println("Press button A.");
            }
        }

        if (skip_flg_5_release) {
            if (M5.BtnA.wasReleasefor(0)) {
                skip_flg_5 = true;
                correctProcessing("the end. Press button B to reset.");
            }
        }
    }

    if (M5.BtnB.wasReleasefor(0)) {
        fromTheBeginning();
    }
}
