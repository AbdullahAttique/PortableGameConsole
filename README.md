# Portable Arduino Game Console

Built for the Arduino pro mini. Can play 3 basic, score-based games on a 1.3 inch, single colour OLED.

## Games:

#### Spike Dodge: 
control a character trying to dodge a spiky ball rebounding and moving all over the screen
- player uses the 4 buttons as directional controls to move the character.
- the spiky ball obstacle moves around the screen by rebounding on the edges.
- the player builds up score for everytime the obstacle rebounds on one of the edges.

#### Flappy Bird: 
a version of the popular mobile game; flappy bird
- a bird moves through a randomly generated obstacle course of pipes coming from above and below
- the bird must pass as many pipes as possible to build up the score.
- the player controls the bird by pressing a button to hop upwards, while the bird is otherwise falling down. 

#### Rex Game: 
a version of the google chrome dinosaur game that appears when there is no internet.
- play as a t-rex jumping over cacti, trying to last as long as possible
- randomly generated side scroller course.
- score builds up as time passes
## Features

- 3 playable games
- Main menu for selecting between the 3 game options
- 1.3 inch OLED display
- 4 buttons for control
- rechargeable battery using usb-c port


## Technical Details
- driven by an arduino pro mini board.
- 4 push buttons for control
- 1.3 inch SSD1306 OLED for display
- TP4056 usb-c battery charging module
## Libraries Used
- graphics - Adafruit_GFX
- display - Adafruit_SSD1306
