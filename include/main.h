

#ifndef _MAIN_H_
#define _MAIN_H_


void checkForButtonPress();
void clearGame();
void initiateGame();
bool setupWord();
bool getWord();
bool getLetter();
void checkLetter();
bool checkForCompletion();
void win();
void lose();
void credits();

void drawSplashScreen();
void drawInitScreen();
void drawGameScreen();
void drawWinLoseScreen(bool win);
void drawCreditsScreen();
void drawSdErrorScreen();
void drawTopMenu();
void drawBottomMenu();
void drawBottomWinLoseMenu();
void drawBottomCreditsMenu();
void drawCurrentGuesses();
void drawAlphabet();
void drawHangman();
void drawHangmanOutline();
void drawHangmanSplash();
void a(uint16_t b, uint16_t c, String d, String e);
void f(char g, uint16_t h, uint16_t i, uint16_t j, uint16_t k, uint16_t l, uint8_t m, uint8_t n);
void o(char p, uint16_t q, uint16_t r, uint16_t s, uint16_t t, uint16_t u);
void v(char w, uint16_t x, uint16_t y, uint16_t z, uint16_t A, uint16_t B);
void C(uint16_t D, uint16_t E, uint16_t F, uint16_t G, String H);

#endif