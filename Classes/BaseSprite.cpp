#include "BaseSprite.h"

using namespace cocos2d;

BaseSprite::BaseSprite():
	m_pIdleAction(NULL),
	m_pWalkAction(NULL),
	m_pAttackAction(NULL),
	m_pHurtAction(NULL),
	m_pDeadAction(NULL),
	m_currActionState(ACTION_STATE_NONE)
{

}
BaseSprite::~BaseSprite()
{
	CC_SAFE_RELEASE_NULL(m_pIdleAction);
	CC_SAFE_RELEASE_NULL(m_pWalkAction);
	CC_SAFE_RELEASE_NULL(m_pAttackAction);
	CC_SAFE_RELEASE_NULL(m_pHurtAction);
	CC_SAFE_RELEASE_NULL(m_pDeadAction);
}

void BaseSprite::runIdleAction()
{
	if(changeState(ACTION_STATE_IDLE))
	{
		this->runAction(m_pIdleAction);
	}
}
	
void BaseSprite::runWalkAction()
{
	if(changeState(ACTION_STATE_WALK))
	{
		this->runAction(m_pWalkAction);
	}
}

void BaseSprite::runAttackAction()
{
	if(changeState(ACTION_STATE_ATTACK))
	{
		this->runAction(m_pAttackAction);
	}
}

void BaseSprite::runHurtAction(int damage)
{
	if(changeState(ACTION_STATE_HURT))
	{
		this->runAction(m_pHurtAction);
		this->m_hp -= damage;
		//log("m_hp=%d damage=%d", this->m_hp, damage);
		if(this->m_hp <= 0)
		{
			this->runDeadAction();
		}
	}
}

void BaseSprite::runDeadAction()
{
	if(changeState(ACTION_STATE_DEAD))
	{
		this->m_hp = 0;
		this->runAction(m_pDeadAction);
	}
}

void BaseSprite::removeSprite()
{
	changeState(ACTION_STATE_REMOVE);
	log("BaseSprite::removeSprite m_currActionState=%d", m_currActionState);
}

Animation* BaseSprite::createAnimation(const char* formatStr, int frameCount, int fps)
{
	Array *pFrames = Array::createWithCapacity(frameCount);
	for(int i = 0; i < frameCount; ++ i)
	{
		const char* imgName = String::createWithFormat(formatStr, i)->getCString();
		SpriteFrame *pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(imgName);
		pFrames->addObject(pFrame);
	}
	return Animation::createWithSpriteFrames(pFrames, 1.0f / fps);
}

CallFunc* BaseSprite::createDeadCallbackFunc()
{
	return CallFunc::create( CC_CALLBACK_0(BaseSprite::onDead, this));
}

CallFunc* BaseSprite::createIdleCallbackFunc()
{
	return  CallFunc::create(CC_CALLBACK_0(BaseSprite::runIdleAction, this));
}

void BaseSprite::onDead()
{
	this->onDeadCallback();
}

void BaseSprite::onAttackActionFinish()
{

}

bool BaseSprite::isLive()
{
	if(this->m_currActionState >= ACTION_STATE_DEAD)
	{
		return false;
	}else {
		return true;
	}
}

bool BaseSprite::changeState(ActionState actionState)
{
	if((m_currActionState == ACTION_STATE_DEAD && actionState != ACTION_STATE_REMOVE) || m_currActionState == actionState)
	{
		return false;
	}

	this->stopAllActions();
	this->m_currActionState = actionState;
	if(actionState == ACTION_STATE_REMOVE)
		return false;
	else
		return true;
}

BoundingBox BaseSprite::createBoundingBox(cocos2d::Point origin, cocos2d::Size size)
{
	BoundingBox boundingBox;
	boundingBox.original.origin= origin;
	boundingBox.original.size= size;
	boundingBox.actual.origin = this->getPosition() + Point(boundingBox.original.origin.x, boundingBox.original.origin.y);
	boundingBox.actual.size= size;
	return boundingBox;
}



void BaseSprite::updateBoxes() {
	bool isFlippedX = this->isFlippedX();
	float x = 0.0f;
	if(isFlippedX) {
		x = this->getPosition().x - m_hitBox.original.origin.x;
	}else {
		x = this->getPosition().x + m_hitBox.original.origin.x;
	}
	m_hitBox.actual.origin = Point(x, this->getPosition().y + m_hitBox.original.origin.y);
    m_bodyBox.actual.origin = this->getPosition() +m_bodyBox.original.origin;
}

void BaseSprite::setPosition(const Point &position)
{
	Sprite::setPosition(position);
	this->updateBoxes();
}
