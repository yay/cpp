#include <iostream>
#include <SDL.h>
#include <SDL_shape.h>
#include <vector>
#include <complex>
#include <random>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int partition(std::vector<int> &A, int lo, int hi) {
    int pivot = A[lo];
    int i = lo - 1;
    int j = hi + 1;

    for (;;) {
        do {
            i++;
        } while (A[i] < pivot);

        do {
            j--;
        } while (A[j] > pivot);

        if (i >= j)
            return j;

        int _ = A[i];
        A[i] = A[j];
        A[j] = _;
    }
}

void quicksort(std::vector<int> &A, int lo, int hi) {
    if (lo < hi) {
        int p = partition(A, lo, hi);
        quicksort(A, lo, p);
        quicksort(A, p + 1, hi);
    }
}

std::vector<int> create_array() {
    std::vector<int> v;
    std::random_device rd;   // used to obtain a seed for the random number engine
    std::mt19937 gen(rd());  // standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(0, SCREEN_WIDTH);
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        v.push_back(dis(gen));
    }
    return v;
}

int main(int argc, char *args[]) {
    SDL_Window *window = nullptr;
    SDL_Surface *screenSurface = nullptr;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not be initialize: " << SDL_GetError() << std::endl;
    } else {
        window = SDL_CreateWindow("SDL Tutorial",
                                  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                  SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == nullptr) {
            std::cout << "Window could not be created: " << SDL_GetError() << std::endl;
        } else {
            screenSurface = SDL_GetWindowSurface(window);
            if (screenSurface == nullptr) {
                std::cout << "Couldn't obtain screen surface: " << SDL_GetError() << std::endl;
            }
        }
    }

    bool quit = false;
    SDL_Event e;

    SDL_Rect rect = {10, 10, 10, 10};
    auto numbers = create_array();
    quicksort(numbers, 0, int(numbers.size()));

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                auto sym = e.key.keysym.sym;

                if (sym == SDLK_UP) {
                    rect.y -= 10;
                } else if (sym == SDLK_DOWN) {
                    rect.y += 10;
                } else if (sym == SDLK_LEFT) {
                    rect.x -= 10;
                } else if (sym == SDLK_RIGHT) {
                    rect.x += 10;
                }
            }
        }

        SDL_FillRect(screenSurface, nullptr, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00));

        for (int i = 0, ln = int(numbers.size()); i < ln; i++) {
            SDL_Rect r = {0, i, numbers[i], 1};
            SDL_FillRect(screenSurface, &r, SDL_MapRGB(screenSurface->format, 0x00, 0xFF, 0x00));
        }

        SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0xFF, 0x00, 0x00));

        SDL_UpdateWindowSurface(window);
    }

    if (window) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();

    return 0;
}