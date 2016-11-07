/* 
 * File:   SaucerDodge.h
 * Author: jakej
 *
 * Created on November 6, 2016, 2:42 PM
 */

#ifndef SAUCERDODGE_H
#define	SAUCERDODGE_H

#include "stdint.h"
#include "stdbool.h"

#ifdef	__cplusplus
extern "C" {
#endif

void gameLoop();
void drawShip(int8_t xDiff);

void generateInitialObstacles();
void processPassedObstaclesAndReplaceThem();
void replacePassedObstacles();
bool moveObstaclesCheckCollision();
void generateTwoByFourAtIndex(uint8_t index);
void generateOneByFourAtIndex(uint8_t index);
void generateOneByThreeAtIndex(uint8_t index);

#ifdef	__cplusplus
}
#endif

#endif	/* SAUCERDODGE_H */

