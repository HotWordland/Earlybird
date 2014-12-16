#include "ComboEffect.h"
void showComboEffect(int size,Node* node){
    /*
	if(size<5)
		return;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Sprite* comboSprite;
	if(size>=10){
		comboSprite = Sprite::create("combo_3.png");
	}else if(size>=7){
		comboSprite = Sprite::create("combo_2.png");
	}else{
		comboSprite = Sprite::create("combo_1.png");
	}
     */
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Sprite* comboSprite;
    comboSprite = Sprite::create("head01.png");
    comboSprite->setScale(0.5f);
	comboSprite->setPosition(Point(visibleSize.width-80,visibleSize.height-80));
	node->addChild(comboSprite);

	Blink* blink = Blink::create(3.0f,5);
	CallFunc* remove = CallFunc::create([=](){comboSprite->removeFromParentAndCleanup(true);});
	Sequence* action = Sequence::create(blink,remove,nullptr);
	comboSprite->runAction(action);
}