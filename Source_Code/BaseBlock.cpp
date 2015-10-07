#include "BaseBlock.h"

struct XInfo;

int BaseBlock::global_width = 20;
int BaseBlock::global_height = 10;

BaseBlock::BaseBlock(int hp, double x, double y, int ci):
GameObject(x, y, _width, _height), _hp(hp), _color_index(ci), 
_col_check(true), _visible(true), _required(true){
	if(ci < 0){
		if(_hp <= 6){
			_color_index = _hp;	
		}else{
			_color_index = 6;
		}
	}
}

BaseBlock::~BaseBlock(){

}

void BaseBlock::InitBlockData(int width, int height){
	BaseBlock::global_width = width;
	BaseBlock::global_height = height;
}

bool BaseBlock::TakeHit(int str){
	_hp -= str;
	
	if(_hp <= 0){
		_col_check = false;
		_visible = false;
	}else{
		if(_hp <= 6){
			_color_index = _hp;	
		}else{
			_color_index = 6;
		}
	}
	return true;
}

void BaseBlock::PrintData(){
	std::cout << "BaseBlock! x: " << GetX() << " y:" << GetY() << std::endl;
}

void BaseBlock::DrawObject(XInfo& myInfo){
	_width = BaseBlock::global_width;
	_height = BaseBlock::global_height;
	if(_visible){
		XFillRectangle(myInfo.display, myInfo.pixmap, myInfo.color_gc[_color_index], GetX(), GetY(), GetWidth(), GetHeight());
	}	
}

bool BaseBlock::Is_Destroyed(){
	return (_hp <= 0);
}

int BaseBlock::GetScore(){
	return 30;
}

void BaseBlock::DestroyObject(){
	_col_check = false;
}