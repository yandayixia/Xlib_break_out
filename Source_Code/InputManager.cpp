#include "InputManager.h"
#include <iostream>
#include <string>

InputManager::InputManager(){}

InputManager::~InputManager(){}

bool InputManager::_left_signal = false;
bool InputManager::_right_signal = false;
bool InputManager::_space_signal = false;
std::string InputManager::LEFT_KEY = "Left";
std::string InputManager::RIGHT_KEY = "Right";
std::string InputManager::SPACE_KEY = "SPACE";
//methods
void InputManager::InitInput(){
	_left_signal = false;
	_right_signal = false;
}

//on keydown, everything else is upped.
void InputManager::KeyDown(std::string& target){
	if(target == "Left"){
		if(!_left_signal){
			InputManager::_left_signal = true;
		}
	}else if(target == "Right"){
		if(!_right_signal){
			InputManager::_right_signal = true;
		}
	}else if(target == "SPACE"){
		if(!_space_signal){
			InputManager::_space_signal = true;
		}
	}else{
		std::cerr << "Un-specified key press: " << target << std::endl;
	}
}

void InputManager::KeyUp(std::string& target){
	if(target == "Left"){
		if(_left_signal){
			InputManager::_left_signal = false;
		}
	}else if(target == "Right"){
		if(_right_signal){
			InputManager::_right_signal = false;
		}
	}else if(target == "SPACE"){
		if(_space_signal){
			InputManager::_space_signal = false;
		}
	}else{
		std::cerr << "Un-specified key release: " << target << std::endl;
	}
}

bool InputManager::LookUp(std::string& target){
	bool result = false;
	if(target == "Left"){
		result = _left_signal;
	}else if(target == "Right"){
		result = _right_signal;
	}else if(target == "SPACE"){
		result = _space_signal;
	}else{
		std::cerr << "Un-specified key look up: " << target << std::endl;
	}
	return result;
}