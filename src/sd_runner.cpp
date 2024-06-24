

#include <Arduino.h>
#include <M5Stack.h>
#include "SD.h"
#include "FS.h"
#include "hangman.h"
#include "main.h"
String filename = "clean_dict.txt";
File f;
char secretWord[MAX_WORD_LENGTH];
bool getWord()
{
  File* fp = nullptr;
  fp->close();

  f = SD.open("/" + filename, FILE_READ);
  for (uint8_t i = 0; i < MAX_WORD_LENGTH; i++)
  {
    secretWord[i] = '\0';
  }
  uint16_t randInt = random(0, f.size() - MAX_WORD_LENGTH - 1);
  uint8_t c[1] = {0};
  f.seek(randInt);
  while (c[0] != '\n')
  {
    if (!f.seek(randInt))
    {
      f.close();
      return false;
    }
    f.read(c, 1);
    randInt++;
  }
  uint16_t first = randInt;
  uint16_t count = 0;
  while (count < MAX_WORD_LENGTH + 10)
  {
    if (!f.seek(first))
    {
      return false;
    }
    f.read(c, 1);
    if (c[0] != '\n')
    {
      secretWord[count] = c[0];
    }
    else
    {
      secretWord[count] = '\0';
      return true;
    }
    count++;
    first++;
  }






  f.close();
  return true;
}
