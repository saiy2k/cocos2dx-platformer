#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

#define COUNT 4

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

    CCMenuItemImage *pCloseItem = CCMenuItemImage::create("CloseNormal.png",
                                                          "CloseSelected.png",
                                                          this,
                                                          menu_selector(HelloWorld::menuCloseCallback) );
    pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 60, 60) );
    pCloseItem->setTag(1);
    pCloseItem->setScale(2.5f);
    
    CCMenuItemImage *pCloseItem1 = CCMenuItemImage::create("CloseNormal.png",
                                                           "CloseSelected.png",
                                                           this,
                                                           menu_selector(HelloWorld::menuCloseCallback) );
    pCloseItem1->setPosition( ccp(60, 60) );
    pCloseItem1->setTag(2);
    pCloseItem1->setScale(2.5f);

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, pCloseItem1, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);   

    pSprite = CCSprite::create("CloseNormal.png");
    pSprite->setPosition( ccp(600, 700) );
    pSprite->setTag(-1);
    this->addChild(pSprite, 0);
    
    lines = (Line *)malloc(sizeof(Line) * COUNT);
    
    lines[0].p1 = ccp(100, 100);
    lines[0].p2 = ccp(400, 100);
    
    lines[1].p1 = ccp(500, 600);
    lines[1].p2 = ccp(700, 600);
    
    lines[2].p1 = ccp(400, 400);
    lines[2].p2 = ccp(600, 500);
    
    lines[3].p1 = ccp(500, 200);
    lines[3].p2 = ccp(1000, 200);
    
    speedX = 0;
    speedY = 0;
    
    this->scheduleUpdate();
    
    return true;
}

void HelloWorld::update(float dt) {
    CCPoint p = pSprite->getPosition();
    
    speedX *= 0.95; // friction reduces the speed over time
    
    if (pSprite->getTag() == -1) { // free falling
        CCLog("searching for platform... hero is falling");
        // iterate through all platforms
        for (int i = 0; i < COUNT; i++) {
            // if hero position lies between the x bounds of any platform
            if ( lines[i].p1.x < p.x && lines[i].p2.x > p.x ) {
                // also it is closer than 100 pixels to the platform
                if ( p.y - lines[i].p1.y < 100 && p.y - lines[i].p1.y > 0) {
                    // assign the platform to the hero
                    pSprite->setTag(i);
                    break;
                }
            }
        }
        // gravity pull decreases the y-speed
        speedY -= 10.0;
        
        // not allowing hero to go down indefinitely, by making him swap to the top
        if ( p.y < 0 ) {
            p.y = 768;
        }
    } else { // resting in or closer to some platform
        int tag = pSprite->getTag();
        CCLog("in platform %d", tag);
        
        // make sure if hero is still in the x bounds of the platform or moved away
        if ( lines[tag].p1.x < p.x && lines[tag].p2.x > p.x ) {
            float x2, y2;
            x2 = p.x;
            // platform may be a sloppy one, so interpolate the y value based on end points of platform and current hero position 
            y2 = (x2 - lines[tag].p1.x) * (lines[tag].p2.y - lines[tag].p1.y) / (lines[tag].p2.x - lines[tag].p1.x) + lines[tag].p1.y;
            if (p.y - y2 > 0) { // if hero is above platform, make him fall down
                speedY -= 10.0;
            } else {            // nullify the y speed and rest on platform
                speedY = 0;
                p.y = y2;
            }
        } else { // switch to free fall mode, if hero is moved away
            pSprite->setTag(-1);
        }
    }
    
    pSprite->setPosition( ccp( p.x + speedX * dt, p.y + speedY * dt ) );
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    if (((CCNode *)pSender)->getTag() == 1) {
        speedX += 100;
    } else {
        speedX -= 100;
    }
}

void HelloWorld::draw() {
    glLineWidth(4.0);
    for (int i = 0; i < COUNT; i++) {
        ccDrawLine( lines[i].p1, lines[i].p2 );
    }
    CCLayer::draw();
}



