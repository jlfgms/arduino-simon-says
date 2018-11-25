#define NOTE_C5  523
#define NOTE_E5  659
#define NOTE_G5  784

// Set to true if you want debug mode
const bool DEBUG_MODE = true;



// ================
//       PINS
// ================

// Buttons pins
const int RED_BUTTON = 0;
const int GREEN_BUTTON = 1;
const int YELLOW_BUTTON = 2;
// LEDs pins
const int RED_LED = 2;
const int GREEN_LED = 3;
const int YELLOW_LED = 4;
// Global button delay
const int BUTTON_DELAY = 50;
// Buzzer pin
const int BUZZER = 11;



// ================
//   GAME CONTROL
// ================

int playerNotes[100];
int aiNotes[100];

bool buzzed = false;
bool playerTurn = false;
int currentLevel = 0;
int currentNote = 0;
int difficulty = 1000;



void setup() {
  // Are we in debug mode?
  if (DEBUG_MODE) {
    Serial.begin(9600);
    Serial.println("SIMON SAYS v0.1");
    Serial.println("Reticulating splines...");
    Serial.println();
  }
  // Set pin modes
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
}



void loop() {

  bool buttonDown = false;

  // If game has just started
  if (currentLevel == 0) {
    startNewGame();
  }

  // Check whose turn it is
  if (playerTurn) {

    // Check for Red
    if (((analogRead(RED_BUTTON) / 10) >= 10) && !buttonDown) {
      buttonDown = true;
      digitalWrite(RED_LED, HIGH);
      if (!buzzed) {
        buzz(NOTE_C5, 100);
        buzzed = true;
      }
      delay(BUTTON_DELAY);
      if ((analogRead(RED_BUTTON) / 100) <= 0) {
        digitalWrite(RED_LED, LOW);
        buttonDown = false;
        buzzed = false;
        playerNotes[currentNote] = 0;
        currentNote++;
        Serial.println("Recorded a red.");
      } else {
        Serial.println("Holding red...");
      }
    }

    // Check for Green
    if (((analogRead(GREEN_BUTTON) / 10) >= 10) && !buttonDown) {
      buttonDown = true;
      digitalWrite(GREEN_LED, HIGH);
      if (!buzzed) {
        buzz(NOTE_E5, 100);
        buzzed = true;
      }
      delay(BUTTON_DELAY);
      if ((analogRead(GREEN_BUTTON) / 100) <= 0) {
        digitalWrite(GREEN_LED, LOW);
        buttonDown = false;
        buzzed = false;
        playerNotes[currentNote] = 1;
        currentNote++;
        Serial.println("Recorded a green.");
      } else {
        Serial.println("Holding green...");
      }
    }

    // Check for Yellow
    if (((analogRead(YELLOW_BUTTON) / 10) >= 10) && !buttonDown) {
      buttonDown = true;
      digitalWrite(YELLOW_LED, HIGH);
      if (!buzzed) {
        buzz(NOTE_G5, 100);
        buzzed = true;
      }
      delay(BUTTON_DELAY);
      if ((analogRead(YELLOW_BUTTON) / 100) <= 0) {
        digitalWrite(YELLOW_LED, LOW);
        buttonDown = false;
        buzzed = false;
        playerNotes[currentNote] = 2;
        currentNote++;
        Serial.println("Recorded a yellow.");
      } else {
        Serial.println("Holding yellow...");
      }
    }

    if (currentNote == currentLevel) {
      checkSequence();
    }

  } else {
    // Simon says...
    Serial.println("Simon says...");
    for (int i = 0; i < currentLevel; i++) {
      playLight(aiNotes[i]);
      delay(difficulty);
    }
    playerTurn = true;
  }

}



void startNewGame() {

  Serial.println("Starting a new game...");
  Serial.println();

  randomSeed(millis());

  for (int i = 0; i < 100; i++) {
    aiNotes[i] = random(0, 3);
  }

  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(YELLOW_LED, HIGH);

  for (int i = 0; i <= 3; i++) {
    buzz(NOTE_C5, 100);
    buzz(NOTE_E5, 100);
    buzz(NOTE_G5, 100);
  }

  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);

  delay(2000);

  currentLevel = 1;
  currentNote = 0;
  difficulty = 1000;
  playerTurn = false;

}



void checkSequence() {
  bool sequenceIsCorrect = true;
  Serial.println();
  Serial.println("Checking answers...");
  for (int i = 0; i < currentLevel; i++) {
    if (playerNotes[i] != aiNotes[i]) {
      sequenceIsCorrect = false;
    }
  }
  if (sequenceIsCorrect) {
    Serial.println();
    Serial.println("Answers are correct!");
    currentLevel++;
    currentNote = 0;
    playerTurn = false;
    difficulty -= 50;
    delay(1000);
  } else {
    gameOver();
  };
}



void gameOver() {
  Serial.println();
  Serial.println("Wront sequence! Game over!!");
  Serial.println();
  startNewGame();
}



void playLight(int item) {
  switch (item) {
    case 0:
      digitalWrite(RED_LED, HIGH);
      buzz(NOTE_C5, 100);
      delay(BUTTON_DELAY);
      Serial.println("Red!");
      digitalWrite(RED_LED, LOW);
      break;
    case 1:
      digitalWrite(GREEN_LED, HIGH);
      buzz(NOTE_E5, 100);
      delay(BUTTON_DELAY);
      Serial.println("Green!");
      digitalWrite(GREEN_LED, LOW);
      break;
    case 2:
      digitalWrite(YELLOW_LED, HIGH);
      buzz(NOTE_G5, 100);
      delay(BUTTON_DELAY);
      Serial.println("Yellow!");
      digitalWrite(YELLOW_LED, LOW);
      break;
  }
}



void buzz(long frequency, long length) {
  // This is from https://www.princetronics.com/supermariothemesong/
  long delayValue = 1000000 / frequency / 2;
  long numCycles = frequency * length / 1000;
  for (long i = 0; i < numCycles; i++) {
    digitalWrite(BUZZER, HIGH);
    delayMicroseconds(delayValue);
    digitalWrite(BUZZER, LOW);
    delayMicroseconds(delayValue);
  }
}