#ifndef _OPERATE_DELEGATE_H_
#define _OPERATE_DELEGATE_H_

#include <cocos2d.h>

class OperateDelegate
{
public:
	virtual void onWalk(cocos2d::Point direction, float distance) = 0;
	virtual void onAttack() = 0;
	virtual void onStop() = 0;
};


#endif