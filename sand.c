#include <SDL2/SDL.h>
#include <stdio.h>

#define W 12
#define H 12
#define SZ 32

enum
{
	NONE,
	SOLID,
	BULK
};

int           arr[H][W];
SDL_Renderer* rend;
int t = 0;

void init()
{
	for (int i = 0; i < H; i++)
		for (int j = 0; j < H; j++)
			arr[i][j] = NONE;
	
	for (int i = 0; i < H; i++)
		arr[i][0] = arr[i][W-1] = SOLID;

	for (int j = 0; j < W; j++)
		arr[0][j] = arr[H-1][j] = SOLID;
	
	for (int i = 3; i < 9; i++)
		for (int j = 3; j < 9; j++)
			arr[i][j] = BULK;
}

void draw()
{
	SDL_RenderClear(rend);

	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < W; j++)
		{
			SDL_Rect r;
			r.x = j*SZ;
			r.y = i*SZ;
			r.w = r.h = SZ;
			
			if (arr[i][j] == NONE)
			{
				SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
			}
			else if (arr[i][j] == SOLID)
			{
				SDL_SetRenderDrawColor(rend, 128, 128, 128, 255);
			}
			else if (arr[i][j] == BULK)
			{
				SDL_SetRenderDrawColor(rend, 255, 255, 64, 255);
			}
			
			SDL_RenderFillRect(rend, &r);
		}
	}
	
	SDL_RenderPresent(rend);
}

void update()
{
	int buf[H][W];
	
	for (int i = 0; i < H; i++)
		for (int j = 0; j < H; j++)
			buf[i][j] = arr[i][j];	
	
	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < W; j++)
		{
			if (arr[i][j] == NONE)
			{
				if (arr[i-1][j] == BULK)
					buf[i][j] = BULK;
			}
			else if (arr[i][j] == BULK)
			{
				if (arr[i+1][j] == NONE)
					buf[i][j] = NONE;
			}
		}
	}
	
	for (int i = 0; i < H; i++)
		for (int j = 0; j < H; j++)
			arr[i][j] = buf[i][j];
	
	t++;
	return;
}

int main()
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* win = SDL_CreateWindow("SAND", 0,0, SZ*W, SZ*H, 0);
	rend = SDL_CreateRenderer(win, -1, 0);
	init();
	
	int cont = 1;
	
	while(cont)
	{
		draw();
		
		SDL_Event ev;
		SDL_WaitEvent(&ev);
		
		if (ev.type == SDL_KEYUP)
		{
			if (ev.key.keysym.sym == SDLK_ESCAPE)
			{
				cont = 0;
			}
			else if (ev.key.keysym.sym == SDLK_RETURN)
			{
				update();
			}
		}	
	}
		
	return 0;
}

