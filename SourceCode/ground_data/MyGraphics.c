//
//This file is developed by Han Li
//
#include "MyGraphics.h"

//These lines use extern to avoid multiple define
//the explanation of definition is in Main.c
extern int is_game_over;
extern int back_to_menue;
extern int is_pause;
extern int is_challenge_mode;
extern int attempts;
extern int score;
extern int level;
extern int nclock;
extern int lclock;
extern int wind_kind;
extern int wind_speeds[];
extern ball balls[];
extern ground grounds[];
extern ship myship;
extern ship ai_ship;


//this function read ground data and stored to prepare for using
void read_ground_data()
{
    //Create an array to pass data
    char path[50]={};
    //Create counter to count
    int i,j;
    //use for loop to iterate over data
    //different level use different files to form their map
    for(j=1;j<=7;j++)
    {
        i=0;
        //Read the horizontal line data prepared in the text document
        //the text data is stored in src folder
        sprintf(path,"src/ground_data/ground%d/hlines.txt",j);
        FILE *fp=fopen(path,"r");
        //these lines try to find how many lines in the file
        while( fscanf(fp,"%d%d%d",&grounds[j].hlines[i].left,&grounds[j].hlines[i].right,&grounds[j].hlines[i].height) == 3 )
            i++;
        //this line get the number of lines
        grounds[j].hline_num=i;

        //these lines work in same principle but prepare the data of vertical lines
        sprintf(path,"src/ground_data/ground%d/vlines.txt",j);
        fp=fopen(path,"r");
        i=0;
        while( fscanf(fp,"%d%d%d",&grounds[j].vlines[i].bottom,&grounds[j].vlines[i].top,&grounds[j].vlines[i].width) == 3 )
            i++;
        grounds[j].vline_num=i;

        //these lines work in same principle but prepare the data of landing lines
        sprintf(path,"src/ground_data/ground%d/safe_line.txt",j);
        fp=fopen(path,"r");
        fscanf(fp,"%d%d%d",&grounds[j].safe_line.left,&grounds[j].safe_line.right,&grounds[j].safe_line.height);
    }
}

//these lines initialize all devices and prepare the data to be extracted
void myinit()
{
    //this function read ground data and stored to prepare for using
    read_ground_data();

    //these lines initiate the window and other devices to prepare
    initwindow(Window_width,Window_height);
    initmouse();
    initkeyboard();
    initfont();
    midi_start();

    //these lines initiate event statements and make them prepared to be used
    create_event_queue();
    reg_mouse_events();
    reg_display_events();
    reg_keyboard_events();
}

//initiate spacecraft parameters
void myship_init()
{
    myship.x=ship_init_posX; myship.y=ship_init_posY;
    //set initiate speed and acceleration as 0
    myship.vx=0;  myship.vy=0;
    myship.dvx=0; myship.dvy=0;
}

//draw the ship outline
void draw_myship()
{
    filled_ellipse(myship.x,myship.y+5,40,10,LIGHTCYAN);
    filled_ellipse(myship.x,myship.y-5,20,10,WHITE);
}

//draw a blank ship outline to erase old image
void clean_myship()
{
    filled_ellipse(myship.x,myship.y+5,40,10,BLACK);
    filled_ellipse(myship.x,myship.y-5,20,10,BLACK);
}

//Calculate the displacement of the spacecraft based on the speed
void myship_move()
{
    myship.x += myship.vx;
    myship.y += myship.vy+gMars;
    //if in level 6 consider the effect of the wind
    if(level==6) myship.x += wind_speeds[wind_kind];
}

//initiate alien ship parameters
void ai_ship_init()
{
    ai_ship.x=alien_init_posX; ai_ship.y=alien_init_posY;
    ai_ship.vx=-10;
}

//draw the alien ship outline
void draw_ai_ship()
{
    filled_ellipse(ai_ship.x,ai_ship.y+5,40,10,LIGHTBLUE);
    filled_ellipse(ai_ship.x,ai_ship.y-5,20,10,RED);
}

//draw a blank ship outline to erase old image of alien ship
void clean_ai_ship()
{
    filled_ellipse(ai_ship.x,ai_ship.y+5,40,10,BLACK);
    filled_ellipse(ai_ship.x,ai_ship.y-5,20,10,BLACK);
}

