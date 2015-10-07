/*
 * Author: Yandayixia
 *
 * Disclaimer:
 * Original Game crated as a CS349 Assignment. for the university of waterloo.
 * Plenty of new features, levels, are contents are added for the pure pleasure
 * of coding insanity and joy and pain and suffering. Sleep is not requested.
 *
 * To build the game, execute the make file with command:
 * $=>make
 * or
 * $=>make run
 * if your want to execute the game right after build.
 *
 * See the REAMME file for details of, cheats, and features.
 */


#include <cstdlib>
#include <iostream>
#include <list>
#include <unistd.h>

#include <string>
#include <sstream>
#include <sys/time.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

// The following module relies on the struct XInfo
#include "Source_Code/TemplateObject.cpp"
#include "Source_Code/GameObject.h"
#include "Source_Code/BaseBlock.h"
#include "Source_Code/Paddle.h"
#include "Source_Code/InputManager.h"
#include "Source_Code/Orb.h"
#include "Source_Code/GameManager.h"

using namespace std;

//a patch that is supposed to fix the to_string problem

string to_string(double n)
{
    ostringstream stream;
    stream << n ;
    return stream.str();
}

double to_num(string n)
{
    istringstream stream(n);
    double result = 0.0f;
    stream >> result;
    return result;
}

struct XFont{
    GC text_gc;
    XFontStruct * font;
};

void error( string str ) {
    cerr << str << endl;
    exit(0);
}

void SetUpAllGC(XInfo& xInfo){
    //white
    XColor my_color;
    Colormap screen_colormap = DefaultColormap(xInfo.display, DefaultScreen(xInfo.display));
    int i = 0;
    
    xInfo.color_gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);

    XAllocNamedColor(xInfo.display, screen_colormap, "white", &my_color, &my_color);

    XSetForeground(xInfo.display, xInfo.color_gc[i], my_color.pixel);
    XSetBackground(xInfo.display, xInfo.color_gc[i], XBlackPixel(xInfo.display, xInfo.screen));
    XSetFillStyle(xInfo.display, xInfo.color_gc[i], FillSolid);

    //red
    i =1;
    xInfo.color_gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);

    XAllocNamedColor(xInfo.display, screen_colormap, "red", &my_color, &my_color);

    XSetForeground(xInfo.display, xInfo.color_gc[i], my_color.pixel);
    XSetBackground(xInfo.display, xInfo.color_gc[i], XBlackPixel(xInfo.display, xInfo.screen));
    XSetFillStyle(xInfo.display, xInfo.color_gc[i], FillSolid);
    
    //orange
    i =2;
    xInfo.color_gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);

    XAllocNamedColor(xInfo.display, screen_colormap, "Orange", &my_color, &my_color);

    XSetForeground(xInfo.display, xInfo.color_gc[i], my_color.pixel);
    XSetBackground(xInfo.display, xInfo.color_gc[i], XBlackPixel(xInfo.display, xInfo.screen));
    XSetFillStyle(xInfo.display, xInfo.color_gc[i], FillSolid);

    //yellow
    i =3;
    xInfo.color_gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);

    XAllocNamedColor(xInfo.display, screen_colormap, "yellow", &my_color, &my_color);

    XSetForeground(xInfo.display, xInfo.color_gc[i], my_color.pixel);
    XSetBackground(xInfo.display, xInfo.color_gc[i], XBlackPixel(xInfo.display, xInfo.screen));
    XSetFillStyle(xInfo.display, xInfo.color_gc[i], FillSolid);
    //green
    i =4;
    xInfo.color_gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);

    XAllocNamedColor(xInfo.display, screen_colormap, "green", &my_color, &my_color);

    XSetForeground(xInfo.display, xInfo.color_gc[i], my_color.pixel);
    XSetBackground(xInfo.display, xInfo.color_gc[i], XBlackPixel(xInfo.display, xInfo.screen));
    XSetFillStyle(xInfo.display, xInfo.color_gc[i], FillSolid);
    
    //blue
    i =5;
    xInfo.color_gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);

    XAllocNamedColor(xInfo.display, screen_colormap, "blue", &my_color, &my_color);

    XSetForeground(xInfo.display, xInfo.color_gc[i], my_color.pixel);
    XSetBackground(xInfo.display, xInfo.color_gc[i], XBlackPixel(xInfo.display, xInfo.screen));
    XSetFillStyle(xInfo.display, xInfo.color_gc[i], FillSolid);

    //purple
    i =6;
    xInfo.color_gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);

    XAllocNamedColor(xInfo.display, screen_colormap, "purple", &my_color, &my_color);

    XSetForeground(xInfo.display, xInfo.color_gc[i], my_color.pixel);
    XSetBackground(xInfo.display, xInfo.color_gc[i], XBlackPixel(xInfo.display, xInfo.screen));
    XSetFillStyle(xInfo.display, xInfo.color_gc[i], FillSolid);

     //black
    i =7;
    xInfo.color_gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);

    XSetForeground(xInfo.display, xInfo.color_gc[i], XBlackPixel(xInfo.display, xInfo.screen));
    XSetBackground(xInfo.display, xInfo.color_gc[i], WhitePixel(xInfo.display, xInfo.screen));
    XSetFillStyle(xInfo.display, xInfo.color_gc[i], FillSolid);

    //purple
    i =8;
    xInfo.color_gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);

    XAllocNamedColor(xInfo.display, screen_colormap, "Dark Gray", &my_color, &my_color);

    XSetForeground(xInfo.display, xInfo.color_gc[i], my_color.pixel);
    XSetBackground(xInfo.display, xInfo.color_gc[i], XBlackPixel(xInfo.display, xInfo.screen));
    XSetFillStyle(xInfo.display, xInfo.color_gc[i], FillSolid);
}

