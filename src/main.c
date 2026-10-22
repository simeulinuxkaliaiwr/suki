#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char** argv) {
  bool title;
  int current = 0;
  int k = 0;
  char* filename[argc] = {};
  if (argc == 1) {
    printf("\033[31mNo arguments provided, exiting...\033[0m\n");
    exit(1);
  } else {
    title = false;
    int is_file;
    for (long i = 1;i < argc;i++) {
      is_file = access(argv[i], F_OK);
      if (is_file == 0) {
        filename[k] = argv[i];
        k++;
      } else {
        if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--title") == 0) {
          title = true;
        }
      }
    }

    if (k == 0) {
      printf("\033[31mNo files were passed or they did not exist, exiting...\033[0m\n");
      exit(1);
    }
  }
  SDL_Window* window = NULL;

  SDL_Surface* screenSurface = NULL;
  SDL_Surface* ttfSurface = NULL;
  SDL_Texture* image_texture = NULL;
  SDL_Texture* ttf_texture = NULL;
  SDL_Rect ttf_texture_destination;
  SDL_Rect bg_rect;
  SDL_Rect texture_destination;
  SDL_Colour color;
  TTF_Font* font = NULL;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL Couldn't initialize, SDL error: %s\n", SDL_GetError());
  } else { // Create the window
    window = SDL_CreateWindow(filename[current], SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
      printf("SDL Couldn't create the window! SDL error: %s\n", SDL_GetError());
    } else {
      SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
      if (title) {
        TTF_Init();
        color.r = (Uint8)0;
        color.g = (Uint8)0;
        color.b = (Uint8)0;
        color.a = (Uint8)255;
        font = TTF_OpenFont("/home/elliot/.fonts/JetBrainsMonoNLNerdFont-BoldItalic.ttf", 26);
      }
      IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
      SDL_Event e;
      bool quit = false;
      bool reload = true;
      while (quit == false) {
        if (reload) {
          SDL_SetWindowTitle(window, filename[current]);
          SDL_DestroyTexture(image_texture);
          if (title) {
            SDL_DestroyTexture(ttf_texture);
          }
          screenSurface = IMG_Load(filename[current]);
          image_texture = SDL_CreateTextureFromSurface(renderer, screenSurface);
          ttfSurface = TTF_RenderUTF8_Blended(font, filename[current], color);
          ttf_texture = SDL_CreateTextureFromSurface(renderer, ttfSurface);
          texture_destination.x = 0;
          if (title) {
            texture_destination.y = ttfSurface->h + (10*2);
          } else {
            texture_destination.y = 0;
          }
          texture_destination.w = screenSurface->w;
          texture_destination.h = screenSurface->h;
          if (title) {
            ttf_texture_destination.x = (texture_destination.w - ttfSurface->w) / 2;
            ttf_texture_destination.y = 10;
            ttf_texture_destination.w = ttfSurface->w;
            ttf_texture_destination.h = ttfSurface->h;
            bg_rect.x = 0;
            bg_rect.y = 0;
            bg_rect.w = texture_destination.w;
            bg_rect.h = ttf_texture_destination.h + 15;
          }
          if (title) {
            SDL_SetWindowSize(window, texture_destination.w, texture_destination.h + bg_rect.h);
          } else {
            SDL_SetWindowSize(window, texture_destination.w, texture_destination.h);
          }
          if (title) {
            SDL_FreeSurface(ttfSurface);
          }
          SDL_FreeSurface(screenSurface);
          reload = false;
        }
        while (SDL_PollEvent(&e)) {
          if (e.type == SDL_QUIT) {
            quit = true;
          }
          if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_RIGHT) {
              if (current == k - 1) {
                current = 0;
              } else {
                current++;
              }
              reload = true;
            } else if (e.key.keysym.sym == SDLK_LEFT) {
              if (current == 0) {
                current = k - 1;
              } else {
                current--;
              }
              reload = true;
            } else if (e.key.keysym.sym == SDLK_q) {
              quit = true;
            }
          }
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, image_texture, NULL, &texture_destination);
        if (title) {
          SDL_SetRenderDrawColor(renderer, 59, 60, 60, 255);
          SDL_RenderFillRect(renderer, &bg_rect);
          
          SDL_RenderCopy(renderer, ttf_texture, NULL, &ttf_texture_destination);
        }
        SDL_RenderPresent(renderer);
      }
    }
  }

  if (title) {
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyTexture(ttf_texture);
  }
  IMG_Quit();
  SDL_DestroyTexture(image_texture);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
