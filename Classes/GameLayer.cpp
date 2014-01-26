#include "GameLayer.h"
#include "Hero.h"
#include "Enemy.h"

#include "SceneManager.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;

bool collisionDetection(const BoundingBox &hitBox, const BoundingBox &bodyBox)
{
	Rect hitRect = hitBox.actual;
	Rect bodyRect = bodyBox.actual;
	if(hitRect.intersectsRect(bodyRect))
	{
		return true;
	}

	return false;
}


GameLayer::GameLayer()
	:m_pTiledMap(NULL),
	m_pSpriteNodes(NULL),
	m_pHero(NULL),
	m_pEnemies(NULL),
	m_pBlood(NULL),
	m_pBloodBg(NULL)
{

}

GameLayer::~GameLayer()
{
	this->unscheduleUpdate();
	CC_SAFE_RELEASE(m_pEnemies);
}

bool GameLayer::init()
{
	bool ret = false;
	do {
		CC_BREAK_IF( !Layer::init());

		auto visibleSize = Director::getInstance()->getVisibleSize();
		this->m_origin = Director::getInstance()->getVisibleOrigin();
		this->m_fScreenWidth = visibleSize.width;
		this->m_fScreenHeight = visibleSize.height;

		m_pCloseItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(GameLayer::exitApp, this));
		m_pCloseItem->setPosition(this->m_origin + Point(visibleSize) - Point(m_pCloseItem->getContentSize() / 2));
		auto menu = Menu::create(m_pCloseItem, NULL);
		menu->setPosition(Point::ZERO);
		this->addChild(menu, 1);

		m_pTiledMap = TMXTiledMap::create("pd_tilemap.tmx");
		this->addChild(m_pTiledMap);
		Size tileSize = m_pTiledMap->getTileSize();
		m_fTileWidth = tileSize.width;
		m_fTileHeight = tileSize.height;

		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("pd_sprites.plist");
		m_pSpriteNodes = SpriteBatchNode::create("pd_sprites.pvr.ccz");
		this->addChild(m_pSpriteNodes);

		m_pHero = Hero::create();
		m_pHero->setPosition( m_origin + Point(100, 100) );
		m_pHero->runIdleAction();
		m_pHero->setZOrder(m_fScreenHeight - m_pHero->getPositionY());
		m_pHero->setAttack(5);
		m_pHero->setHP(100);
		m_pHero->onDeadCallback = CC_CALLBACK_0(GameLayer::onHeroDead, this, m_pHero);
		m_pHero->attack = CC_CALLBACK_0(GameLayer::onHeroAttack, this);
		m_pHero->stop = CC_CALLBACK_0(GameLayer::onHeroStop, this);
		m_pHero->walk = CC_CALLBACK_2(GameLayer::onHeroWalk, this);

		Sprite *pBloodSprite = Sprite::create("blood.png");
		this->m_pBlood = ProgressTimer::create(pBloodSprite);
		this->m_pBlood->setType(ProgressTimer::Type::BAR);
		this->m_pBlood->setMidpoint(Point(0, 0));
		this->m_pBlood->setBarChangeRate(Point(1, 0));
		this->m_pBlood->setAnchorPoint(Point(0, 1));
		this->m_pBlood->setPosition(m_origin + Point(2, m_fScreenHeight - 10));
		this->m_pBlood->setPercentage(100);

		this->m_pBloodBg = ProgressTimer::create(Sprite::create("bloodBg.png"));
		this->m_pBloodBg->setType(ProgressTimer::Type::BAR);
		this->m_pBloodBg->setMidpoint(Point(0, 0));
		this->m_pBloodBg->setBarChangeRate(Point(1, 0));
		this->m_pBloodBg->setAnchorPoint(Point(0, 1));
		this->m_pBloodBg->setPosition(this->m_pBlood->getPosition());
		this->m_pBloodBg->setPercentage(100);

		this->addChild(m_pBloodBg, 100);
		this->addChild(m_pBlood, 100);

		m_pSpriteNodes->addChild(m_pHero);

		m_pEnemies = Array::createWithCapacity(MIN_ENEMY_COUNT);
		m_pEnemies->retain();
		for(int i = 0; i < MIN_ENEMY_COUNT; ++ i)
		{
			this->addEnemy();
		}

		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(PATH_BG_MUSIC, true);
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(PATH_HERO_TALK_EFFECT);

		this->scheduleUpdate();

		ret = true;
	} while(0);

	return ret;
}

void GameLayer::exitApp(Object* pSender)
{
	Director::getInstance()->end();
}


