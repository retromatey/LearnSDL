// Increase or decrease the speed at which the player moves.
// Restrict the player to the bounds of the screen.

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#define SDL_MAIN_HANDLED
#endif

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define SMILEY_IMG "img/spaceship.png"
#define BULLET_IMG "img/bullet.png"

typedef struct _app {
	SDL_Window* window;
	SDL_Renderer* renderer;
	bool up_active;
	bool down_active;
	bool left_active;
	bool right_active;
	bool fire_active;
} App;

typedef struct _entity {
	SDL_Texture* texture;
	int dx;
	int dy;
	int x_pos;
	int y_pos;
	bool alive;
} Entity;

App app;
Entity ship;
Entity bullet;
bool game_running = true;

void error(const char* message);
void cleanup(void);
void sdl_init(App* app);
SDL_Texture* load_texture(App* app, const char* filename);
void main_loop(void);
void prepare_scene(App* app);
void handle_input(App* app);
void on_key_down(App* app, SDL_KeyboardEvent* keyboard_event);
void on_key_up(App* app, SDL_KeyboardEvent* keyboard_event);
void draw_texture(App* app, Entity* entity);
void present_scene(App* app);

int main(int argc, char* argv[])
{
	memset(&app, 0, sizeof(App));
	memset(&ship, 0, sizeof(Entity));
	memset(&bullet, 0, sizeof(Entity));
	atexit(cleanup);
	sdl_init(&app);

	ship.x_pos = 100;
	ship.y_pos = 100;
	ship.texture = load_texture(&app, SMILEY_IMG);

	bullet.texture = load_texture(&app, BULLET_IMG);

	if (ship.texture == NULL || bullet.texture == NULL)
	{
		error("Failed to load image file(s)");
	}

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(main_loop, 0, 1);
#else
	while (game_running)
	{
		main_loop();
		SDL_Delay(16);
	}
#endif

	return 0;
}

void error(const char* message)
{
	printf("%s: %s\n", message, SDL_GetError());
	exit(1);
}

void cleanup(void)
{
	SDL_DestroyTexture(ship.texture);

	SDL_DestroyRenderer(app.renderer);
	SDL_DestroyWindow(app.window);

	IMG_Quit();
	SDL_Quit();
}

void sdl_init(App* app)
{
	if (SDL_Init(SDL_INIT_VIDEO) == 0)
	{
		app->window = SDL_CreateWindow("LearnSDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

		if (app->window)
		{
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
			app->renderer = SDL_CreateRenderer(app->window, -1, SDL_RENDERER_ACCELERATED);

			if (app->renderer)
			{
				if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0)
				{
					error("Failed to init img");
				}
			}
			else
			{
				error("Failed to init renderer");
			}
		}
		else
		{
			error("Failed to init window");
		}
	}
	else
	{
		error("Failed to init SDL");
	}
}

SDL_Texture* load_texture(App* app, const char* filename)
{
	return IMG_LoadTexture(app->renderer, filename);
}

void main_loop(void)
{
	if (!game_running)
	{
#ifdef __EMSCRIPTEN__
		emscripten_cancel_main_loop();
#endif
	}

	prepare_scene(&app);
	handle_input(&app);

	if (app.down_active) ship.y_pos += 4;
	if (app.up_active) ship.y_pos -= 4;
	if (app.left_active) ship.x_pos -= 4;
	if (app.right_active) ship.x_pos += 4;

	if (app.fire_active && bullet.alive == false)
	{
		bullet.x_pos = ship.x_pos;
		bullet.y_pos = ship.y_pos;
		bullet.dx = 16;
		bullet.dy = 0;
		bullet.alive = true;
	}

	bullet.x_pos += bullet.dx;
	bullet.y_pos += bullet.dy;

	if (bullet.x_pos > SCREEN_WIDTH) { bullet.alive = false; }

	if (bullet.alive) { draw_texture(&app, &bullet); }
	draw_texture(&app, &ship);

	present_scene(&app);
}

void prepare_scene(App* app)
{
	SDL_SetRenderDrawColor(app->renderer, 50, 100, 200, 255);
	SDL_RenderClear(app->renderer);
}

void handle_input(App* app)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			game_running = false;
			break;
		case SDL_KEYDOWN:
			on_key_down(app, &event.key);
			break;
		case SDL_KEYUP:
			on_key_up(app, &event.key);
			break;
		default:
			break;
		}
	}
}

void on_key_down(App* app, SDL_KeyboardEvent* keyboard_event)
{
	if (keyboard_event->repeat == 0)
	{
		if (keyboard_event->keysym.scancode == SDL_SCANCODE_LEFT) app->left_active = true;
		else if (keyboard_event->keysym.scancode == SDL_SCANCODE_RIGHT) app->right_active = true;
		else if (keyboard_event->keysym.scancode == SDL_SCANCODE_UP) app->up_active = true;
		else if (keyboard_event->keysym.scancode == SDL_SCANCODE_DOWN) app->down_active = true;
		else if (keyboard_event->keysym.scancode == SDL_SCANCODE_SPACE) app->fire_active = true;
	}
}

void on_key_up(App* app, SDL_KeyboardEvent* keyboard_event)
{
	if (keyboard_event->repeat == 0)
	{
		if (keyboard_event->keysym.scancode == SDL_SCANCODE_LEFT) app->left_active = false;
		else if (keyboard_event->keysym.scancode == SDL_SCANCODE_RIGHT) app->right_active = false;
		else if (keyboard_event->keysym.scancode == SDL_SCANCODE_UP) app->up_active = false;
		else if (keyboard_event->keysym.scancode == SDL_SCANCODE_DOWN) app->down_active = false;
		else if (keyboard_event->keysym.scancode == SDL_SCANCODE_SPACE) app->fire_active = false;
	}
}

void draw_texture(App* app, Entity* entity)
{
	SDL_Rect dest_rect;
	dest_rect.x = entity->x_pos;
	dest_rect.y = entity->y_pos;
	SDL_QueryTexture(entity->texture, NULL, NULL, &dest_rect.w, &dest_rect.h);
	dest_rect.w /= 3;
	dest_rect.h /= 3;
	SDL_RenderCopy(app->renderer, entity->texture, NULL, &dest_rect);
}

void present_scene(App* app)
{
	SDL_RenderPresent(app->renderer);
}