/*
 * Create a window
 */
void initX(int argc, char* argv[], XInfo& xInfo, XFont& xFont) {
    // Open a new window
    xInfo.display = XOpenDisplay( "" );
    // Do error checking
    if ( !xInfo.display )	{
        error( "Can't open display." );
    }

    XSizeHints& hints = xInfo.my_hints;

    // Get default screen index
    xInfo.screen = DefaultScreen( xInfo.display ); 
    // Set the black/white pixel
    unsigned long white, black;
    // Get the default pixel value for color white of the returned window
    white = XWhitePixel( xInfo.display, xInfo.screen ); 
    // Get the default pixel value for color black of the returned window
    black = XBlackPixel( xInfo.display, xInfo.screen );

    //get the size of user's monitor (not funcitoning)
    int screen_width = DisplayWidth(xInfo.display, xInfo.screen);
    int screen_heigh = DisplayHeight(xInfo.display, xInfo.screen);
    
    int window_width = 0.4 * screen_width;
    int window_height = 0.8 * window_width;

    int display_x = 0.5* (screen_width - window_width);
    int display_y = 0.5* (screen_heigh - window_height);

    hints.x = display_x;
    hints.y = display_y;
    hints.width = window_width;
    hints.height = window_height;
    hints.flags = PPosition | PSize;

    xInfo.width = window_width;
    xInfo.height = window_height;
    xInfo.default_width = window_width;
    xInfo.default_height = window_height;
    xInfo.window = XCreateSimpleWindow(
       xInfo.display,				// display where window appears
       DefaultRootWindow( xInfo.display ), // window's parent in window tree
       hints.x, hints.y,	   // sceen center location
       hints.width, hints.height,	  // size of the window
       5,						     // width of window's border
       black,						// window border colour
       black );					    // window background colour

    // extra window properties like a window title
    XSetStandardProperties(
        xInfo.display,		// display containing the window
        xInfo.window,		// window whose properties are set
        "x1_openWindow",	// window's title
        "OW",				// icon's title
        None,				// pixmap for the icon
        argv, argc,			// applications command line args
        &hints );			// size hints for the window


    //set up GCs
    //0-6
    SetUpAllGC(xInfo);
    
    //set up graphics buffer
    int depth = DefaultDepth(xInfo.display, DefaultScreen(xInfo.display));
    xInfo.pixmap = XCreatePixmap(xInfo.display, xInfo.window, xInfo.width, xInfo.height, depth);

    XSelectInput(xInfo.display, xInfo.window, 
        ButtonPressMask | //btn press
        KeyPressMask |  //keypress
        StructureNotifyMask | //resize
        KeyReleaseMask // key release
        );

    //set up XFont
    xFont.text_gc = XCreateGC (xInfo.display, xInfo.window, 0, 0);
    XSetForeground (xInfo.display, xFont.text_gc, XWhitePixel(xInfo.display, xInfo.screen));
    XSetBackground (xInfo.display, xFont.text_gc, XBlackPixel(xInfo.display, xInfo.screen)); 
    
    xFont.font = XLoadQueryFont (xInfo.display, "fixed");
    XSetFont (xInfo.display, xFont.text_gc, xFont.font->fid);



    /* Don't paint the background -- reduce flickering
     */
    XSetWindowBackgroundPixmap(xInfo.display, xInfo.window, None);
    /*
     * Put the window on the screen.
     */
    XMapRaised( xInfo.display, xInfo.window );
}

