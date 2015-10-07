#include "Paddle.h"
#include "InputManager.h"
#include <iostream>

double Paddle::global_length = 20;
double Paddle::global_height = 10;
double Paddle::_x_left_bound = 0; 	// the left most point paddle is capable of moving to(velocity zero)
double Paddle::_x_right_bound = 10;	// the right most point(+width) paddle is capable of moving to(velocity zero)
double Paddle::_default_x = 5;
double Paddle::_default_y = 5;

Paddle::Paddle(double spd, double x, double y):GameObject(x, y, Paddle::global_length, Paddle::global_height), 
	_max_speed(spd), _velocity(0), _acc(spd / 10.0f), _catch_penalty(8), _crit_catch(false), 
	_hit_x(Paddle::global_length/2), _hit_y(0){
		if(x == -1){
			_x= _default_x;
		}
		if(y == -1){
			_y= _default_y;
		}
	}

Paddle::~Paddle(){}

//init sets _length, _width, _x_left_bound, and _x_right_bound
void Paddle::InitPaddleData(double length, double height, double xleft, double xright, double default_x, double default_y){
	Paddle::global_length = length;
	Paddle::global_height = height;
	Paddle::_x_left_bound = xleft;
	Paddle::_x_right_bound = xright;
	Paddle::_default_x = default_x;
	Paddle::_default_y = default_y;
}

//will never be called as paddle is neever destructed.
void Paddle::DestroyObject(){}

int Paddle::GetPenalty(){
	int result = -_catch_penalty;
	if(_crit_catch){
		result = 0;
		_crit_catch = false;
	}
	return result;
}
//Update utilizes the InputManager to update the position of the paddle
void Paddle::Update(){
	_width = Paddle::global_length;
	_height = Paddle::global_height;
	//if space is pressed, release all held balls
	if(InputManager::LookUp(InputManager::SPACE_KEY)){
		ReleaseAllBalls();	
	}

	//moving left
	if(InputManager::LookUp(InputManager::LEFT_KEY)){
		if(_velocity >= (-1)*_max_speed + _acc){
			_velocity -= _acc;	
		}else{
			_velocity = (-1) * _max_speed;
		}
	//moving right
	}else if(InputManager::LookUp(InputManager::RIGHT_KEY)){
		if(_velocity <= _max_speed - _acc){
			_velocity += _acc;	
		}else{
			_velocity = _max_speed;
		}
		
	//no key pressed, slowly reduce _Velocity to zero
	}else{
		if(_velocity < 0.0f){
			_velocity = _velocity * 0.9;
			if(_velocity > -1){
				_velocity = 0;
			}
		}else if(_velocity > 0.0f){
			_velocity = _velocity * 0.9;
			if(_velocity < 1){
				_velocity = 0;
			}
		}else{
			_velocity = 0;
		}
	}
	float delta_x = 0.0f;

	//actually moving the x-coordinate
	//check left bound
	if(_x + _velocity < _x_left_bound){
		delta_x =  -_x - _x_left_bound;
		_x = _x_left_bound;
		_velocity = 0;	
	//check right bound
	}else if(_x + _velocity + _width > _x_right_bound){
		delta_x =  (_x_right_bound - _width) - _x;
		_x = _x_right_bound - _width;
		_velocity = 0;
	}else{
		delta_x = _velocity;
		_x += _velocity;
	}

	//update all the orbs held
	for(std::list<Orb*>::iterator i = _orb_held.begin();
		i != _orb_held.end();
		i++){
		(*i)->_x += delta_x;
		//multiplier 0.8 make everything realistic
		(*i)->_y = _y - ((*i)->Orb::_height * 0.8);
	}
}
//for bebugging purposes
void Paddle::PrintData(){
	std::cout << "*****Paddle data:" << std::endl;
	std::cout << "x cord: " << GetX() << std::endl;
	std::cout << "y cord: " << GetY() << std::endl;
	std::cout << "length: " << GetWidth() << std::endl;
	std::cout << "height: " << GetHeight() << std::endl;

	std::cout << "max_spd: " << _max_speed << std::endl;
	std::cout << "acceleration: " << _acc << std::endl;

	std::cout << "left_bound: " << _x_left_bound << std::endl;
	std::cout << "right_bound: " << _x_right_bound << std::endl;

	std::cout << "_default_x: " << _default_x << std::endl;
	std::cout << "_default_y: " << _default_y << std::endl;
	std::cout << "Orb held: " << _orb_held.size() << std::endl;
}

void Paddle::DrawObject(XInfo& myInfo){

	double center_size = GetWidth() *0.7;
	double side_size = (GetWidth() - center_size) / 2;
	double x_offset = side_size / 10.0f;
	//white center part of the paddle
	XFillRectangle(myInfo.display, myInfo.pixmap, myInfo.color_gc[0], GetX(), GetY(), GetWidth(), GetHeight());
	//red critical side parts of the paddle
	//left
	XFillRectangle(myInfo.display, myInfo.pixmap, myInfo.color_gc[1], GetX() - x_offset, GetY(), side_size + 2 * x_offset, GetHeight());
	//right
	XFillRectangle(myInfo.display, myInfo.pixmap, myInfo.color_gc[1], GetX() + side_size + center_size - x_offset, GetY(), side_size + 2 * x_offset, GetHeight());

	//Blue indicator of where the orb hit:
	double indicator_width = 5.0f;
	double indicator_height = 5.0f;
	XFillArc(myInfo.display, myInfo.pixmap, myInfo.color_gc[5], GetX() + _hit_x * _x_fac - indicator_width/2, GetY() + _hit_y * _y_fac - indicator_height/2, indicator_width, indicator_height, 0, 360*64);

}

void Paddle::HoldThisBall(Orb* target){
	_orb_held.push_back(target);
	//std::cout << "Orb held: " << _orb_held.size() << std::endl;
}

void Paddle::ReleaseAllBalls(){
	//release all the orbs held
	for(std::list<Orb*>::iterator i = _orb_held.begin();
		i != _orb_held.end();
		i++){
		(*i)->ReleaseBall();
	}
	_orb_held.clear();
	//std::cout << "RELEASE!!!" << std::endl;
	//std::cout << "Now Orb left: " << _orb_held.size() << std::endl;
}


void Paddle::JudgeCriticalHit(double hit_x, double hit_y , Orb* target){
	double center_size = _width *0.7;
	double side_size = (_width - center_size) / 2;

	double x_offset = side_size / 20.0f;

	bool is_crit = false;

	_hit_x = hit_x - _x;
	_hit_y = hit_y - _y;


	//left
	if((_hit_x > - x_offset) 
		&& (_hit_x < side_size + x_offset)){
		is_crit = true;
	}

	//right

	if((_hit_x > side_size + center_size - x_offset) 
		&& (_hit_x < _width + x_offset)){
		is_crit = true;
	}
	
	//if true:
	_crit_catch = is_crit;
	if(_crit_catch){
		target->CatchFire();
	}else{
		target->CancelFire();
	}
}