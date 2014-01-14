#include <cocos2d.h>
#include "Hero.h"

using namespace cocos2d;

Hero::Hero()
{}

Hero::~Hero()
{}

bool Hero::init()
{
	bool ret = false;
	do {
		this->initWithSpriteFrameName("hero_idle_00.png");

		CallFunc *pCallbackIdle = CallFunc::create(std::bind(&Hero::idle, this));

		Animation *pIdleAnim = this->createAnimation("hero_idle_%02d.png", 6, 12);
		this->setIdleAction(RepeatForever::create(Animate::create(pIdleAnim)));

		Animation *pWalkAnim = this->createAnimation("hero_walk_%02d.png", 7, 14);
		this->setWalkAction(RepeatForever::create(Animate::create(pWalkAnim)));
		
		Animation *pAttackAnim = this->createAnimation("hero_attack_00_%02d.png", 3, 20);
		this->setAttackAction(Sequence::create(Animate::create(pAttackAnim), pCallbackIdle, NULL));

		Animation *pHurtAnim = this->createAnimation("hero_hurt_%02d.png", 3, 12);
		this->setHurtAction(Sequence::create(Animate::create(pHurtAnim), pCallbackIdle, NULL));

		Animation *pKnockOutAnim = this->createAnimation("hero_knockout_%02d.png", 5, 12);
		this->setKnockOutAction(Sequence::create(Animate::create(pKnockOutAnim), NULL));

		this->setAttack(5);
		this->setHP(100);
		this->setVelocity(1);
		this->setDirection(Point::ZERO);

		//this->m_pBlood = ProgressTimer::create(Sprite::create("blood.jpg"));
		////this->m_pBlood->setContentSize(Size(55, 32));
		//this->m_pBlood->setMidpoint(Point(0, 0));
		//this->m_pBlood->setBarChangeRate(Point(1, 0));
		//this->m_pBlood->setAnchorPoint(Point(0, 0));
		//this->m_pBlood->setPosition(Point(0, 32));
		//this->m_pBlood->setPercentage(100);
		//this->m_pBlood->setType(kCCProgressTimerTypeBar);

		//this->m_pBloodBg = ProgressTimer::create(Sprite::create("bloodBg.jpg"));
		//this->m_pBloodBg->setContentSize(Size(55, 32));
		//this->m_pBloodBg->setAnchorPoint(Point(0, 0));
		//this->m_pBloodBg->setPosition(Point(0, 32));
		//this->m_pBloodBg->setPercentage(100);

		//this->addChild(m_pBloodBg);
		//this->addChild(m_pBlood, 1, 1);

		Size heroShowSize = this->getDisplayFrame()->getRect().size;
		this->m_hitBox = this->createBoundingBox( Point(-heroShowSize.width / 2, -heroShowSize.height / 2), heroShowSize);
		this->m_bodyBox = this->createBoundingBox( Point(heroShowSize.width / 2, -10), Size(20, 20));

		ret = true;
	} while(0);

	return ret;
}