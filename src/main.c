#include <SDL/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "apple.h"
#include "player.h"

typedef struct Paddle {
    int x;
    int y;
} paddle_t;


SDL_Surface *screen;
SDL_Surface *background;
SDL_Surface *img_gameover;
SDL_Surface *img_menu;
SDL_Surface *img_paddle;
SDL_Surface *img_ball;

Uint8 done;

paddle_t paddle;

void init(void){
    SDL_Init(SDL_INIT_VIDEO);
    screen = SDL_SetVideoMode(320, 240, 32, SDL_SWSURFACE);
    done = SDL_FALSE;
}

void quit(void){
    SDL_FreeSurface(screen);
    SDL_FreeSurface(img_paddle);
    SDL_FreeSurface(img_ball);
    

    SDL_Quit();
    done = SDL_TRUE;
}

SDL_Surface* load_image(const char *filename){
    SDL_Surface *image = SDL_LoadBMP(filename);
    SDL_Surface *op_image = SDL_DisplayFormat(image);
    SDL_FreeSurface(image);
    image = NULL;
    return op_image;
}

Uint8 randomUint(Uint8 min, Uint8 max){
    return (rand() % (max - min + 1));
}

void draw_image(SDL_Surface* src, SDL_Surface* des, int x, int y){
    SDL_Rect rect_pos;
    rect_pos.x = x;
    rect_pos.y = y;
    SDL_BlitSurface(src, NULL, des, &rect_pos);
}


void handle_key(){
    Uint8 *keystates = SDL_GetKeyState(NULL);


    if(keystates[SDLK_LEFT]){
        if((paddle.x) - 4 > 0){
            paddle.x -= 4;
        }
    }

    if(keystates[SDLK_RIGHT]){
        if(paddle.x + 64 + 4 < 320){
            paddle.x += 4;
        }
    }

    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch (event.type)
        {
        case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_ESCAPE){
                quit();
            }
            break;
        
        default:
            break;
        }
    }

}

void render(){
    //Draw Background
    draw_image(background, screen, 0, 0);
    SDL_Flip(screen);

    //Draw Paddle
    draw_image(img_paddle, screen, paddle.x, paddle.y);
    SDL_Flip(screen);


}


int main(int argc, char *argv[]){
    srand(time(0));

    init();

    background = load_image("background.bmp");
    img_gameover = load_image("gameover.bmp");
    img_menu = load_image("menu.bmp");
    img_paddle = load_image("bumper.bmp");
    img_ball = load_image("ball.bmp");

    paddle.x = (320 / 2) - 32;
    paddle.y = 240 - 16;
    

    while(!done){
        
        render();
        

        handle_key();
        
        
        SDL_Flip(screen);
        SDL_Delay(20);
    }

    SDL_Delay(500);
    SDL_Quit();
    return 0;
}

