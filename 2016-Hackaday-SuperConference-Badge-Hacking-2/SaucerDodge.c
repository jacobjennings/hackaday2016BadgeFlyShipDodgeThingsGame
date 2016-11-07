#include "SaucerDodge.h"
#include "HaD_Badge.h"
#include "stdbool.h"

static uint8_t shipLeftPosition = 3;
static uint8_t gameTick = 0;

typedef struct point {
    int8_t x;
    int8_t y;
} point;

static uint8_t const numberOfEachObstacleType = 3;

static point twoByFourObstacle[numberOfEachObstacleType];
static point oneByFourObstacle[numberOfEachObstacleType];
static point oneByThreeObstacle[numberOfEachObstacleType];

void gameLoop() {
    //    displayPixel(ballX, ballY, ON);
    //    displayLatch();
    uint32_t nextTime = getTime();

    generateInitialObstacles();

    while (1) {

        controlDelayMs(100);
        displayClear();
        
        switch (getControl()) {
            case (LEFT):
                drawShip(-1);
                break;
            case (RIGHT):
                drawShip(1);
                break;
            case (UP):
                drawShip(0);
                break;
            case (DOWN):
                drawShip(0);
                break;
            default:
                drawShip(0);
                break;
        }

        if (moveObstaclesCheckCollision()) {
            Buffer[0] =  0b00000000;
            Buffer[1] =  0b00000000;
            Buffer[2] =  0b00000000;
            Buffer[3] =  0b10000001;
            Buffer[4] =  0b01000010;
            Buffer[5] =  0b00100100;
            Buffer[6] =  0b00011000;
            Buffer[7] =  0b00011000;
            Buffer[8] =  0b00100100;
            Buffer[9] =  0b01000010;
            Buffer[11] = 0b10000001;
            Buffer[12] = 0b00000000;
            Buffer[13] = 0b00000000;
            Buffer[14] = 0b00000000;
            Buffer[15] = 0b00000000;
            displayLatch();
            break;
        }
        
        replacePassedObstacles();
        
        
        gameTick++;
    }
    
    while(1) {
        
    }
}

void generateInitialObstacles() {
    for (uint8_t idx = 0; idx < numberOfEachObstacleType; idx++) {
        generateOneByFourAtIndex(idx);
        generateOneByThreeAtIndex(idx);
        generateTwoByFourAtIndex(idx);
    }
}

void replacePassedObstacles() {
    for (uint8_t idx = 0; idx < numberOfEachObstacleType; idx++) {
        if (twoByFourObstacle[idx].y < -4) {
            generateTwoByFourAtIndex(idx);
        }
        if (oneByFourObstacle[idx].y < -4) {
            generateOneByFourAtIndex(idx);
        }
        if (oneByThreeObstacle[idx].y < -3) {
            generateOneByThreeAtIndex(idx);
        }
    }
}

bool moveObstaclesCheckCollision() {
    for (uint8_t idx = 0; idx < numberOfEachObstacleType; idx++) {
        twoByFourObstacle[idx].y -= 1;
        if (
                ((twoByFourObstacle[idx].x >= shipLeftPosition - 1) &&
                (twoByFourObstacle[idx].x <= shipLeftPosition + 2)) &&
                ((twoByFourObstacle[idx].y >= -4) &&
                (twoByFourObstacle[idx].y <= 1))) {
            return true;
        }
        displayPixel(twoByFourObstacle[idx].x, twoByFourObstacle[idx].y, 1);
        displayPixel(twoByFourObstacle[idx].x + 1, twoByFourObstacle[idx].y, 1);
        displayPixel(twoByFourObstacle[idx].x, twoByFourObstacle[idx].y + 1, 1);
        displayPixel(twoByFourObstacle[idx].x + 1, twoByFourObstacle[idx].y + 1, 1);
        displayPixel(twoByFourObstacle[idx].x, twoByFourObstacle[idx].y + 2, 1);
        displayPixel(twoByFourObstacle[idx].x + 1, twoByFourObstacle[idx].y + 2, 1);
        displayPixel(twoByFourObstacle[idx].x, twoByFourObstacle[idx].y + 3, 1);
        displayPixel(twoByFourObstacle[idx].x + 1, twoByFourObstacle[idx].y + 3, 1);

        oneByFourObstacle[idx].y -= 1;
        if (
                ((oneByFourObstacle[idx].x >= shipLeftPosition) &&
                (oneByFourObstacle[idx].x <= shipLeftPosition + 2)) &&
                ((oneByFourObstacle[idx].y >= -4) &&
                (oneByFourObstacle[idx].y <= 1))) {
            return true;
        }
        
        displayPixel(oneByFourObstacle[idx].x, oneByFourObstacle[idx].y, 1);
        displayPixel(oneByFourObstacle[idx].x, oneByFourObstacle[idx].y + 1, 1);
        displayPixel(oneByFourObstacle[idx].x, oneByFourObstacle[idx].y + 2, 1);
        displayPixel(oneByFourObstacle[idx].x, oneByFourObstacle[idx].y + 3, 1);

        oneByThreeObstacle[idx].y -= 1;
        if (
                ((oneByThreeObstacle[idx].x >= shipLeftPosition) &&
                (oneByThreeObstacle[idx].x <= shipLeftPosition + 2)) &&
                ((oneByThreeObstacle[idx].y >= -4) &&
                (oneByThreeObstacle[idx].y <= 1))) {
            return true;
        }
        
        displayPixel(oneByThreeObstacle[idx].x, oneByThreeObstacle[idx].y, 1);
        displayPixel(oneByThreeObstacle[idx].x, oneByThreeObstacle[idx].y + 1, 1);
        displayPixel(oneByThreeObstacle[idx].x, oneByThreeObstacle[idx].y + 2, 1);
        
    }
    return false;
}

void drawShip(int8_t xDiff) {
    if (xDiff < 0 && shipLeftPosition > 0) {
        shipLeftPosition--;
    } else if (xDiff > 0 && shipLeftPosition < 5) {
        shipLeftPosition++;
    }

    Buffer[14] = 0b01000000 >> shipLeftPosition;
    Buffer[15] = 0b11100000 >> shipLeftPosition;
}

void generateTwoByFourAtIndex(uint8_t index) {
    controlDelayMs(5);
    pollAccel();
    point pointA;
    pointA.x = (AccXhigh + AccXlow + AccZhigh + AccZlow + AccYhigh + AccZlow) % 8;
    pointA.y = ((AccYhigh + AccYlow) % 16) + 16;
    twoByFourObstacle[index] = pointA;
}

void generateOneByFourAtIndex(uint8_t index) {
    controlDelayMs(5);
    pollAccel();
    point pointB;
    pointB.x = (AccXhigh + AccXlow + AccZhigh + AccZlow + AccYhigh + AccZlow) % 8;
    pointB.y = ((AccYhigh + AccYlow) % 16) + 16;
    oneByFourObstacle[index] = pointB;
}

void generateOneByThreeAtIndex(uint8_t index) {
    controlDelayMs(5);
    pollAccel();
    point pointC;
    pointC.x = (AccXhigh + AccXlow + AccZhigh + AccZlow + AccYhigh + AccZlow) % 8;
    pointC.y = ((AccYhigh + AccYlow) % 16) + 16;
    oneByThreeObstacle[index] = pointC;
}