void KeyInputHandler(XInfo& myInfo, XEvent& my_event, bool is_press){
    KeySym key;
    int BufferSize = 10;
    char text[BufferSize];
    /*
     * Exit when 'q' is typed.
     * This is a simplified approach that does NOT use localization.
     */
    int i = XLookupString( 
        (XKeyEvent *)&my_event,    // the keyboard event
        text,                   // buffer when text will be written
        BufferSize,             // size of the text buffer
        &key,                   // workstation-independent key symbol
        NULL );                 // pointer to a composeStatus structure (unused)
    
    //if the input is keyPress
    if(is_press){
        if ( i == 1) {
            //cout << "Got key press -- " << text[0] << endl;
        }
        switch(key){
            case XK_Left:
                //left arrow key
                InputManager::KeyDown(InputManager::LEFT_KEY);
                break;
            case XK_Right:
                //right arrow key
                InputManager::KeyDown(InputManager::RIGHT_KEY);
                break;
            case XK_space:
                //the space key
                InputManager::KeyDown(InputManager::SPACE_KEY);
                break;
        }
    //if the input is key Release    
    }else{
        if ( i == 1) {
            //cout << "Got key release -- " << text[0] << endl;
        }
        switch(key){
            case XK_Left:
                //left arrow key
                InputManager::KeyUp(InputManager::LEFT_KEY);
                break;
            case XK_Right:
                //right arrow key
                InputManager::KeyUp(InputManager::RIGHT_KEY);
                break;
             case XK_space:
                //the space key
                InputManager::KeyUp(InputManager::SPACE_KEY);
                break;
        }
    }
    
}

void EstablishCanvas(XInfo& myInfo, double width, double height){
    XFillRectangle(myInfo.display, myInfo.window, myInfo.color_gc[8], 
        0, 0, width, height);
}

void HandleResize(XInfo& myInfo, XEvent& event){
    XConfigureEvent xce = event.xconfigure;
    XWindowAttributes my_attr;
    XGetWindowAttributes(myInfo.display, myInfo.window, &my_attr);
    //size lower bound
    if(xce.width < 100){
        xce.width = 100;
        xce.height = myInfo.target_ratio * xce.width;
        my_attr.width = xce.width;
        my_attr.height = xce.height;
    }else if((xce.height < 100)){
        xce.height = 100;
        xce.width = xce.height /myInfo.target_ratio;
        my_attr.width = xce.width;
        my_attr.height = xce.height;
    }

    //calculating new screen width& update all the relative fields
    double window_width = xce.width;
    double window_height = myInfo.target_ratio * xce.width;

    //if window_height larger than xce.height, change scale focus
    if(window_height > xce.height){
        window_height = xce.height;
        window_width = window_height / myInfo.target_ratio;
    }

    // the factor used to re-position everything
    double x_factor = window_width/ myInfo.default_width;
    double y_factor = window_height/ myInfo.default_height;

    myInfo.width = window_width;
    myInfo.height = window_height;

    int depth = DefaultDepth(myInfo.display, DefaultScreen(myInfo.display));
    myInfo.pixmap = XCreatePixmap(myInfo.display, myInfo.window, myInfo.width, myInfo.height, depth);

    GameObject::SetXFactor(x_factor);
    GameObject::SetYFactor(y_factor);
    
    EstablishCanvas(myInfo, xce.width, xce.height);
}

