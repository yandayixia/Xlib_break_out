#include "GameObject.h"

double GameObject::_x_fac = 1.0f;
double GameObject::_y_fac = 1.0f; 

GameObject::GameObject(double x, double y, double width, double height, bool sb):
_x(x),_y(y),_width(width),_height(height), _single_bounce(sb){

}

GameObject::~GameObject(){
    
}

void GameObject::PrintData(){
    std::cout << "abstract Game Object!" << std::endl;
}

void GameObject::Update(){
}

void GameObject::SetXFactor(double target){
    GameObject::_x_fac = target;
}

void GameObject::SetYFactor(double target){
    GameObject::_y_fac = target;
}

double GameObject::GetX(){
    return _x * _x_fac;
}

double GameObject::GetY(){
    return _y * _y_fac;
}

double GameObject::GetWidth(){
    return _width * _x_fac;
}

double GameObject::GetHeight(){
    return _height * _y_fac;
}