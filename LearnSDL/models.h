typedef struct Entity Entity;

typedef struct {
	void (*draw)(void);
	void (*update)(void);
} Delegate;

typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
	Delegate delegate;
	int keyboard[KEYBOARD_INPUT];
} App;

struct Entity {
	float x;
	float y;
	float dx;
	float dy;
	int width;
	int height;
	SDL_Texture* texture;
	int reload;
	Entity* next;
};

typedef struct {
	Entity ship_head;
	Entity* ship_tail;
	Entity missile_head;
	Entity* missile_tail;
} Player;