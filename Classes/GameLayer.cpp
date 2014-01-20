#include "GameLayer.h"
#include "Hero.h"
#include "Enemy.h"

#include "SceneManager.h"

using namespace cocos2d;

long millisecondNow()
{
	struct timeval now_t;
	gettimeofday(&now_t,NULL);      
	return now_t.tv_sec * 1000 + now_t.tv_usec / 1000;  
}

bool collisionDetection(const BoundingBox &hitBox, const BoundingBox &bodyBox)
{
	Rect hitRect = hitBox.actual;
	Rect bodyRect = bodyBox.actual;
	log("hitRect x=%f, y=%f", hitRect.origin.x, hitRect.origin.y);
	log("bodyRect x=%f, y=%f", bodyRect.origin.x, bodyRect.origin.y);
	if(hitRect.intersectsRect(bodyRect))
	{
		return true;
	}

	return false;
}

#define CURTIME millisecondNow()

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

		Size winSize = Director::getInstance()->getWinSize();
		m_fScreenWidth = winSize.width;
		m_fScreenHeight = winSize.height;

		m_pTiledMap = TMXTiledMap::create("pd_tilemap.tmx");
		this->addChild(m_pTiledMap);
		Size tileSize = m_pTiledMap->getTileSize();
		m_fTileWidth = tileSize.width;
		m_fTileHeight = tileSize.height;

		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("pd_sprites.plist");
		m_pSpriteNodes = SpriteBatchNode::create("pd_sprites.pvr.ccz");
		this->addChild(m_pSpriteNodes);

		m_pHero = Hero::create();
		m_pHero->setPosition( Point(100, 100) );
		m_pHero->idle();
		m_pHero->setZOrder(m_fScreenHeight - m_pHero->getPositionY());
		m_pHero->setAttack(5);
		m_pHero->setHP(100);
		m_pHero->setDirection(Point::ZERO);
		m_pHero->onDeadCallback = std::bind(&GameLayer::onHeroDead, this, m_pHero);


		Sprite *pBloodSprite = Sprite::create("blood.jpg");
		this->m_pBlood = ProgressTimer::create(pBloodSprite);
		this->m_pBlood->setType(ProgressTimer::Type::BAR);
		this->m_pBlood->setMidpoint(Point(0, 0));
		this->m_pBlood->setBarChangeRate(Point(1, 0));
		this->m_pBlood->setAnchorPoint(Point(0, 1));
		this->m_pBlood->setPosition(Point(2, winSize.height - 10));
		this->m_pBlood->setPercentage(100);
		this->m_pBlood->setScaleX(4.0f);

		this->m_pBloodBg = ProgressTimer::create(Sprite::create("bloodBg.jpg"));
		this->m_pBloodBg->setType(ProgressTimer::Type::BAR);
		this->m_pBloodBg->setMidpoint(Point(0, 0));
		this->m_pBloodBg->setBarChangeRate(Point(1, 0));
		this->m_pBloodBg->setAnchorPoint(Point(0, 1));
		this->m_pBloodBg->setPosition(Point(2, winSize.height - 10));
		this->m_pBloodBg->setPercentage(100);
		this->m_pBloodBg->setScaleX(4.0f);

		this->addChild(m_pBloodBg, 100);
		this->addChild(m_pBlood, 100);

		m_pSpriteNodes->addChild(m_pHero);

		const int enemyCount = 3;
		m_pEnemies = Array::createWithCapacity(enemyCount);
		m_pEnemies->retain();
		for(int i = 0; i < enemyCount; ++ i)
		{
			this->addEnemy();
		}

		this->scheduleUpdate();

		ret = true;
	} while(0);

	return ret;
}


void GameLayer::onHeroWalk(Point direction, float distance)
{
	if(m_pHero->isLive())
	{
		m_pHero->setFlippedX(direction.x < 0 ? true : false);
		m_pHero->walk();

		m_heroVelocity = direction * (distance < 96 ? 1 : 2);
	}
}

void GameLayer::onHeroAttack()
{

	if(m_pHero->isLive())
	{
		m_pHero->attack();
	
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
						pEnemy->hurt(damage);
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
		m_pHero->idle();
	}
}

void GameLayer::onHeroDead(BaseSprite *pTarget)
{
	if(m_pHero->getCurrActionState() == ACTION_STATE_DEAD)
	{
		log("GameLayer::onHeroDead*******************");
		pTarget->remove();
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
		Point expectP = m_pHero->getPosition() + m_heroVelocity;
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
			this->setPositionX(this->getPositionX() - m_heroVelocity.x);
		}else if(expectP.x < halfHeroFrameWidth || expectP.x >= mapWidth - halfHeroFrameWidth)
		{
			actualP.x = m_pHero->getPositionX();
		}
		m_pHero->setPosition(actualP);
		m_pHero->setZOrder(m_fScreenHeight - m_pHero->getPositionY());
	}
}

void GameLayer::updateEnemies(float dt) {
	int alive = 0;
    Object *pObj = NULL;
	Point distance = Point::ZERO;
	//log("enemies count = %d", m_pEnemies->count());
	if(m_pEnemies->count() < 3)
	{
		this->addEnemy();
	}

	Point heroLocation = m_pHero->getPosition();
	if(!m_pHero->isLive())
	{
		heroLocation = Point(-1000, -1000);
	}

    CCARRAY_FOREACH(m_pEnemies, pObj)
	{
		Enemy *pEnemy = (Enemy*)pObj;
		if(pEnemy->getCurrActionState() == ACTION_STATE_REMOVE)
		{
			m_pEnemies->removeObject(pEnemy);
			m_pSpriteNodes->removeChild(pEnemy, true);
			continue;
		}
		pEnemy->execute(heroLocation);

		if(pEnemy->getCurrActionState() == ACTION_STATE_WALK) 
		{
			++ alive;
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
	//if(alive == 0 && m_pEnemies->count() > 0)
	//{
	//	m_pEnemies->removeAllObjects();
	//}

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
				m_pHero->hurt(damage);
				//log("hero hp=%d", m_pHero->getHP());
				this->m_pBlood->setPercentage( (m_pHero->getHP() / 100.0f) * 100);
			}
		}
	}
}

void GameLayer::onEnemyDead(BaseSprite *pTarget)
{
	log("GameLayer::onEnemyDead*******************");
	pTarget->remove();
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

	pEnemy->onAttack = std::bind(&GameLayer::onEnemyAttack, this, pEnemy);
	pEnemy->onDeadCallback = std::bind(&GameLayer::onEnemyDead, this, pEnemy);
	pEnemy->setPosition(location);
	pEnemy->setZOrder(m_fScreenHeight - pEnemy->getPositionY());
	pEnemy->idle();
	pEnemy->setAttack(5);
	pEnemy->setHP(30);
	pEnemy->setVelocity(0.5f);
	pEnemy->setDirection(Point::ZERO);
	pEnemy->setEyeArea(200);
	pEnemy->setAttackArea(30);

	m_pEnemies->addObject(pEnemy);
	m_pSpriteNodes->addChild(pEnemy);
}