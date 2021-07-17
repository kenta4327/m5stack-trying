#include <M5Stack.h>

bool skipFlg1Press   = false;
bool skipFlg1        = false;
bool skipFlg2Press   = false;
bool skipFlg2        = false;
bool skipFlg3Press   = false;
bool skipFlg3        = false;
bool skipFlg4        = false;
bool skipFlg5Release = false;
bool skipFlg5        = false;
int  pressedForCnt   = 0;

void correctProcessing(String nextQuestion) {
    M5.Lcd.println();
    M5.Lcd.println("correct!!!");
    M5.Lcd.println();
    M5.Lcd.println("One moment, please.");
    delay(5000);

    // 画面クリア
    M5.Lcd.clear(BLACK);
    M5.Lcd.setCursor(0, 0);

    M5.Lcd.println(nextQuestion);
}

void fromTheBeginning() {
    skipFlg1Press   = false;
    skipFlg1        = false;
    skipFlg2Press   = false;
    skipFlg2        = false;
    skipFlg3Press   = false;
    skipFlg3        = false;
    skipFlg4        = false;
    skipFlg5Release = false;
    skipFlg5        = false;
    pressedForCnt   = 0;

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

    if (!skipFlg1Press) {
        // readの瞬間に押されていた
        if (M5.BtnA.isPressed()) {
            M5.Lcd.println("BtnA.isPressed()");
            skipFlg1Press = true;
        }
    }

    if (skipFlg1Press && !skipFlg1) {
        // readの瞬間は離れていた
        if (M5.BtnA.isReleased()) {
            M5.Lcd.println("BtnA.isReleased()");
            skipFlg1 = true;
            correctProcessing("Press button A for more than 5 seconds.");
        }
    }

    if (skipFlg1 && !skipFlg2Press) {
        // 前回のread時に押された (変化があった)
        if (M5.BtnA.wasPressed()) {
            M5.Lcd.println("BtnA.wasPressed()");
            skipFlg2Press = true;
        }
    }

    if (skipFlg1 && skipFlg2Press && !skipFlg2) {
        // 前回のread時に離れた (押され始めたときから 引数 ミリ秒より多かったとき)
        if (M5.BtnA.wasReleasefor(5000)) {
            M5.Lcd.println("BtnA.wasReleasefor(5000)");
            skipFlg2 = true;
            correctProcessing("Press button A for more than 10 seconds.");
        }
    }

    if (skipFlg1 && skipFlg2 && !skipFlg3) {

        if (!skipFlg3Press) {
            // 引数 ミリ秒以上押されてる間
            if (M5.BtnA.pressedFor(5000)) {
                M5.Lcd.println("BtnA.pressedFor(5000)");
                M5.Lcd.println("A little more.");
                skipFlg3Press = true;
            }
        } 

        // 引数 ミリ秒以上押されてる間 かつ 前回このメソッドが呼ばれてから 引数2 ミリ秒以上たった
        if (M5.BtnA.pressedFor(5000, 5000)) {
            pressedForCnt++;
            if (pressedForCnt > 1) {
                M5.Lcd.println("BtnA.pressedFor(5000, 5000)");
                skipFlg3 = true;
                correctProcessing("Release button A in less than 5 seconds.");
            }
        }
    }

    if (skipFlg1 && skipFlg2 && skipFlg3 && !skipFlg4) {
        // 前回のread時に離れた (上より短いタイムのとき)
        // wasReleasefor() が呼ばれない限りは -1 未満じゃないとだめなのここ通らなそう
        if (M5.BtnA.wasReleased()) {
            M5.Lcd.println("BtnA.wasReleased()");
            skipFlg4 = true;
            correctProcessing("Please release the button and wait a moment.");
        }
    }

    if (skipFlg1 && skipFlg2 && skipFlg3 && skipFlg4 && !skipFlg5) {

        if (!skipFlg5Release) {
            // 引数 ミリ秒以上離れてる間
            if (M5.BtnA.releasedFor(9000)) {
                M5.Lcd.println("BtnA.releasedFor(9000)");
                skipFlg5Release = true;
                M5.Lcd.println();
                M5.Lcd.println("Press button A.");
            }
        }

        if (skipFlg5Release) {
            if (M5.BtnA.wasReleasefor(0)) {
                skipFlg5 = true;
                correctProcessing("the end. Press button B to reset.");
            }
        }
    }

    if (M5.BtnB.wasReleasefor(0)) {
        fromTheBeginning();
    }
}
