long wait = 0;
long now = 0;
int current_led;
int lives = 3;
int game_points = 0;
int delay_time = 900;
int flag = 0;
int level;


void setup() {
  Serial.begin(9600);
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
  
      Serial.println("========== START THE GAME ==========");
      delay(5000);
  
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
        Serial.println("**** LEVELED UP ****");
      }
    }
    
    if(game_points % 10 == 0  && game_points != 0) {
      lives ++;
      Serial.println("^^^^ LIFE'S UP ^^^^");
    }
    
    Serial.print("Points: ");
    Serial.print(game_points);
    Serial.print(" ===> Lives: ");
    Serial.println(lives);
    
    current_led = random(2,6);
    
    digitalWrite(current_led, HIGH);
    
    now = millis();
    wait = now + delay_time;
     
    while(millis() != wait) {
      if(digitalRead(current_led + 6) == LOW) {
        game_points ++;
        flag = 1;
        break;
      }
    }
    
    if(flag == 0) {
      lives --;
    }
    
    delay(delay_time - 150);
    digitalWrite(current_led, LOW);
    delay(delay_time);
    
    if(lives  == 0) {
      Serial.println("---------------------------------------------");
      Serial.print("Total score: "); Serial.println(game_points);
      Serial.print("Died on level: "); Serial.println(level);
      Serial.println("========== YOU ARE DEAD! GAME OVER ==========");
      delay(5000);
      break;
    }
  }
}
