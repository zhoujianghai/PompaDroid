#include "BaseSprite.h"

using namespace cocos2d;

BaseSprite::BaseSprite():
	m_pIdleAction(NULL),
	m_pWalkAction(NULL),
	m_pAttackAction(NULL),
	m_pHurtAction(NULL),
	m_pKnockOutAction(NULL)
{

}
BaseSprite::~BaseSprite()
{
	CC_SAFE_RELEASE_NULL(m_pIdleAction);
	CC_SAFE_RELEASE_NULL(m_pWalkAction);
	CC_SAFE_RELEASE_NULL(m_pAttackAction);
	CC_SAFE_RELEASE_NULL(m_pHurtAction);
	CC_SAFE_RELEASE_NULL(m_pKnockOutAction);
}

void BaseSprite::idle()
{
	if(changeState(ACTION_STATE_IDLE))
	{
		this->runAction(m_pIdleAction);
		this->m_fVelocity = 0.0f;
	}
}
	
void BaseSprite::walk()
{
	if(changeState(ACTION_STATE_WALK))
	{
		this->runAction(m_pWalkAction);
	}
}

void BaseSprite::attack()
{
	if(changeState(ACTION_STATE_ATTACK))
	{
		this->runAction(m_pAttackAction);
	}
}

void BaseSprite::hurt(int damage)
{
	if(changeState(ACTION_STATE_HURT))
	{
		this->runAction(m_pHurtAction);
		this->m_hp -= damage;
		log("m_hp=%d damage=%d", this->m_hp, damage);
		if(this->m_hp <= 0)
		{
			this->knockout();
		}
	}
}

void BaseSprite::knockout()
{
	if(changeState(ACTION_STATE_KNOCKOUT))
	{
		this->runAction(m_pKnockOutAction);
		this->m_hp = 0;
	}
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


bool BaseSprite::changeState(ActionState actionState)
{
	if(ACTION_STATE_KNOCKOUT == m_currActionState || m_currActionState == actionState)
	{
		return false;
	}

	this->stopAllActions();
	this->m_currActionState = actionState;
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

void BaseSprite::transformBoxes() {
	float scaleX =  this->getScaleX();
	float scaleY =  this->getScaleY();
	m_hitBox.actual.origin = this->getPosition() + Point(m_hitBox.original.origin.x * scaleX, m_hitBox.original.origin.y * scaleY);
    m_hitBox.actual.size = Size(m_hitBox.original.size.width * scaleX, m_hitBox.original.size.height * scaleY);
    m_bodyBox.actual.origin = this->getPosition() + Point(m_bodyBox.original.origin.x * scaleX, m_bodyBox.original.origin.y * scaleY);
    m_bodyBox.actual.size = Size(m_bodyBox.original.size.width * scaleX, m_bodyBox.original.size.height * scaleY);
}

void BaseSprite::setPosition(const Point &position)
{
	Sprite::setPosition(position);
	this->transformBoxes();
}