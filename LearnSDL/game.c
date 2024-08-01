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
		}
	}
}

static void game_loop(App* app, Entity* ship, Entity* missile, int delay)
{
	while (app->game_running)
	{
		prepare_scene(app);
		handle_input(app);
		draw_texture(app, ship, 0.25);
		draw_texture(app, missile, 1.0);
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