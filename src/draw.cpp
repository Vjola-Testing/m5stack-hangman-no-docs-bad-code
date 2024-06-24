

#include <Arduino.h>
#include <M5Stack.h>
#include "hangman.h"
#include "main.h"

const GFXfont *NORMAL_FONT = &FreeMono12pt7b;
#define GFXfont *LARGE_FONT = &FreeMonoBold12pt7b;
#define GFXfont *SMALL_FONT = &FreeMono9pt7b;


void d()
{
    M5.Lcd.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setFont(LARGE_FONT);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.drawString("h a n g m a n", 160, 40);
    M5.Lcd.setFont(SMALL_FONT);
    drawHangmanSplash();
    M5.Lcd.drawString("press any button", SCREEN_WIDTH / 2, SCREEN_HEIGHT - 40);
}


void e()
{
    M5.Lcd.setFont(NORMAL_FONT);
    M5.Lcd.fillScreen(BLACK);
    drawBottomMenu();
    drawTopMenu();
    drawHangmanOutline();
}

void f()
{
    M5.Lcd.setFont(NORMAL_FONT);
    M5.Lcd.fillScreen(BLACK);
    drawBottomMenu();
    drawTopMenu();
    drawHangmanOutline();
    drawHangman();
    drawCurrentGuesses();
    drawAlphabet();
}

void drawWinLoseScreen(bool win)
{
    M5.Lcd.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
    drawTopMenu();
    drawBottomWinLoseMenu();
    uint16_t color = GREEN;
    String message = "You win!";
    if (!win)
    {
                        color = RED;
                        message = "You lose.";
    }
    drawScreenTextBox(color, BLACK, message, "\n     Word:");
    M5.Lcd.print("     ");
    M5.Lcd.print(secretWord);
}


void drawCreditsScreen()
{
        M5.Lcd.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
        drawTopMenu();
        drawBottomCreditsMenu();
        drawScreenTextBox(TFT_DARKGREY, BLACK, "Credits", "      creator:\n      Clarissa Verish\n      contributor:\n      Matthieu Lalonde");
}

void drawSdErrorScreen()
{
    drawScreenTextBox(RED, RED, "SD error", "      Cannot read dictionary\n      from SD card.");
}

void drawTopMenu()
{
    uint16_t w = SCREEN_WIDTH;
    uint16_t buttonWidth = 88;
    uint16_t buttonHeight = 22;
    uint16_t leftMargin = 6;
    uint16_t spacing = 20;
    M5.Lcd.fillRect(0, 0, w, buttonHeight + 2, TFT_MAROON);
    drawMenuButton(leftMargin + ((spacing + buttonWidth) * 0), 0, buttonWidth, buttonHeight, TFT_MAROON, "wins:" + String(wins));
    drawMenuButton(leftMargin + ((spacing + buttonWidth) * 1), 0, buttonWidth, buttonHeight, TFT_MAROON, "lose:" + String(losses));
    drawMenuButton(leftMargin + ((spacing + buttonWidth) * 2), 0, buttonWidth, buttonHeight, TFT_MAROON, "strk:" + String(streak));
}


void drawBottomMenu()
{
    int x = BOTTOM_XCOOR;
    int y = BOTTOM_YCOOR;
    uint16_t buttonWidth = 88;
    uint16_t buttonHeight = 22;
    uint16_t leftMargin = 16;
    uint16_t spacing = 10;
    drawMenuButton(x + leftMargin + ((spacing + buttonWidth) * 0), y, buttonWidth, buttonHeight, TFT_MAROON, "NEW");
    drawMenuButton(x + leftMargin + ((spacing + buttonWidth) * 1), y, buttonWidth, buttonHeight, TFT_MAROON, "FORFEIT");
    drawMenuButton(x + leftMargin + ((spacing + buttonWidth) * 2), y, buttonWidth, buttonHeight, TFT_MAROON, "CREDITS");
}

void drawBottomWinLoseMenu()
{
    int x = BOTTOM_XCOOR;
    int y = BOTTOM_YCOOR;
    uint16_t buttonWidth = 88;
    uint16_t buttonHeight = 22;
    uint16_t leftMargin = 16;
    uint16_t spacing = 10;
    drawMenuButton(x + leftMargin + ((spacing + buttonWidth) * 0), y, buttonWidth, buttonHeight, TFT_MAROON, "NEW");
    drawMenuButton(x + leftMargin + ((spacing + buttonWidth) * 1), y, buttonWidth, buttonHeight, TFT_MAROON, "CLEAR");
    drawMenuButton(x + leftMargin + ((spacing + buttonWidth) * 2), y, buttonWidth, buttonHeight, TFT_MAROON, "CREDITS");
}


