#ifndef __GAMEOVJECT_H__
#define __GAMEOVJECT_H__

#include <iostream>

#include "TemplateObject.cpp"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

//Information to draw on the window.
struct XInfo {
    Display*    display;
    int      screen;
    Window   window;

    //7 graphics context
    GC color_gc[9];
    //white
    
    //red
    //orange
    //yellow
    //green
    //blue
    //purple

    //black
    //gray

    Pixmap  pixmap;     // double buffer
    // size of pixmap & window
    double width;      
    double height;

    // the standard size of window, used to scaling
    double default_width;
    double default_height;

    //window_height / window_width;
    double target_ratio;

    XSizeHints my_hints; //size hints
};


//All physics are carried out in the original coordiante space
//call the Getter funcitons to draw it. (DO NOT change _x, _y, 
//_width, _height in the main code), and DO NOT call the getter
//functions in physics calculation.

//Scaling is accomplished by changing the scaling factor

//GameObject is the ancestor of all in game objects that will engage in collision detection
class GameObject: public TemplateObject{
public:
 	//constructor, loads the source path for the image.
 	//GameObject(std::source_path):_source_path(source_path){}
 	GameObject(double x, double y, double width, double height, bool sb = false);
 	virtual ~GameObject();
 
 	virtual void DrawObject(XInfo& myInfo)=0;
 	virtual void DestroyObject()=0;
 	virtual void PrintData();
    //the default update method does nothing
    virtual void Update();

    static void SetXFactor(double target);

    static void SetYFactor(double target);

    virtual double GetX();

    virtual double GetY();

    virtual double GetWidth();

    virtual double GetHeight();

 	/* data */
 	double _x, _y;
 	double _width, _height;
    //if single_bounce, orb can only be reflected in one axis
    bool _single_bounce;
    static double _x_fac, _y_fac;    
 }; 

#endif