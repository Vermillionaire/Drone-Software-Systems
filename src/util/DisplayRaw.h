#include <SDL2/SDL.h>

class DisplayRaw {

    public:
        DisplayRaw() {};
        static const int WIDTH=640;
        static const int HEIGHT=480;
        static void post(short* frameData);
        static void init();
        static void destroy();
    private:
        //static SDL_Surface* hello;
        //static SDL_Surface* screen;
        static SDL_Window* win;
        static SDL_Renderer *ren;




};
