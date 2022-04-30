
int rexInterval = 200;

// holds the current state of the play area where 1 is a cactus
boolean currentState[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int stateSize = 17;

float obstacleProbability = 20;
float doubleObstacleProbability = 40;

unsigned long rexCurrentMillis = 0;
int rexPos = 30;
int rexHitBoxStart = 3;
int rexHitBoxEnd = 8;

int displacement = 0;
int rexSpeed = 4;//4
int rexJumpSpeed = 6;//6
float rexJumpAcceleration = -1;
int t = 0;

int rexAnimation = 0;

int rexHeight = 30;
float rexDisplacement = 0;
int rexJumpHeight = 25;

boolean rexJumping = false;

unsigned long flappyScoreOffset;
int rexScore = 0;

void runRexGame() {
  countDown();
  scoreOffset = millis();
  while (!rexHit()) {
    rexGame();
  }
  for (int i = 0; i < stateSize; i++) {
    currentState[i] = false;
  }

  displayScore(rexScore);

  while (digitalRead(5) == HIGH && digitalRead(8) == HIGH
      && digitalRead(9) == HIGH) {
    if (digitalRead(7) == LOW) {
      runRexGame();
    }
  }
}

boolean rexHit() {
  if ((rexHeight+rex_HEIGHT)-rexDisplacement >= 39) { 
    for (int i = 1; i < 7; i++) {
      if (currentState[i]) {
        if (((i+1)*cactus_WIDTH - displacement) >= (rexPos + rex_WIDTH - rexHitBoxEnd) && (i*cactus_WIDTH - displacement) <= (rexPos + rex_WIDTH - rexHitBoxEnd)) {
          return true;
        }
        if (((i+1)*cactus_WIDTH - displacement) >= (rexPos + rexHitBoxStart) && (i*cactus_WIDTH - displacement) <= (rexPos + rexHitBoxStart)) {
          return true;
        }
      }
    }
  }
  return false;
}

void rexGame() {
  display.clearDisplay();
  
  if (digitalRead(up) == LOW || digitalRead(down) == LOW ||
      digitalRead(left) == LOW || digitalRead(right) == LOW) {
    if (!rexJumping) {
      rexJumping = true;
      t = 0;
    }
  }

  if (rexJumping) {
    //Serial.println("jumping");
    if (rexDisplacement >= 0 && rexJumping) {
      rexDisplacement = rexDisplacement + (rexJumpSpeed + rexJumpAcceleration*float(t)); 
      t++;
      //Serial.println(rexDisplacement);
    }
    else {
      rexJumping = false;
      t = 0;
    }

    if (rexDisplacement < 0) {
      rexDisplacement = 0;
      rexJumping = false;
    }
  }

  rexScore = millis()-scoreOffset;
  display.setCursor(5, 5);
  display.setTextSize(2);  
  display.setTextColor(SSD1306_WHITE); 
  display.println((int)rexScore/100);
    
  
  display.drawBitmap(rexPos, rexHeight-rexDisplacement, rex[rexAnimation], rex_WIDTH, rex_HEIGHT, SSD1306_WHITE);

  if (millis()-rexCurrentMillis >= rexInterval) {
    if (rexAnimation<1 && ! rexJumping) {
      rexAnimation ++;
    }
    else {
      rexAnimation = 0;
    }
    rexCurrentMillis = millis();
  }
  
  for (int i = 0; i < stateSize; i++) {
    //Serial.print(currentState[i]);
    if (currentState[i] == true) {
      display.drawBitmap(i*cactus_WIDTH-displacement, 39, cactus, cactus_WIDTH, cactus_HEIGHT, SSD1306_WHITE);
    }
    else {
      display.drawBitmap(i*ground_WIDTH-displacement, 50, ground, ground_WIDTH, ground_HEIGHT, SSD1306_WHITE);
    }
  } 
  //Serial.println();
  display.display();
  displacement = displacement + rexSpeed;

  // add new cell, remove first
  if (displacement == cactus_WIDTH) {
    
    // move state backwards to make room for new cell
    for (int i = 0; i < stateSize-1; i++) {
      currentState[i] = currentState[i+1];
    }
    currentState[stateSize-1] = false;

    // if last 4 cells were empty, potentialy insert obstacle
    if (!currentState[stateSize-2] && !currentState[stateSize-3] && 
        !currentState[stateSize-4] && !currentState[stateSize-5] && 
        !currentState[stateSize-6] && !currentState[stateSize-7]) {
      // 50% chance of placing obstacle if no obstacle in past four cells
      if (random(100) < obstacleProbability) {
        //Serial.println("add cactus");
        currentState[stateSize-1] = true;
      }
    }
    // potentially add double obstacle
    if (currentState[stateSize-2] && !currentState[stateSize-3]) {
      if (random(100) < doubleObstacleProbability) {
        //Serial.println("add cactus");
        currentState[stateSize-1] = true;
      }  
    }
    
    // reset displacement
    displacement = 0;
  }
}