//Calculate the displacement of the alien ship based on the speed
void ai_ship_move()
{
    //before change speed detect if the spacecraft touch the side boundary
    //if touch boundary reverse speed and keep moving
    if(ai_ship.x<400) ai_ship.vx *= -1;
    else if(ai_ship.x>1100) ai_ship.vx *= -1;
    ai_ship.x+=ai_ship.vx;
}

//initiate parameters of three fire balls
void ball_init(int number)
{
    filled_circle(balls[number].x,balls[number].y,balls[number].r,BLACK);
    switch(number)
    {
        case 0: balls[number].x=ball_1_InitX; balls[number].y=ball_1_InitY;
                break;
        case 1: balls[number].x=ball_2_InitX; balls[number].y=ball_2_InitY;
                break;
        case 2: balls[number].x=ball_3_InitX; balls[number].y=ball_3_InitY;
    }
}

//draw three fire balls in one go by iteration loop
void draw_balls()
{
    int i;
    for(i=0;i<3;i++)
    {
        filled_circle(balls[i].x,balls[i].y,balls[i].r,RED);
    }
}

//draw blank balls to erase the old image
void clean_balls()
{
    int i;
    for(i=0;i<3;i++)
    {
        filled_circle(balls[i].x,balls[i].y,balls[i].r,BLACK);
    }
}

//Calculate the displacement of the fire balls based on the speed
void balls_move()
{
    int i;
    for(i=0;i<3;i++)
    {
        balls[i].x+=balls[i].vx;
        balls[i].y+=balls[i].vy;
    }
}

//draw the background, terrain and small spacecraft
void draw_menue_background()
{
    filled_rectangle(0,0,Window_width,Window_height,LIGHTBLUE);

    //these number will be used only once so not use #define to show
    filled_rectangle(0,500,300,800,TANGERINE);
    filled_rectangle(300,450,600,800,TANGERINE);
    filled_rectangle(600,550,800,800,TANGERINE);
    filled_rectangle(800,475,1200,800,TANGERINE);
    //these lines shows ship in menu bar
    filled_ellipse(100,100+5,40,10,LIGHTCYAN);
    filled_ellipse(100,100-5,20,10,WHITE);
}

void choose_level()
{
    //draw the background, terrain and small spacecraft
    draw_menue_background();

    //show every level in appropriate position that user can choose
    outtextxy(525,100,"Normal");
    outtextxy(525,150,"Hard");
    outtextxy(525,200,"Slop");
    outtextxy(525,250,"Cloud");
    outtextxy(525,300,"Alien");
    outtextxy(525,350,"Martian wind");
    outtextxy(525,400,"Solar flare");

    update_display();

    //Set selection logic, let user select level by mouse
     while(true)
    {
        //clear all queue to avoid interference
        al_flush_event_queue(event_queue);
        //when mouse is in window capture its coordinate
        wait_for_event();
        get_mouse_coordinates();
        //when left button down check which bar is selected
        wait_for_event();
        if(event_mouse_left_button_down())
        {
            //check the position that the mouse click on
            //change the variable of 'level' to enter corresponding level
                 if(YMOUSE>=104 && YMOUSE<=110 && XMOUSE>520 && XMOUSE<570) {level=1; return;}
            else if(YMOUSE>=154 && YMOUSE<=162 && XMOUSE>520 && XMOUSE<560) {level=2; return;}
            else if(YMOUSE>=204 && YMOUSE<=212 && XMOUSE>520 && XMOUSE<560) {level=3; return;}
            else if(YMOUSE>=254 && YMOUSE<=262 && XMOUSE>520 && XMOUSE<565) {level=4; return;}
            else if(YMOUSE>=304 && YMOUSE<=312 && XMOUSE>520 && XMOUSE<565) {level=5; return;}
            else if(YMOUSE>=354 && YMOUSE<=362 && XMOUSE>520 && XMOUSE<625) {level=6; return;}
            else if(YMOUSE>=404 && YMOUSE<=412 && XMOUSE>520 && XMOUSE<615) {level=7; return;}
            //clear all queue to avoid interference
            al_flush_event_queue(event_queue);
        }
    }
}

