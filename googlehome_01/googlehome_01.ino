#include <M5Stack.h>
#include <Preferences.h>
#include <WiFi.h>
#include <esp8266-google-home-notifier.h>

struct {
    String wifiSsid;
    String wifiPassword;
    String googleHomeDisplayName;
}nvsData;

void setup() {
    M5.begin();

    // NVSに格納されているデータの取得
    nvsData.wifiSsid                 = retrieveNvsData("wifi"        , "ssid");
    nvsData.wifiPassword             = retrieveNvsData("wifi"        , "password");
    nvsData.googleHomeDisplayName    = retrieveNvsData("google-home" , "display-name");

    // WiFi接続
    connectWifi(nvsData.wifiSsid, nvsData.wifiPassword);

    // GoogleHome接続
    makeGoogleHomeTalk(nvsData.googleHomeDisplayName);
}

void loop() {
}

/**
 * NVSに格納されているデータを取得する
 */
String retrieveNvsData(String nameSpace, String key) {
    Preferences preferences;
    bool isReadOnly = true;
    preferences.begin(nameSpace.c_str(), isReadOnly);
    String value = preferences.getString(key.c_str());
    preferences.end();

    return value;
}

/**
 * WiFi接続
 */
void connectWifi(const String &wifiSsid, const String &wifiPassword) {
    M5.Lcd.print("Connecting to WiFi");
    WiFi.begin(wifiSsid.c_str(), wifiPassword.c_str());
    // 接続できるまで最大30秒間試行
    for (int i = 0; WiFi.status() != WL_CONNECTED; i++) {
        if (i >= 30) {
            // 接続できない場合はリセット
            M5.Power.reset();
        }
        delay(1000);
        M5.Lcd.print(".");
    }
}

/**
 * Google Home に喋らせる
 */
void makeGoogleHomeTalk(const String &googleHomeDisplayName) {
    GoogleHomeNotifier ghn;
    ghn.device(googleHomeDisplayName.c_str(), "ja");
    ghn.notify("おはよう");
}
