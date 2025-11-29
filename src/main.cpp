#include <Arduino.h>
#include "config.h"
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

// 아키텍처 개요
// - 통신: 공유기 없이 ESP-NOW 사용 예정 (브로드캐스트 형태로 "Alarm" 상태 전파)
// - 상태: Normal_mode / Alarm_mode 토글
// - 입력: BTN_PIN (눌림 시 Alarm_mode 진입 트리거)
// - 출력: LED_PIN (Alarm_mode에서 토글)
// - 시리얼: 수신된 "Alarm" 여부 및 현재 모드 로깅

enum class Mode {
  Normal = 0,
  Alarm  = 1
};

static Mode g_mode = Mode::Normal;

enum class RxSig { None = 0, Alarm };
static volatile RxSig g_lastRxSig = RxSig::None;
static uint32_t lastLedToggleMs = 0;
static uint32_t lastLedStatToggleMs = 0;
static const uint8_t kBroadcastAddr[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
static void onDataRecv(const uint8_t *mac, const uint8_t *data, int len);
static void onDataSent(const uint8_t *mac, esp_now_send_status_t status);
static bool initEspNow();
static void sendAlarm();

// 버튼 상태 추적
static bool lastBtnState = true; // PULLUP 기준: true=HIGH(미눌림), false=LOW(눌림)
static uint32_t lastDebounceMs = 0;
static const uint32_t debounceDelayMs = 30;

static bool initEspNow() {
  if (esp_now_init() != ESP_OK) {
    Serial.println("[ESP-NOW] init 실패");
    return false;
  }
  esp_now_register_recv_cb(onDataRecv);
  esp_now_register_send_cb(onDataSent);

  esp_now_peer_info_t peer{};
  memcpy(peer.peer_addr, kBroadcastAddr, 6);
  peer.channel = ESPNOW_CHANNEL;
  peer.encrypt = false;
  peer.ifidx = WIFI_IF_STA;  // STA 인터페이스 사용
  if (esp_now_add_peer(&peer) != ESP_OK) {
    Serial.println("[ESP-NOW] 브로드캐스트 peer 추가 실패");
    return false;
  }
  Serial.println("[ESP-NOW] 초기화 완료");
  return true;
}

static void onDataSent(const uint8_t *mac, esp_now_send_status_t status) {
  // 필요시 로깅(간단 출력)
  // Serial.printf("[ESP-NOW] send status=%d\n", status);
}

static void onDataRecv(const uint8_t *mac, const uint8_t *data, int len) {
  if (len >= (int)strlen(ALARM_MSG) && memcmp(data, ALARM_MSG, strlen(ALARM_MSG)) == 0) {
    g_lastRxSig = RxSig::Alarm;
    Serial.println("[ESP-NOW][RX] 'alarm' 수신");
  }
}

static void sendAlarm() {
  const uint8_t* dst = kBroadcastAddr;
  const uint8_t* payload = reinterpret_cast<const uint8_t*>(ALARM_MSG);
  size_t len = strlen(ALARM_MSG);
  esp_now_send(dst, payload, len);
}

void setup() {
  Serial.begin(SERIAL_BAUD);
  while (!Serial) { ; }

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(LED_STAT_PIN, OUTPUT);

  pinMode(BTN_PIN, INPUT_PULLUP);  // 내부 풀업 사용(눌림=LOW)
  lastBtnState = digitalRead(BTN_PIN);

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP("ESP32-ALARM", "", ESPNOW_CHANNEL, 0);  // 채널 고정(1)
  esp_wifi_set_channel(ESPNOW_CHANNEL, WIFI_SECOND_CHAN_NONE);
  if (!initEspNow()) { Serial.println("[FATAL] ESP-NOW 초기화 실패"); }
  Serial.printf("[NET] AP SSID=ESP32-ALARM, IP=%s\n", WiFi.softAPIP().toString().c_str());

  Serial.println();
  Serial.println("==== ESP32 Alarm Broadcast Project ====");
  Serial.println("- 보드: esp32dev");
  Serial.println("- 프레임워크: Arduino (PlatformIO)");
  Serial.println("- 현재 단계: STEP 1 - 프로젝트 스켈레톤 구성 완료");
  Serial.println("- 다음 단계: STEP 2 - ESP-NOW 브로드캐스트/수신 구현");
  Serial.println("채팅으로 '다음 단계 진행'을 알려주시면 ESP-NOW 구현을 진행합니다.");
  Serial.println("=======================================");
}

void loop() {
  // 1. 수신된 ESP-NOW 신호 처리
  if (g_lastRxSig == RxSig::Alarm) {
    if (g_mode == Mode::Normal) {
      g_mode = Mode::Alarm;
      Serial.println("[모드변경] Normal -> Alarm (신호수신)");
    } else {
      g_mode = Mode::Normal;
      Serial.println("[모드변경] Alarm -> Normal (신호수신)");
    }
  }
  g_lastRxSig = RxSig::None; // 신호 처리 후 플래그 초기화

  // 2. 버튼 입력 처리 (디바운싱)
  bool reading = digitalRead(BTN_PIN);
  if (reading != lastBtnState) {
    lastDebounceMs = millis();
    lastBtnState = reading;
  }

  if ((millis() - lastDebounceMs) > debounceDelayMs) {
    static bool lastStableState = true;
    if (reading != lastStableState) {
      lastStableState = reading;
      if (reading == LOW) { // 버튼이 눌렸을 때 (Falling Edge)
        if (g_mode == Mode::Normal) {
          g_mode = Mode::Alarm;
          Serial.println("[모드변경] Normal -> Alarm (버튼입력)");
        } else {
          g_mode = Mode::Normal;
          Serial.println("[모드변경] Alarm -> Normal (버튼입력)");
        }
        sendAlarm(); // 상태 변경을 다른 장치에 전파
      }
    }
  }

  // 3. 현재 모드에 따른 출력 제어
  if (g_mode == Mode::Alarm) {
    // 알람 모드: 내장 LED 토글, 상태 LED 점멸
    if (millis() - lastLedToggleMs >= LED_TOGGLE_INTERVAL_MS) {
      lastLedToggleMs = millis();
      digitalWrite(LED_PIN, !digitalRead(LED_PIN)); // 내장 LED
    }
    if (millis() - lastLedStatToggleMs >= LED_STAT_BLINK_INTERVAL_MS) {
      lastLedStatToggleMs = millis();
      digitalWrite(LED_STAT_PIN, !digitalRead(LED_STAT_PIN)); // 상태 LED
    }
  } else {
    // 일반 모드: 내장 LED 끔, 상태 LED 켬
    digitalWrite(LED_PIN, LOW);
    digitalWrite(LED_STAT_PIN, HIGH);
  }

  // 4. 주기적인 상태 정보 출력
  static uint32_t lastInfo = 0;
  if (millis() - lastInfo >= STATUS_PRINT_INTERVAL_MS) {
    lastInfo = millis();
    IPAddress ipAP  = WiFi.softAPIP();
    bool btnPressed = (digitalRead(BTN_PIN) == LOW);
    const char* modeStr = (g_mode == Mode::Alarm) ? "ALARM" : "NORMAL";
    
    Serial.printf("[상태] AP=%s, 모드=%s, 버튼=%s\n",
                  ipAP.toString().c_str(),
                  modeStr,
                  btnPressed ? "눌림" : "안눌림");
  }
}
