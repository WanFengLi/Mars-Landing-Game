//
//This file is developed by Han Li
//
#include "MyGraphics.h"

//this variable indicates if the game is over(is = 1,no = 0)
int is_game_over=0;
//this variable indicates if screen should back to menu(is = 1,no = 0)
int back_to_menue=0;
//this variable indicates if key 'p' is pressed during playing(is = 1,no = 0)
int is_pause=0;
//this variable indicates if user select the challenge mode(is = 1,no = 0)
int is_challenge_mode=1;
//this variable indicates number of attempts in challenge mode(start from 1)
int attempts=1;
//this variable indicates the score in challenge mode(Start with 100, -1 for each failure)
int score=100;
//this variable indicates the level now(start from 1)
int level=1;
//this variable indicates the clock to count time for wind change
int nclock=0,lclock=0;
//this variable indicates the wind stage(6 stages in total to recycle and start from 0)
int wind_kind=0;
//this variable indicates all wind stage and regular their magnitude
int wind_speeds[]={-3,6,-9,3,-6,9};

//These lines initiate value of fireballs to prepare for using in level 7
ball balls[]={{ball_1_ID,ball_1_InitX,ball_1_InitY,ball_1_Vx,ball_1_Vy,ball_radius},
              {ball_2_ID,ball_2_InitX,ball_2_InitY,ball_2_Vx,ball_2_Vy,ball_radius},
              {ball_3_ID,ball_3_InitX,ball_3_InitY,ball_3_Vx,ball_3_Vy,ball_radius}};

//Define an array containing ten ground structures to prepare for using and detecting
ground grounds[10]; //set ground[0]as nothing and use value from ground[1] for intuitive

//Define two ship structures to store data of own ship and alien ship
ship myship,ai_ship;


int main(void)
{
    //initialize all devices and prepare the data to be extracted
    myinit();

    //main loop of the game
    while(true)
    {
        //draw the background, text hint and provide an interface for selecting modes
        menue();
        //initialize the variable to go to play stage
        back_to_menue=0;
        //check if game over, if is close the whole window
        if(is_game_over) break;

        //Draw terrain,ship, hint text and special images for each level
        draw_map();
        //every time before start, give one second to led user prepare
        Sleep(1000);
        //clear all queue to avoid interference
        al_flush_event_queue(event_queue);

        //the main loop that controls the game process
        //first check if logic needs to back to menu
        while(!back_to_menue)
        {
            //use clock() to count time for wind change
            nclock=clock()/CLOCKS_PER_SEC;
            //check if user want to quit or pause the game
            check_keyboard();
            //only when not pause,we start playing
            if(!is_pause)
            {
                //change wind direction and wind force regularly
                wind_change();
                //Set influence of the mouse position on the speed of the spacecraft
                check_mouse();
                //draw a blank ship outline to erase old image
                clean_myship();
                //Calculate the displacement of the spacecraft based on the speed
                myship_move();
                //draw the ship outline with parameters now
                draw_myship(myship);
                //in level 5 we should draw alien ship and let it move
                //the description of those functions are very detailed in MyGraphics.c
                if(level==5)
                {
                    clean_ai_ship();
                    ai_ship_move();
                    draw_ai_ship();
                }
                //in level 7 we should set fire balls to interference the ship
                //the description of those functions are very detailed in MyGraphics.c
                if(level==7)
                {
                    clean_balls();
                    balls_move();
                    draw_balls();
                }
                //refresh the screen every 0.1 seconds
                Sleep(100);
                update_display();
                //This function completes all collision detection
                check_boundary(grounds[level]);
            }
        }
    }

    closegraph();
    return 0;
}
