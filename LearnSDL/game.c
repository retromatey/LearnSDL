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

static void on_key_down(App* app, SDL_KeyboardEvent* event)
{
	if (event->repeat == 0 && event->keysym.scancode >= 0 && event->keysym.scancode < MAX_ACTIVE_KEYBOARD_KEYS)
	{
		app->active_keys[event->keysym.scancode] = true;
	}
}

static void on_key_up(App* app, SDL_KeyboardEvent* event)
{
	if (event->repeat == 0 && event->keysym.scancode >= 0 && event->keysym.scancode < MAX_ACTIVE_KEYBOARD_KEYS)
	{
		app->active_keys[event->keysym.scancode] = false;
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

static void update_ship(App* app, Entity* ship)
{
	ship->dx = 0;
	ship->dy = 0;

	if (app->active_keys[SDL_SCANCODE_UP]) { ship->dy = -SPEED_SPACESHIP; }
	if (app->active_keys[SDL_SCANCODE_DOWN]) { ship->dy = SPEED_SPACESHIP; }
	if (app->active_keys[SDL_SCANCODE_LEFT]) { ship->dx = -SPEED_SPACESHIP; }
	if (app->active_keys[SDL_SCANCODE_RIGHT]) { ship->dx = SPEED_SPACESHIP; }

	ship->x_pos += ship->dx;
	ship->y_pos += ship->dy;

	if (ship->x_pos < 0) { ship->x_pos = 0; }
	if (ship->x_pos + ship->w > SCREEN_WIDTH) { ship->x_pos = SCREEN_WIDTH - ship->w; }

	if (ship->y_pos < 0) { ship->y_pos = 0; }
	if (ship->y_pos + ship->h > SCREEN_HEIGHT) { ship->y_pos = SCREEN_HEIGHT - ship->h; }
}

static void update_missile(App* app, Entity* missile, Entity* ship)
{
	missile->dx = SPEED_MISSILE;
	missile->dy = 0;

	if (app->active_keys[SDL_SCANCODE_SPACE] && missile->alive == false)
	{ 
		missile->alive = true;
		missile->x_pos = ship->x_pos + ship->w - (missile->w);
		missile->y_pos = ship->y_pos + (ship->h / 2) - (missile->h / 2);
	}

	if (missile->alive)
	{
		missile->x_pos += missile->dx;
	}

	if (missile->x_pos + missile->w > SCREEN_WIDTH)
	{ 
		missile->alive = false;
	}
}

static void update_entities(App* app, Entity* ship, Entity* missile)
{
	update_ship(app, ship);
	update_missile(app, missile, ship);
}

static void draw_texture(App* app, Entity* entity)
{
	SDL_Rect dest_rect;
	dest_rect.x = entity->x_pos;
	dest_rect.y = entity->y_pos;
	dest_rect.w = entity->w;
	dest_rect.h = entity->h;
	SDL_RenderCopy(app->renderer, entity->texture, NULL, &dest_rect);
}

static void draw_ship(App* app, Entity* ship)
{
	draw_texture(app, ship);
}

static void draw_missile(App* app, Entity* missile)
{
	if (missile->alive)
	{
		draw_texture(app, missile);
	}
}

static void draw_entities(App* app, Entity* ship, Entity* missile)
{
	draw_ship(app, ship);
	draw_missile(app, missile);
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