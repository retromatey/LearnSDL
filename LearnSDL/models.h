#pragma once

#include "common.h"

typedef struct _app {
	SDL_Window* window;
	SDL_Renderer* renderer;
	bool game_running;
	bool up_active;
	bool down_active;
	bool left_active;
	bool right_active;
	bool fire_active;
} App;

typedef struct _entity {
	SDL_Texture* texture;
	int w;
	int h;
	int x_pos;
	int y_pos;
	int dx;
	int dy;
	bool alive;
} Entity;


void create_app(App* app);
void destroy_app(App* app);
void create_entities(App* app, Entity* ship, Entity* missile);
void destroy_entities(Entity* ship, Entity* missile);