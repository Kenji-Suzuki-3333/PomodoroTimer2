#include <M5Stack.h>

//ポモドーロ時間設定（集中）
const int activeMm =25;
const int activeSs = 0;

//ポモドーロ時間設定（休憩）
const int breakMm = 5;
const int breakSs = 0;

int targetTime = 0;
int omm = 99;
int oss = 99;

int mm = activeMm;
int ss = activeSs;
bool hasStop = true;
bool hasEnd = false;
bool active = true;

void setup() {

  Serial.begin(115200);

  M5.begin();

  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Lcd.setTextSize(3);

  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextFont(7);
  M5.Lcd.setTextSize(2);

  targetTime = millis() + 1000;

  resetActive();

  Serial.println("START");
}

void loop() {
  // update button state
  M5.update();
  
  if (M5.BtnA.wasPressed()) {
    Serial.println("BtnA wasPressed");
    resetActive();
    hasStop = true;
  }

  if (M5.BtnB.wasPressed()) {
    Serial.println("BtnB wasPressed");
    resetBreak();
    hasStop = true;
  }

  if (M5.BtnC.wasPressed() && !hasStop) {
    Serial.println("BtnC wasPressed 1");
    hasStop = true;
  } else if (M5.BtnC.wasPressed() && hasStop) {
    Serial.println("BtnC wasPressed 2");
    hasStop = false;
  }

  if (targetTime < millis()) {
    targetTime = millis() + 1000;
    Serial.println("1second");

    // 時間停止してない場合
    if (!hasStop) {
      ss--;
      if (ss == -1) {
        // 分秒ともに0なら時間停止＆タイマー終了
        if (mm == 0) {
          ss = 0;
          hasStop = true;
          hasEnd = true;
        } else {
          ss = 59;
          omm = mm;
          mm--;
        }
      }
    }

    //タイマー終了した場合
    if (hasEnd) {
      hasEnd = false;
      hasStop = false;
      if (active) {
        resetBreak();
      } else {
        resetActive();
      }
    }

    // 分の更新
    if (omm != mm) {
      omm = mm;
      drawMinutes(mm);
    }

    // 秒の更新
    if (oss != ss) {
      oss = ss;
      drawIndivator(ss, hasStop);
      drawSeconds(ss);
    }

    M5.update();
  }
}

// 分の表示
void drawMinutes(int mm) {

  M5.Lcd.setTextColor(0x1903);
  M5.Lcd.setCursor( 15, 80);
  M5.Lcd.print("88");
  M5.Lcd.setTextColor(TFT_ORANGE);

  if (mm < 10) {
    M5.Lcd.setCursor( 15, 80);
    M5.Lcd.print("0");
    M5.Lcd.print(mm);
  } else {
    M5.Lcd.setCursor( 15, 80);
    M5.Lcd.print(mm);
  }
}

// 秒の表示
void drawSeconds(int ss) {

  M5.Lcd.setTextColor(0x1903);
  M5.Lcd.setCursor( 180, 80);
  M5.Lcd.print("88");
  M5.Lcd.setTextColor(TFT_ORANGE);

  if (ss < 10) {
    M5.Lcd.setCursor( 180, 80);
    M5.Lcd.print("0");
    M5.Lcd.print(ss);
  } else {
    M5.Lcd.setCursor( 180, 80);
    M5.Lcd.print(ss);
  }
}

// インジケーターの表示
void drawIndivator(int ss, bool hasStop) {
  if (ss % 2 && !hasStop) {
    M5.Lcd.setCursor( 150, 80);
    M5.Lcd.setTextColor(0x1903);
    M5.Lcd.print(":");
    M5.Lcd.setTextColor(TFT_ORANGE);
  } else {
    M5.Lcd.setCursor( 150, 80);
    M5.Lcd.print(":");
  }
}

//集中に切り替え
void resetActive() {

  M5.Lcd.clear();
  M5.Lcd.drawJpgFile(SD, "/active.jpg");

  active = true;
  mm = activeMm;
  ss = activeSs;
  drawSeconds(ss);
  drawMinutes(mm);

  M5.update();

}

//休憩に切り替え
void resetBreak() {

  M5.Lcd.clear();
  M5.Lcd.drawJpgFile(SD, "/break.jpg");

  active = false;
  mm = breakMm;
  ss = breakSs;
  drawSeconds(ss);
  drawMinutes(mm);

  M5.update();

}