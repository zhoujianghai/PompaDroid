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

	std::function<void(void)> onAttack;

	CC_SYNTHESIZE(unsigned int, m_nextDecisionTime, NextDecisionTime);
};


#endif