#pragma once

// 핀 설정 (보드에 맞춰 필요시 조정)
#ifndef LED_PIN
#define LED_PIN 2           // ESP32-DevKit 내장 LED (일반적으로 GPIO2)
#endif

#ifndef BTN_PIN
#define BTN_PIN 12    // hw_description.md: btn_mode.plus = gpio12
#endif

// hw_description.md 에 따른 추가 핀 설정
#define LED_STAT_PIN 13
#define SPK_PIN 5
#define GAS_SENSOR_PIN 18 // hw_description.md: sen_gas.aout = gpio18
// Note: gpio4 충돌이 해결되었습니다. btn_mode는 gpio12로, temp 센서는 gpio4로 할당되었습니다.
#define TEMP_SENSOR_PIN 4 // hw_description.md: temp.a0= gpio4
// Note: hw_description.md 업데이트로 gpio12 충돌이 해결되었습니다.
#define SW_LASER_PIN 21 // hw_description.md: sw_laser.plus = gpio21
#define SW_MOTOR_PIN 23 // hw_description.md: sw_motor.plus = gpio23

// 시리얼 설정
#ifndef SERIAL_BAUD
#define SERIAL_BAUD 115200
#define STATUS_PRINT_INTERVAL_MS 1000
#endif

// 개발 단계 관리 (아키텍트 프로세스)
#define STEP_1_INITIALIZED 1
// 다음 단계로 진행하려면 채팅으로 승인 후, STEP_2_IMPLEMENT_ESP_NOW를 코드에서 활성화 예정
#define STEP_2_IMPLEMENT_ESP_NOW 1
#define ESPNOW_CHANNEL 1
#define LED_TOGGLE_INTERVAL_MS 300
#define LED_STAT_BLINK_INTERVAL_MS 500
#define MODE_BROADCAST_INTERVAL_MS 500
#define SENSOR_READ_INTERVAL_MS 2000
#define BEEP_INTERVAL_MS 1000
#define GAS_THRESHOLD 600
#define ALARM_MSG "alarm"
#define NORMAL_MSG "normal"
// 참고: BTN_PIN은 32(PULLUP)로 유지