//draw the background, text hint and provide an interface for selecting modes
void menue()
{
    //draw the menu back ground
    draw_menue_background();

    //show welcome and mode selection bar
    setcolor(BLACK);
    outtextxy(225,100,"Welcome to Mars Landing!");
    outtextxy(525,250,"Challenge mode");
    outtextxy(525,300,"Free mode");
    outtextxy(525,350,"Exit");
    update_display();

    //Set selection logic, let user select mode by mouse
    while(true)
    {
        //clear all queue to avoid interference
        al_flush_event_queue(event_queue);
        //when mouse is in window capture its coordinate
        wait_for_event();
        get_mouse_coordinates();
        //when left button down check which bar is selected
        wait_for_event();
        if(event_mouse_left_button_down())
        {
            //if mouse click at the position of challenge bar start challenge mode
            if(YMOUSE>=254 && YMOUSE<=262 && XMOUSE>520 && XMOUSE<610)
            {
                is_challenge_mode=1;
                //if in challenge mode start from level 1
                attempts=1; score=100;
                level=1;
                return;
            }
            //if mouse click at the position of free bar start free mode
            else if(YMOUSE>=304 && YMOUSE<=312 && XMOUSE>520 && XMOUSE<600)
            {
                is_challenge_mode=0;
                //if in free mode switch to selection interface
                choose_level();
                return;
            }
            //if mouse click at the position of exit bar
            else if(YMOUSE>=354 && YMOUSE<=362 && XMOUSE>520 && XMOUSE<560)
            {
	            //set mode to game over and close game window
	            is_game_over=1;
                return;
            }
            //clear all queue to avoid interference
            al_flush_event_queue(event_queue);
        }
    }
}

//set prompts according to the difficulty of each level
void print_tips()
{
    //The prompt starts with "be careful"
    outtextxy(15,300,"Be careful:");
    //each level have different character
    switch(level)
    {
        case 1:
            outtextxy(hint_text_pos_X,hint_text_pos_Y,"Try to control");
            outtextxy(hint_text_pos_X,hint_text_pos_Y+25,"the ship to");
            outtextxy(hint_text_pos_X,hint_text_pos_Y+50,"land at the");
            outtextxy(hint_text_pos_X,hint_text_pos_Y+75,"blue platform");
            break;
        case 2:
            outtextxy(hint_text_pos_X,hint_text_pos_Y,"The platform is");
            outtextxy(hint_text_pos_X,hint_text_pos_Y+25,"smaller than level 1");
            outtextxy(hint_text_pos_X,hint_text_pos_Y+50,", try not hit the");
            outtextxy(hint_text_pos_X,hint_text_pos_Y+75,"rock surface of Mars");
            break;
        case 3:
            outtextxy(hint_text_pos_X,hint_text_pos_Y,"You should land");
            outtextxy(hint_text_pos_X,hint_text_pos_Y+25,"in a valley");
            outtextxy(hint_text_pos_X,hint_text_pos_Y+50,"in this level,");
            outtextxy(hint_text_pos_X,hint_text_pos_Y+75,"try not hit the");
            outtextxy(hint_text_pos_X,hint_text_pos_Y+100,"rock surface of Mars");
            break;
        case 4:
            outtextxy(hint_text_pos_X,hint_text_pos_Y,"Three are harmful");
            outtextxy(hint_text_pos_X,hint_text_pos_Y+25,"gases in circle clouds.");
            outtextxy(hint_text_pos_X,hint_text_pos_Y+50,"Keep away from them");
            outtextxy(hint_text_pos_X,hint_text_pos_Y+75,"and land at the");
            outtextxy(hint_text_pos_X,hint_text_pos_Y+100,"blue platform!");
            break;
        case 5:
            outtextxy(hint_text_pos_X,hint_text_pos_Y,"This area have");
            outtextxy(hint_text_pos_X,hint_text_pos_Y+25,"aggressive alien");
            outtextxy(hint_text_pos_X,hint_text_pos_Y+50,"ship. Avoid them");
            outtextxy(hint_text_pos_X,hint_text_pos_Y+75,"while landing");
            break;
         case 6:
            outtextxy(hint_text_pos_X,hint_text_pos_Y,"This area suffer");
            outtextxy(hint_text_pos_X,hint_text_pos_Y+25,"form Martin wind");
            outtextxy(hint_text_pos_X,hint_text_pos_Y+50,"Try to balance");
            outtextxy(hint_text_pos_X,hint_text_pos_Y+75,"your ship here");
            break;
        case 7:
            outtextxy(hint_text_pos_X,hint_text_pos_Y,"This area suffer");
            outtextxy(hint_text_pos_X,hint_text_pos_Y+25,"form Solar flare");
            outtextxy(hint_text_pos_X,hint_text_pos_Y+50,"Try to control");
            outtextxy(hint_text_pos_X,hint_text_pos_Y+75,"your ship to");
            outtextxy(hint_text_pos_X,hint_text_pos_Y+100,"avoid the fire ball");
            break;
    }
}

