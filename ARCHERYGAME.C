
/*-------------------------------------------------------------
  Project Title: Archery Game in C (Turbo C++)
  Developed by : Susan Rijal
  Submission To: [Kuleswor Awas Campus,kathmandu,Nepal]
                 [Mr.Narayan GC Sir]
  Description  : A graphical archery shooting game with score
                 tracking using Turbo c++
-------------------------------------------------------------*/
#include <graphics.h>
#include <conio.h>
#include <dos.h>
#include <stdlib.h>
#include <stdio.h>
#define MAX_ARROWS 5
#define MAX_TARGET_MOVE 100

int score = 0;
int arrowCount = 0;

//concentric target with rings
void drawTarget(int x, int y) {
    setcolor(RED);
    circle(x, y, 30);
    setfillstyle(SOLID_FILL, RED);
    floodfill(x, y, RED);

    setcolor(YELLOW);
    circle(x, y, 20);
    setfillstyle(SOLID_FILL, YELLOW);
    floodfill(x, y, YELLOW);

    setcolor(WHITE);
    circle(x, y, 10);
    setfillstyle(SOLID_FILL, WHITE);
    floodfill(x, y, WHITE);
}

//archer with bow and string animation
void drawArcher(int ay, int stringOffset) {
    // Body
    setcolor(WHITE);
    line(40, ay, 40, ay - 30);        // Body
    circle(40, ay - 40, 10);          // Head
    line(40, ay - 20, 20, ay - 10);  // Left arm

    // Bow arc
    setcolor(BROWN);
    arc(55, ay - 15, 270, 90, 15);

    // Bow string - move stringOffset pixels left or right to simulate pull
    setcolor(WHITE);
    line(55 + stringOffset, ay - 30, 55 + stringOffset, ay);
}

// Display the current score and remaining arrows
void displayScoreAndArrows() {
    char str[30];
    setcolor(LIGHTCYAN);
    sprintf(str, "Score: %d", score);
    outtextxy(10, 10, str);
    sprintf(str, "Arrows left: %d", MAX_ARROWS - arrowCount);
    outtextxy(10, 30, str);
}

//an arrow  effect
void drawArrow(int x, int y) {
    setcolor(WHITE);
    line(x, y, x + 20, y);  // Arrow shaft
    circle(x + 20, y, 2);   // Arrowhead
}

// Animated arrow flying toward the target
void animateArrow(int startX, int startY, int targetX, int targetY) {
    int x, y;
    int length = 20;
    int prevX, prevY;

    x = startX;
    y = startY;
    prevX = startX;
    prevY = startY;

    while (x < targetX) {
        cleardevice();

        drawTarget(targetX, targetY);

        // Draw archer with bowstring animation (string pulled while shooting)
        drawArcher(startY, -5);

        displayScoreAndArrows();

        setcolor(LIGHTGRAY);
        line(prevX, prevY, prevX + length, prevY);

        // Draw current arrow
        drawArrow(x, y);

        delay(15);
        prevX = x;
        prevY = y;
        x += 8;

        // Slightly move arrow towards targetY for smooth aiming
        if (y < targetY) y++;
        else if (y > targetY) y--;
    }

    // Arrow impact logic and scoring
    {
        int dist = abs(x - targetX) + abs(y - targetY);

        if (dist < 15) {
            score += 15;
            outtextxy(200, 50, "Bullseye! +15 points");
        } else if (dist < 30) {
            score += 10;
            outtextxy(200, 50, "Great shot! +10 points");
        } else if (dist < 50) {
            score += 5;
            outtextxy(200, 50, "Good try! +5 points");
        } else {
            outtextxy(200, 50, "Missed! No points");
        }
        delay(1000);
    }
}

void showWelcomeScreen() {
    cleardevice();
    setcolor(LIGHTGREEN);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    outtextxy(120, 80, "Turbo C++ Archery Game");
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(130, 130, "Press any key to continue...");
    getch();
    cleardevice();
}

