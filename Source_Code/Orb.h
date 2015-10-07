#ifndef __ORB_H__
#define __ORB_H__

#include "TemplateObject.cpp"
#include "GameObject.h"
#include "GameManager.h"

#include <cmath>
#include <iostream>


class GameObject;
struct XInfo;

class Vec_2{
public:

	Vec_2(double x, double y):_x_cord(x), _y_cord(y){}
	~Vec_2(){};
	double _x_cord;
	double _y_cord;

	void Normalize(){
		double length = sqrt(pow( _x_cord , 2.0f) + pow(_y_cord , 2.0f));
		if(length != 0.0f){
			_x_cord /= length;
			_y_cord /= length;	
		}
	}

	bool GoingLeft(){
		return _x_cord < 0.0f;
	}


	bool GoingRight(){
		return _x_cord > 0.0f;
	}


	bool GoingUp(){
		return _y_cord  < 0.0f;
	}


	bool GoingDown(){
		return _y_cord  > 0.0f;
		
	}

	void Reflect_Vertical(){
		_x_cord *= -1;
		Normalize();
	}

	void Reflect_Horizontal(){
		_y_cord *= -1;
		Normalize();
	}

	void PrintData(){
		std::cout << "(" << _x_cord << ", " << _y_cord << ")" << std::endl;
	}
};

class Orb:public GameObject{
public:
	Orb(double x = 0, double y = 0, double spd = 10);
	~Orb();

	bool TestColliding(GameObject& target);
	void DrawObject(XInfo& myInfo);
	void DestroyObject();
	void PrintData();
	//Update souldn't cause the change of the gamermanager::OrbArry content
	void Update();
	void ReleaseBall();
	void Bounce(bool is_vertical_collide, bool is_horizontal_collide);
	int GetStrength();
	//call when been hit by another block
	void HitPowerUp();
	void CatchFire();
	void CancelFire();
	void ClearCollisionCD();


	//pass in -1 in raduis to not change anything
	static void InitOrbData(double left, double right, double up, double down, double radius);
	int _collision_cd;
	int _power_up_count;
	// if _held, the orb stays on the paddle
	bool _held;
	bool _fallen;
	bool _on_fire;
	static double _left_boundary;
	static double _right_boundary;
	static double _up_boundary;
	static double _down_boundary;
	static double _radius;
	double _speed;
	Vec_2* _direction;

	double _last_hit_x;
	double _last_hit_y;

private:
	void SetCollisionCD();
};

#endif