void DetectEvents(XInfo& myInfo){
    XEvent my_event;

    if (XPending(myInfo.display) > 0){
        XNextEvent(myInfo.display, &my_event);
        switch( my_event.type ){
            case ButtonPress:
                cout << "Got button press!" << endl;
                break;
            case KeyPress:
                KeyInputHandler(myInfo, my_event, true);
                break;
            case KeyRelease:
                KeyInputHandler(myInfo, my_event, false);
                break;
            case ConfigureNotify:
                HandleResize(myInfo, my_event);
                break;
        }
    }
}

void UpdateObjects(XInfo& myInfo){
    //update balls
    int old_size = GameManager::ExtraBall;
    for(list<Orb*>::iterator i = (GameManager::OrbArray).begin();
        i != (GameManager::OrbArray).end();
        i++){
        //update does NOT change the size of OrbArray
        (*i)->Update();
    }
    //update Paddle
    (GameManager::HeroPaddle)->Update();
    //HAVE to call this before doing anything else others
    GameManager::UpdateManager();
}

//DrawGame draws everything the game Manager contains 
void DrawGame(XInfo& myInfo, XFont& xFont){
    //re-fresh buffer
    XFillRectangle(myInfo.display, myInfo.pixmap, myInfo.color_gc[7], 
        0, 0, myInfo.width, myInfo.height);

    //draw balls
    for(list<Orb*>::iterator i = (GameManager::OrbArray).begin();
        i != (GameManager::OrbArray).end();
        i++){

        (*i)->DrawObject(myInfo);
    }
    //draw peddle
    (GameManager::HeroPaddle)->DrawObject(myInfo);

    int counter = 0;
    //draw all blocks matrix
    for(list<BaseBlock *>::iterator i = (GameManager::BlockArray).begin();
        i != (GameManager::BlockArray).end();
        i++){

        (*i)->DrawObject(myInfo);

    /*
        //debug, draw the painting order of the blocks
        string to_print = "";
        to_print = to_string(counter);
        XDrawString (myInfo.display, myInfo.pixmap, xFont.text_gc,
                     (*i)->GetX() + (*i)->GetWidth()/2, (*i)->GetY() + (*i)->GetHeight()/2, to_print.c_str(), to_print.length());
    */  
        counter++;
    }

    //draw the text fields
    string to_print = "";

    to_print = "FPS: " + to_string(GameManager::FPS);
    //draw FPS:
    XDrawString (myInfo.display, myInfo.pixmap, xFont.text_gc,
                 3, 15, to_print.c_str(), to_print.length());

    to_print = "SCORE: " + to_string(GameManager::GameScore);
    //draw Score:
    XDrawString (myInfo.display, myInfo.pixmap, xFont.text_gc,
                 3, 30, to_print.c_str(), to_print.length());

    //for now, print the life count on screen
    to_print = "Life: " + to_string(GameManager::ExtraBall);
    //draw life
    XDrawString (myInfo.display, myInfo.pixmap, xFont.text_gc,
                 3, myInfo.height - 10, to_print.c_str(), to_print.length());
    
    //draw stage 
    to_print = "Stage: " + to_string(GameManager::LevelCount);
    XDrawString (myInfo.display, myInfo.pixmap, xFont.text_gc,
                 myInfo.width -50, 15, to_print.c_str(), to_print.length());

    //copy the double buffer and flush
    XCopyArea(myInfo.display, myInfo.pixmap, myInfo.window, myInfo.color_gc[0],
        0, 0, myInfo.width, myInfo.height, // region of pixmap to copy
        0, 0); // position to put top left corner of pixmap in window
    
    XFlush(myInfo.display);
}

//the 4th digit is the second (*10^-6 to get to second unit)
unsigned long now() {
    timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}

