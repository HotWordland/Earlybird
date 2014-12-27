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
    return true;
}
void IndicatorLayer::displayGoodWindow()
{
    if (!ball) {
        ball = Sprite::create("ball.png");
    }
    ball->setPosition(Point(this->originPoint.x + this->visibleSize.width / 2, this->originPoint.y - ball->getContentSize().height));
    this->addChild(ball);
    // Start next action
    auto ballMoveTo = MoveTo::create(0.8f ,Point(this->originPoint.x + this->visibleSize.width / 2,this->originPoint.y + this->visibleSize.height/2 - 10.0f));
    // add variable motion for the action
    EaseExponentialOut* sineIn = EaseExponentialOut::create(ballMoveTo);
//    CallFunc *actionDone = CallFunc::create(bind(&StatusLayer::fadeInRestartBtn, this));
    auto sequence = Sequence::create(sineIn, NULL);
    ball->stopAllActions();
//    SimpleAudioEngine::getInstance()->playEffect("sfx_swooshing.ogg");
    ball->runAction(sequence);
}