#ifndef __PADDLE_H__
#define __PADDLE_H__

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <iostream>
#include <list>
#include "GameObject.h"
#include "GameManager.h"
#include "Orb.h"
struct XInfo;

class Orb;

class Paddle:public GameObject{
public:
	Paddle(double spd, double x = -1.0f, double y = -1.0f);
	~Paddle();
	
	//methods
	//init sets _length, _width, _x_left_bound, and _x_right_bound
	static void InitPaddleData(double length, double height, double xleft, double xright, double default_x, double default_y);

	//will never be called as paddle is neever destructed.
	void DestroyObject();
	int GetPenalty();
	//Update utilizes the InputManager to update the position of the paddle
	void Update();
	void PrintData();
	//does NOT actually draw the balls held.
	void DrawObject(XInfo& myInfo);
	void HoldThisBall(Orb* target);
	void ReleaseAllBalls();
	//ignite target if is a critical hit, set target onfire
	void JudgeCriticalHit(double hit_x, double hit_y , Orb* target);

	//fields
	double _max_speed;		// the fastest speed paddle can travel at 
	double _velocity; 		// the current velocity of the paddle
	double _acc; 			// the acceleration of velocitys 
	double _catch_penalty; // the penalty to core on each catch

	//DO NOT delet the balls here
	std::list<Orb*> _orb_held;

	static double _x_left_bound; 	// the left most point paddle is capable of moving to(velocity zero)
	static double _x_right_bound;	// the right most point(+width) paddle is capable of moving to(velocity zero)
	static double global_length; 	//the length of the paddle to a certain ratio to the window
	static double global_height;	//the hieght of the paddle to a certain ratio to the window
	//the default coordinate of the paddle
	static double _default_x;
	static double _default_y;

private:
	bool _crit_catch; // if not critical catch, apply _Catch penalty
	double _hit_x;
	double _hit_y;
};

#endif