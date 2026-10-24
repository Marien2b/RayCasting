#include <iostream>
#include <vector>
#include <cstdint>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "utils.cpp"


struct Player {
    int x;
    int y;
    int radius;
};

struct Ray {
    float Dir_x;    // Direction du rayon
    float Dir_y;    // Direction du rayon
    float Plane_x;  // Plan de la caméra
    float Plane_y;  // Plan de la caméra
};


void handle_events(bool& running, Player& player, const std::vector<int>& map, int largeur_map, int hauteur_map, int largeur, int hauteur) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {

            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }

            if (event.type == SDL_EVENT_KEY_DOWN){
                switch (event.key.key) {
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                    case SDLK_UP:
                    // Vérifier si le joueur peut se déplacer vers le haut
                        if (!collision(player.x, player.y - 5, map, largeur_map, hauteur_map, largeur, hauteur)) {
                            player.y -= 5;
                        }
                        break;
                    case SDLK_DOWN:
                        if (!collision(player.x, player.y + 5, map, largeur_map, hauteur_map, largeur, hauteur)) {
                            player.y += 5;
                        }
                        break;
                    case SDLK_LEFT:
                        if (!collision(player.x - 5, player.y, map, largeur_map, hauteur_map, largeur, hauteur)) {
                            player.x -= 5;
                        }
                        break;
                    case SDLK_RIGHT:
                        if (!collision(player.x + 5, player.y, map, largeur_map, hauteur_map, largeur, hauteur)) {
                            player.x += 5;
                        }
                        break;
                }
            }
        }

    }


int main(int argc, char* argv[]) {
    int largeur = 800;
    int hauteur = 600;
    std::vector<int> map = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 1, 1, 1, 1, 0, 0, 1,
        1, 0, 1, 0, 0, 0, 0, 1, 0, 1,
        1, 0, 1, 0, 0, 1, 0, 1, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1, 0, 1,
        1, 0, 1, 1, 1, 0, 1, 1 ,0 ,1,
        1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1,
        1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1
    };
    int largeur_map = 10;
    int hauteur_map = 9;

    std::vector<uint32_t> pixels(
        largeur * hauteur, 0xFF000000
    );

    // Dessiner la carte
    draw_map(pixels, map, largeur, hauteur, largeur_map, hauteur_map);

    // Position du joueur
    Player player;
    player.x = 100;
    player.y = 100;
    player.radius = 10;

    //Rayon de vision du joueur
    Ray ray;
    
    fill_circle(pixels, largeur, hauteur, player.x, player.y, player.radius, 0xFF00FF00); //Joueur vert

    // Initialiser SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << SDL_GetError() << '\n';
        return 1;
    }

    
    // Créer la fenêtre et le renderer
    SDL_Window* window = nullptr; //pointer vers la fenêtre SDL
    SDL_Renderer* renderer = nullptr; //pointer vers le renderer SDL

    if (!SDL_CreateWindowAndRenderer(
            "RayCaster",
            largeur, hauteur, 0,
            &window, &renderer)) {
        std::cerr << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    // Créer une texture
    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        largeur, hauteur
    );

    if (!texture) {
        std::cerr << SDL_GetError() << '\n';
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;

    while (running) {

        handle_events(running, player, map, largeur_map, hauteur_map, largeur, hauteur);
        
        draw_map(pixels, map, largeur, hauteur, largeur_map, hauteur_map);
        SDL_PumpEvents();

        //On dessine une ligne de direction pointant vers la souris
        float souris_x, souris_y;
        SDL_GetMouseState(&souris_x, &souris_y);
        ray.Dir_x = souris_x - player.x;
        ray.Dir_y = souris_y - player.y;

        
        
        fill_circle(pixels, largeur, hauteur, player.x, player.y, player.radius, 0xFF00FF00);

        SDL_UpdateTexture(
            texture, nullptr,
            pixels.data(),
            largeur * sizeof(uint32_t)
        );

        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
