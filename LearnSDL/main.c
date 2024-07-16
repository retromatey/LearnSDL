// Increase or decrease the speed at which the player moves.
// Restrict the player to the bounds of the screen.

#define SDL_MAIN_HANDLED

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/sdl.h>
#include <SDL2/sdl_image.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define IMG_FILE_NAME "img/smiley.png"

typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
	int up;
	int down;
	int left;
	int right;
} App;

typedef struct {
	SDL_Texture* texture;
	int x_pos;
	int y_pos;
} Entity;

void mainloop(void);
void init_sdl(App* app);
void prepare_scene(App* app);
void present_scene(App* app);
void cleanup(void);
void handle_input(App* app);
SDL_Texture* load_image(App* app);
void draw_image(App* app, Entity* entity);

App app;
Entity ship;

int main(int argc, char* argv[])
{
	memset(&app, 0, sizeof(App));
	memset(&ship, 0, sizeof(Entity));

	init_sdl(&app);
	atexit(cleanup);

	ship.x_pos = 100;
	ship.y_pos = 100;
	ship.texture = load_image(&app);

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(mainloop, 0, 1);
#else
	while (1)
	{
		mainloop();
		SDL_Delay(16);
	}
#endif

	return 0;
}

void mainloop(void)
{
	prepare_scene(&app);
	handle_input(&app);

	if (app.down) ship.y_pos += 4;
	if (app.up) ship.y_pos -= 4;
	if (app.left) ship.x_pos -= 4;
	if (app.right) ship.x_pos += 4;

	draw_image(&app, &ship);
	present_scene(&app);
}

void init_sdl(App* app)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Failed to init sdl: %s\n", SDL_GetError());
		exit(1);
	}

	app->window = SDL_CreateWindow("Session 03", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	if (!app->window)
	{
		printf("Failed to init window: %s\n", SDL_GetError());
		exit(1);
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	app->renderer = SDL_CreateRenderer(app->window, -1, SDL_RENDERER_ACCELERATED);
	if (!app->renderer)
	{
		printf("Failed to init renderer: %s\n", SDL_GetError());
		exit(1);
	}

	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
}

void prepare_scene(App* app)
{
	SDL_SetRenderDrawColor(app->renderer, 0, 0, 100, 255);
	SDL_RenderClear(app->renderer);
}

void present_scene(App* app)
{
	SDL_RenderPresent(app->renderer);
}

void cleanup(void)
{
	SDL_DestroyTexture(ship.texture);
	IMG_Quit();
	SDL_DestroyRenderer(app.renderer);
	SDL_DestroyWindow(app.window);
	SDL_Quit();
}

void on_key_down(App* app, SDL_KeyboardEvent* keyboard_event)
{
	if (keyboard_event->repeat == 0)
	{
		if (keyboard_event->keysym.scancode == SDL_SCANCODE_UP)
			app->up = 1;
		else if (keyboard_event->keysym.scancode == SDL_SCANCODE_DOWN)
			app->down = 1;
		else if (keyboard_event->keysym.scancode == SDL_SCANCODE_LEFT)
			app->left = 1;
		else if (keyboard_event->keysym.scancode == SDL_SCANCODE_RIGHT)
			app->right = 1;
	}
}

void on_key_up(App* app, SDL_KeyboardEvent* keyboard_event)
{
	if (keyboard_event->repeat == 0)
	{
		if (keyboard_event->keysym.scancode == SDL_SCANCODE_UP)
			app->up = 0;
		else if (keyboard_event->keysym.scancode == SDL_SCANCODE_DOWN)
			app->down = 0;
		else if (keyboard_event->keysym.scancode == SDL_SCANCODE_LEFT)
			app->left = 0;
		else if (keyboard_event->keysym.scancode == SDL_SCANCODE_RIGHT)
			app->right = 0;
	}
}

void handle_input(App* app)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
#ifdef __EMSCRIPTEN__
			emscripten_cancel_main_loop();  /* this should "kill" the app. */
#else
			exit(0);
#endif
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

SDL_Texture* load_image(App* app)
{
	return IMG_LoadTexture(app->renderer, IMG_FILE_NAME);
}

void draw_image(App* app, Entity* entity)
{
	SDL_Rect dest_rect;
	dest_rect.x = entity->x_pos;
	dest_rect.y = entity->y_pos;
	SDL_QueryTexture(entity->texture, NULL, NULL, &dest_rect.w, &dest_rect.h);
	SDL_RenderCopy(app->renderer, entity->texture, NULL, &dest_rect);
}
