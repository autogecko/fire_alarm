### HW_INFO
- board : arduino wifi uno
- display: 1.44in ST7735 , SPI
- varResist: voltage divider
- bellBtn : digialpin out to on/off pc817
- Button : push button
- ivSensor : analog input
- spk : piezzo speaker

### CONNECTION
#### display
1. display.sck = 13
2. display.mosi = 11
3. display.cs = 10
4. display.dc = 9
5. display.rst = 8
6. diplay.bl = 3.3v
#### other parts
1. btn.plus = gpio4
2. varResist.out = A1
3. bellBtn.plus =  gpio5
4. Button.plus = gpio4
5. ivSensor = A0
6. spk.out = gpio3

### Function Description
1. 기기는 {normal_mode}  과 {emergency_mode} 두 가지 모드로 동작한다.
2.  if ivSensor > ivRef:
    enter {emergency_mode}
### Button description
1. Button2 library 를 사용하라.
2. short press in {normal_mode} 
   - bellBTN 을 3초동한 High
2. short press in {emergency_mode}
   - ivRef 값을 a1 값으로 update 후 3초뒤 enter {normal_mode}
3. longpress  in {setting_mode}
     update ivRef = a1 and  after  3secods later enter {normal_mode}
4. longpress in {normal_mode} 
  enter {setting_mode}
#### normal_mode
1. display: 화면 가운데 "GOOD" 글자, Background : green
2. piezzo: off
3. bellBtn: low
#### emergency_mode
1. display: 화면 가운데 "CAUTION", bliking Background: Red
2. piezzo speaker: 3초 Beep On ,1초 Beep Off 
3. bellBtn: high for 2 seconds
#### setting_mode
1. {Button} longpress  in {setting_mode}
     update ivRef = a1 and  after  3secods later enter {normal_mode}
#### booting
1. display: "Hello" and change colors "RED,YELLOW, BLUE" in 2second once.
2. piezzo  speaker: beep 3 seconds 
### Serial monitoring 
1. 2초간격으로 아래 출력
 - ivValue, ivRef, mode
