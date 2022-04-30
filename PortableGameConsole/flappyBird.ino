int birdInterval = 200;

// holds the current state of the play area
int flappyCurrentState[] = {0,0,0,0,0};
int flappyStateSize = 5;

int cellWidth = 32;

int openingSize = 45;
int pipeMin = -5;
int pipeMax = SCREEN_HEIGHT - 5 - openingSize;

unsigned long birdCurrentMillis = 0;
int birdPos = 30;
int birdHitBoxStart = 3;
int birdHitBoxEnd = 8;

int flappyDisplacement = 0;
int birdSpeed = 4;//4
int birdJumpSpeed = 3;//6
float birdJumpAcceleration = -0.8;
int flappyT = 0;

int birdHeight = 30;
float birdDisplacement = 0;
int birdJumpHeight = 25;

boolean birdJumping = false;

unsigned long scoreOffset;
int birdScore = 0;

void runFlappy() {
  countDown();
  scoreOffset = millis();
  while (!birdHit()) {
    Flappy(); 
  }
  for (int i = 0; i < flappyStateSize; i++) {
    flappyCurrentState[i] = 0;
  }

  displayScore(birdScore);

  while (digitalRead(5) == HIGH && digitalRead(8) == HIGH
      && digitalRead(9) == HIGH) {
    if (digitalRead(7) == LOW) {
      runFlappy();
    }
  }
}

void Flappy() {
  display.clearDisplay();
  if (digitalRead(up) == LOW || digitalRead(down) == LOW ||
      digitalRead(left) == LOW || digitalRead(right) == LOW) {
    birdJumping = true;
    flappyT = 0;
  }

  // make character jump
  if (birdJumping) {
    birdDisplacement = birdDisplacement + (birdJumpSpeed + birdJumpAcceleration*float(flappyT)); 
    flappyT++;
  }

  // display score
  birdScore = (int)(millis()-scoreOffset)/1000;
  display.setCursor(5, 5);
  display.setTextSize(2);  
  display.setTextColor(SSD1306_WHITE); 
  display.println(birdScore);
    
  // display bird
  display.drawBitmap(birdPos, birdHeight-birdDisplacement, bird, bird_WIDTH, bird_HEIGHT, SSD1306_WHITE);
  // draw pipes
  for (int i = 0; i < flappyStateSize; i++) {
    drawPipe(i);
  } 
  //Serial.println();
  display.display();
  flappyDisplacement = flappyDisplacement + birdSpeed;

  // add new cell, remove first
  if (flappyDisplacement >= cellWidth) {
    
    // move state backwards to make room for new cell
    for (int i = 0; i < flappyStateSize-1; i++) {
      flappyCurrentState[i] = flappyCurrentState[i+1];
    }
    flappyCurrentState[flappyStateSize-1] = 0;
    // if last cell doesnt have a pipe, insert new pipe
    if (flappyCurrentState[flappyStateSize-2] == 0) {
      flappyCurrentState[flappyStateSize-1] = (int)random(pipeMax-pipeMin)+pipeMin;
    }
    
    // reset displacement
    flappyDisplacement = 0;
  }
}

void drawPipe(int cell) {
  if (flappyCurrentState[cell] != 0) {
    display.drawBitmap(cell*cellWidth-flappyDisplacement, flappyCurrentState[cell], head, head_WIDTH, head_HEIGHT, SSD1306_WHITE);
    display.drawBitmap(cell*cellWidth-flappyDisplacement, flappyCurrentState[cell]+openingSize, head, head_WIDTH, head_HEIGHT, SSD1306_WHITE);

    for (int i = flappyCurrentState[cell]-1; i>=0; i--) {
      display.drawBitmap(cell*cellWidth-flappyDisplacement + 1, i, pipe, pipe_WIDTH, pipe_HEIGHT, SSD1306_WHITE);
    }

    for (int i = flappyCurrentState[cell]+openingSize+1; i<=SCREEN_HEIGHT; i++) {
      display.drawBitmap(cell*cellWidth-flappyDisplacement + 1, i, pipe, pipe_WIDTH, pipe_HEIGHT, SSD1306_WHITE);
    }
  }
}

boolean birdHit() {
  for (int cell=0; cell < 4; cell++) {
    if (flappyCurrentState[cell] != 0) {
      if (checkCollision(birdPos, birdHeight-birdDisplacement, bird_WIDTH, bird_HEIGHT, cell*cellWidth-flappyDisplacement, 0, head_WIDTH, flappyCurrentState[cell]+head_HEIGHT)) {
        //Serial.println("hit top");
        return true;
      }
      if (checkCollision(birdPos, birdHeight-birdDisplacement, bird_WIDTH, bird_HEIGHT, cell*cellWidth-flappyDisplacement, flappyCurrentState[cell]+openingSize, head_WIDTH, SCREEN_HEIGHT-(flappyCurrentState[cell]+openingSize))) {
        //Serial.println("bottom");
        return true;
      }
    }
  }
  return false;
}

boolean checkCollision(float playerX, float playerY, float playerW, float playerH, float obstacleX, float obstacleY, float obstacleW, float obstacleH) {
  if (playerX>obstacleX && playerX<obstacleX+obstacleW 
      && playerY>obstacleY && playerY<obstacleY+obstacleH) {
    return true;
  }
  if (playerX+obstacleW>obstacleX && playerX+obstacleW<obstacleX+obstacleW 
      && playerY>obstacleY && playerY<obstacleY+obstacleH) {
    return true;
  }
  if (playerX+obstacleW>obstacleX && playerX+obstacleW<obstacleX+obstacleW 
      && playerY+obstacleH>obstacleY && playerY+obstacleH<obstacleY+obstacleH) {
    return true;
  }
  if (playerX>obstacleX && playerX<obstacleX+obstacleW 
      && playerY+obstacleH>obstacleY && playerY+obstacleH<obstacleY+obstacleH) {
    return true;
  }
  return false;
}