void showInstructions() {
    cleardevice();
    setcolor(LIGHTBLUE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(100, 60, "Instructions:");
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(80, 100, "1. Use UP and DOWN arrow keys to aim.");
    outtextxy(80, 120, "2. Press SPACE to shoot.");
    outtextxy(80, 140, "3. You have 5 arrows per game.");
    outtextxy(80, 160, "4. Try to hit the moving target.");
    outtextxy(80, 200, "Press any key to return to menu...");
    getch();
    cleardevice();
}

// Menu options declared as fixed strings
#define MENU_OPTIONS 3
void showMainMenu(int* choice) {
    int highlight = 0;
    int i;
    int ch;
    char option0[] = "Start Game";
    char option1[] = "Instructions";
    char option2[] = "Exit";

    while (1) {
        cleardevice();
        setcolor(LIGHTMAGENTA);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
        outtextxy(160, 50, "Main Menu");

        for (i = 0; i < MENU_OPTIONS; i++) {
            if (i == highlight) {
                setcolor(BLACK);
                setfillstyle(SOLID_FILL, LIGHTBLUE);
                bar(150, 120 + i * 30, 330, 145 + i * 30);
                setcolor(WHITE);
            } else {
                setcolor(LIGHTGRAY);
            }
            if (i == 0) outtextxy(160, 130 + i*30, option0);
            else if (i == 1) outtextxy(160, 130 + i*30, option1);
            else if (i == 2) outtextxy(160, 130 + i*30, option2);
        }

        ch = getch();
        if (ch == 0) ch = getch();

        if (ch == 72) {  // Up arrow
            highlight--;
            if (highlight < 0) highlight = MENU_OPTIONS - 1;
        } else if (ch == 80) {  // Down arrow
            highlight++;
            if (highlight >= MENU_OPTIONS) highlight = 0;
        } else if (ch == 13) {  // Enter key
            *choice = highlight;
            return;
        }
    }
}

int main() {
    int gd = DETECT, gm;
    int aimY, ch;
    int targetX, targetY;
    char scoreStr[40];
    int menuChoice;

    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");

    showWelcomeScreen();

    while (1) {
        showMainMenu(&menuChoice);

        if (menuChoice == 0) {  // Start Game
            score = 0;
            arrowCount = 0;
            aimY = getmaxy() / 2;

            while (arrowCount < MAX_ARROWS) {
                targetX = getmaxx() - 100;
                targetY = 100 + (arrowCount % 2) * 100; // Smooth up/down movement

                while (1) {
                    cleardevice();
                    drawTarget(targetX, targetY);
                    drawArcher(aimY, 0);
                    displayScoreAndArrows();
                    outtextxy(150, getmaxy() - 40, "Use UP/DOWN to aim, SPACE to shoot");

                    ch = getch();

                    if (ch == 0) {
                        ch = getch();
                        if (ch == 72 && aimY > 60)   // Up arrow
                            aimY -= 10;
                        else if (ch == 80 && aimY < getmaxy() - 40)  // Down arrow
                            aimY += 10;
                    } else if (ch == 32) {  // Space to shoot
                        arrowCount++;
                        animateArrow(60, aimY, targetX, targetY);
                        break;
                    }
                }
            }

            cleardevice();
            sprintf(scoreStr, "Game Over! Your final score: %d", score);
            // Center text horizontally
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
            {
                int tw = textwidth(scoreStr);
                int th = textheight(scoreStr);
                int x = (getmaxx() - tw) / 2;
                int y = (getmaxy() - th) / 2;
                setcolor(LIGHTGREEN);
                outtextxy(x, y, scoreStr);
            }
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
            {
                char prompt[] = "Press any key to return to menu...";
                int tw = textwidth(prompt);
                int x = (getmaxx() - tw) / 2;
                int y = (getmaxy() / 2) + 30;
                setcolor(WHITE);
                outtextxy(x, y, prompt);
            }
            getch();
            cleardevice();

        } else if (menuChoice == 1) {  // Instructions
            showInstructions();

        } else {  // Exit
            break;
        }
    }

    closegraph();
    return 0;
}