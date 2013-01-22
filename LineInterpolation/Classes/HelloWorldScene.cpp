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
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback) );
    pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );
    pCloseItem->setTag(1);
    
    CCMenuItemImage *pCloseItem1 = CCMenuItemImage::create(
                                                          "CloseNormal.png",
                                                          "CloseSelected.png",
                                                          this,
                                                          menu_selector(HelloWorld::menuCloseCallback) );
    pCloseItem1->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 60, 20) );
    pCloseItem1->setTag(2);

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, pCloseItem1, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Thonburi", 34);

    // ask director the window size
    CCSize size = CCDirector::sharedDirector()->getWinSize();

    // position the label on the center of the screen
    pLabel->setPosition( ccp(size.width / 2, size.height - 20) );

    // add the label as a child to this layer
    this->addChild(pLabel, 1);

    // add "HelloWorld" splash screen"
    pSprite = CCSprite::create("CloseNormal.png");

    // position the sprite on the center of the screen
    pSprite->setPosition( ccp(600, 700) );
    pSprite->setTag(-1);

    // add the sprite as a child to this layer
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
    
    speedX *= 0.95;
    
    if (pSprite->getTag() == -1) {
        CCLog("searching for platform...");
        for (int i = 0; i < COUNT; i++) {
            if ( lines[i].p1.x < p.x && lines[i].p2.x > p.x ) {
                if ( p.y - lines[i].p1.y < 100 && p.y - lines[i].p1.y > 0) {
                    pSprite->setTag(i);
                    break;
                }
            }
        }
        speedY -= 10.0;
        if ( p.y < 0 ) {
            p.y = 768;
        }
    } else {
        int tag = pSprite->getTag();
        CCLog("in platform %d", tag);
        if ( lines[tag].p1.x < p.x && lines[tag].p2.x > p.x ) {
            float x2, y2;
            x2 = p.x;
            y2 = (x2 - lines[tag].p1.x) * (lines[tag].p2.y - lines[tag].p1.y) / (lines[tag].p2.x - lines[tag].p1.x) + lines[tag].p1.y;
            if (p.y - y2 > 0) {
                speedY -= 10.0;
            } else {
                speedY = 0;
                p.y = y2;
            }
        } else {
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
    for (int i = 0; i < COUNT; i++) {
        ccDrawLine( lines[i].p1, lines[i].p2 );
    }
    CCLayer::draw();
}
