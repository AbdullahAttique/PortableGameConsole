const int SPIKE_SPEED = 3;
const int PLAYER_SPEED = 4;

int playerCoords[] = {(SCREEN_WIDTH/2)-(player_WIDTH/2),(SCREEN_HEIGHT/2)-(player_HEIGHT/2)};

int playerMaxCoords[] = {SCREEN_WIDTH - (player_WIDTH),SCREEN_HEIGHT - (player_HEIGHT)};

int spikeCoords[] = {1, 1};
int spikeDirection[] = {SPIKE_SPEED, SPIKE_SPEED};

boolean inputted = false;
int input[] = {0,0};

unsigned long currentMillis = 0;
unsigned long newMillis = 20;

int score = 0;

int animation = 0;

void runSpikeDodge() {
  countDown();
  
  while (!checkSpikeHit()) {
    spikeDodge();  

    if (animation == 0){
      animation = 1;
    }
    else {
      animation = 0;
    }
      
  }
    
  playerCoords[0] = (SCREEN_WIDTH/2)-(player_WIDTH/2);
  playerCoords[1] = (SCREEN_HEIGHT/2)-(player_HEIGHT/2);
  spikeCoords[0] = 5;
  spikeCoords[1] = 5;
  spikeDirection[0] = SPIKE_SPEED;
  spikeDirection[1] = SPIKE_SPEED;

  displayScore(score);
  score = 0;

  while (digitalRead(5) == HIGH && digitalRead(8) == HIGH
      && digitalRead(9) == HIGH) {
    if (digitalRead(7) == LOW) {
      runSpikeDodge();
    }
  }
}

void spikeDodge() {
  spikeDodgeInput();
  
  // run at 50hz
  if (newMillis - currentMillis >= REFRESH_INTERVAL) {
    //Serial.println(inputted);
    if (inputted) {
      //Serial.println("new input");
      if ((playerCoords[0] + input[0]) < playerMaxCoords[0] && (playerCoords[0] + input[0]) > 0) {
        playerCoords[0] += input[0];
      }
      if ((playerCoords[1] + input[1]) < playerMaxCoords[1] && (playerCoords[1] + input[1]) > 0) {
        playerCoords[1] += input[1];
      }
    }

    display.clearDisplay();
    
    spikeRebound();
    spikeCoords[0] += spikeDirection[0];
    spikeCoords[1] += spikeDirection[1];
    
    // draw spike
    display.drawBitmap(spikeCoords[0], spikeCoords[1], spike[animation], 18, 18, SSD1306_WHITE);
    
    display.drawBitmap(playerCoords[0], playerCoords[1], player, 15, 15, SSD1306_WHITE);

    display.setCursor(5, 5);
    display.setTextSize(2);  
    display.setTextColor(SSD1306_WHITE); 
    display.println(score);
    
    display.display();

    // allow new input
    input[0] = 0;
    input[1] = 0;
    inputted = false;
    
    // reset timer
    currentMillis = newMillis;
    newMillis = millis();
  }
}

