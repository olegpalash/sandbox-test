#include <SDL2/SDL.h>
#include <stdio.h>
#include <time.h>

#define W  12
#define H  12
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


int right(int i, int j)
{
	return (i+j+t)%2;
}

int left(int i, int j)
{
	return (i+j+t)%2 == 0;
}

int update_cell(int i, int j)
{
	if (arr[i][j] == NONE)
	{
		if (arr[i-1][j] == BULK) // падение сверху
			return BULK;
		else if (arr[i-1][j-1] == BULK && // падение слева
			right(i-1, j-1) &&
			arr[i][j-1] != NONE)
			return BULK;			
		else if (arr[i-1][j+1] == BULK && // падение справа
			left(i-1, j+1) &&
			arr[i][j+1] != NONE)
			return BULK;			
		else return NONE;
	}
	else if (arr[i][j] == BULK)
	{
		if (arr[i+1][j] == NONE) return NONE; // падение вниз
		else if (arr[i+1][j-1] == NONE && // падение влево 
			left(i, j) &&
			arr[i][j-1] != BULK)
			return NONE;
		else if (arr[i+1][j+1] == NONE && // падение вправо
			right(i, j) &&
			arr[i][j+1] != BULK)
			return NONE;
		else return BULK;
	}
	else return SOLID;
}

void update()
{
	clock_t t1 = clock();
	
	int buf[H][W];
	
	clock_t t2 = clock();
		
	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < W; j++)
		{
			buf[i][j] = update_cell(i, j);
		}
	}
	
	clock_t t3 = clock();
	
	int s_none = 0;
	int s_solid = 0;
	int s_bulk = 0;
	
	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < H; j++)
		{
			if (buf[i][j] == NONE) s_none++;  
			else if (buf[i][j] == SOLID) s_solid++;  
			else if (buf[i][j] == BULK) s_bulk++; 

			arr[i][j] = buf[i][j];
		}
	}
	
	clock_t t4 = clock();
	
	t++;

	float t_1 = (t2-t1)*1000000.0/CLOCKS_PER_SEC;
	float t_2 = (t3-t2)*1000000.0/CLOCKS_PER_SEC;
	float t_3 = (t4-t3)*1000000.0/CLOCKS_PER_SEC;
	
	printf("%d: NONE: %d, SOLID: %d, BULK: %d\n", t, s_none, s_solid, s_bulk);
	printf("time: %f, %f, %f\n", t_1, t_2, t_3);
	
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