void HandelCollision(){
    //balls to paddle
    for(list<Orb*>::iterator i = (GameManager::OrbArray).begin();
        i != (GameManager::OrbArray).end();
        i++){

        //if the ball is caught
        if((*i)->TestColliding(*(GameManager::HeroPaddle))){
            //if hit, test crit
            GameManager::HeroPaddle->JudgeCriticalHit((*i)->_last_hit_x, (*i)->_last_hit_y, *i);
            GameManager::AddPoints((GameManager::HeroPaddle)->GetPenalty());
        }
    }

    //balls to blocks
    int counter = 0;
    for(list<Orb*>::iterator i = (GameManager::OrbArray).begin();
        i != (GameManager::OrbArray).end();
        i++){
        for(list<BaseBlock*>::iterator n = (GameManager::BlockArray).begin();
            n != (GameManager::BlockArray).end();
            n++){
            if((*n)->_col_check){
                counter++;
                //What on hit, let target take damage
                if((*i)->TestColliding(*(*n))){
                    //if block is damaged, power up orb
                    if((*n)->TakeHit((*i)->GetStrength())){
                       (*i)->HitPowerUp();
                    }
                    //block collision don't need CD
                    (*i)->ClearCollisionCD();
                    if((*n)->Is_Destroyed()){
                        GameManager::AddPoints((*n)->GetScore());
                        GameManager::RemoveBlock(n);
                    }
                }    
            }        
        }
    }
}


void GameLoop(XInfo& myInfo, XFont& xFont){
    //the main loop
    const int sample_size = 40;
    //constant value for magic number
    const long double ms_per_second = 1000000.0f; 
    
    unsigned long old_time = 0;
    unsigned long current_time = 0;
    long double frame_time = 0.0f; // micro second / frame
    long double desired_ftime = ms_per_second / GameManager::Target_FPS; //micro second / frame
    long double instance_fps = 0.0f;
    long double fps_accumulated = 0.0f;
    int desire_counter = 0; 

    // implementation # 2
    old_time = now();
    while(GameManager::RunningGame){
        current_time = now();
        frame_time =  (current_time - old_time);
        //always update the game input
        DetectEvents(myInfo);
        //only check physics for every frame
        if(frame_time >= desired_ftime){
            old_time += desired_ftime;
            instance_fps = ms_per_second /frame_time;
            fps_accumulated += instance_fps;
            DrawGame(myInfo, xFont);
            UpdateObjects(myInfo);
            HandelCollision(); 
            desire_counter++;
            if(desire_counter >= sample_size){
                GameManager::FPS = fps_accumulated / desire_counter;
                fps_accumulated = 0;
                desire_counter = 0;
                //cout << GameManager::FPS << endl;   
            }
        }else{
            /*
            cout << frame_time << endl;
            cout << "***: " << desired_ftime << endl;
            cout << "***: " << desired_ftime - frame_time << endl;
            */
            usleep(desired_ftime - frame_time );
        }
    }
}

//This process standarize the size of each block, it also
//prints out debug log about the screen, window, and block.
void InitRelativeGameObjectSize(XInfo& myInfo){
    int screen_width = DisplayWidth(myInfo.display, myInfo.screen);
    int screen_heigh = DisplayHeight(myInfo.display, myInfo.screen);

    XWindowAttributes my_attr;

    XGetWindowAttributes(myInfo.display, myInfo.window, &my_attr);

    int window_width = my_attr.width;
    int window_height = my_attr.height;

    int width_per_block = window_width / 16 ;
    int height_per_block = window_height / 25 ;

    myInfo.target_ratio = myInfo.height / myInfo.width;
    
    GameManager::Block_Spacing = width_per_block /10.0f ;
    
    /*
    cout << "***DEBUG***"<< endl;
    cout << "ratio: " << myInfo.height << "/" << myInfo.width << endl;

    cout << "screen_width: " << screen_width << endl;
    cout << "screen_heigh: " << screen_heigh  << endl;

    cout << "window_width: " << window_width << endl;
    cout << "window_height: " << window_height << endl;
    */

    BaseBlock::InitBlockData(width_per_block, height_per_block);

    int Paddle_x = 0.5 * (window_width - width_per_block);
    int Paddle_y = window_height * 0.9;
    Paddle::InitPaddleData( 
        window_width/ 6.0f,      //paddle length 
        window_height/ 40.0f,     //paddle height
        0,                    //x left bound
        window_width,         //x right bound
        Paddle_x,   //default x (screen center) 
        Paddle_y                    //default y (window height)
        );

    Orb::InitOrbData(0.0f, window_width, 0.0f, window_height, window_width / 100.0f);

    /*
    cout << "***********" << endl;
    cout << "width_per_block: " << BaseBlock::global_width << endl;
    cout << "height_per_block: " << BaseBlock::global_height << endl;
    cout << "block spacing: " << GameManager::Block_Spacing << endl;
    cout << "paddle width: " << Paddle::global_length << endl;
    cout << "paddle height: " << Paddle::global_height << endl;
    cout << "orb radius: " << Orb::_radius << endl;
    */
}