void spikeDodgeInput() {
  if (digitalRead(5) == LOW && digitalRead(7) == LOW) {
    // up-right
    //Serial.println("up-right");
    if (!inputted) {
      input[0] = 1*PLAYER_SPEED;
      input[1] = -1*PLAYER_SPEED;
      inputted = true;
    }
  }
  if (digitalRead(5) == LOW && digitalRead(8) == LOW) {
    // down-right
    //Serial.println("down-right");
    if (!inputted) {
      input[0] = 1*PLAYER_SPEED;
      input[1] = 1*PLAYER_SPEED;
      inputted = true;
    }
  }
  if (digitalRead(9) == LOW && digitalRead(8) == LOW) {
    // down-left
    //Serial.println("down-left");
    if (!inputted) {
      input[0] = -1*PLAYER_SPEED;
      input[1] = 1*PLAYER_SPEED;
      inputted = true;
    }
  }
  if (digitalRead(9) == LOW && digitalRead(7) == LOW) {
    // up-left
    //Serial.println("up-left");
    if (!inputted) {
      input[0] = -1*PLAYER_SPEED;
      input[1] = -1*PLAYER_SPEED;
      inputted = true;
    }
  }
  if (digitalRead(5) == LOW) {
    // right
    //Serial.println("right");
    if (!inputted) {
      input[0] = 1*PLAYER_SPEED;
      input[1] = 0*PLAYER_SPEED;
      inputted = true;
    }
  }
  if (digitalRead(7) == LOW) {
    // up
    //Serial.println("up");
    if (!inputted) {
      //Serial.println("new input");
      input[0] = 0*PLAYER_SPEED;
      input[1] = -1*PLAYER_SPEED;
      inputted = true;
    }
  }
  if (digitalRead(8) == LOW) {
    // down
    //Serial.println("down");
    if (!inputted) {
      input[0] = 0*PLAYER_SPEED;
      input[1] = 1*PLAYER_SPEED;
      inputted = true;
    }
  }
  if (digitalRead(9) == LOW) {
    // left
    //Serial.println("left");
    if (!inputted) {
      input[0] = -1*PLAYER_SPEED;
      input[1] = 0*PLAYER_SPEED;
      inputted = true;
    }
  }
}

void spikeRebound() {
  //Serial.println("spikeRebounding");
  // hit right
  if ((spikeCoords[0]+spike_WIDTH) >= SCREEN_WIDTH) {
    //Serial.println("hit right");
    spikeDirection[0] = -spikeDirection[0];
    spikeDirection[1] = spikeDirection[1];
    score ++;
  }
  // hit left
  if (spikeCoords[0] <= 0 ) {
    //Serial.println("hit left");
    spikeDirection[0] = -spikeDirection[0];
    spikeDirection[1] = spikeDirection[1];
    score ++;
  }
  // hit top
  if (spikeCoords[1] <= 0) {
    //Serial.println("hit top");
    spikeDirection[0] = spikeDirection[0];
    spikeDirection[1] = -spikeDirection[1];
    score ++;
  }
  // hit bottom
  if ((spikeCoords[1]+spike_HEIGHT) >= SCREEN_HEIGHT) {
    //Serial.println("hit bottom");
    spikeDirection[0] = spikeDirection[0];
    spikeDirection[1] = -spikeDirection[1];
    score ++;
  }
}

boolean checkSpikeHit() {
  if (playerCoords[0]>spikeCoords[0] && playerCoords[1]>spikeCoords[1] 
      && playerCoords[0]<spikeCoords[0]+spike_WIDTH && playerCoords[1]<spikeCoords[1]+spike_HEIGHT) {
    //Serial.println("Top Left");
    return true;
  }
  if (playerCoords[0]+player_WIDTH>spikeCoords[0] && playerCoords[1]+player_HEIGHT>spikeCoords[1] 
      && playerCoords[0]+player_WIDTH<spikeCoords[0]+spike_WIDTH && playerCoords[1]+player_HEIGHT<spikeCoords[1]+spike_HEIGHT) {
    //Serial.println("Bottom Right");
    return true;
  }
  if (playerCoords[0]+player_WIDTH>spikeCoords[0] && playerCoords[1]>spikeCoords[1] 
      && playerCoords[0]+player_WIDTH<spikeCoords[0]+spike_WIDTH && playerCoords[1]<spikeCoords[1]+spike_HEIGHT) {
    //Serial.println("Top Right");
    return true;
  }
  if (playerCoords[0]>spikeCoords[0] && playerCoords[1]+player_HEIGHT>spikeCoords[1] 
      && playerCoords[0]<spikeCoords[0]+spike_WIDTH && playerCoords[1]+player_HEIGHT<spikeCoords[1]+spike_HEIGHT) {
    //Serial.println("Bottom Left");
    return true;
  }
  return false;
}
