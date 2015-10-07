#include "Orb.h"
#include <cmath>
#include <iostream>
#include "GameManager.h"
class Vec_2;
double Orb::_left_boundary = 0;
double Orb::_right_boundary = 0;
double Orb::_up_boundary = 0;
double Orb::_down_boundary = 0;
double Orb::_radius = 5;

Orb::Orb(double x, double y, double spd):
GameObject(x, y, Orb::_radius*2, Orb::_radius*2), _speed(spd), _held(true), 
_fallen(false), _collision_cd(0), _on_fire(false), _last_hit_x(0.0f), _last_hit_y(0.0f),
_power_up_count(0){
	_direction = new Vec_2(0.5f, -0.5f);
	_direction->Normalize();	
}

Orb::~Orb(){
	delete _direction;
	_direction = NULL;
}

void Orb::SetCollisionCD(){
	_collision_cd = 3;
}

//ball colliding against a square, it will not call the destroyer for target.
bool Orb::TestColliding(GameObject& target){
	bool is_vertical_collide = false;
	bool is_horizontal_collide = false;
	bool single_bounce = target._single_bounce;
	bool result = false;
	double opt_x = 0;
	double opt_y = 0;
	double opt_dis_buffer = 2 * Orb::_radius;
	double opt_distance = 2 * Orb::_radius;

	double push_x = 0;
	double push_y = 0;

	double center_x = _x + Orb::_radius;
	double center_y = _y + Orb::_radius;
	
	//0 left
	//1 down
	//2 right
	//3 up
	int side_index = -1;

	double target_center_x = target._x + target._width/2;
	double target_center_y = target._y + target._height/2;

	double temp_x = 0;
	double temp_y = 0;
	double temp_dist = 0.0f;
	double corner_offset = _radius/10.0f;
	//if ball is held, do not bounce it, but keep its old momentum, tell no collision
	if(_held){
		return false;
	}

	//if ball has _fallen, return
	if(_fallen){
		return false;
	}

	//according to the orientation of target center to cernter of the orb, only find the closest 
	//points on the ideal sides:

	//orb is above target
	if(target_center_y - target._height/2 >= center_y - corner_offset){
		//goes to _x + height, _y, going left (horizontal collide)
		temp_y = target._y;
		for(temp_x = target._x + target._width; temp_x > target._x; temp_x--){
			temp_dist = sqrt(pow( temp_x - center_x , 2.0f) + pow( temp_y - center_y , 2.0f));
			//if temp_dist is greater than opt_distance, update everything
			if(temp_dist < opt_dis_buffer){
				opt_x = temp_x;
				opt_y = temp_y;
				opt_dis_buffer= temp_dist;
				if(_direction->GoingDown()){
					is_horizontal_collide = true;
				}
				side_index = 3;
			}
		}
	}else{
		//orb is below target
		if(target_center_y + target._height/2 <= center_y + corner_offset){
			//goes to _x, _y + height, going right ( horizontal collide)
			temp_y = target._y + target._height;
			for(temp_x = target._x; temp_x < target._x + target._width; temp_x++){
				temp_dist = sqrt(pow( temp_x - center_x , 2.0f) + pow( temp_y - center_y , 2.0f));
				//if temp_dist is greater than opt_distance, update everything
				if(temp_dist < opt_dis_buffer){
					opt_x = temp_x;
					opt_y = temp_y;
					opt_dis_buffer= temp_dist;
					if(_direction->GoingUp()){
						is_horizontal_collide = true;
					}
					side_index = 1;
				}
			}
		}
	}

	if(opt_dis_buffer < opt_distance){
		opt_distance = opt_dis_buffer;	
	}
	//reset optdistance buffer
	if(!single_bounce){
		opt_dis_buffer = 2 * Orb::_radius;	
	}
		

	//orb is to the left of target
	if(target_center_x - target._width/2 >= center_x - corner_offset){
		//Find the closest point to  target
		//starting at _x, _y, going down (vertical collide)
		temp_x = target._x;
		for(temp_y = target._y; temp_y < target._y + target._height; temp_y++){
			temp_dist = sqrt(pow( temp_x - center_x , 2.0f) + pow( temp_y - center_y , 2.0f));
			//if temp_dist is greater than opt_distance, update everything
			if(temp_dist < opt_dis_buffer){
				opt_x = temp_x;
				opt_y = temp_y;
				opt_dis_buffer= temp_dist;
				if(_direction->GoingRight()){
					
					is_vertical_collide = true;
					if(single_bounce){
						is_horizontal_collide = false;
					}
					
				}
				side_index = 0;
			}
		}
	}else{
		//orb is to the right of target
		if(target_center_x + target._width/2 <= center_x + corner_offset){
			//goes to _x + width, _ y + height, going up (vertical collide)
			temp_x = target._x + target._width;
			for(temp_y = target._y + target._height; temp_y > target._y; temp_y--){
				temp_dist = sqrt(pow( temp_x - center_x , 2.0f) + pow( temp_y - center_y , 2.0f));
				//if temp_dist is greater than opt_distance, update everything
				if(temp_dist < opt_dis_buffer){
					opt_x = temp_x;
					opt_y = temp_y;
					opt_dis_buffer= temp_dist;

					if(_direction->GoingLeft()){
						
						is_vertical_collide = true;
						if(single_bounce){
							is_horizontal_collide = false;
						}
					}
					side_index = 2;
				}
			}
		}
	}
	
	if(opt_dis_buffer < opt_distance){
		opt_distance = opt_dis_buffer;	
	}
	
	//then check if the optimal point is within reach.
	//if yes, return true and bounce as planned, apply 0.8 multiplier to make collision solid

	//_radius is expected to stay half of _wdith
	if(opt_distance <= Orb::_radius ){
		result = true;
		_last_hit_x = opt_x;
		_last_hit_y = opt_y;

		push_x = _x + Orb::_radius - opt_x;
		push_y = _y + Orb::_radius - opt_y;

		_x += push_x;
		_y += push_y;

		//push the ball out the opt_distance to the correct distance
		switch(side_index){
			case 0:
				break;
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case -1:
				throw "Error, invalid side_index";
		}


		//don't return true if cd isn't reached, still do the pushing
		if(_collision_cd > 0){
			//SetCollisionCD();
			return false;
		}

		//set _collision_cd:
		SetCollisionCD(); 
		/* //debug:
		std::cout << "************" << std::endl;
		std::cout << "dis: " << opt_distance << std::endl;
		std::cout << "x: " << opt_x << std::endl;
		std::cout << "y: " << opt_y << std::endl;
		std::cout << "side_index: " << side_index << std::endl;
		*/
		Bounce(is_vertical_collide, is_horizontal_collide);
	//else return false
	}else{
		result = false;
	}
	return result;
}

