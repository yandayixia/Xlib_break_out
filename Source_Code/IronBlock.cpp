#include "IronBlock.h"

struct XInfo;

IronBlock::IronBlock(double x, double y):BaseBlock(10, x, y, 8){
	_single_bounce = true;
	_required = false;
}

IronBlock::~IronBlock(){}

int my_rand(int a, int b)
{
  return a + (b - a) * (double)rand() / RAND_MAX;

}

bool IronBlock::TakeHit(int str){
	bool result = false;
	//iron block doesn't take damage
	if(str > 1){
		_hp -= 1;

		for (int i=0; i < 3 + (10 - _hp) /2 ; i++) {
		    XPoint p;
			//draw cracks
		    p.x = my_rand(GetX(), GetX() + GetWidth());
		    p.y = my_rand(GetY(), GetY() + GetHeight());
		    points.push_back(p);
		}

		result = true;
	}

	return result;
}

int IronBlock::GetScore(){
	return 300;
}

void IronBlock::DrawObject(XInfo& myInfo){
	_width = BaseBlock::global_width;
	_height = BaseBlock::global_height;
	if(_visible){
		//graw the gray block
		XFillRectangle(myInfo.display, myInfo.pixmap, myInfo.color_gc[_color_index], GetX(), GetY(), GetWidth(), GetHeight());
		//draw the white highlight
		XFillRectangle(myInfo.display, myInfo.pixmap, myInfo.color_gc[0], GetX() +  GetWidth() * 0.2f, GetY(), GetWidth() * 0.2f, GetHeight());//draw the white highlight
		//draw the gray highlight
		XFillRectangle(myInfo.display, myInfo.pixmap, myInfo.color_gc[_color_index], GetX() + GetWidth() * 0.22f, GetY(), GetWidth() * 0.1f, GetHeight());

			// note the trick to pass a stl vector as an C array
	        XDrawLines(myInfo.display, myInfo.pixmap, myInfo.color_gc[7],
                   &points[0], points.size(),  // vector of points
                   CoordModeOrigin ); // use absolute coordinates

	}	
}
