#include <cocos2d.h>
#include "Enemy.h"

using namespace cocos2d;

Enemy::Enemy():
	m_nextDecisionTime(0)
{}

Enemy::~Enemy()
{}

bool Enemy::init()
{
	bool ret = false;
	do {
		this->initWithSpriteFrameName("robot_idle_00.png");

		CallFunc *pCallbackIdle = CallFunc::create(std::bind(&Enemy::idle, this));

		Animation *pIdleAnim = this->createAnimation("robot_idle_%02d.png", 5, 12);
		this->setIdleAction(RepeatForever::create(Animate::create(pIdleAnim)));

		Animation *pWalkAnim = this->createAnimation("robot_walk_%02d.png", 6, 20);
		this->setWalkAction(RepeatForever::create(Animate::create(pWalkAnim)));
		
		Animation *pAttackAnim = this->createAnimation("robot_attack_%02d.png", 5, 12);
		this->setAttackAction(Sequence::create(Animate::create(pAttackAnim), pCallbackIdle, NULL));

		Animation *pHurtAnim = this->createAnimation("robot_hurt_%02d.png", 3, 12);
		this->setHurtAction(Sequence::create(Animate::create(pHurtAnim), pCallbackIdle, NULL));

		Animation *pKnockOutAnim = this->createAnimation("robot_knockout_%02d.png", 5, 12);
		this->setKnockOutAction(Sequence::create(Animate::create(pKnockOutAnim), NULL));

		this->setAttack(2);
		this->setHP(30);
		this->setVelocity(1);
		this->setDirection(Point::ZERO);

		Size heroShowSize = this->getDisplayFrame()->getRect().size;
		this->m_bodyBox = this->createBoundingBox(Point(-heroShowSize.width / 2, -heroShowSize.width / 2), heroShowSize);
		this->m_hitBox = this->createBoundingBox(Point(heroShowSize.width / 2, -5), Size(25, 20));

		ret = true;
	} while(0);

	return ret;
}

void Enemy::execute(const Point& target)
{
	if(m_nextDecisionTime == 0)
	{
		this->decide(target);
	}else {
		-- m_nextDecisionTime;
	}
}

void Enemy::decide(const Point& target)
{
	Point location = this->getPosition();
	float distance = location.getDistance(target);
	log("distance=%f", distance);
	if(distance < 150)
	{
		this->walk();
		m_moveDirection = (target - location).normalize();
		this->setFlippedX(m_moveDirection.x < 0 ? true : false);
		if(m_moveDirection.x > 0)
		{
			m_moveDirection.x += ENEMY_VELOCITY;
		}else {
			m_moveDirection.x -= ENEMY_VELOCITY;
		}

		if(m_moveDirection.y > 0)
		{
			m_moveDirection.y += ENEMY_VELOCITY;
		}else {
			m_moveDirection.y -= ENEMY_VELOCITY;
		}
		m_nextDecisionTime = 10;

	}else {
		this->walk();
        m_moveDirection.x = CCRANDOM_MINUS1_1();
        if (m_moveDirection.x > 0) {
            m_moveDirection.x += ENEMY_VELOCITY;
        } else {
            m_moveDirection.x -= ENEMY_VELOCITY;
        }
        m_moveDirection.y = CCRANDOM_MINUS1_1();
        if (m_moveDirection.y > 0) {
            m_moveDirection.y += ENEMY_VELOCITY;
        } else {
            m_moveDirection.y -= ENEMY_VELOCITY;
        }
        m_nextDecisionTime = CCRANDOM_0_1() * 100;
	}
}