#include "models.h"

static App _app;
static Entity _ship;
static Entity _missile;

static void prepare_scene(App* app)
{
	SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
	SDL_RenderClear(app->renderer);
}

static void present_scene(App* app)
{
	SDL_RenderPresent(app->renderer);
}

static void draw_texture(App* app, Entity* entity, float scale)
{
	SDL_Rect dest_rect;
	dest_rect.x = entity->x_pos;
	dest_rect.y = entity->y_pos;
	SDL_QueryTexture(entity->texture, NULL, NULL, &dest_rect.w, &dest_rect.h);
	dest_rect.w *= scale;
	dest_rect.h *= scale;
	SDL_RenderCopy(app->renderer, entity->texture, NULL, &dest_rect);

	// TODO: not sure if this is the best place to set the Entities width & height.
	entity->w = dest_rect.w;
	entity->h = dest_rect.h;
}

static void on_key_down(App* app, SDL_KeyboardEvent* event)
{
	if (event->repeat == 0)
	{
		if (event->keysym.scancode == SDL_SCANCODE_UP) { app->up_active = true; }
		if (event->keysym.scancode == SDL_SCANCODE_DOWN) { app->down_active = true; }
		if (event->keysym.scancode == SDL_SCANCODE_LEFT) { app->left_active = true; }
		if (event->keysym.scancode == SDL_SCANCODE_RIGHT) { app->right_active = true; }
	}
}

static void on_key_up(App* app, SDL_KeyboardEvent* event)
{
	if (event->repeat == 0)
	{
		if (event->keysym.scancode == SDL_SCANCODE_UP) { app->up_active = false; }
		if (event->keysym.scancode == SDL_SCANCODE_DOWN) { app->down_active = false; }
		if (event->keysym.scancode == SDL_SCANCODE_LEFT) { app->left_active = false; }
		if (event->keysym.scancode == SDL_SCANCODE_RIGHT) { app->right_active = false; }
	}
}

static void handle_input(App* app)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				app->game_running = false;
				break;
			case SDL_KEYDOWN:
				on_key_down(app, &event);
				break;
			case SDL_KEYUP:
				on_key_up(app, &event);
				break;
		}
	}
}

static void update_entities(App* app, Entity* ship, Entity* missile)
{
	if (app->down_active) { ship->y_pos += ship->dy; }
	if (app->up_active) { ship->y_pos -= ship->dy; }
	if (app->left_active) { ship->x_pos -= ship->dx; }
	if (app->right_active) { ship->x_pos += ship->dx; }

	if (ship->x_pos < 0) { ship->x_pos = 0; }
	if (ship->x_pos + ship->w > SCREEN_WIDTH) { ship->x_pos = SCREEN_WIDTH - ship->w; }

	if (ship->y_pos < 0) { ship->y_pos = 0; }
	if (ship->y_pos + ship->h > SCREEN_HEIGHT) { ship->y_pos = SCREEN_HEIGHT - ship->h; }
}

static void draw_entities(App* app, Entity* ship, Entity* missile)
{
	draw_texture(app, ship, 0.25);
	draw_texture(app, missile, 1.0);
}

static void game_loop(App* app, Entity* ship, Entity* missile, int delay)
{
	while (app->game_running)
	{
		prepare_scene(app);
		handle_input(app);
		update_entities(app, ship, missile);
		draw_entities(app, ship, missile);
		present_scene(app);
		SDL_Delay(delay);
	}
}

void game_init(void)
{
	create_app(&_app);
	create_entities(&_app, &_ship, &_missile);
}

void game_start(void)
{
	_app.game_running = true;
	game_loop(&_app, &_ship, &_missile, 16);
}

void game_destroy(void)
{
	destroy_entities(&_ship, &_missile);
	destroy_app(&_app);
	IMG_Quit();
	SDL_Quit();
}