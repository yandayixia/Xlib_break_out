#ifndef __BASEBLOCK_H__
#define __BASEBLOCK_H__

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "GameObject.h"

struct XInfo;

class BaseBlock:public GameObject{
public:
	BaseBlock(int hp=1, double x=0, double y=0, int ci=-1);
	~BaseBlock();
	//call InitBlockData to set the size of a block into 1/10 of the window width
	//and 1/20 of window height.
	static void InitBlockData(int width, int height);
	
	//return true if block is indeed damaged
	virtual bool TakeHit(int str);
	bool Is_Destroyed();
	//returns the amount of points this object is worth on hit
	virtual int GetScore();

	virtual void DrawObject(XInfo& myInfo);
 	virtual void DestroyObject();

	void PrintData();
	//Data
	int _hp;
	int _color_index;
	bool _col_check;
	bool _visible;
	bool _required;

	//default value
	static int global_width;
	static int global_height;
};

#endif