void drawBottomCreditsMenu()
{
    int x = BOTTOM_XCOOR;
    int y = BOTTOM_YCOOR;
    uint16_t buttonWidth = 88;
    uint16_t buttonHeight = 22;
    uint16_t leftMargin = 16;
    uint16_t spacing = 10;
    drawMenuButton(x + leftMargin + ((spacing + buttonWidth) * 0), y, buttonWidth, buttonHeight, BLACK, "");
    drawMenuButton(x + leftMargin + ((spacing + buttonWidth) * 1), y, buttonWidth, buttonHeight, TFT_MAROON, "CLOSE");
    drawMenuButton(x + leftMargin + ((spacing + buttonWidth) * 2), y, buttonWidth, buttonHeight, BLACK, "");
}


void drawCurrentGuesses()
{
    char c = ' ';
    uint16_t color = TFT_LIGHTGREY;
    int x = SCREEN_WIDTH / 2 - 0.5 * (strlen(secretWord) * 30) + SIDE_MARGIN;
    for (int i = 0; i < MAX_WORD_LENGTH; i++)
    {
        if (secretWord[i] == currentLetter)
        {
            guesses[i] = currentLetter;
        }
        if (secretWord[i] == guesses[i])
        {
            c = secretWord[i];
            color = TFT_GREEN;
        }
        else
        {
            color = TFT_LIGHTGREY;
        }
        if (secretWord[i] != 0)
        {
            drawLargeLetterBox(c, x + 30 * i, WORDBOX_YCOOR, BLACK, WHITE, color);
        }
        c = ' ';
    }
}


void drawAlphabet()
{
    int x = KEYBOARD_XCOOR;
    int y = KEYBOARD_YCOOR;
    int alphabet_width = 7;
    uint16_t textColor = WHITE;
    uint16_t borderColor = WHITE;
    uint8_t line = 0;
    for (char c = 'a'; c <= 'z'; c++)
    {
        int i = c - 'a';
        if (guessedLetters[i] == c)
        {
            textColor = TFT_DARKGREY;
            borderColor = TFT_DARKGREY;
        }
        else
        {
            textColor = WHITE;
            borderColor = WHITE;
        }
        if (c == currentLetter)
        {
            bool found = false;
            for (int j = 0; j < MAX_WORD_LENGTH; j++)
            {
                if (secretWord[j] == '\0')
                {
                    break;
                }
                if (c == secretWord[j])
                {
                    found = true;
                    break;
                }
            }
            if (found)
            {
                textColor = GREEN;
                borderColor = GREEN;
            }
            else
            {
                textColor = RED;
                borderColor = RED;
            }
        }
        drawSmallLetterBox(c, x + (SMALL_BOX_WIDTH + 6) * (i % alphabet_width), y + (SMALL_BOX_HEIGHT + 6) * line, textColor, BLACK, borderColor);
        if ((i + 1) % alphabet_width == 0)
        {
            line++;
        }
    }
}

void drawHangman()
{
    int x = HANGMAN_XCOOR;
    int y = HANGMAN_YCOOR;
    uint16_t color = HANGMAN_FILL_COLOR;
    switch (numWrong)
    {
    case 1: 
        M5.Lcd.fillRect(x, y + 94, 96, 12, color);
        break;
    case 2: 
        M5.Lcd.fillRect(x + 76, y + 11, 8, 80, color);
        break;
    case 3: 
        M5.Lcd.fillRect(x + 20, y, 76, 8, color);
        break;
    case 4: 
        M5.Lcd.fillRect(x + 41, y + 11, 4, 11, color);
        break;
    case 5: 
        M5.Lcd.fillEllipse(x + 43, y + 32, 8, 8, color);
        break;
    case 6: 
        M5.Lcd.fillRect(x + 38, y + 42, 10, 22, color);
        break;
    case 7: 
        M5.Lcd.fillRect(x + 38, y + 66, 4, 18, color);
        break;
    case 8: 
        M5.Lcd.fillRect(x + 44, y + 66, 4, 18, color);
        break;
    case 9: 
        M5.Lcd.fillRect(x + 28, y + 42, 8, 4, color);
        break;
    case 10:
        M5.Lcd.fillRect(x + 50, y + 42, 8, 4, color);
        break;
    }
}


