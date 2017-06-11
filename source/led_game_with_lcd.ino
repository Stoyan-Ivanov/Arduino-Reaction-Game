/*
  A4 = SDA
  A5 = SCL
 */

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

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
int record_points = 0;
String record_player;

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  
}

void loop() {
  lives = 3;
  game_points = 0;
  delay_time = 900;
  flag = 0;
  level = 0;
  int points_needed_to_revive = 10;

  //read_EEPROM();

  lcd.setCursor(0,0);
  lcd.print(" START THE GAME ");
  delay(3000);
  lcd.clear();
  
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  delay(300);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
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
      
      if(revive(points_needed_to_revive)) {
          lcd.setCursor(8,0);
          lcd.print("3");
          delay(1000);
          lcd.clear();
  
          lcd.setCursor(8,0);
          lcd.print("2");
          delay(1000);
          lcd.clear();
  
          lcd.setCursor(8,0);
          lcd.print("1");
          delay(1000);
          lcd.clear();
          
          points_needed_to_revive += 5;
          
      } else {
         // if(game_points > record_points) {
            enter_high_score();
        //  }
          break;
      }
    }
  }
}

void stage_one() {
  first_led = random(2,7);
    
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
    first_led = random(2,7);
    second_led = random(2,7);
  
    while(second_led == first_led) {
        second_led = random(2,7);
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

bool revive(int points_needed_to_revive) {
  if(game_points >= points_needed_to_revive) {
      lcd.setCursor(0,0);
      lcd.print("Click Special:");
      lcd.setCursor(0,1);
      lcd.print("to revive (");
      lcd.print(points_needed_to_revive);
      lcd.print(")");
      
      now = millis(); 
      while(millis() <= now + 5000) {
        if(digitalRead(7) == LOW) {
           game_points -= points_needed_to_revive;
           lives = 3;
           lcd.clear();
           return true;
        } 
      }
   }
   lcd.clear();
   return false;
}

void enter_high_score() {
   char alphabet[27] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '\0'};
   int pos = 0;
   int x = 0;
   int y = 0;
   int i = 0;
   char name_string[16];
   char letter;
   int name_counter = 0;
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print(alphabet[0]);
   letter = alphabet[i];
   
  while(1) {
    if(digitalRead(7) == LOW) {
       name_string[name_counter] = letter;
       break;
    }
  
    if(digitalRead(8) == LOW) {
       i++; 
       if(i == 26) {
          i = 0;
       }
  
       letter = alphabet[i];
       lcd.setCursor(x,y);
       lcd.print(letter);
       delay(300);
    }
  
    if(digitalRead(9) == LOW) {
        name_string[name_counter] = letter;
        name_counter++;
        x++;
        lcd.setCursor(x,y);
        i = 0;
        letter = alphabet[i];
        lcd.print(letter);
        delay(300);
    } 
  }
  
   lcd.clear();
   lcd.setCursor(0,0);
   //lcd.print("name:");
   lcd.setCursor(0,1);
   String printable = "";
   
   for(i = 0; i <= name_counter; i++) {
      if(name_string[i] >= 'A' && name_string[i] <= 'Z') {
          printable += name_string[i];
      }
        
   }
   Serial.print(printable);
   Serial.print(" -> ");
   Serial.println(game_points);
   //lcd.print(printable);
   delay(2000);
   lcd.clear();
}


