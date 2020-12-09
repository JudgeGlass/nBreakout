#include <os.h>
#include <SDL/SDL_config.h>
#include <SDL/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "apple.h"
#include "bumper.h"
#include "gameover.h"
#include "background.h"
#include "green_brick.h"
#include "menu.h"
#include "orange_brick.h"
#include "red_brick.h"
#include "yellow_brick.h"

enum Color{
    RED,
    ORANGE,
    GREEN,
    YELLOW
};


typedef struct Paddle {
    int x;
    int y;
} paddle_t;

typedef struct Bricks {
    int x;
    int y;
    int w;
    int h;
    Uint8 active;
    enum Color color;
} bricks_t;


SDL_Surface *screen;
SDL_Surface *background;
SDL_Surface *img_gameover;
SDL_Surface *img_menu;
SDL_Surface *img_paddle;
SDL_Surface *img_ball;
SDL_Surface *img_red_brick;
SDL_Surface *img_orange_brick;
SDL_Surface *img_green_brick;
SDL_Surface *img_yellow_brick;

Uint8 done;
int dx = 2;
int dy = -2;
int ball_x = (320 / 2) - 4;
int ball_y = 240 - 32;
Uint8 lives = 5;

paddle_t paddle;
bricks_t bricks[8*11];

void init(void){
    SDL_Init(SDL_INIT_VIDEO);
    screen = SDL_SetVideoMode(320, 240, has_colors ? 16 : 8, SDL_SWSURFACE);
    done = SDL_FALSE;
}

void quit(void){
    SDL_FreeSurface(screen);
    SDL_FreeSurface(img_paddle);
    SDL_FreeSurface(img_ball);
    SDL_FreeSurface(img_yellow_brick);

    SDL_FreeSurface(img_green_brick);
    SDL_FreeSurface(img_red_brick);
    SDL_FreeSurface(background);
    SDL_FreeSurface(img_gameover);
    

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

Uint8 is_colliding(int x, int y, bricks_t brick){
    if(x < (brick.x + brick.w) && (x + 8) > brick.x && y < (brick.y + brick.h) && y + 8 > brick.y){
        return 1;
    }

    return 0;
}


void handle_key(){
    Uint8 *keystates = SDL_GetKeyState(NULL);

    if(keystates[SDLK_LEFT] || keystates[SDLK_4]){
        if((paddle.x) - 4 > 0){
            paddle.x -= 4;
        }
    }

    if(keystates[SDLK_RIGHT] || keystates[SDLK_6]){
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

void make_direction(){
    Uint8 cx = randomUint(0, 1);
    Uint8 cy = randomUint(0, 1);

    if(cx){
        dx = -dx;
    }

    dy = -dy;
}


void render(){
    if(lives == 0){
        draw_image(img_gameover, screen, 0 , 0);
        SDL_Flip(screen);
        SDL_Delay(2000);
        quit();
    }

    //Draw Background
    draw_image(background, screen, 0, 0);
    //SDL_Flip(screen);

    //Draw Paddle
    draw_image(img_paddle, screen, paddle.x, paddle.y);
    //SDL_Flip(screen);

    Uint8 colliding = 0;

    //Draw bricks
    for(int i = 0; i < 8*11; i++){
        SDL_Surface *img = malloc(sizeof(img_red_brick));
        switch (bricks[i].color)
        {
        case RED:
            img = img_red_brick;
            break;
        case ORANGE:
            img = img_orange_brick;
            break;
        case GREEN:
            img = img_green_brick;
            break;
        case YELLOW:
            img = img_yellow_brick;
            break;
        default:
            break;
        }

        colliding = is_colliding(ball_x + dx, ball_y + dy, bricks[i]) && bricks[i].active;
        if(colliding) {
            bricks[i].active = 0;
            make_direction();
        }

        if(bricks[i].active){
            draw_image(img, screen, bricks[i].x, bricks[i].y);
            //SDL_Flip(screen);
        }
    }

    bricks_t brick;
    brick.h = 8;
    brick.w = 64;
    brick.x = paddle.x;
    brick.y = paddle.y;
    //Draw ball
    if(ball_x + dx > 320 - 4 || ball_x + dx < 4 || colliding || is_colliding(ball_x + dx, ball_y + dy, brick)){
        make_direction();
    }else 

    if(ball_y + dy > 240 - 4){
        lives--;
        ball_x = (320 / 2) - 4;
        ball_y = 240 - 32;
        make_direction();
    }else

    if(ball_y + dy < 4 || colliding || is_colliding(ball_x + dx, ball_y + dy, brick)){
        make_direction();
    }

    ball_x += dx;
    ball_y += dy;

    draw_image(img_ball, screen, ball_x, ball_y);
    //SDL_Flip(screen);
}


int main(int argc, char *argv[]){
    srand(time(0));

    init();

    background = nSDL_LoadImage(image_background);
    img_gameover = nSDL_LoadImage(image_gameover);
    img_menu = nSDL_LoadImage(image_menu);
    img_paddle = nSDL_LoadImage(image_bumper);
    img_ball = nSDL_LoadImage(image_ball);
    // background = load_image("background.bmp");
    // img_gameover = load_image("gameover.bmp");
    // img_menu = load_image("menu.bmp");
    // img_paddle = load_image("bumper.bmp");
    // img_ball = load_image("ball.bmp");


    img_red_brick = nSDL_LoadImage(image_red_brick);
    img_green_brick = nSDL_LoadImage(image_green_brick);
    img_orange_brick = nSDL_LoadImage(image_orange_brick);
    img_yellow_brick = nSDL_LoadImage(image_yellow_brick);
    // img_red_brick = load_image("red_brick.bmp");
    // img_orange_brick = load_image("orange_brick.bmp");
    // img_green_brick = load_image("green_brick.bmp");
    // img_yellow_brick = load_image("yellow_brick.bmp");

    paddle.x = (320 / 2) - 32;
    paddle.y = 240 - 16;

    draw_image(img_menu, screen, 0, 0);
    SDL_Flip(screen);
    SDL_Delay(2000);

    for(int x = 0; x < 11; x++){
        for(int y = 1; y < 8; y++){
            bricks[x + y * 11].x = (x * 28) + 8;
            bricks[x + y * 11].y = (y * 16);
            bricks[x + y * 11].w = 24;
            bricks[x + y * 11].h = 8;
            bricks[x + y * 11].active = 1;
            
            enum Color color = RED;

            if(y == 1 || y == 2){
                color = RED;
            }else if(y == 3 || y == 4){
                color = ORANGE;
            }else if(y == 5 || y == 6){
                color = GREEN;
            }else{
                color = YELLOW;
            }

            bricks[x + y*11].color = color;
        }
    }
    

    while(!done){
        
        render();
        

        handle_key();
        
        
        SDL_Flip(screen);
        SDL_Delay(30);
    }

    //SDL_Delay(500);
    //SDL_Quit();
    return 0;
}

