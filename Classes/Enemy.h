#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "BaseSprite.h"

class Enemy : public BaseSprite
{
public:
	Enemy();
	~Enemy();

	bool init();

	CREATE_FUNC(Enemy);
};


#endif