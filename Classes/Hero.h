#ifndef _HERO_H_
#define _HERO_H_

#include "BaseSprite.h"

class Hero : public BaseSprite
{
public:
	Hero();
	~Hero();

	bool init();

	CREATE_FUNC(Hero);

	std::function<void(cocos2d::Point, float)> walk;
	std::function<void(void)> stop;
};


#endif