//draw the ground in the case of each level
void draw_ground(ground groundn)
{
    //set the ground color
    setcolor(TANGERINE);
    //use iterate loop to draw the terrain in correct level
    int i;
    //this loop draws the horizontal line
    for(i=0;i<groundn.hline_num;i++)
        line(groundn.hlines[i].left,Window_height-groundn.hlines[i].height,groundn.hlines[i].right,Window_height-groundn.hlines[i].height,3);
    //this loop draws the vertical line
    for(i=0;i<groundn.vline_num;i++)
        line(groundn.vlines[i].width,Window_height-groundn.vlines[i].bottom,groundn.vlines[i].width,Window_height-groundn.vlines[i].top,3);
    //this loop coloring the Mars under the lines above
    for(i=0;i<groundn.hline_num;i++)
        filled_rectangle(groundn.hlines[i].left,Window_height-groundn.hlines[i].height,groundn.hlines[i].right,Window_height,TANGERINE);
    //draws the blue landing line
    setcolor(BLUE);
    line(groundn.safe_line.left,Window_height-groundn.safe_line.height,groundn.safe_line.right,Window_height-groundn.safe_line.height,4);
}


void draw_map()
{
    //set a buffer to pass variable to outtextxy()
    char buf[50];

    //draw overall frame of game window and prompt bar
    filled_rectangle(          0 ,           0 ,Window_width,Window_height,LIGHTBLUE);
    filled_rectangle(Prompt_width,Prompt_height,Window_width,Window_height,    BLACK);

    //set hints and score record on appropriate position in prompt bar
    setcolor(BLACK);
    //set prompts according to the difficulty of each level
    print_tips();

    //if in challenge mode show the scores recorded in real time
    if(is_challenge_mode)
    {
        sprintf(buf,"Your attempts:%d",attempts);
        outtextxy(500,80,buf);
        sprintf(buf,"Your score:%d",score);
        outtextxy(800,80,buf);
    }
    //initiate spacecraft parameters
    myship_init();

    //draw the ship outline
    draw_myship();
    //draw circle cloud in level 4
    if(level==4)
    {
        filled_circle(cloud_pos1_X,cloud_pos1_Y,cloud_radius1,LIGHTGRAY);
        filled_circle(cloud_pos2_X,cloud_pos2_Y,cloud_radius2,LIGHTGRAY);
    }
    //draw alien ship in level 5
    if(level==5)
    {
        ai_ship_init();
        draw_ai_ship();
    }
    //use red text to hint magnitude and direction of wind in level 6
    if(level==6)
    {
        setcolor(RED);
        //these position use only once so not use #define to state
        outtextxy(25,475,"Wind Level:");
        outtextxy(50,500,"LEFT 1");
    }
    //draw a sun outline in the right top corner of game window
    if(level==7)
    {
        setcolor(RED);
        arc(sun_pos_X,sun_pos_Y,sun_radius,sun_angle,sun_angle,10);
    }
    //draw the ground in the case of each level
    draw_ground(grounds[level]);
    update_display();
}

