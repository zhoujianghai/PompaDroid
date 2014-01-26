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

		Animation *pIdleAnim = this->createAnimation("robot_idle_%02d.png", 5, 12);
		this->setIdleAction(RepeatForever::create(Animate::create(pIdleAnim)));

		Animation *pWalkAnim = this->createAnimation("robot_walk_%02d.png", 6, 12);
		this->setWalkAction(RepeatForever::create(Animate::create(pWalkAnim)));
		
		Animation *pAttackAnim = this->createAnimation("robot_attack_%02d.png", 5, 20);
		this->setAttackAction(Sequence::create(Animate::create(pAttackAnim), BaseSprite::createIdleCallbackFunc(), NULL));

		Animation *pHurtAnim = this->createAnimation("robot_hurt_%02d.png", 3, 20);
		this->setHurtAction(Sequence::create(Animate::create(pHurtAnim), BaseSprite::createIdleCallbackFunc(), NULL));

		Animation *pDeadAnim = this->createAnimation("robot_knockout_%02d.png", 5, 12);
		this->setDeadAction(Sequence::create(Animate::create(pDeadAnim), Blink::create(3, 9), BaseSprite::createDeadCallbackFunc(), NULL));

		Size enemyShowSize = this->getDisplayFrame()->getRect().size;
		this->m_bodyBox = this->createBoundingBox(Point(-enemyShowSize.width / 2, -enemyShowSize.height / 2), enemyShowSize);
		this->m_hitBox = this->createBoundingBox(Point(enemyShowSize.width / 2, -5), Size(25, 20));

		ret = true;
	} while(0);

	return ret;
}




void Enemy::execute(const Point& target, float targetBodyWidth)
{
	if(m_nextDecisionTime == 0)
	{
		this->decide(target, targetBodyWidth);
	}else {
		-- m_nextDecisionTime;
	}
}

void Enemy::decide(const Point& target, float targetBodyWidth)
{
	Point location = this->getPosition();
	float distance = location.getDistance(target);
	distance = distance - (targetBodyWidth / 2 + this->getDisplayFrame()->getRect().size.width / 2) + 30;
	//log("distance=%f, m_fVelocity=%f", distance, m_fVelocity);

	bool isFlippedX = this->isFlippedX();
	bool isOnTargetLeft = (location.x < target.x ? true : false);
	if((isFlippedX && isOnTargetLeft) || (!isFlippedX && !isOnTargetLeft)) {
		this->m_aiState = CCRANDOM_0_1() > 0.5f ? AI_PATROL : AI_IDLE;
	}else {
		if(distance < m_eyeArea)
		{
			this->m_aiState = distance < m_attackArea ? AI_ATTACK : AI_PURSUIT;
		}else {
			this->m_aiState = CCRANDOM_0_1() > 0.5f ? AI_PATROL : AI_IDLE;
		}
	}
	switch(m_aiState)
	{
	case AI_ATTACK:
		{
			this->runAttackAction();
			this->attack();
			this->m_nextDecisionTime = 50;
		}
		break;
	case AI_IDLE:
		{
			this->runIdleAction();
			this->m_nextDecisionTime = CCRANDOM_0_1() * 100;
		}
		break;
	case AI_PATROL:
		{
			this->runWalkAction();
			this->m_moveDirection.x = CCRANDOM_MINUS1_1();
			this->m_moveDirection.y = CCRANDOM_MINUS1_1();
			m_moveDirection.x  = m_moveDirection.x > 0 ? (m_moveDirection.x + m_fVelocity.x) : (m_moveDirection.x - m_fVelocity.x);
			m_moveDirection.y  = m_moveDirection.y > 0 ? (m_moveDirection.y + m_fVelocity.y) : (m_moveDirection.y - m_fVelocity.y);
			this->m_nextDecisionTime = CCRANDOM_0_1() * 100;
		}
		break;
	case AI_PURSUIT:
		{
			this->runWalkAction();
			//v.normalize() function return the unit vector of v
			this->m_moveDirection = (target - location).normalize();
			this->setFlippedX(m_moveDirection.x < 0 ? true : false);
			m_moveDirection.x  = m_moveDirection.x > 0 ? (m_moveDirection.x + m_fVelocity.x) : (m_moveDirection.x - m_fVelocity.x);
			m_moveDirection.y  = m_moveDirection.y > 0 ? (m_moveDirection.y + m_fVelocity.y) : (m_moveDirection.y - m_fVelocity.y);
			this->m_nextDecisionTime = 10;
		}
		break;
	}
}