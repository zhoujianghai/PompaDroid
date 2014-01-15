#ifndef _OPERATE_DELEGATE_H_
#define _OPERATE_DELEGATE_H_

#include <cocos2d.h>

class OperateDelegate
{
public:
	virtual void onHeroWalk(cocos2d::Point direction, float distance) = 0;
	virtual void onHeroAttack() = 0;
	virtual void onHeroStop() = 0;
};


#endif