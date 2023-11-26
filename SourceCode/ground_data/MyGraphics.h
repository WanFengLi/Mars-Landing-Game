//
//This file is developed by Han Li
//
#include <graphics_lib.h>
#include <amio_lib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

//Use real Mars acceleration data to influence the spacecraft
#define gMars 3.72

//define the size of window
#define Window_width   1200
#define Window_height   800
#define Prompt_width    200
#define Prompt_height   100

//define the position of hints in every level
#define hint_text_pos_X  25
#define hint_text_pos_Y 350

//define the width and height of spacecraft
#define ship_width       80
#define ship_height      40

//these lines shows the initiate position of ships
#define ship_init_posX  300
#define ship_init_posY  200
#define alien_init_posX 800
#define alien_init_posY 200

//These lines define parameters of Cloud
#define cloud_pos1_X    500
#define cloud_pos1_Y    200
#define cloud_pos2_X    975
#define cloud_pos2_Y    525
#define cloud_radius1   100
#define cloud_radius2    60

//These lines define parameters of Sun
#define sun_pos_X  1200
#define sun_pos_Y   100
#define sun_radius  200
#define sun_angle    90

//These lines define initiate value of fireballs
#define ball_radius   6
#define ball_1_ID     0
#define ball_2_ID     1
#define ball_3_ID     2

#define ball_1_InitX   993
#define ball_1_InitY   161
#define ball_2_InitX  1045
#define ball_2_InitY   255
#define ball_3_InitX  1144
#define ball_3_InitY   312

#define ball_1_Vx     -20
#define ball_1_Vy     5.2
#define ball_2_Vx      -8
#define ball_2_Vy       8
#define ball_3_Vx    -1.3
#define ball_3_Vy       5

//The explanation of structures and functions
//have detailed comments in the source code

typedef struct pos
{
    int x;
    int y;
}pos;

typedef struct hline
{
    int left,right,height;
}hline;

typedef struct vline
{
    int bottom,top,width;
}vline;

typedef struct ground
{
    hline hlines[10];
    vline vlines[10];
    int hline_num,vline_num;
    hline safe_line;
}ground;

typedef struct ball
{
    int number;
    int x,y;
    int vx,vy;
    int r;
}ball;

typedef struct ship
{
    int x,y;
    int vx,vy;
    int dvx,dvy;
}ship;

void read_ground_data();
void myinit();

void myship_init();
void draw_myship();
void clean_myship();
void myship_move();

void ai_ship_init();
void draw_ai_ship();
void clean_ai_ship();
void ai_ship_move();

void ball_init(int number);
void draw_balls();
void clean_balls();
void balls_move();

void draw_menue_background();
void choose_level();
void menue();

void print_tips();
void draw_ground(ground groundn);
void draw_map();

void challenges_completed();
void success();
void fail();

void check_boundary(ground groundn);
void check_mouse();
void check_keyboard();

void wind_change();
