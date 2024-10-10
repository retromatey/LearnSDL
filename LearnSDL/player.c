#include "common.h"
#include "player.h"
#include "draw.h"

extern App app;
extern Player player;

static Entity* ship;
static SDL_Texture* missile_texture;

static void update(void);
static void draw(void);
static void init_ship(void);
static void update_ship(void);
static void draw_ship(void);
static void fire_missile(void);
static void update_missiles(void);
static void draw_missiles(void);

void init_player(void)
{
	app.delegate.draw = draw;
	app.delegate.update = update;

	memset(&player, 0, sizeof(Player));
	player.ship_tail = &player.ship_head;
	player.missile_tail = &player.missile_head;

	init_ship();

	// Null checking needed here
	// create a "load texture" function in the draw.c file
	missile_texture = IMG_LoadTexture(app.renderer, "img/bullet.png");
}

static void init_ship(void)
{
	ship = malloc(sizeof(Entity));

	if (ship != NULL)
	{
		memset(ship, 0, sizeof(Entity));
		player.ship_tail->next = ship;
		player.ship_tail = ship;

		ship->x = 100;
		ship->y = 100;

		// Null checking needed here
		// create a "load texture" function in the draw.c file
		ship->texture = IMG_LoadTexture(app.renderer, "img/spaceship.png");
		SDL_QueryTexture(ship->texture, NULL, NULL, &ship->width, &ship->height);
	}
}

static void update(void)
{
	update_ship();
	update_missiles();
}

static void update_ship(void)
{
	ship->dx = 0;
	ship->dy = 0;

	if (app.keyboard[SDL_SCANCODE_LEFT]) ship->dx = -SHIP_SPEED;
	if (app.keyboard[SDL_SCANCODE_RIGHT]) ship->dx = SHIP_SPEED;
	if (app.keyboard[SDL_SCANCODE_UP]) ship->dy = -SHIP_SPEED;
	if (app.keyboard[SDL_SCANCODE_DOWN]) ship->dy = SHIP_SPEED;

	ship->x += ship->dx;
	ship->y += ship->dy;

	if (ship->x > SCREEN_WIDTH - ship->width / SHIP_SCALE) ship->x = SCREEN_WIDTH - ship->width / SHIP_SCALE;
	if (ship->x < 0) ship->x = 0;

	if (ship->y > SCREEN_HEIGHT - ship->height / SHIP_SCALE) ship->y = SCREEN_HEIGHT - ship->height / SHIP_SCALE;
	if (ship->y < 0) ship->y = 0;

	if (ship->reload > 0) ship->reload -= 1;

	if (ship->reload == 0 && app.keyboard[SDL_SCANCODE_SPACE]) fire_missile();
}

static void fire_missile(void)
{
	ship->reload = 8;
	Entity* missile = malloc(sizeof(Entity));

	if (missile != NULL)
	{
		memset(missile, 0, sizeof(Entity));
		missile->texture = missile_texture;
		SDL_QueryTexture(missile->texture, NULL, NULL, &missile->width, &missile->height);
		missile->dx = MISSILE_SPEED;
		missile->x = ship->x + ship->width / SHIP_SCALE - 10;
		missile->y = ship->y + (ship->height / SHIP_SCALE / 2.0) - (missile->height / 2.0);

		player.missile_tail->next = missile;
		player.missile_tail = missile;
	}
}

static void update_missiles(void)
{
	Entity* missile;
	Entity* previous_missile;

	previous_missile = &player.missile_head;

	for (missile = player.missile_head.next; missile != NULL; missile = missile->next)
	{
		missile->x += missile->dx;
		missile->y += missile->dy;

		if (missile->x > SCREEN_WIDTH)
		{
			if (missile == player.missile_tail)
			{
				player.missile_tail = previous_missile;
			}

			previous_missile->next = missile->next;
			free(missile);
			missile = previous_missile;
		}

		previous_missile = missile;
	}
}

static void draw(void)
{
	draw_ship();
	draw_missiles();
}

static void draw_ship(void)
{
	render_entity(ship, SHIP_SCALE);
}

static void draw_missiles(void)
{
	Entity* missile;

	for (missile = player.missile_head.next; missile != NULL; missile = missile->next)
	{
		render_entity(missile, 1.0);
	}
}