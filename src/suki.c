#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int is_file(char* filename) {
  int ftmp;
  ftmp = access(filename, F_OK);
  if (ftmp == 0) {
    return 1;
  } else {
    return 0;
  }
}

int is_dir(char* filename) {
  struct stat sttr;
  if (stat(filename, &sttr) == 0) {
    if (S_ISDIR(sttr.st_mode)) {
      return 1;
    } else {
      return 0;
    }
  } else {
    printf("\033[31mstat() failed, exiting...\033[0m\n");
    exit(1);
  }
}

char** double_capacity(char** original, int *capacity) {
  *capacity = *capacity * 2;
  char** tmp = realloc(original, *capacity * sizeof(char*));
  if (tmp == NULL) {
    printf("\033[31mError while allocating memory.\033[0m\n");
    exit(1);
  }
  original = tmp;
  return original;
}

int main(int argc, char** argv) {
  bool title;
  int current = 0;
  float zoom = 1.0;
  int arr = 1;
  int k = 0;
  int capacity = argc;
  char** llfname = malloc(capacity * sizeof(char*));
  if (llfname == NULL) {
    printf("\033[31mCouldn't alocate memory, exiting...\033[0m\n");
    exit(1);
  }
  title = false;
  for (long i = 1;i < argc;i++) {
    int is_filev = is_file(argv[i]);
    if (is_filev == 1) {
      if (is_dir(argv[i]) == 1) {
        DIR* directory = opendir(argv[i]);
        if (directory == NULL) {
          printf("\033[31m[FATAL] Couldn't open folder, exiting...\033[0m\n");
          exit(1);
        }
        struct dirent* value;
        value = readdir(directory);
        while (value != NULL) {
          if (!(strcmp(value->d_name, ".") == 0) && !(strcmp(value->d_name, "..") == 0)) {
            char filepath[PATH_MAX];
            snprintf(filepath, PATH_MAX, "%s/%s", argv[i], value->d_name);
            if (k == capacity) {
              llfname = double_capacity(llfname, &capacity);
            }
            char* ccc = strdup(filepath);
            if (ccc == NULL) {
              printf("Error on strdup., Exiting...\n");
              exit(1);
            }
            llfname[k] = ccc;
            k++;
          }
          value = readdir(directory);
        }
        closedir(directory);
      } else {
        if (k == capacity) {
          llfname = double_capacity(llfname, &capacity);
        }
        char* vcompy = strdup(argv[i]);
        if (vcompy == NULL) {
          printf("Error while allocating memory, exiting...\n");
          exit(2);
        }
        llfname[k] = vcompy;
        k++;
      }
    } else {
      if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--title") == 0) {
        title = true;
      }
    }
  }

  if (k == 0) {
    printf("\033[33mNo files were given as arguments, opening the current folder...\033[0m\n");
    DIR* directory = opendir(".");
    if (directory == NULL) {
      printf("\033[31mCouldn't open folder, exiting...\033[0m\n");
      exit(1);
    }
    struct dirent* value;
    value = readdir(directory);
    while (value != NULL) {
      if (!(strcmp(value->d_name, ".") == 0) && !(strcmp(value->d_name, "..") == 0)) {
        char filepath[PATH_MAX];
        snprintf(filepath, PATH_MAX, "%s/%s", ".", value->d_name);
        if (k == capacity) {
          llfname = double_capacity(llfname, &capacity);
        }
        char* ccc = strdup(filepath);
        if (ccc == NULL) {
          printf("Error on strdup., Exiting...\n");
          exit(1);
        }
        llfname[k] = ccc;
        k++;
      }
      value = readdir(directory);
    }
    closedir(directory);
    if (k == 0) {
      printf("\033[31mCouldn't find any images, empty folder maybe?\033[0m\n");
      exit(1);
    }
  }
  SDL_Window* window = NULL;

  int w_image;
  int h_image;
  int win_height;
  int win_width;
  SDL_Surface* screenSurface = NULL;
  SDL_Surface* ttfSurface = NULL;
  SDL_Texture* image_texture = NULL;
  SDL_Texture* ttf_texture = NULL;
  SDL_Rect ttf_texture_destination;
  SDL_Rect bg_rect;
  SDL_Rect texture_destination;
  SDL_Renderer* renderer = NULL;
  SDL_Colour color;
  TTF_Font* font = NULL;

  SDL_SetHint(SDL_HINT_VIDEODRIVER, "wayland,x11");
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL Couldn't initialize, SDL error: %s\n", SDL_GetError());
  } else { // Create the window
    window = SDL_CreateWindow(llfname[current], SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
      printf("SDL Couldn't create the window! SDL error: %s\n", SDL_GetError());
    } else {
      renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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
          SDL_SetWindowTitle(window, llfname[current]);
          SDL_DestroyTexture(image_texture);
          if (title) {
            SDL_DestroyTexture(ttf_texture);
          }
          screenSurface = IMG_Load(llfname[current]);
          zoom = 1.0;
          if (screenSurface == NULL) {
            printf("\033[33m[WARN] Error while loading file %s, skipping\033[0m\n", llfname[current]);
            int em = k - current - 1;
            free(llfname[current]);
            memmove(&llfname[current], &llfname[current + 1], em * sizeof(char*));
            k--;
            if (arr == 1) {
              current--;
            }
            if (k == 0) {
              printf("\033[31mCouldn't show any images, exiting...\033[0m\n");
              exit(1);
            }
            if (arr == 1) {
              if (current == k - 1) {
                current = 0;
              } else {
                current++;
              }
            } else if (arr == -1) { 
              if (current == 0) {
                current = k - 1;
              } else {
                current--;
              }
            }
            continue;
          }
          image_texture = SDL_CreateTextureFromSurface(renderer, screenSurface);
          ttfSurface = TTF_RenderUTF8_Blended(font, llfname[current], color);
          ttf_texture = SDL_CreateTextureFromSurface(renderer, ttfSurface);
          texture_destination.x = 0;
          if (title) {
            texture_destination.y = ttfSurface->h + (10*2);
          } else {
            texture_destination.y = 0;
          }
          w_image = screenSurface->w;
          h_image = screenSurface->h;
          if (title) {
            ttf_texture_destination.y = 10;
            ttf_texture_destination.w = ttfSurface->w;
            ttf_texture_destination.h = ttfSurface->h;
            bg_rect.x = 0;
            bg_rect.y = 0;
            bg_rect.h = ttf_texture_destination.h + 15;
          }
          SDL_Rect screen;
          SDL_GetDisplayUsableBounds(0, &screen);

          int bar_h = 0;
          if (title) {
            bar_h = bg_rect.h;
          }

          int room_w = (float)screen.w * 0.9;
          int room_h = (float)screen.h * 0.9 - bar_h;

          float reg_w = (float)room_w / w_image;
          float reg_h = (float)room_h / h_image;

          float reg_scale;
          if (reg_w > reg_h) {
            reg_scale = reg_h;
          } else {
            reg_scale = reg_w;
          }

          if (reg_scale > 1) {
            reg_scale = 1;
          }

          SDL_SetWindowSize(window, w_image * reg_scale, h_image * reg_scale + bar_h);
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
              arr = 1;
              if (current == k - 1) {
                current = 0;
              } else {
                current++;
              }
              reload = true;
            } else if (e.key.keysym.sym == SDLK_LEFT) {
              arr = -1;
              if (current == 0) {
                current = k - 1;
              } else {
                current--;
              }
              reload = true;
            } else if (e.key.keysym.sym == SDLK_EQUALS) {
              zoom = zoom * 1.25f;
              if (zoom > 20) {
                zoom = 20.0f;
              }
            } else if (e.key.keysym.sym == SDLK_MINUS) {
              zoom = zoom / 1.25f;
              if (zoom < 0.1) {
                zoom = 0.1f;
              }
            } else if (e.key.keysym.sym == SDLK_q) {
              quit = true;
            }
          }
        }
        SDL_GetWindowSize(window, &win_width, &win_height);
        if (title) {
          ttf_texture_destination.x = (win_width - ttf_texture_destination.w) / 2;
          bg_rect.w = win_width;
        }
        int a_width = win_width;
        int a_height;
        if (title) {
          a_height = win_height - bg_rect.h;
        } else {
          a_height = win_height;
        }
        float scale_w = (float)a_width / w_image;
        float scale_h = (float)a_height / h_image;
        float scale;
        if (scale_w < scale_h) {
          scale = scale_w;
        } else {
          scale = scale_h;
        }
        if (scale > 1) {
          scale = 1;
        }
        texture_destination.w = w_image * scale * zoom;
        texture_destination.h = h_image * scale * zoom;
        texture_destination.x = (a_width - texture_destination.w) / 2;
        if (title) {
          texture_destination.y = bg_rect.h + (a_height - texture_destination.h) / 2;
        } else {
          texture_destination.y = (a_height - texture_destination.h) / 2;
        }

        SDL_SetRenderDrawColor(renderer, 100, 91, 98, 255);
        SDL_RenderClear(renderer);
        SDL_Rect board;
        board.w = 16;
        board.h = 16;
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        for (int y = texture_destination.y;y < texture_destination.y + texture_destination.h;y += 16) {
          board.y = y;
          if (y + 16 > texture_destination.y + texture_destination.h) {
            board.h = (texture_destination.y + texture_destination.h) - y;
          } else {
            board.h = 16;
          }
          for (int x = texture_destination.x;x < texture_destination.x + texture_destination.w;x += 16) {
            if (x + 16 > texture_destination.x + texture_destination.w) {
              board.w = (texture_destination.x + texture_destination.w) - x;
            } else {
              board.w = 16;
            }
            board.x = x;
            int column = (x - texture_destination.x) / 16;
            int row = (y - texture_destination.y) / 16;
            int sum = column + row;
            if (sum % 2 == 0) {
              SDL_SetRenderDrawColor(renderer, 168, 167, 165, 255);
            } else {
              SDL_SetRenderDrawColor(renderer, 115, 114, 112, 255);
            }
            SDL_RenderFillRect(renderer, &board);
          }
        }
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

  for (long i = 0;i < k;i++) {
    free(llfname[i]);
  }
  free(llfname);
  if (title) {
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyTexture(ttf_texture);
  }
  IMG_Quit();
  SDL_DestroyTexture(image_texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
