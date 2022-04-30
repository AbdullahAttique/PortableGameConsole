#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "bitmaps.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define up             7
#define down           8
#define right          5
#define left           9
 
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define REFRESH_INTERVAL   20

String gameList[] = {"Spike Dodge", "Rex Game", "Flappy Bird"};
int numOfGames = 3;
int highlighted = 0;
int selected = -1;

void setup() {
  Serial.begin(9600);

  // setup buttons
  pinMode(9, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);

  randomSeed(analogRead(0));

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  // display splash screen
  splashScreen(3000);
  display.clearDisplay(); 
  display.setTextSize(1); 
  display.setCursor(20, 30);
  display.setTextColor(SSD1306_WHITE); 
  display.println("press anything");
  display.display(); 
}

void loop() {
  display.clearDisplay(); 
  display.setTextSize(1); 
  display.setCursor(0, 0);
  display.setTextColor(SSD1306_WHITE); 
  for (int i = 0; i < numOfGames; i++) {
    display.setTextSize(0.5); 
    display.println("");
    display.setTextSize(1.5); 
    if (highlighted == i) {
      display.println(gameList[i] + " <");
    }
    else {
      display.println(gameList[i]);
    }
  }
  display.display();

  if (digitalRead(up) == LOW || digitalRead(down) == LOW ||
      digitalRead(left) == LOW || digitalRead(right) == LOW) {
    if (digitalRead(up) == LOW) {
      if (highlighted > 0) {
        highlighted = highlighted - 1;
        while (digitalRead(up) == LOW) {
          //do nothing
        }
      }
    }
    else if (digitalRead(down) == LOW) {
      if (highlighted < numOfGames-1) {
        highlighted = highlighted + 1;
        while (digitalRead(down) == LOW) {
          //do nothing
        }
      }
    }
    else if (digitalRead(right) == LOW) {
      selected = highlighted;
      while (digitalRead(right) == LOW) {
        //do nothing
      }
    }
      
  }

  if (selected == 0) {
    selected = -1;
    runSpikeDodge();
    delay(500);
  }

  if (selected == 1) {
    selected = -1;
    runRexGame();
    delay(500);
  }

  if (selected == 2) {
    selected = -1;
    runFlappy();
    delay(500);
  }
  /*if (digitalRead(5) == LOW || digitalRead(7) == LOW ||
      digitalRead(8) == LOW || digitalRead(9) == LOW) {
    runSpikeDodge();
    display.clearDisplay(); 
    display.setTextSize(1); 
    display.setCursor(20, 30);
    display.setTextColor(SSD1306_WHITE); 
    display.println("back in menu");
    display.display();
    delay(1000); 
  }*/
}

int splashScreen(int duration) {
  display.clearDisplay(); 
  display.drawBitmap(SCREEN_WIDTH/2-splash_WIDTH/2, SCREEN_HEIGHT/2-splash_HEIGHT/2, splash, splash_WIDTH, splash_HEIGHT, SSD1306_WHITE);
  display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);
  display.drawRect(2, 2, SCREEN_WIDTH-4, SCREEN_HEIGHT-4, SSD1306_WHITE);
  display.drawRect(4, 4, SCREEN_WIDTH-8, SCREEN_HEIGHT-8, SSD1306_WHITE);
  display.display();
  delay(duration);
  display.clearDisplay();
}

void countDown() {
  for (int i = 3; i > 0; i--) {
    display.clearDisplay();
    display.setCursor(60, 25);
    display.setTextSize(2);  
    display.setTextColor(SSD1306_WHITE); 
    display.println(i);
    display.display();
    delay(1000);
  }
}

void drawCentreString(const String &buf, int x, int y)
{
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(buf, x, y, &x1, &y1, &w, &h); //calc width of new string
    display.setCursor(x - w / 2, y);
    display.println(buf);
}

void displayScore(int finalScore) {
  display.clearDisplay();
  display.setCursor(55, 10);
  display.setTextSize(2);  
  display.setTextColor(SSD1306_WHITE); 
  drawCentreString(String(finalScore), SCREEN_WIDTH/2, 10);
  display.setTextSize(1); 
  display.setCursor(7, 30);
  display.println("press up to restart");
  display.setCursor(25, 40);
  display.println("anything else");
  drawCentreString("to go back", SCREEN_WIDTH/2, 50);
  display.display();
  delay(500);
}