//GameInit sets off the initial values of the game
void GameInit(XInfo& myInfo){
    InitRelativeGameObjectSize(myInfo);
    InputManager::InitInput();
}

void ReadyScreen(XInfo& myInfo, XFont& myFont){
    string to_print;
    //praint everything black
    XFillRectangle(myInfo.display, myInfo.pixmap, myInfo.color_gc[7], 
    0, 0, myInfo.width, myInfo.height);

    //white loading
    to_print = "LOADING";
    XDrawString (myInfo.display, myInfo.pixmap, myInfo.color_gc[0],
             myInfo.width/2 -5 , myInfo.height/2 - 12, to_print.c_str(), to_print.length());

    //copy the double buffer and flush
    XCopyArea(myInfo.display, myInfo.pixmap, myInfo.window, myInfo.color_gc[0],
        0, 0, myInfo.width, myInfo.height, // region of pixmap to copy
        0, 0); // position to put top left corner of pixmap in window
    
    XFlush(myInfo.display);

    usleep(500000.0f);

    //***********************************
     //praint everything black
    XFillRectangle(myInfo.display, myInfo.pixmap, myInfo.color_gc[7], 
    0, 0, myInfo.width, myInfo.height);

    //white loading
    to_print = "****3****";
    XDrawString (myInfo.display, myInfo.pixmap, myInfo.color_gc[0],
             myInfo.width/2 - 10 , myInfo.height/2 - 10, to_print.c_str(), to_print.length());

    //copy the double buffer and flush
    XCopyArea(myInfo.display, myInfo.pixmap, myInfo.window, myInfo.color_gc[0],
        0, 0, myInfo.width, myInfo.height, // region of pixmap to copy
        0, 0); // position to put top left corner of pixmap in window
    
    XFlush(myInfo.display);

    usleep(500000.0f);

    //***********************************
     //praint everything black
    XFillRectangle(myInfo.display, myInfo.pixmap, myInfo.color_gc[7], 
    0, 0, myInfo.width, myInfo.height);

    //white loading
    to_print = "****2****";
    XDrawString (myInfo.display, myInfo.pixmap, myInfo.color_gc[0],
             myInfo.width/2 - 10 , myInfo.height/2 - 10, to_print.c_str(), to_print.length());

    //copy the double buffer and flush
    XCopyArea(myInfo.display, myInfo.pixmap, myInfo.window, myInfo.color_gc[0],
        0, 0, myInfo.width, myInfo.height, // region of pixmap to copy
        0, 0); // position to put top left corner of pixmap in window
    
    XFlush(myInfo.display);

    usleep(500000.0f);

    //***********************************
     //praint everything black
    XFillRectangle(myInfo.display, myInfo.pixmap, myInfo.color_gc[7], 
    0, 0, myInfo.width, myInfo.height);

    //white loading
    to_print = "****1****";
    XDrawString (myInfo.display, myInfo.pixmap, myInfo.color_gc[0],
             myInfo.width/2 - 10 , myInfo.height/2 - 10, to_print.c_str(), to_print.length());

    //copy the double buffer and flush
    XCopyArea(myInfo.display, myInfo.pixmap, myInfo.window, myInfo.color_gc[0],
        0, 0, myInfo.width, myInfo.height, // region of pixmap to copy
        0, 0); // position to put top left corner of pixmap in window
    
    XFlush(myInfo.display);

    usleep(500000.0f);

    //***********************************
     //praint everything black
    XFillRectangle(myInfo.display, myInfo.pixmap, myInfo.color_gc[7], 
    0, 0, myInfo.width, myInfo.height);

    //white loading
    to_print = "!!!!GO!!!!";
    XDrawString (myInfo.display, myInfo.pixmap, myInfo.color_gc[0],
             myInfo.width/2 - 10, myInfo.height/2 - 10, to_print.c_str(), to_print.length());

    //copy the double buffer and flush
    XCopyArea(myInfo.display, myInfo.pixmap, myInfo.window, myInfo.color_gc[0],
        0, 0, myInfo.width, myInfo.height, // region of pixmap to copy
        0, 0); // position to put top left corner of pixmap in window
    
    XFlush(myInfo.display);

    usleep(500000.0f);
}

