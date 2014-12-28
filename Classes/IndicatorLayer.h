//
//  IndicatorLayer.h
//  Earlybird
//
//  Created by Ronaldinho on 14/12/28.
//
//

#ifndef __Earlybird__IndicatorLayer__
#define __Earlybird__IndicatorLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "GameLayer.h"
using namespace cocos2d;

class IndicatorLayer:public Layer, public IndicatorDelegate{
public:
    IndicatorLayer(void);
    ~IndicatorLayer(void);
    virtual bool init();
    CREATE_FUNC(IndicatorLayer);
    void displayGoodWindow();
	//�ӳ�ִ�еĺ�������Ҫ��float dt ����
	void hideGoodWindow(float dt);
private:
    Size visibleSize ;
    Vec2 originPoint;
     Sprite* displaySprite;
};

#endif /* defined(__Earlybird__IndicatorLayer__) */