void GameLayer::onHeroWalk(Point direction, float distance)
{
	if(m_pHero->isLive())
	{
		m_pHero->setFlippedX(direction.x < 0 ? true : false);
		m_pHero->runWalkAction();

		Point velocity = direction * (distance < 78 ? 1 : 3);
		m_pHero->setVelocity(velocity);
	}
}

void GameLayer::onHeroAttack()
{

	if(m_pHero->isLive())
	{
		m_pHero->runAttackAction();
	
		if(m_pHero->getCurrActionState() == ACTION_STATE_ATTACK)
		{
			Object *enemyObj = NULL;
			CCARRAY_FOREACH(m_pEnemies, enemyObj)
			{
				Enemy *pEnemy = (Enemy*)enemyObj;
				if(pEnemy->getCurrActionState() >= ACTION_STATE_DEAD)
				{
					continue;
				}
				if(fabsf(m_pHero->getPosition().y - pEnemy->getPosition().y) < 10)
				{
					BoundingBox heroHitBox = m_pHero->getHitBox();
					BoundingBox enemyBodyBox = pEnemy->getBodyBox();

					if(::collisionDetection(heroHitBox, enemyBodyBox))
					{
						int damage = m_pHero->getAttack();
						pEnemy->runHurtAction();
						pEnemy->setHP(pEnemy->getHP() - damage);
			
						if(pEnemy->getHP() <= 0)
						{
							pEnemy->runDeadAction();
						}
						CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(PATH_HERO_HIT_EFFECT);
					}
				}
			}
		}
	}

}

void GameLayer::onHeroStop()
{
	if(m_pHero->isLive())
	{
		m_pHero->runIdleAction();
	}
}

void GameLayer::onHeroDead(BaseSprite *pTarget)
{
	if(m_pHero->getCurrActionState() == ACTION_STATE_DEAD)
	{
		pTarget->removeSprite();
		SceneManager::getInstance()->showScene(GAME_OVER_SCENE);
	}
}

void GameLayer::update(float dt)
{
	this->updateHero(dt);
	this->updateEnemies(dt);
}

void GameLayer::updateHero(float dt)
{
	if(m_pHero->getCurrActionState() == ACTION_STATE_WALK)
	{
		float halfHeroFrameHeight = (m_pHero->getDisplayFrame()->getRect().size.height) / 2;
		Point expectP = m_pHero->getPosition() + m_pHero->getVelocity();
		Point actualP = expectP;
		//can not walk on the wall or out of map
		if(expectP.y < halfHeroFrameHeight || expectP.y > (m_fTileHeight * 3 + halfHeroFrameHeight) )
		{
			actualP.y = m_pHero->getPositionY();
		}
		float mapWidth = m_pTiledMap->getContentSize().width;
		float halfWinWidth = m_fScreenWidth / 2;
		float halfHeroFrameWidth = (m_pHero->getDisplayFrame()->getRect().size.width) / 2;
		if(expectP.x > halfWinWidth && expectP.x <= (mapWidth - halfWinWidth))
		{
			this->setPositionX(this->getPositionX() - m_pHero->getVelocity().x);
			this->m_pBlood->setPositionX(this->m_pBlood->getPositionX() + m_pHero->getVelocity().x);
			this->m_pBloodBg->setPositionX(this->m_pBloodBg->getPositionX() + m_pHero->getVelocity().x);
			this->m_pCloseItem->setPositionX(this->m_pCloseItem->getPositionX() + m_pHero->getVelocity().x);
		}else if(expectP.x < halfHeroFrameWidth || expectP.x >= mapWidth - halfHeroFrameWidth)
		{
			actualP.x = m_pHero->getPositionX();
		}
		m_pHero->setPosition(actualP);
		m_pHero->setZOrder(m_fScreenHeight - m_pHero->getPositionY());
	}
}