void StartScreen(XInfo& myInfo, XFont& myFont){
    bool start_screen = true;
    while(start_screen){
        //praint everything white
        XFillRectangle(myInfo.display, myInfo.pixmap, myInfo.color_gc[7], 
            0, 0, myInfo.width, myInfo.height);

        //praint everything black
        XFillRectangle(myInfo.display, myInfo.pixmap, myInfo.color_gc[0], 
            0, 0, myInfo.width/2, myInfo.height);

        //draw the text fields
        string to_print = "";

        to_print = "By: Yandayixia the Cursed One";
        XDrawString (myInfo.display, myInfo.pixmap, myInfo.color_gc[7],
                     3, 15, to_print.c_str(), to_print.length());

        to_print = "How to Play:";
        XDrawString (myInfo.display, myInfo.pixmap, myInfo.color_gc[0],
                     myInfo.width/2 + 3 , myInfo.height/4 - 15, to_print.c_str(), to_print.length());    

        to_print = "Using the arror keys to move the paddle.";
        XDrawString (myInfo.display, myInfo.pixmap, myInfo.color_gc[0],
                     myInfo.width/2 + 3 , myInfo.height/4 + 40, to_print.c_str(), to_print.length());

        to_print = "Using the space key to shoot the caught orbs.";
        XDrawString (myInfo.display, myInfo.pixmap, myInfo.color_gc[0],
                     myInfo.width/2 + 3 , myInfo.height/4 + 55, to_print.c_str(), to_print.length());

        to_print = "Using the edge of the paddle to power up your orbs!";
        XDrawString (myInfo.display, myInfo.pixmap, myInfo.color_gc[7],
                     3 , myInfo.height/4 + 110, to_print.c_str(), to_print.length());

        to_print = "Break all blocks to gain access to the next level!";
        XDrawString (myInfo.display, myInfo.pixmap, myInfo.color_gc[7],
                    3 , myInfo.height/4 + 125, to_print.c_str(), to_print.length());

        to_print = "Some blocks can't be broken without powering up you orbs first!";
        XDrawString (myInfo.display, myInfo.pixmap, myInfo.color_gc[7],
                    3 , myInfo.height/4 + 140, to_print.c_str(), to_print.length());

        to_print = "Try power up one orb 2 times in a row!";
        XDrawString (myInfo.display, myInfo.pixmap, myInfo.color_gc[7],
                    3 , myInfo.height/4 + 155, to_print.c_str(), to_print.length());

        to_print = "Press the space key to start from level 1.";
        XDrawString (myInfo.display, myInfo.pixmap, myInfo.color_gc[0],
                     myInfo.width/2 + 3 , myInfo.height - 30, to_print.c_str(), to_print.length());

        //copy the double buffer and flush
        XCopyArea(myInfo.display, myInfo.pixmap, myInfo.window, myInfo.color_gc[0],
            0, 0, myInfo.width, myInfo.height, // region of pixmap to copy
            0, 0); // position to put top left corner of pixmap in window
        
        XFlush(myInfo.display);

        //always update the game input
        DetectEvents(myInfo);

        if(InputManager::LookUp(InputManager::SPACE_KEY)){
            start_screen = false;
            //draw loading screen

            ReadyScreen(myInfo, myFont);

            InputManager::KeyUp(InputManager::SPACE_KEY);
        }

        usleep((1000000.0f/GameManager::Target_FPS));
    }    
}

