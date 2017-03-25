/*
  A4 = SDA
  A5 = SCL
 */

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F,16,2);

long wait = 0;
long now = 0;
int first_led;
int second_led;
int lives = 3;
int game_points = 0;
int delay_time = 900;
int flag = 0;
int level;

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  
}

void loop() {
  lives = 3;
  game_points = 0;
  delay_time = 900;
  flag = 0;
  level = 0;

  lcd.setCursor(0,0);
  lcd.print(" START THE GAME ");
  delay(3000);
  lcd.clear();
  
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  delay(300);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  delay(300);
 
  while(1) {
    flag = 0;
    
    if(game_points % 5 == 0 && game_points != 0) {
      if(delay_time >= 400){
        level ++;
        delay_time -= 75;
        
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("** LEVELED UP **");
        delay(500);
      }
    }
    
    if(game_points % 10 == 0  && game_points != 0) {
      lives ++;

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.println("^^ LIFE'S UP ^^");
      delay(1000);
    }

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Points: ");
    lcd.print(game_points);

    lcd.setCursor(0,1);
    lcd.print(" Lives: ");
    lcd.print(lives);

    if(game_points >= 10) {
      stage_two();
    } else {
       stage_one();
    }
   
    
    if(lives  == 0) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Total score: "); lcd.print(game_points);
     
      lcd.setCursor(0,1);
      lcd.print("DEAD! GAME OVER");
      delay(5000);
      lcd.clear();
      break;
    }
  }
}

void stage_one() {
  first_led = random(2,6);
    
    digitalWrite(first_led, HIGH);
    
    now = millis();
    wait = now + delay_time;
     
    while(millis() <= wait) {
      if(digitalRead(first_led + 6) == LOW) {
        game_points ++;
        flag = 1;
        break;
      }
    }
    
    if(flag == 0) {
      lives --;
    }
    
    delay(delay_time - 150);
    digitalWrite(first_led, LOW);
    delay(delay_time);
}

void stage_two() {
    first_led = random(2,6);
    second_led = random(2,6);
  
    while(second_led == first_led) {
        second_led = random(2,6);
    }
  
    digitalWrite(first_led, HIGH);
    digitalWrite(second_led, HIGH);
  
    now = millis();
    wait = now + delay_time + 100;
     
    while(millis() <= wait) {
      if(digitalRead(first_led + 6) == LOW && digitalRead(second_led + 6) == LOW) {
        game_points ++;
        flag = 1;
        break;
      }
    }
  
    if(flag == 0) {
      lives --;
    }
  
    delay(delay_time);
    digitalWrite(first_led, LOW);
    digitalWrite(second_led, LOW);
    delay(delay_time);
}

