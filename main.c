#define SDL_MAIN_HANDLED

#ifdef __WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define WIDTH 768
#define HEIGHT 576

#define SNAKE_GRID_SIZE 32

#define SNAKE_USING_SDL
#define SNAKE_LIMIT_X (WIDTH/SNAKE_GRID_SIZE - 1)
#define SNAKE_LIMIT_Y (HEIGHT/SNAKE_GRID_SIZE - 1)
#define SNAKE_WRAP

#include "snake.h"

int main() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0 ) {
    printf("Error: %s\n", SDL_GetError());
    return -1;
  }

  SDL_Window* window = SDL_CreateWindow(
    "snake",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    WIDTH,
    HEIGHT,
    SDL_WINDOW_SHOWN
  );

  if (window == NULL) {
    printf("Error: %s\n", SDL_GetError());
    return -1;
  }

  SDL_Surface* surface = SDL_GetWindowSurface(window);


  srand(time(NULL));
  SDL_Rect rect = {0, 0, SNAKE_GRID_SIZE, SNAKE_GRID_SIZE};
  snake_t snake = snake_init(WIDTH / SNAKE_GRID_SIZE / 2,
                              HEIGHT / SNAKE_GRID_SIZE / 2, 4);
  int move_interval = 200;

  vec2i apple = {
    rand() % (WIDTH / SNAKE_GRID_SIZE),
    rand() % (HEIGHT / SNAKE_GRID_SIZE)
  };

  Uint32 last_move = SDL_GetTicks();

  bool should_quit = false;
  while (!should_quit) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        should_quit = true;
      } else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
          case SDLK_RIGHT:
          case SDLK_d:
            if (snake.last_dir.x < 0) break;
            snake.dir.x =  1;
            snake.dir.y =  0;
            break;
          case SDLK_LEFT:
          case SDLK_a:
            if (snake.last_dir.x > 0) break;
            snake.dir.x = -1;
            snake.dir.y =  0;
            break;
          case SDLK_UP:
          case SDLK_w:
            if (snake.last_dir.y > 0) break;
            snake.dir.x =  0;
            snake.dir.y = -1;
            break;
          case SDLK_DOWN:
          case SDLK_s:
            if (snake.last_dir.y < 0) break;
            snake.dir.x =  0;
            snake.dir.y =  1;
            break;
          default:
            break;
        }
      }
    }



    Uint32 time_now = SDL_GetTicks();
    if (time_now >= last_move + move_interval) {
      if (!snake_move(&snake)) {
        char buff[64];
        memset(buff, 0, 64);
        sprintf(buff, "%lu points", snake.size);
        int ok = SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                                          ":-(", buff, window);
        if (ok < 0) puts(buff);
        should_quit = true;
      }

      if (snake_overlaps(&snake, apple) == 0) {
        apple.x = rand() % SNAKE_LIMIT_X;
        apple.y = rand() % SNAKE_LIMIT_Y;

        snake_grow(&snake);
      }
      while (snake_overlaps(&snake, apple) >= 0) {
        apple.x = rand() % SNAKE_LIMIT_X;
        apple.y = rand() % SNAKE_LIMIT_Y;
      }

      snake.last_dir = snake.dir;
      last_move = time_now;
    }


    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 0));

    snake_draw(&snake, surface);

    rect.x = apple.x * SNAKE_GRID_SIZE;
    rect.y = apple.y * SNAKE_GRID_SIZE;
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 255, 0, 0));

    SDL_UpdateWindowSurface(window);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