/*
 *   Start executing here.
 *	 First initialize window.
 *	 Next loop responding to events.
 *	 Exit forcing window manager to clean up - cheesy, but easy.
 */
int main ( int argc, char* argv[] ) {
    try{
        int target_level = -1;
        cout << "***************" << endl;
        //argc: # of parameters + 1
        //argv[0] = ./a1
        //argv[1] = input_FPS
        //argv[2] = input_SPD
        //argv[3] = input_Level


        if(argc == 3){
            //frame per second
            double input_FPS = to_num(argv[1]);
            //pixel/second
            double input_SPD = to_num(argv[2]);

            const long double ms_per_second = 1000000.0f; 
            long double desired_ftime = ms_per_second / GameManager::Target_FPS; //micro second / frame
            long double desired_fPixel = ms_per_second / input_SPD; //ms/ pixel
            long double desired_fSPD = desired_ftime / desired_fPixel;
            //setting the attributes in GameManager
            GameManager::Target_FPS = input_FPS;
            GameManager::Ball_SPD = desired_fSPD;

            cout << "input_FPS: " << GameManager::Target_FPS << endl;
            cout << "input_SPD: " << input_SPD << endl;    
        }else if(argc == 4){
            //frame per second
            double input_FPS = to_num(argv[1]);
            //pixel/second
            double input_SPD = to_num(argv[2]);
            //skip level
            target_level = to_num(argv[3]);

            const long double ms_per_second = 1000000.0f; 
            long double desired_ftime = ms_per_second / GameManager::Target_FPS; //micro second / frame
            long double desired_fPixel = ms_per_second / input_SPD; //ms/ pixel
            long double desired_fSPD = desired_ftime / desired_fPixel;
            //setting the attributes in GameManager
            GameManager::Target_FPS = input_FPS;
            GameManager::Ball_SPD = desired_fSPD;
            cout << "input_FPS: " << GameManager::Target_FPS << endl;
            cout << "input_SPD: " << input_SPD << endl;    
            cout << "target_level: " << target_level << endl;
        }else if(argc == 2){
            target_level = to_num(argv[1]);
            cout << "target_level: " << target_level << endl; 
            double input_SPD = 200;
            const long double ms_per_second = 1000000.0f; 
            long double desired_ftime = ms_per_second / GameManager::Target_FPS; //micro second / frame
            long double desired_fPixel = ms_per_second / input_SPD; //ms/ pixel
            long double desired_fSPD = desired_ftime / desired_fPixel;
            GameManager::Ball_SPD = desired_fSPD;

        }else if(argc == 1){
            
            double input_SPD = 200;
            const long double ms_per_second = 1000000.0f; 
            long double desired_ftime = ms_per_second / GameManager::Target_FPS; //micro second / frame
            long double desired_fPixel = ms_per_second / input_SPD; //ms/ pixel
            long double desired_fSPD = desired_ftime / desired_fPixel;
            GameManager::Ball_SPD = desired_fSPD;
        }else{
            cout << "invalid parameter number, use default value." << endl;
            for(int i =0; i < argc; i++){
                cout << argv[i] << endl;
            }
            double input_SPD = 200;
            const long double ms_per_second = 1000000.0f; 
            long double desired_ftime = ms_per_second / GameManager::Target_FPS; //micro second / frame
            long double desired_fPixel = ms_per_second / input_SPD; //ms/ pixel
            long double desired_fSPD = desired_ftime / desired_fPixel;
            GameManager::Ball_SPD = desired_fSPD;
        }

        XInfo xInfo;
        XFont xFont;
        // Sets up everything and displays the window
        initX(argc, argv, xInfo, xFont);
        // Initialize the game manager
        GameInit(xInfo);

        //when gameover/beaten, warp back to starting screen
        while(true){
            // set level 1
            GameManager::InitNewGame(xInfo);
            //print the game start screen
            StartScreen(xInfo, xFont);
            // load level 1
            GameManager::SetLevel(target_level);
            // Enter the main Game loop
            GameLoop(xInfo, xFont);    
        }
        
        XCloseDisplay(xInfo.display);
    }catch(char const* msg){
        cerr << "**********" << endl;
        cerr << msg << endl;
        cerr << "**********" << endl;
    }
}