//show the score, attempts and succeed information if all level passed
void challenges_completed()
{
    //set a buffer to pass value to outtextxy()
    char buf[50];
    //only display this when in challenge mode
    if(is_challenge_mode)
    {
        //use menu background here
        draw_menue_background();
        //draw the text to cheer up
        setcolor(BLACK);
        outtextxy(500,275,"Congratulations!");
        outtextxy(500,300,"You completed all the challenges!");
        //draw the text to show score result of user
        sprintf(buf,"Your attempts is %d.",attempts);
        outtextxy(500,350,buf);
        sprintf(buf,"You got score:%d",score);
        outtextxy(500,375,buf);
        update_display();
        al_flush_event_queue(event_queue);
        //let user back to menu after succeed
        while(true)
        {
            wait_for_event();
            if(event_key_down() || event_mouse_left_button_down())
            {
                back_to_menue=1;
                return;
            }
        }
    }
    //if in free mode go back to menu directly
    else
    {
        back_to_menue=1;
        return;
    }
}

//Show the success interface, and let the user go ahead or quit
void success()
{
    //show succeed text on prompt bar
    setcolor(RED);
    outtextxy(20,150,"Success!");
    outtextxy(20,170,"Press g for next level");
    outtextxy(20,190,"Press q to quit");
    update_display();
    //display succeed sound
    //midi_note(80,1,60);
    midi_note(72,1,60);
    pause(150);
    midi_note(72,1,60);
    pause(150);
    midi_note(76,1,60);
    pause(150);
    midi_note(79,1,60);
    pause(300);
    midi_note(76,1,60);
    pause(150);
    midi_note(79,1,60);
    pause(600);
    //let user choose what to do next
    while(true)
    {
        al_flush_event_queue(event_queue);
        wait_for_event();
        //if g pressed after success, set next level to play
        if(event_key('g') || event_key('G'))
        {
            level+=1;
            //if this level is final one, show complete prompt
            if(level>7)
            {
                //show the score, attempts and succeed information if all level passed
                challenges_completed();
                return;
            }
            draw_map();
            Sleep(1500);
            al_flush_event_queue(event_queue);
            break;
        }
        //if q pressed back to menu
        else if(event_key('q') || event_key('Q'))
        {
            back_to_menue=1;
            break;
        }
    }
}

//Show the failure interface, and let the user go ahead or quit
void fail()
{
    //show failed text on prompt bar
    setcolor(RED);
    outtextxy(20,150,"Failed!");
    outtextxy(20,170,"Press g to try again");
    outtextxy(20,190,"Press q to quit");
    update_display();
    //display failed sound
    midi_note(30,1,70);
    pause(200);
    midi_note(29,1,70);
    pause(200);
    midi_note(28,1,70);
    pause(200);
    midi_note(27,1,70);
    pause(250);
    //let user choose what to do next
    while(true)
    {
        al_flush_event_queue(event_queue);
        wait_for_event();
        //if g pressed after fail, reset same level to play
        if(event_key('g') || event_key('G'))
        {
            if(is_challenge_mode){attempts+=1; score-=1;}
            draw_map();
            Sleep(1500);
            al_flush_event_queue(event_queue);
            break;
        }
        //if q pressed back to menu
        else if(event_key('q') || event_key('Q'))
        {
            back_to_menue=1;
            break;
        }
    }
}

