#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "BaseSprite.h"

#define ENEMY_VELOCITY 0.2f

class Enemy : public BaseSprite
{
public:
	Enemy();
	~Enemy();

	bool init();

	CREATE_FUNC(Enemy);

	std::function<void(void)> onAttack;

	CC_SYNTHESIZE(cocos2d::Point, m_moveDirection, MoveDirection);

	void execute(const cocos2d::Point& target);

private:
	void decide(const cocos2d::Point& target);

	unsigned int m_nextDecisionTime;
};


#endif