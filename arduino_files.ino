#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

bool willTheBallGoTowardsPlayerTwo = true;
bool isInputAllowed = true;

const int playerOne = 12;
const int goalPlayerOne = 13;
const int buttonPlayerOne = 3;

const int playerTwo = 5;
const int goalPlayerTwo = 4;
const int buttonPlayerTwo = 2;

int scoreOfPlayerOne = 0;
int scoreOfPlayerTwo = 0;

const unsigned long initialMillisecondsPerLED = 400;
const unsigned long initialDeltaMillisecondsPerLED = 50;
unsigned long millisecondsPerLED = initialMillisecondsPerLED;
unsigned long deltaMillisecondsPerLED = initialDeltaMillisecondsPerLED;
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;

int currentPosition = playerOne;        //Player one starts the game.
int previousPosition = playerOne + 1;
int deltaPosition = 0;

int buttonStatePlayerOne = 0;
int lastButtonStatePlayerOne = 0;
int buttonStatePlayerTwo = 0;
int lastButtonStatePlayerTwo = 0;

void setup() {
  lcd.backlight();
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("1D-Pong Game");
  lcd.setCursor(3, 1);
  lcd.print("by mircemk");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Player One: 0");
  lcd.setCursor(0, 1);
  lcd.print("Player Two: 0");

  // Initialize pins for LEDs and buttons
  for (int pin = 4; pin <= 13; pin++) {
    pinMode(pin, OUTPUT);
  }

  pinMode(buttonPlayerOne, INPUT);  // Player One button
  pinMode(buttonPlayerTwo, INPUT);  // Player Two button
}

void loop() {
  ListenForInput();
  currentMillis = millis();
  if (currentMillis - previousMillis >= millisecondsPerLED) {
    CheckGoalConditions();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Player One: ");
    lcd.setCursor(12, 0);
    lcd.print(scoreOfPlayerOne);

    lcd.setCursor(0, 1);
    lcd.print("Player Two: ");
    lcd.setCursor(12, 1);
    lcd.print(scoreOfPlayerTwo);
    CheckEndGame();
    DetermineNextPosition();
    MoveBallToNextPosition();
    previousMillis = currentMillis;
  }
}

void ListenForInput() {
  buttonStatePlayerOne = digitalRead(buttonPlayerOne);
  buttonStatePlayerTwo = digitalRead(buttonPlayerTwo);

  if (buttonStatePlayerOne != lastButtonStatePlayerOne && isInputAllowed) {
    if (buttonStatePlayerOne == HIGH) {
      if (currentPosition == playerOne) {
        ToggleBallDirection();
        IncreaseSpeed();
      } else {
        ScoreForPlayer(2);
      }
    }
    lastButtonStatePlayerOne = buttonStatePlayerOne;
  }

  if (buttonStatePlayerTwo != lastButtonStatePlayerTwo && isInputAllowed) {
    if (buttonStatePlayerTwo == HIGH) {
      if (currentPosition == playerTwo) {
        ToggleBallDirection();
        IncreaseSpeed();
      } else {
        ScoreForPlayer(1);
      }
    }
    lastButtonStatePlayerTwo = buttonStatePlayerTwo;
  }
}

void ToggleBallDirection() {
  willTheBallGoTowardsPlayerTwo = !willTheBallGoTowardsPlayerTwo;
  isInputAllowed = false;   // Prevent multiple inputs per frame
}

void IncreaseSpeed() {
  millisecondsPerLED -= deltaMillisecondsPerLED;
  if (deltaMillisecondsPerLED > 50) {
    deltaMillisecondsPerLED -= 50;
  }
}

void MoveBallToNextPosition() {
  previousPosition = currentPosition;
  digitalWrite(previousPosition, LOW);
  currentPosition = currentPosition + deltaPosition;
  digitalWrite(currentPosition, HIGH);
  tone(1, 500, 50);
  isInputAllowed = true;
}

void DetermineNextPosition() {
  if (willTheBallGoTowardsPlayerTwo) {
    deltaPosition = -1;
  } else {
    deltaPosition = 1;
  }
}

void CheckGoalConditions() {
  if (currentPosition == goalPlayerTwo) {
    ScoreForPlayer(1);
  } else if (currentPosition == goalPlayerOne) {
    ScoreForPlayer(2);
  }
}

void ScoreForPlayer(int playerWhoScored) {
  isInputAllowed = false;
  FlashAllLEDs(1);
  if (playerWhoScored == 1) {
    scoreOfPlayerOne++;
    Reset(1);
  } else if (playerWhoScored == 2) {
    scoreOfPlayerTwo++;
    Reset(2);
  }
}

void CheckEndGame() {
  if (scoreOfPlayerOne == 10) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Player One Win");
    EndGameCeremonyFor(1);
  }
  if (scoreOfPlayerTwo == 10) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Player Two Win");
    EndGameCeremonyFor(2);
  }
}

void Reset(int playerCurrentlyScored) {
  if (playerCurrentlyScored == 1) {
    digitalWrite(playerOne, HIGH);
    currentPosition = playerOne;
    willTheBallGoTowardsPlayerTwo = true;
    tone(1, 200, 150);
  } else if (playerCurrentlyScored == 2) {
    digitalWrite(playerTwo, HIGH);
    currentPosition = playerTwo;
    willTheBallGoTowardsPlayerTwo = false;
    tone(1, 300, 150);
  }

  delay(1000);  // 1 second pause
  millisecondsPerLED = initialMillisecondsPerLED;
  deltaMillisecondsPerLED = initialDeltaMillisecondsPerLED;
}

void EndGameCeremonyFor(int winner) {
  FlashAllLEDs(50);
  TurnOffAllLEDs();
  scoreOfPlayerOne = 0;
  scoreOfPlayerTwo = 0;
  delay(1000);
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Starting");
  lcd.setCursor(4, 1);
  lcd.print("New Game");
  delay(2000);
}

void FlashAllLEDs(int blinkCount) {
  for (int i = 0; i < blinkCount; i++) {
    for (int pin = 4; pin <= 13; pin++) {
      digitalWrite(pin, HIGH);  // Turn on all LEDs
    }
    tone(1, 1000, 35);
    delay(150);  // Adjust delay to control blinking speed
    for (int pin = 4; pin <= 13; pin++) {
      digitalWrite(pin, LOW);  // Turn off all LEDs
    }
    delay(150);
  }
}

void TurnOffAllLEDs() {
  for (int pin = 4; pin <= 13; pin++) {
    digitalWrite(pin, LOW);
  }
}
