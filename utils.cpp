#include <iostream>
#include <vector>
#include <cstdint>

int conversion(int x, int y, int largeur, int hauteur) {
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
     for (int i = cx - r; i <= cx + r; i++) {
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
) {
    int rapport1 = largeur_pix / largeur_map;
    int rapport2 = hauteur_pix / hauteur_map;
    for (int i = 0; i < map.size(); i++){
        if (map[i] == 0){
            int abs = i%largeur_map;
            int ord = i/largeur_map;
            fill_rectangle(pixels, largeur_pix, hauteur_pix, abs * rapport1, ord * rapport2, rapport1, rapport2, 0xFFFFFFFF);

        }
        else if (map[i] == 1){
            int abs = i%largeur_map;
            int ord = i/largeur_map;
            fill_rectangle(pixels, largeur_pix, hauteur_pix, abs * rapport1, ord * rapport2, rapport1, rapport2, 0xFF000000);

        };

    }
}


bool collision(int pos_x, int pos_y, const std::vector<int>& map, int largeur_map, int hauteur_map, 
int largeur_pix, int hauteur_pix) {
    int rapport1 = largeur_pix / largeur_map;
    int rapport2 = hauteur_pix / hauteur_map;
    int x = pos_x / rapport1;
    int y = pos_y / rapport2;
    if (x < 0 || x >= largeur_map || y < 0 || y >= hauteur_map) {
        return true;
    }
    else {
        return map[y * largeur_map + x] == 1;
    }
}


int main(){
    return 0;
}