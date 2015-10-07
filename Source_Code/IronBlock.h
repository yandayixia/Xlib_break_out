#ifndef __IRONBLOCK_H__
#define __IRONBLOCK_H__

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "BaseBlock.h"
#include <vector>
#include <cstdlib>


struct XInfo;

class IronBlock:public BaseBlock{
public:
	IronBlock(double x=0, double y=0);
	~IronBlock();
	
	virtual bool TakeHit(int str);
	virtual	int GetScore();
	virtual void DrawObject(XInfo& myInfo);

	std::vector< XPoint> points;
};

#endif