//draw a ball
void Orb::DrawObject(XInfo& myInfo){
	//if ball has _fallen, return
	if(_fallen){
		return;
	}
	if(_on_fire){
		//Red
		XFillArc(myInfo.display, myInfo.pixmap, myInfo.color_gc[1], GetX(), GetY(), GetWidth(), GetHeight(), 0, 360*64);
	}else{
		//white
		XFillArc(myInfo.display, myInfo.pixmap, myInfo.color_gc[0], GetX(), GetY(), GetWidth(), GetHeight(), 0, 360*64);
	}
}

//simply destroy the object
void Orb::DestroyObject(){

}

//print debug data
void Orb::PrintData(){
	std::cout << "Direction: ";
	_direction->PrintData();
	std::cout << "Radius: " << Orb::_radius << std::endl;
	std::cout << "Speed: " << _speed << std::endl;

	std::cout << "_left_boundary: " << _left_boundary << std::endl;
	std::cout << "_right_boundary: " << _right_boundary << std::endl;
	std::cout << "_up_boundary: " << _up_boundary << std::endl;
	std::cout << "_down_boundary: " << _down_boundary << std::endl;
}

void Orb::Update(){
	double temp_spd = _speed;

	//update the GameObject:_width & height fields
	_width = Orb::_radius * 2;
	_height = Orb::_radius * 2;
	
	//if ball has _fallen, return
	if(_fallen){
		return;
	}
	//if ball is held, do not move it, but keep its old direction
	if(_held){
		return;
	}
	//reduce collision cd
	if(_collision_cd > 0){
		_collision_cd--;	
	}
	//if on fire, 1.5 speed
	if(_on_fire){
		temp_spd *= 1.5;
	}else{
		double temp_cur = _power_up_count;
		double max_pow = GameManager::PowerUpLimit;
		temp_spd *= 1.0f + temp_cur /max_pow;
	}


	_x += _direction->_x_cord * temp_spd;
	_y += _direction->_y_cord * temp_spd;
	
	float _left_side = _x;
	float _right_side = _x + Orb::_radius * 2;
	float _top_side = _y;
	float _bottom_sdie =_y + Orb::_radius * 2;
	//check boundary
	//left
	if(_left_side <_left_boundary){
		_x = _left_boundary;
		Bounce(true, false);
	}
	//right
	//edge touches
	if(_right_side> _right_boundary){
		_x = _right_boundary - Orb::_radius * 2;
		Bounce(true, false);
	}

	//up
	//edge touches
	if(_top_side < _up_boundary){
		_y = _up_boundary;
		Bounce(false, true);
	}

	//down
	//reduce life and reset
	//if the entire ball is out of boundary
	if(_top_side > _down_boundary){
		_fallen = true;
		GameManager::DropOrb();
	}
}


void Orb::ReleaseBall(){
	SetCollisionCD();
	_held = false;
}

void Orb::Bounce(bool is_vertical_collide, bool is_horizontal_collide){
	if(is_vertical_collide){
		_direction->Reflect_Vertical();
	}
	if(is_horizontal_collide){
		_direction->Reflect_Horizontal();
	}
}

void Orb::InitOrbData(double left, double right, double up, double down, double radius){
	Orb::_left_boundary = left;
	Orb::_right_boundary = right;
	Orb::_up_boundary = up;
	Orb::_down_boundary = down;
	if(radius > 0.0f){
		Orb::_radius = radius;
	}
}

//only to be called when hit something
int Orb::GetStrength(){
	int result = 1;

	if(_on_fire){
		result *= 2;
	}
	return result;
}

void Orb::HitPowerUp(){
	_power_up_count+= 1;

	if(_power_up_count >= GameManager::PowerUpLimit/2){
		_on_fire = true;
	}

	if(_power_up_count >= GameManager::PowerUpLimit){
		GameManager::SpawnOrb(this);
		_power_up_count = 0;
	}
}

void Orb::CatchFire(){
	_power_up_count += GameManager::PowerUpLimit/2 ;
	
	if(_power_up_count >= GameManager::PowerUpLimit/2){
		_on_fire = true;
	}
	
	if(_power_up_count >= GameManager::PowerUpLimit){
		GameManager::SpawnOrb(this);
		_power_up_count = 0;
	}
}

void Orb::CancelFire(){
	_on_fire = false;
	_power_up_count = 0;
}

void Orb::ClearCollisionCD(){
	_collision_cd=0;
}