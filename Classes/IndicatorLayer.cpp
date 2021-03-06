//
//  IndicatorLayer.cpp
//  Earlybird
//
//  Created by Ronaldinho on 14/12/28.
//
//

#include "IndicatorLayer.h"
IndicatorLayer::IndicatorLayer(){};
IndicatorLayer::~IndicatorLayer(){};
bool IndicatorLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    visibleSize = Director::getInstance()->getVisibleSize();
    originPoint = Director::getInstance()->getVisibleOrigin();
    displaySprite = NULL;
    return true;
}
void IndicatorLayer::displayGoodWindow(int score)
{
    if (displaySprite ==NULL) {
        displaySprite = Sprite::create("head01.png");
        displaySprite->setScale(0.5f);
    }
    if (score>1) {
        Texture2D* texture = Director::getInstance()->getTextureCache()->addImage("ball.png");
        displaySprite->setTexture(texture);
        displaySprite->setScale(0.1f);
    }
    displaySprite->setPosition(Point(this->originPoint.x + this->visibleSize.width / 2, this->originPoint.y - displaySprite->getContentSize().height));
    if (displaySprite->getParent()==NULL) {
        this->addChild(displaySprite);
    }
    // Start next action
    auto displaySpriteMoveTo = MoveTo::create(0.8f ,Point(this->originPoint.x + this->visibleSize.width / 2,this->originPoint.y + this->visibleSize.height/2 - 10.0f));
    // add variable motion for the action
    EaseExponentialOut* sineIn = EaseExponentialOut::create(displaySpriteMoveTo);
    //    CallFunc *actionDone = CallFunc::create(bind(&StatusLayer::fadeInRestartBtn, this));
    auto sequence = Sequence::create(sineIn, NULL);
    displaySprite->stopAllActions();
    //    SimpleAudioEngine::getInstance()->playEffect("sfx_swooshing.ogg");
    displaySprite->runAction(sequence);
    scheduleOnce(schedule_selector(IndicatorLayer::hideGoodWindow),2.0f);
}
void IndicatorLayer::hideGoodWindow(float dt)
{
    auto displaySpriteMoveBack = MoveTo::create(0.8f ,Point(this->originPoint.x + this->visibleSize.width / 2, this->originPoint.y - displaySprite->getContentSize().height));
		  EaseExponentialIn* sineIn = EaseExponentialIn::create(displaySpriteMoveBack);
    //    CallFunc *actionDone = CallFunc::create(bind(&StatusLayer::fadeInRestartBtn, this));
    auto sequence = Sequence::create(sineIn, NULL);
    displaySprite->stopAllActions();
    //    SimpleAudioEngine::getInstance()->playEffect("sfx_swooshing.ogg");
    displaySprite->runAction(sequence);
}