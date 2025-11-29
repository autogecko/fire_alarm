# TODO List (Architect Process)

## 단계 개요
1) 프로젝트 스켈레톤 구성 및 절차 수립
2) ESP-NOW 브로드캐스트/수신 초기화
3) 버튼 입력 처리(디바운스) 및 상태 전이
4) 상태 머신 구현: Normal_mode <-> Alarm_mode
5) Alarm_mode LED 토글 구현
6) Alarm 상태 브로드캐스트 로직 구현
7) 시리얼 모니터링 메시지(Alarm 수신 여부, 현재 모드) 출력
8) 설정값 분리 및 주석 정리
9) 통합 테스트 및 핫픽스

## 체크리스트
- [x] 1. 프로젝트 초기화 및 아키텍처 정의 (platformio.ini, 기본 구조)
- [x] 2. 기본 스켈레톤 코드 작성 (src/main.cpp, include/config.h)
- [x] BTN 핀(GPIO34) 반영 및 초기화 동기화
- [x] BTN 핀(GPIO32)로 변경 및 내부 풀업 사용
- [x] 2초 주기 IP 및 버튼 상태 시리얼 출력 구현
- [x] 3. ESP-NOW 통신 초기화 및 브로드캐스트 프레임 설계
- [x] 4. 수신 콜백 구현: "Alarm" 패킷 처리 및 상태 전이 (Normal->Alarm, Alarm에서 다시 "Alarm" 수신 시 Normal로 토글)
- [x] 5. 송신 로직 구현: Alarm_mode에서 주기적 브로드캐스트
- [x] 버튼 눌림 시 'ALARM' 1회 송신으로 변경(주기 브로드캐스트 제거)
- [x] 6. 버튼 입력 처리 및 디바운싱 정교화 + 상태 전이 연동
- [x] 7. LED 토글 구현 (Alarm_mode에서만)
- [x] 8. 시리얼 모니터링: 현재 "Alarm" 수신 여부/현재 모드 주기적 출력
- [x] 9. 설정값/상수 분리, 주석 보강, 에러 처리
- [x] MODE 기반 주기 브로드캐스트('normal'/'alarm') 구현
- [x] BTN_ACTION: 현재 모드 반대 신호 1회 전파(자기 모드 변경 없음)
- [x] 수신 시 모드 동기화('normal' 수신→Normal, 'alarm' 수신→Alarm)
- [x] MODE_ACTION: 두 모드 모두 LED 토글
- [x] 시리얼: mode state, IP address, signal from other device(LAST_RX) 출력
- [ ] 10. 통합 테스트 및 문제 해결
- [x] MODE_ACTION 업데이트: normal_mode→LED TOGGLE + "alarm" 전파, alarm_mode→LED TOGGLE + "normal" 전파
- [x] BTN_ACTION 업데이트: normal_mode에서 버튼→ alarm_mode 전환, alarm_mode에서 버튼→ "normal" 1회 전파
- [x] 수신 신호는 모드 동기화하지 않고 로그/표시만 수행

## 사용자 확인 포인트
- [ ] 통합 테스트(보드 2대 동시 실행) 진행할까요? 문제 발생 시 로그/증상 공유해 주세요.
  - 진행 승인 시: STEP 2 구현을 위한 파일/코드(src/ 및 include/에 모듈 추가)를 작성하고 체크리스트를 업데이트합니다.

테스트 가이드
- 보드 A/B 각각 업로드 후 모니터 실행.
- 정상 동작 기대
  - 각 보드는 500ms 주기로 자신의 모드 문자열을 브로드캐스트("normal" 또는 "alarm").
  - 버튼을 누르면 현재 모드의 반대 문자열을 1회 전송(자기 모드 유지).
  - 상대 보드는 해당 신호 수신 즉시 모드 동기화.
  - LED는 양쪽 모두 모드와 무관하게 주기 토글.
  - 2초마다 [STATUS] … MODE=…, LAST_RX=alarm/normal/- 출력.