void GameLayer::updateEnemies(float dt) {
    Object *pObj = NULL;
	Point distance = Point::ZERO;
	//log("enemies count = %d", m_pEnemies->count());
	if(m_pEnemies->count() < MIN_ENEMY_COUNT)
	{
		this->addEnemy();
	}

	Point heroLocation = m_pHero->getPosition();
	if(!m_pHero->isLive())
	{
		heroLocation = Point(-1000, -1000);
	}
	Array *pRemovedEnemies = Array::create();
    CCARRAY_FOREACH(m_pEnemies, pObj)
	{
		Enemy *pEnemy = (Enemy*)pObj;
		if(pEnemy->getCurrActionState() == ACTION_STATE_REMOVE)
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(PATH_ENEMY_DEAD_EFFECT);
			pRemovedEnemies->addObject(pEnemy);
			continue;
		}
		pEnemy->execute(heroLocation, m_pHero->getDisplayFrame()->getRect().size.width);

		if(pEnemy->getCurrActionState() == ACTION_STATE_WALK) 
		{
			Point location = pEnemy->getPosition();
			Point direction = pEnemy->getMoveDirection();
			
			Point expect = location + direction;
			float halfEnemyFrameHeight = (pEnemy->getDisplayFrame()->getRect().size.height) / 2;
			if(expect.y < halfEnemyFrameHeight || expect.y > (m_fTileHeight * 3 + halfEnemyFrameHeight) )
			{
				direction.y = 0;
			}
			pEnemy->setFlippedX(direction.x < 0 ? true : false);
			pEnemy->setPosition(location + direction);
			pEnemy->setZOrder(pEnemy->getPositionY());
		}
	}

	CCARRAY_FOREACH(pRemovedEnemies, pObj)
	{
		Enemy *pEnemy = (Enemy*)pObj;
		m_pEnemies->removeObject(pEnemy);
		m_pSpriteNodes->removeChild(pEnemy, true);
	}

	pRemovedEnemies->removeAllObjects();
}

void GameLayer::onEnemyAttack(BaseSprite *pSprite)
{
	Object *enemyObj = NULL;
	CCARRAY_FOREACH(m_pEnemies, enemyObj)
	{
		Enemy *pEnemy = (Enemy*)enemyObj;
		if(pEnemy->getCurrActionState() == ACTION_STATE_ATTACK)
		{
			if(!m_pHero->isLive())
			{
				break;
			}
			pEnemy->setPositionY(m_pHero->getPositionY());
			BoundingBox heroBodyBox = m_pHero->getBodyBox();
			BoundingBox enemyHitBox = pEnemy->getHitBox();

			if(::collisionDetection(enemyHitBox, heroBodyBox))
			{
				int damage = pEnemy->getAttack();
				m_pHero->runHurtAction();
				m_pHero->setHP(m_pHero->getHP() - damage);
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(PATH_ENEMY_HIT_EFFECT);

				if(m_pHero->getHP() <= 0)
				{
					m_pHero->runDeadAction();
					CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(PATH_HERO_DEAD_EFFECT);
				}
				this->m_pBlood->setPercentage( (m_pHero->getHP() / 100.0f) * 100);
				
			}
		}
	}
}

void GameLayer::onEnemyDead(BaseSprite *pTarget)
{
	pTarget->removeSprite();
}

void GameLayer::addEnemy()
{
	Size winSize = Director::getInstance()->getWinSize();
	Point location = Point::ZERO;

	Enemy *pEnemy = Enemy::create();
	//log("m_pTiledMap->getMapSize() mapSize=%f", m_pTiledMap->getMapSize().width);
	float halfEnemyFrameHeight = (pEnemy->getDisplayFrame()->getRect().size.height) / 2;
	float heroPosX = m_pHero->getPositionX();
	float halfWinWidth = (winSize.width / 2);
	while(fabsf(heroPosX - location.x) < 150)
	{
		if(heroPosX < halfWinWidth)
		{
			location.x = m_pHero->getPositionX() + CCRANDOM_0_1()  * halfWinWidth;
		}else if(heroPosX > (m_pTiledMap->getMapSize().width * m_fTileWidth - halfWinWidth)) {
			location.x = m_pHero->getPositionX() - CCRANDOM_0_1()  * halfWinWidth;
		}else {
			location.x = m_pHero->getPositionX() + CCRANDOM_MINUS1_1()  * halfWinWidth;
		}
	}

	float maxY = m_fTileHeight * 3 + halfEnemyFrameHeight;
	location.y = CCRANDOM_0_1() * maxY;
	if(location.y < halfEnemyFrameHeight)
	{
		location.y = halfEnemyFrameHeight;
	}

	pEnemy->attack = CC_CALLBACK_0(GameLayer::onEnemyAttack, this, pEnemy);
	pEnemy->onDeadCallback = CC_CALLBACK_0(GameLayer::onEnemyDead, this, pEnemy);
	pEnemy->setPosition(m_origin + location);
	pEnemy->setZOrder(m_fScreenHeight - pEnemy->getPositionY());
	pEnemy->runIdleAction();
	pEnemy->setAttack(5);
	pEnemy->setHP(30);
	pEnemy->setVelocity(Point(0.5f, 0.5f));
	pEnemy->setEyeArea(200);
	pEnemy->setAttackArea(25);

	m_pEnemies->addObject(pEnemy);
	m_pSpriteNodes->addChild(pEnemy);
}