//This function completes all collision detection
void check_boundary(ground groundn)
{
    //if ship touch side boundary, game failed
    if(myship.x-(ship_width/2)<=Prompt_width || myship.x+(ship_width/2)>=Window_width ||
       myship.y-(ship_height/2)<=Prompt_height || myship.y+(ship_height/2)>=Window_height)
    {
        //Show the failure interface, and let the user go ahead or quit
        fail();
        return;
    }
    //if ship touch blue landing line, game succeed
    if(myship.x>groundn.safe_line.left && myship.x<groundn.safe_line.right &&
       myship.y+(ship_height/2)>=Window_height-groundn.safe_line.height)
    {
        //Show the success interface, and let the user go ahead or quit
        success();
        return;
    }

    //use loop to iterate the check about collision
    int i;
    //check if ship collide with horizontal ground lines
    for(i=0;i<groundn.hline_num;i++)
    {
        if(myship.x>groundn.hlines[i].left && myship.x<groundn.hlines[i].right &&
           myship.y+(ship_height/2)>=Window_height-groundn.hlines[i].height)
        {
            //if collide game failed
            fail();
            return;
        }
    }
    //check if ship collide with vertical ground lines
    for(i=0;i<groundn.vline_num;i++)
    {
        if(abs(myship.x-groundn.vlines[i].width)<=(ship_width/2) &&
           myship.y+(ship_height/2)>Window_height-groundn.vlines[i].top)
        {
            //if collide game failed
            fail();
            return;
        }
    }
    //in level 4 check if ship collide with the circle cloud
    if(level==4)
    {
        //when the spacecraft is too close to the clouds game failed
        if(pow(myship.x-cloud_pos1_X,2)+pow(myship.y-cloud_pos1_Y,2)<=15000) fail();
        if(pow(myship.x-cloud_pos2_X,2)+pow(myship.y-cloud_pos2_Y,2)<=7000) fail();
    }
    //in level 5 check if ship collide with the alien ship
    if(level==5 && abs(myship.x-ai_ship.x)<(ship_width+10) && abs(myship.y-ai_ship.y)<ship_height) fail();
    //in level 7 check if ship collide with the fire balls
    if(level==7)
    {
        //when the spacecraft is too close to the sun game failed
        if((pow(myship.x+45-Window_width,2)+pow(myship.y+15-Prompt_height,2)<=40000 ||
            pow(myship.x-Window_width,2)+pow(myship.y-15-Prompt_height,2)<=40000))
                fail();
        //when the spacecraft is too close to the fire balls game failed
        for(i=0;i<3;i++)
        {
            if(pow(myship.x-balls[i].x,2)+pow(myship.y-balls[i].y,2)<=900) {fail(); break;}
            else if(balls[i].x<=525 || balls[i].y>=435) ball_init(i);
        }
    }

}

//This function writes spacecraft force/speed logic
//Set influence of the mouse position on the displacement of the spacecraft
void check_mouse()
{
    //only new event occur we check it
    if(check_if_event())
    {
        wait_for_event();
        if(event_mouse_position_changed())
        {
            //before any change reset the speed of ship
            myship.vx -= myship.dvx;
            myship.vy -= myship.dvy;
            //check the coordinate position of mouse
            get_mouse_coordinates();
            //change ship acceleration according to the ratio of position gap
            myship.dvx=(myship.x-XMOUSE)/20;
            myship.dvy=((myship.y-YMOUSE)/20);
            //change the speed of ship according to acceleration
            myship.vx += myship.dvx;
            myship.vy += myship.dvy;
            //clean all event to prepare for next frame
            al_flush_event_queue(event_queue);
        }
    }
}

//check if user want to quit or pause the game
void check_keyboard()
{
    //only have new event can we wait for the event
    if(check_if_event())
    {
        wait_for_event();
        //check if any key was pressed
        if(event_key_down())
        {
            wait_for_event();
            //press p to pause and press again to back to play
            if(event_key('p')||event_key('P'))
            {
                is_pause=!is_pause;
            }
            //press q to quit and back to menu at any time
            if(is_pause && (event_key('q')||event_key('Q')))
            {
                is_pause=0;
                back_to_menue=1;
                return;
            }
        }
    }
}

//change wind direction and wind force regularly
void wind_change()
{
    //set a buffer to pass variables to outtextxy()
    char buf[50];
    //only in level 6 we have the Mars wind
    //we change the wind type every 5 seconds
    if(level==6 && nclock-lclock>=5)
    {
        //when the wind speed change set sound to hint
        midi_note(84,1,60);
        pause(100);
        midi_note(84,1,60);
        pause(100);
        lclock=nclock;
        //keep wind_kind smaller than 6 because only 6 types
        //change wind kind using remainder operation
        wind_kind=(wind_kind+1)%6;
        //use text and number to prompt wind direction and wind level
        setcolor(RED);
        filled_rectangle(40,490,105,530,LIGHTBLUE);
        if(wind_speeds<0) outtextxy(50,500,"LEFT ");
        else outtextxy(50,500,"RIGHT ");
        //wind level is displayed in proportion to keep reasonable wind effect
        sprintf(buf,"%d",abs(wind_speeds[wind_kind])/3);
        outtextxy(95,500,buf);
    }
}
