#include <iostream>
#include <vector>
#include <cstdint>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>


int conversion(int x, int y, int largeur, int hauteur){
    return y * largeur + x;
}

void fill_rectangle(
    std::vector<uint32_t>& pixels,
    int largeur,
    int hauteur,
    int x0,
    int y0,
    int w,
    int h,
    uint32_t color
){
    
    for (int i = x0 ; i < x0 + w; i++){
        for (int j = y0; j < y0 + h; j++ ){
            pixels[conversion(i, j, largeur, hauteur)] = color;

        }

    }


}

void fill_circle(
    std::vector<uint32_t>& pixels,
    int largeur,
    int hauteur,
    int cx,
    int cy,
    int r,
    uint32_t color
){
     for (int i = cx - r; i <= cx + r; i++){
        for (int j = cy - r; j <= cy + r; j++ ){
            if ((cx - i) * (cx - i) + (cy - j) * (cy - j) <= r*r) {
                if (i >= 0 && i < largeur &&
                    j >= 0 && j < hauteur) {
                        pixels[conversion(i, j, largeur, hauteur)] = color;

                    }
                
            }

        }

    }
}

// Prend une map de 0 et 1 et dessine les cases correspondantes sur l'image
void draw_map(
    std::vector<uint32_t>& pixels,
    const std::vector<int>& map,
    int largeur_pix,
    int hauteur_pix,
    int largeur_map,
    int hauteur_map
){
    int rapport1 = largeur_pix / largeur_map;
    int rapport2 = hauteur_pix / hauteur_map;
    for (int i = 0; i < map.size(); i++){
        if (map[i] == 0){
            int abs = i%largeur_map;
            int ord = i/largeur_map;
            fill_rectangle(pixels, largeur_pix, hauteur_pix, abs * rapport1, ord * rapport2, rapport1, rapport2, 0xFFFFFFFF);

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

    // Initialiser SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << SDL_GetError() << '\n';
        return 1;
    }

    // Créer la fenêtre et le renderer
    SDL_Window* window = nullptr; //pointer vers la fenêtre SDL
    SDL_Renderer* renderer = nullptr; //pointer vers le renderer SDL

    if (!SDL_CreateWindowAndRenderer(
            "Mon moteur graphique",
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
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

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