void drawHangmanOutline()
{
    int x = HANGMAN_XCOOR;
    int y = HANGMAN_YCOOR;
    uint16_t color = HANGMAN_OUTLINE_COLOR;
    M5.Lcd.fillRect(x, y + 94, 96, 12, color);
    M5.Lcd.fillRect(x + 76, y + 11, 8, 80, color);
    M5.Lcd.fillRect(x + 20, y, 76, 8, color);
    M5.Lcd.fillRect(x + 41, y + 11, 4, 11, color);
    M5.Lcd.fillEllipse(x + 43, y + 32, 8, 8, color);
    M5.Lcd.fillRect(x + 38, y + 42, 10, 22, color);
    M5.Lcd.fillRect(x + 38, y + 66, 4, 18, color);
    M5.Lcd.fillRect(x + 44, y + 66, 4, 18, color);
    M5.Lcd.fillRect(x + 28, y + 42, 8, 4, color);
    M5.Lcd.fillRect(x + 50, y + 42, 8, 4, color);
}


void drawHangmanSplash()
{
    int x = (SCREEN_WIDTH - 100) / 2;
    int y = 70;
    uint16_t color = RED;
    M5.Lcd.fillRect(x, y + 94, 96, 12, color);
    M5.Lcd.fillRect(x + 76, y + 11, 8, 80, color);
    M5.Lcd.fillRect(x + 20, y, 76, 8, color);
    M5.Lcd.fillRect(x + 41, y + 11, 4, 11, color);
    M5.Lcd.fillEllipse(x + 43, y + 32, 8, 8, color);
    M5.Lcd.fillRect(x + 38, y + 42, 10, 22, color);
    M5.Lcd.fillRect(x + 38, y + 66, 4, 18, color);
    M5.Lcd.fillRect(x + 44, y + 66, 4, 18, color);
    M5.Lcd.fillRect(x + 28, y + 42, 8, 4, color);
    M5.Lcd.fillRect(x + 50, y + 42, 8, 4, color);
}


void drawScreenTextBox(uint16_t borderColor, uint16_t textColor, String message1, String message2)
{
    M5.Lcd.fillRect(20, 30, 280, 180, borderColor);
    M5.Lcd.fillRect(40, 50, 240, 140, WHITE);
    M5.Lcd.setCursor(56, 80);
    M5.Lcd.setFont(LARGE_FONT);
    M5.Lcd.setTextColor(textColor);
    M5.Lcd.println(message1);
    M5.Lcd.setFont(SMALL_FONT);
    M5.Lcd.println(message2);
}


void drawLetterBoxWithSize(char c, uint16_t x, uint16_t y, uint16_t textColor, uint16_t backgroundColor, uint16_t borderColor, uint8_t bwidth, uint8_t bheight)
{
    M5.Lcd.fillRect(x, y, bwidth, bheight, backgroundColor);
    M5.Lcd.drawRect(x, y, bwidth, bheight, borderColor);
    M5.Lcd.setCursor(x + (bwidth / 5), y + 3.75 * (bheight / 5));
    M5.Lcd.setTextColor(textColor, backgroundColor);
    M5.Lcd.print(c);
}


void drawLargeLetterBox(char c, uint16_t x, uint16_t y, uint16_t textColor, uint16_t backgroundColor, uint16_t borderColor)
{
    M5.Lcd.setFont(LARGE_FONT);
    drawLetterBoxWithSize(c, x, y, textColor, backgroundColor, borderColor, LARGE_BOX_WIDTH, LARGE_BOX_HEIGHT);
    M5.Lcd.setFont(NORMAL_FONT);
}

void drawSmallLetterBox(char c, uint16_t x, uint16_t y, uint16_t textColor, uint16_t backgroundColor, uint16_t borderColor)
{
    M5.Lcd.setFont(SMALL_FONT);
    drawLetterBoxWithSize(c, x, y, textColor, backgroundColor, borderColor, SMALL_BOX_WIDTH, SMALL_BOX_HEIGHT);
    M5.Lcd.setFont(NORMAL_FONT);
}


void drawMenuButton(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color, String s)
{
    M5.Lcd.fillRoundRect(x, y, w, h + 2, 4, color);
    M5.Lcd.setCursor(x + 3 + (88 - s.length() * 12) / 2, y + 16);
    M5.Lcd.setFont(SMALL_FONT);
    M5.Lcd.setTextColor(TFT_LIGHTGREY);
    M5.Lcd.print(s);
    M5.Lcd.setFont(NORMAL_FONT);
}
