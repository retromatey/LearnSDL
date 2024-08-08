#pragma once

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>

#define SDL_MAIN_HANDLED

#define GAME_TITLE "Game 01"

#define MAX_ACTIVE_KEYBOARD_KEYS 350

#define SCALE_SHIP 0.25
#define SCALE_MISSILE 1.0

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define IMG_SPACESHIP "img/spaceship.png"
#define IMG_MISSILE "img/bullet.png"

#define SPEED_SPACESHIP 4
#define SPEED_MISSILE 16

void error(const char* message);
