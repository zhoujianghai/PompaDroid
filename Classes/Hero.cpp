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

		Animation *pIdleAnim = this->createAnimation("hero_idle_%02d.png", 6, 12);
		this->setIdleAction(RepeatForever::create(Animate::create(pIdleAnim)));

		Animation *pWalkAnim = this->createAnimation("hero_walk_%02d.png", 7, 14);
		this->setWalkAction(RepeatForever::create(Animate::create(pWalkAnim)));
		
		Animation *pAttackAnim = this->createAnimation("hero_attack_00_%02d.png", 3, 20);
		this->setAttackAction(Sequence::create(Animate::create(pAttackAnim), BaseSprite::createIdleCallbackFunc(), NULL));

		Animation *pHurtAnim = this->createAnimation("hero_hurt_%02d.png", 3, 12);
		this->setHurtAction(Sequence::create(Animate::create(pHurtAnim), BaseSprite::createIdleCallbackFunc(), NULL));

		Animation *pDeadAnim = this->createAnimation("hero_knockout_%02d.png", 5, 12);
		this->setDeadAction(Sequence::create(Animate::create(pDeadAnim), Blink::create(3, 9), BaseSprite::createDeadCallbackFunc(), NULL));

		Size heroShowSize = this->getDisplayFrame()->getRect().size;
		this->m_bodyBox = this->createBoundingBox( Point(-heroShowSize.width / 2, -heroShowSize.height / 2), heroShowSize);
		this->m_hitBox = this->createBoundingBox( Point(heroShowSize.width / 2, -10), Size(20, 20));

		ret = true;
	} while(0);

	return ret;
}
