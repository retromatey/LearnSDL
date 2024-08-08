#include "models.h"

void create_app(App* app)
{
	memset(app, 0, sizeof(App));
	app->game_running = false;

	if (SDL_Init(SDL_INIT_VIDEO) == 0)
	{
		app->window = SDL_CreateWindow(GAME_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);	// flags is zero here?

		if (app->window)
		{
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
			app->renderer = SDL_CreateRenderer(app->window, -1, SDL_RENDERER_ACCELERATED);

			if (app->renderer)
			{
				if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) < 1)
				{
					error("Error creating app, failed to initialize imaging");
				}
			}
			else
			{
				error("Error creating app, failed to initialize renderer");
			}
		}
		else
		{
			error("Error creating app, failed to initialize window");
		}
	}
	else
	{
		error("Error creating app, failed to initialize SDL");
	}
}

void destroy_app(App* app)
{
	SDL_DestroyWindow(app->window);
	SDL_DestroyRenderer(app->renderer);
}

void create_entities(App* app, Entity* ship, Entity* missile)
{
	memset(ship, 0, sizeof(Entity));
	memset(missile, 0, sizeof(Entity));

	ship->alive = true;
	ship->x_pos = 100;
	ship->y_pos = 100;
	ship->dx = SPEED_SPACESHIP;
	ship->dy = SPEED_SPACESHIP;

	ship->texture = IMG_LoadTexture(app->renderer, IMG_SPACESHIP);

	if (!ship->texture)
		error("Error creating entities, failed to load ship texture");

	SDL_Rect ship_dest_rect;
	ship_dest_rect.x = ship->x_pos;
	ship_dest_rect.y = ship->y_pos;
	SDL_QueryTexture(ship->texture, NULL, NULL, &ship_dest_rect.w, &ship_dest_rect.h);
	ship_dest_rect.w *= SCALE_SHIP;
	ship_dest_rect.h *= SCALE_SHIP;
	ship->w = ship_dest_rect.w;
	ship->h = ship_dest_rect.h;




	missile->alive = false;
	missile->x_pos = ship->x_pos;
	missile->y_pos = ship->y_pos;
	missile->dx = SPEED_MISSILE;
	missile->dy = SPEED_MISSILE;

	missile->texture = IMG_LoadTexture(app->renderer, IMG_MISSILE);

	if (!missile->texture)
		error("Error creating entities, failed to load missile texture");

	SDL_Rect missile_dest_rect;
	missile_dest_rect.x = missile->x_pos;
	missile_dest_rect.y = missile->y_pos;
	SDL_QueryTexture(missile->texture, NULL, NULL, &missile_dest_rect.w, &missile_dest_rect.h);
	missile_dest_rect.w *= SCALE_MISSILE;
	missile_dest_rect.h *= SCALE_MISSILE;
	missile->w = missile_dest_rect.w;
	missile->h = missile_dest_rect.h;
}

void destroy_entities(Entity* ship, Entity* missile)
{
	SDL_DestroyTexture(ship->texture);
	SDL_DestroyTexture(missile->texture);
}