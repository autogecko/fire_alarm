### HW_INFO
- board : esp32dev (ESP32-DevKit)
- btn_mode : alarm_mode, normal_mode toggle 용 button
- temp: dht11
- spk : piezo speaker
- sen_gas: MQ7 (ADC 12-bit: 0~4095)
- sw_laser: laser 전원 switching 을 위한 digital port
- sw_motor: motor 전원 switching 을 위한 digital port
- led_stat: 상태 LED
- note: DHT11 데이터 핀은 문서에 미기재 → 본 프로젝트에서는 gpio4 사용
### HW_CONNECTION
1. btn_mode.plus = gpio4
2. spk.out = gpio5
3. sen_gas.aout = gpio25
4. led_stat.plus = gpio13
5. temp.a0= gpio4
### Function Description
1. 주기성 처리는 비차단 방식(millis) 타이머로 구현
2. 두 대의 esp32 가  wifi 를 이용해 broadcast 한다.(ESP-NOW 사용)
3. 공유기 없이 broadcast 로 상황을 전파한다.
4. dht11.temperature > 30 또는 gas 원시값 > 600 이면 {Alarm mode} 진입, 
5. btn_mode 가 눌러지면 {Normal_mode} ,{Alarm_mode} toggle 됨.
6. 현재 mode 와 비교해서 다른 상태 이면 상태를 바꾼다.
   ex) 현재 {normal_mode} 인데 'normal_mode' 신호 받음 --> 상태 안바뀜.
        'Alarm' 신호 받음 --> {Alarm_mode} 상태 변경 
### Alarm_mode
0. "Alarm" 상태 값을 전달 받으면 {Alarm_mode} 이다. 
1. 내장 LED를 Toggle 한다.
2. {Alarm_mode} 상태에서 "Alarm" 신호를 받으면 {Nomrmal_mode} 로 toggle 된다.
3. "Alarm" 이란 상태값 을 전파한다.
#### Alarm_mode_action
1. spk : 1초마다 beep 반복
2. led_stat: 0.5초마다 blinking
3. sw_laser : HIGH
4. sw_motor: HIGH
### Normal_mode
0. "normal" 상태 값을 전달 받으면 {Normal_mode} 이다. 
1. led_stat : 계속 ON 상태  
2. {Alarm_mode} 상태에서 "Alarm" 신호를 받으면 {Nomrmal_mode} 로 toggle 된다.
3. "normal" 이란 상태값 을 전파한다.
#### Normal_mode_action
1. led_stat :  ON
2. beep, sw_laswer, sw_motor : off
### Serial monitoring 
1. IPAddress, mode, btn 값  제목과 함께 출력  for every 1 seconds.
