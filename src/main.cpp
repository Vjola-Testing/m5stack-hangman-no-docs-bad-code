

#include <Arduino.h>
#include <M5Stack.h>
#include "hangman.h"
#include "main.h"

char guesses[MAX_WORD_LENGTH];
char guessedLetters[NUM_LETTERS];
char currentLetter;

int numWrong = 0;
int numGuesses = 10;
bool forfeit = false;

uint16_t wins = 0;
uint16_t losses = 0;
uint16_t streak = 0;

typedef enum
{
  INIT,
  STAGE_NEW_GAME,
  STAGE_INPUT_LETTER,
  STAGE_CHECK_LETTER,
  STAGE_CHECK_WORD,
  STAGE_FINISH,
  STAGE_CREDITS,
} appStages_t;

appStages_t stage = INIT;


void setup()
{
  M5.begin();
  SD.begin();
  Wire.begin();
  M5.Lcd.setTextDatum(TC_DATUM);
  pinMode(KEYBOARD_INT, INPUT_PULLUP);
}

void loop()
{
  M5.update();
  checkForButtonPress();

  if (stage == INIT) {
        clearGame();
        while (!M5.BtnA.wasReleased() && !M5.BtnB.wasReleased() && !M5.BtnC.wasReleased()) {
            M5.update();
        }
        stage = STAGE_NEW_GAME;
  }
  switch (stage)
  {
  case STAGE_NEW_GAME:
    initiateGame();
    stage = STAGE_INPUT_LETTER;
    break;
  case STAGE_INPUT_LETTER:
    if (getLetter())
    {
      stage = STAGE_CHECK_LETTER;
    }
    break;
  case STAGE_CHECK_LETTER:
    checkLetter();
    stage = STAGE_CHECK_WORD;
    break;
  case STAGE_CHECK_WORD:
    if (checkForCompletion())
    {
      stage = STAGE_FINISH;
    }
    else
    {
      stage = STAGE_INPUT_LETTER;
    }
    break;
  case STAGE_FINISH:
    if (m5.BtnA.wasReleased())
    {
      stage = STAGE_NEW_GAME;
    }
    if (m5.BtnB.wasReleased())
    {
      stage = INIT;
    }
    if (m5.BtnC.wasReleased())
    {
      stage = STAGE_CREDITS;
    }
    break;
  case STAGE_CREDITS:
    credits();
    stage = STAGE_CHECK_WORD;
    break;
  }
}


void checkForButtonPress()
{
  if (stage == STAGE_FINISH && M5.BtnB.wasReleased())
  {
    stage = INIT;
    M5.update();
  }
  else if (stage != INIT && stage != STAGE_FINISH && m5.BtnB.wasReleased())
  {
    lose();
    forfeit = true;
    stage = STAGE_FINISH;
    M5.update();
  }
  else if (stage != INIT && stage != STAGE_CREDITS && m5.BtnA.wasReleased())
  {
    stage = STAGE_NEW_GAME;
    M5.update();
  }
  else if (stage != INIT && m5.BtnC.wasReleased())
  {
    stage = STAGE_CREDITS;
    M5.update();
  }
}


void clearGame()
{
  drawSplashScreen();
  wins = 0;
  losses = 0;
  streak = 0;
  currentLetter = 0;
  forfeit = false;
}


void initiateGame()
{
  currentLetter = 0;
  drawInitScreen();

  if (!setupWord())
  {
    drawSdErrorScreen();
    while (true)
    {
    }
  }
  for (int i = 0; i < NUM_LETTERS; i++)
  {
    guessedLetters[i] = 0;
  }
  numWrong = 0;
  drawGameScreen();
}


bool setupWord()
{
  for (int i = 0; i < MAX_WORD_LENGTH; i++)
  {
    guesses[i] = 0;
    secretWord[i] = 0;
  }
  return getWord();
}


bool getLetter()
{
  M5.Lcd.setCursor(150, 1);
  currentLetter = 0;
  if (digitalRead(KEYBOARD_INT) == LOW)
  {
    Wire.requestFrom(KEYBOARD_I2C_ADDR, 1);
    while (Wire.available())
    {
      uint8_t key_val = Wire.read();
      if (key_val != 0)
      {
        if (key_val >= 32 && key_val < 127)
        { 
          Serial.print((char)key_val);
          if ((key_val >= 97 && key_val <= 122) && key_val != currentLetter)
          {
            currentLetter = (char)key_val;
            return true;
          }
        }
      }
    }
  }
  return false;
}


void checkLetter()
{
  if (currentLetter != guessedLetters[currentLetter - 'a'] && currentLetter != 0)
  {
    guessedLetters[currentLetter - 'a'] = currentLetter;
    drawCurrentGuesses();
    drawAlphabet();
    for (int i = 0; i < MAX_WORD_LENGTH; i++)
    {
      char c = secretWord[i];
      if (c == currentLetter && c != 0)
      {
        return;
      }
    }
    numWrong++;
    drawHangman();
  }
}


bool checkForCompletion()
{
  for (uint16_t i = 0; i < MAX_WORD_LENGTH; i++)
  {
    if (secretWord[i] != guesses[i] && numGuesses != numWrong)
    {
      return false;
    }
  }
  if (numGuesses == numWrong || forfeit)
  {
    lose();
  }
  else
  {
    win();
  }
  drawTopMenu();
  return true;
}

void win()
{
  drawWinLoseScreen(true);
  wins++;
  streak++;
}


void lose()
{
  drawWinLoseScreen(false);
  losses++;
  streak = 0;
}


void credits()
{
  drawCreditsScreen();
  while (!m5.BtnB.wasReleased())
  {
    M5.update();
  }
  drawBottomMenu();
  drawGameScreen();
}