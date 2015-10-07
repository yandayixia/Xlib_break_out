#ifndef __TEMPLATE_OBJECT__
#define __TEMPLATE_OBJECT__

struct XInfo;

class TemplateObject{
public:
	TemplateObject(){

	}

	virtual ~TemplateObject(){
	}
	
	virtual void DrawObject(XInfo& myInfo)=0;
 	virtual void DestroyObject()=0;
 	virtual void PrintData()=0;
	virtual void Update()=0;
};

#endif