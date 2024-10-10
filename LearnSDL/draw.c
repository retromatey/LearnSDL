#include "common.h"
#include "draw.h"

extern App app;

void prepare_scene(void)
{
	SDL_SetRenderDrawColor(app.renderer, 255, 0, 0, 255);
	SDL_RenderClear(app.renderer);
}

void render_scene(void)
{
	SDL_RenderPresent(app.renderer);
}

void render_entity(Entity* entity, float scale)
{
	SDL_Rect rect;
	rect.x = entity->x;
	rect.y = entity->y;
	rect.w = entity->width / scale;
	rect.h = entity->height / scale;
	SDL_RenderCopy(app.renderer, entity->texture, NULL, &rect);
}