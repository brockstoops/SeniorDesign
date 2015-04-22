#include <EEPROM.h>
#include <SPI.h>
#include <GD2.h>

int screen = 0;
int lockernum = 0;
boolean existing = false;
boolean recreate = false;
boolean l1used = false;
boolean l2used = false;
boolean l3used = false;
boolean l4used = false;
int pinlength = 0;
int solenoid1 = 0;
int solenoid2 = 1;
int solenoid3 = 3;
int solenoid4 = 4;
String pinone = "";
String pintwo = "";
String pinthree = "";
String pinfour = "";
int wrongpincnt = 0;
boolean leds[8];
int clockPin = 7;
int latchPin = 6;
int dataPin = 5;
int sensorPin1 = A2;
int sensorPin2 = A3;
int sensorPin3 = A1;
int sensorPin4 = A0;
long sensorValue1 = 0;
long sensorValue2= 0;
long sensorValue3 = 0;
long sensorValue4 = 0;
long previousMillis = 0;
long interval = 1000;

void setup()
{
  pinMode(solenoid1, OUTPUT);
  pinMode(solenoid2, OUTPUT);
  pinMode(solenoid3, OUTPUT);
  pinMode(solenoid4, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
  GD.begin();
}

#define DARK_GREEN      0x007000
#define LIGHT_GREEN     0x33ff33
#define DARK_RED        0x700000
#define LIGHT_RED       0xff3333
#define DARK_YELLOW     0x707000
#define LIGHT_YELLOW    0xffff33
#define DARK_BLUE       0x007070
#define LIGHT_BLUE      0x33ffff

void drawscreen(int screen, int pressed)
{
  if(screen == 0)
    mainscreen(pressed);
  else if(screen == 1)
    LockerNumberscreen(pressed);
  else if (screen == 2)
    PinLockerScreen(pressed);
  else if(screen == 10)
    startScreen(pressed);
}

void play(int pressed)
{
  for (int i = 0; i < 10; i++)
    drawscreen(screen, pressed);
  for (int i = 0; i < 2; i++)
    drawscreen(screen, 0);
}

void LockerNumberscreen(int pressed){
  
  GD.get_inputs();
  GD.Clear();
  
  GD.cmd_text(240, 40, 25, OPT_CENTER, "Please select your locker number");

  GD.Begin(RECTS);
  GD.Tag(5);
  if (pressed == 5 || pressed == 6)
    GD.ColorRGB(LIGHT_GREEN);
  else
    GD.ColorRGB(DARK_GREEN);
  GD.Vertex2ii(175, 100);
  GD.Vertex2ii(250, 175);

  GD.Tag(7);
  if (pressed == 7 || pressed == 8)
    GD.ColorRGB(LIGHT_RED);
  else
    GD.ColorRGB(DARK_RED);
  GD.Vertex2ii(260, 100);
  GD.Vertex2ii(335, 175);
  
  GD.Tag(9);
  if (pressed == 9 || pressed == 10)
    GD.ColorRGB(LIGHT_YELLOW);
  else
    GD.ColorRGB(DARK_YELLOW);
  GD.Vertex2ii(175, 185);
  GD.Vertex2ii(250, 260);

  GD.Tag(11);
  if (pressed == 11 || pressed == 12)
    GD.ColorRGB(LIGHT_BLUE);
  else
    GD.ColorRGB(DARK_BLUE);
  GD.Vertex2ii(260, 185);
  GD.Vertex2ii(335, 260);
  
  GD.ColorRGB(0xffffff);
  GD.Tag(6);
  GD.cmd_text(215, 135, 25, OPT_CENTER, "1");
  GD.Tag(8);
  GD.cmd_text(292, 135, 25, OPT_CENTER, "2");
  GD.Tag(10);
  GD.cmd_text(217, 223, 25, OPT_CENTER, "3");
  GD.Tag(12);
  GD.cmd_text(291, 223, 25, OPT_CENTER, "4");

  GD.swap();
}

void PinLockerScreen(int pressed){
  
  GD.get_inputs();
  GD.Clear();
  
  if(existing){
    if(lockernum == 1)
      GD.cmd_text(240, 40, 25, OPT_CENTER, "Please enter the PIN for locker #1");
    else if(lockernum == 2)
      GD.cmd_text(240, 40, 25, OPT_CENTER, "Please enter the PIN for locker #2");
    else if(lockernum == 3)
      GD.cmd_text(240, 40, 25, OPT_CENTER, "Please enter the PIN for locker #3");
    else if(lockernum == 4)
      GD.cmd_text(240, 40, 25, OPT_CENTER, "Please enter the PIN for locker #4");
  }
  else if(!existing && !recreate){
     if(lockernum == 1)
      GD.cmd_text(240, 40, 25, OPT_CENTER, "Please create a PIN for locker #1");
    else if(lockernum == 2)
      GD.cmd_text(240, 40, 25, OPT_CENTER, "Please create a PIN for locker #2");
    else if(lockernum == 3)
      GD.cmd_text(240, 40, 25, OPT_CENTER, "Please create a PIN for locker #3");
    else if(lockernum == 4)
      GD.cmd_text(240, 40, 25, OPT_CENTER, "Please create a PIN for locker #4");
  }
  else{
     if(lockernum == 1)
      GD.cmd_text(240, 40, 25, OPT_CENTER, "Please re-enter PIN for locker #1");
    else if(lockernum == 2)
      GD.cmd_text(240, 40, 25, OPT_CENTER, "Please re-enter PIN for locker #2");
    else if(lockernum == 3)
      GD.cmd_text(240, 40, 25, OPT_CENTER, "Please re-enter PIN for locker #3");
    else if(lockernum == 4)
      GD.cmd_text(240, 40, 25, OPT_CENTER, "Please re-enter PIN for locker #4");
  }

  GD.Begin(RECTS);
  GD.Tag(150);
  GD.ColorRGB(DARK_YELLOW);
  GD.Vertex2ii(120, 65);
  GD.Vertex2ii(360, 90);
  
  GD.Tag(13);
  if (pressed == 13 || pressed == 14)
    GD.ColorRGB(LIGHT_BLUE);
  else
    GD.ColorRGB(DARK_BLUE);
  GD.Vertex2ii(185, 100);
  GD.Vertex2ii(235, 150);

  GD.Tag(15);
  if (pressed == 15 || pressed == 16)
    GD.ColorRGB(LIGHT_BLUE);
  else
    GD.ColorRGB(DARK_BLUE);
  GD.Vertex2ii(245, 100);
  GD.Vertex2ii(295, 150);
  
  GD.Tag(17);
  if (pressed == 17 || pressed == 18)
    GD.ColorRGB(LIGHT_BLUE);
  else
    GD.ColorRGB(DARK_BLUE);
  GD.Vertex2ii(305, 100);
  GD.Vertex2ii(355, 150);

  GD.Tag(19);
  if (pressed == 19 || pressed == 20)
    GD.ColorRGB(LIGHT_BLUE);
  else
    GD.ColorRGB(DARK_BLUE);
  GD.Vertex2ii(185, 160);
  GD.Vertex2ii(235, 210);
  
  GD.Tag(21);
  if (pressed == 21 || pressed == 22)
    GD.ColorRGB(LIGHT_BLUE);
  else
    GD.ColorRGB(DARK_BLUE);
  GD.Vertex2ii(245, 160);
  GD.Vertex2ii(295, 210);

  GD.Tag(23);
  if (pressed == 23 || pressed == 24)
    GD.ColorRGB(LIGHT_BLUE);
  else
    GD.ColorRGB(DARK_BLUE);
  GD.Vertex2ii(305, 160);
  GD.Vertex2ii(355, 210);
  
  GD.Tag(25);
  if (pressed == 25 || pressed == 26)
    GD.ColorRGB(LIGHT_BLUE);
  else
    GD.ColorRGB(DARK_BLUE);
  GD.Vertex2ii(185, 220);
  GD.Vertex2ii(235, 270);

  GD.Tag(27);
  if (pressed == 27 || pressed == 28)
    GD.ColorRGB(LIGHT_BLUE);
  else
    GD.ColorRGB(DARK_BLUE);
  GD.Vertex2ii(245, 220);
  GD.Vertex2ii(295, 270);
  
  GD.Tag(29);
  if (pressed == 29 || pressed == 30)
    GD.ColorRGB(LIGHT_BLUE);
  else
    GD.ColorRGB(DARK_BLUE);
  GD.Vertex2ii(305, 220);
  GD.Vertex2ii(355, 270);

  GD.Tag(31);
  if (pressed == 31 || pressed == 32)
    GD.ColorRGB(LIGHT_BLUE);
  else
    GD.ColorRGB(DARK_BLUE);
  GD.Vertex2ii(125, 100);
  GD.Vertex2ii(175, 150);
  
  GD.Tag(33);
  if (pressed == 33 || pressed == 34)
    GD.ColorRGB(LIGHT_RED);
  else
    GD.ColorRGB(DARK_RED);
  GD.Vertex2ii(125, 160);
  GD.Vertex2ii(175, 210);

  GD.Tag(35);
  if (pressed == 35 || pressed == 36)
    GD.ColorRGB(LIGHT_GREEN);
  else
    GD.ColorRGB(DARK_GREEN);
  GD.Vertex2ii(125, 220);
  GD.Vertex2ii(175, 270);
  
  GD.ColorRGB(0xffffff);
  GD.Tag(14);
  GD.cmd_text(210, 125, 25, OPT_CENTER, "1");
  GD.Tag(16);
  GD.cmd_text(270, 125, 25, OPT_CENTER, "2");
  GD.Tag(18);
  GD.cmd_text(330, 125, 25, OPT_CENTER, "3");
  GD.Tag(20);
  GD.cmd_text(210, 185, 25, OPT_CENTER, "4");
  GD.Tag(22);
  GD.cmd_text(270, 185, 25, OPT_CENTER, "5");
  GD.Tag(24);
  GD.cmd_text(330, 185, 25, OPT_CENTER, "6");
  GD.Tag(26);
  GD.cmd_text(210, 245, 25, OPT_CENTER, "7");
  GD.Tag(28);
  GD.cmd_text(270, 245, 25, OPT_CENTER, "8");
  GD.Tag(30);
  GD.cmd_text(330, 245, 25, OPT_CENTER, "9");
  GD.Tag(32);
  GD.cmd_text(150, 125, 25, OPT_CENTER, "0");
  GD.Tag(34);
  GD.cmd_text(150, 185, 25, OPT_CENTER, "C");
  GD.Tag(36);
  GD.cmd_text(150, 245, 25, OPT_CENTER, "E");
  
  GD.Tag(151);
  GD.ColorRGB(0xffffff);
  if(pinlength > 0){
   if(pinlength == 1)
     GD.cmd_text(340, 90, 25, OPT_CENTER, "*");
   else if(pinlength == 2)
     GD.cmd_text(332, 90, 25, OPT_CENTER, "**");
   else if(pinlength == 3)
     GD.cmd_text(326, 90, 25, OPT_CENTER, "***");
   else if(pinlength == 4)
     GD.cmd_text(319, 90, 25, OPT_CENTER, "****");
  }

  GD.swap();
}

void mainscreen(int pressed)
{
  GD.get_inputs();
  GD.Clear();
  
  GD.cmd_text(240, 40, 25, OPT_CENTER, "Do you have an existing locker?");

  GD.Begin(RECTS);
  GD.Tag(1);
  if (pressed == 1 || pressed == 2)
    GD.ColorRGB(LIGHT_GREEN);
  else
    GD.ColorRGB(DARK_GREEN);
  GD.Vertex2ii(100, 100);
  GD.Vertex2ii(200, 200);

  GD.Tag(3);
  if (pressed == 4 || pressed == 3)
    GD.ColorRGB(LIGHT_RED);
  else
    GD.ColorRGB(DARK_RED);
  GD.Vertex2ii(250, 100);
  GD.Vertex2ii(350, 200);
  
  GD.ColorRGB(0xffffff);
  GD.Tag(2);
  GD.cmd_text(150, 150, 25, OPT_CENTER, "YES");
  GD.Tag(4);
  GD.cmd_text(300, 150, 25, OPT_CENTER, "NO");

  GD.swap();
}

void startScreen(int pressed)
{
  GD.get_inputs();
  GD.Clear();
  
  GD.Begin(RECTS);
  GD.Tag(100);
  if (pressed == 100 || pressed == 101)
    GD.ColorRGB(LIGHT_GREEN);
  else
    GD.ColorRGB(DARK_GREEN);
  GD.Vertex2ii(150, 100);
  GD.Vertex2ii(300, 175);
  
  GD.ColorRGB(0xffffff);
  GD.Tag(101);
  GD.cmd_text(220, 133, 25, OPT_CENTER, "Start");
  GD.swap();
}

void failScreen(){
  //GD.ClearColorRGB(0x000000);
  GD.Clear();
  if(wrongpincnt <3){
    GD.cmd_text(240, 116, 31, OPT_CENTER, "Wrong PIN");
    GD.cmd_text(240, 176, 31, OPT_CENTER, "Please try again");
    screen = 2;
  }
  else{
    GD.cmd_text(200, 136, 31, OPT_CENTER, "3 incorrect PIN attempts in a row.");
    GD.cmd_text(300, 136, 31, OPT_CENTER, "Exiting");
    screen = 10;
    wrongpincnt = 0;
  }
  pinlength = 0;
  //testpin = "";
  GD.swap();
}

void allUsedScreen(){
  //GD.ClearColorRGB(0x000000);
  GD.Clear();
  GD.cmd_text(200, 136, 31, OPT_CENTER, "Sorry, all lockers are taken.");
  screen = 10;
  delay(3000);
  GD.swap();
}

void successScreen(){
  //GD.ClearColorRGB(0x000000);
  GD.Clear();
  if(lockernum == 1){
    GD.cmd_text(240, 136, 31, OPT_CENTER, "Locker 1 Opened");
    GD.__end();
    delay(10);
    digitalWrite(solenoid1, HIGH);
    leds[0] = HIGH;
    updateShiftRegister();
    delay(500);
    digitalWrite(solenoid1, LOW);
    GD.resume();
    if(existing){
      l1used = false;
      pinone = "";
    }
    else
      l1used = true;
  }
  else if(lockernum == 2){
    GD.cmd_text(240, 136, 31, OPT_CENTER, "Locker 2 Opened");
    GD.__end();
    digitalWrite(solenoid2, HIGH);
    delay(500);
    digitalWrite(solenoid2, LOW);
    GD.resume();
    if(existing){
      l2used = false;
      pintwo = "";
    }
    else
      l2used = true;
  }
  else if(lockernum == 3){
    GD.cmd_text(240, 136, 31, OPT_CENTER, "Locker 3 Opened");
    GD.__end();
    digitalWrite(solenoid3, HIGH);
    delay(500);
    digitalWrite(solenoid3, LOW);
    GD.resume();
    if(existing){
      l3used = false;
      pinthree = "";
    }
    else
      l3used = true;
  }
  else if(lockernum == 4){
    GD.cmd_text(240, 136, 31, OPT_CENTER, "Locker 4 Opened");
    GD.__end();
    digitalWrite(solenoid4, HIGH);
    delay(500);
    digitalWrite(solenoid4, LOW);
    GD.resume();
    if(existing){
      l4used = false;
      pinfour = "";
    }
    else
      l4used = true;
  }
  pinlength = 0;
  screen = 10;
  GD.swap();
}

static int get_note()
{
  byte pressed = 0;
  while (pressed == 0) {
    drawscreen(screen, 0);
    if(screen == 0){
      if ((1 <= GD.inputs.tag) && (GD.inputs.tag <= 4))
        pressed = GD.inputs.tag;
    }
    else if(screen == 1){
      if ((5 <= GD.inputs.tag) && (GD.inputs.tag <= 12))
        pressed = GD.inputs.tag;
    }
    else if(screen == 2){
      if ((13 <= GD.inputs.tag) && (GD.inputs.tag <= 36))
        pressed = GD.inputs.tag;
    }
    else if(screen == 10){
      if(100 <= GD.inputs.tag && GD.inputs.tag <= 101)
        pressed = GD.inputs.tag;
    }
    updateLEDS();
  }
  play(pressed);
  return pressed;
}

void loop()
{
  byte pressed = 0;
  screen = 10;
  String testpin = "";

  while (1) {
    delay(500);
    pressed = 0;
    pressed = get_note();
    if (pressed == 1 || pressed == 2){
      screen = 1;
      existing = true;
      recreate = false;
      drawscreen(screen, 0);
    }
    else if(pressed == 100 || pressed == 101){
      screen = 0;
      drawscreen(screen, 0);
    }
    else if(pressed == 3 || pressed == 4){
      screen = 2;
      if(!l1used)
        lockernum = 1;
      else if(!l2used)
        lockernum = 2;
      else if(!l3used)
        lockernum = 3;
      else if(!l4used)
        lockernum = 4;
      else{
        allUsedScreen();
        screen = 10;
      }
      existing = false;
      recreate = false;
      drawscreen(screen, 0);
    }
    else if(pressed>4 && pressed <13){
      screen = 2;
      existing = true;
      if(pressed == 5 || pressed == 6)
        lockernum = 1;
      else if(pressed == 7 || pressed == 8)
        lockernum = 2;
      else if(pressed == 9 || pressed == 10)
        lockernum = 3;
      else if(pressed == 11 || pressed == 12)
        lockernum = 4;
      drawscreen(2, 0);
    }
    else if(pressed > 12 && pressed <37){
      if(pressed == 33 || pressed == 34){
        pinlength = 0;
        if(recreate || existing)
          testpin = "";
        else if(lockernum == 1)
          pinone = "";
        else if(lockernum == 2)
          pintwo = "";
        else if(lockernum == 3)
          pinthree = "";
        else if(lockernum == 4)
          pinfour = "";
      }
      else if(pressed == 35 || pressed == 36){
        if(recreate || existing){
          if(lockernum == 1)
            if(pinone == testpin){
              successScreen();
              //l1used = false;
              delay(2000);
            }
            else{
              failScreen();
              delay(2000);
            }
          else if(lockernum == 2)
            if(pintwo == testpin){
              successScreen();
              //l2used = false;
              delay(2000);
            }
            else
                failScreen();
          else if(lockernum == 3)
            if(pinthree == testpin){
              successScreen();
              //l3used = false;
              delay(2000);
            }
            else
              failScreen();
          else if(lockernum == 4)
            if(pinfour == testpin){
              //l4used = false;
              successScreen();
            }
            else{
              failScreen();
              delay(2000);
            }
        }
        else{
          pinlength = 0;
          testpin = "";
          recreate = true;
          drawscreen(screen, 0);
        }
        testpin = "";
        pinlength = 0;
      }
      else{
        if(pinlength<=3){
          pinlength += 1;
          if(existing || recreate){
            if(pressed%2 == 0)
              testpin += (pressed-1)+"";
            else
              testpin += pressed+"";
          }
          else{
            int temp = pressed;
            if(pressed%2 ==0)
              temp = temp - 1;
            if(lockernum == 1)
              pinone += temp+"";
            else if(lockernum == 2)
              pintwo += temp+"";
            else if(lockernum == 3)
              pinthree += temp+"";
            else if(lockernum == 4)
              pinfour += temp+"";
          }
        }
      }
    }
    updateLEDS();
  }
}
void updateShiftRegister(){
  digitalWrite(latchPin, LOW);
  
  for(int i=7; i>=0; i--){
    digitalWrite(clockPin, LOW);
    digitalWrite(dataPin, leds[i]);
    digitalWrite(clockPin, HIGH);
  }
  digitalWrite(latchPin, HIGH);
}
void updateLEDS(){
  GD.__end();
  sensorValue1 = analogRead(sensorPin1);
  delay(20);
  sensorValue1 = analogRead(sensorPin1);
  sensorValue2 = analogRead(sensorPin2);
  delay(20);
  sensorValue2 = analogRead(sensorPin2);
  sensorValue3 = analogRead(sensorPin3);
  delay(20);
  sensorValue3 = analogRead(sensorPin3);
  sensorValue4 = analogRead(sensorPin4);
  delay(20);
  sensorValue4 = analogRead(sensorPin4);
  unsigned long currentMillis = millis();
  
  //sensorValue1 = (514 - sensorValue1)*27.03 / 1023;
  //sensorValue2 = (514 - sensorValue1)*27.03 / 1023;
  //sensorValue3 = (514 - sensorValue1)*27.03 / 1023;
  //sensorValue4 = (514 - sensorValue1)*27.03 / 1023;
  //2.513
  //2.593
 
  if(currentMillis - previousMillis > interval) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;   
    
    if (sensorValue1 > 520){
      leds[0] = HIGH;
      leds[1] = LOW;
    }
    else if(sensorValue1 > 511){
      leds[1] = HIGH;
      leds[0] = LOW;
    }
    else{
      leds[0] = LOW;
      leds[1] = LOW;
    }
      
    if (sensorValue2 > 518){
      leds[2] = HIGH;
      leds[3] = LOW;
    }
    else if (sensorValue2 > 509){
      leds[3] = HIGH;
      leds[2] = LOW;
    }
    else{
      leds[2] = LOW;
      leds[3] = LOW;
    }
      
    if (sensorValue3 > 515){
      leds[4] = HIGH;
      leds[5] = LOW;
    }
    else if (sensorValue3 > 507){
      leds[5] = HIGH;
      leds[4] = LOW;
    }
    else{
      leds[4] = LOW;
      leds[5] = LOW;
    }
      
    if (sensorValue4 > 520){
      leds[6] = HIGH;
      leds[7] = LOW;
    }
    else if (sensorValue4 > 511){
      leds[7] = HIGH;
      leds[6] = LOW;
    }
    else{
      leds[6] = LOW;
      leds[7] = LOW;
    }    
    // set the LED with the ledState of the variable:
    updateShiftRegister();
    delay(500);  
    GD.resume();
  }
}
