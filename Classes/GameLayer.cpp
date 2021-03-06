#include "GameLayer.h"
#include "StarParticle.h"
#include "ComboEffect.h"
GameLayer::GameLayer(){}

GameLayer::~GameLayer(){}

bool GameLayer::init(){
    if(Layer::init()) {
        //get the origin point of the X-Y axis, and the visiable size of the screen
        Size visiableSize = Director::getInstance()->getVisibleSize();
        Point origin = Director::getInstance()->getVisibleOrigin();
        
        this->gameStatus = GAME_STATUS_READY;
        this->score = 0;
        
        // Add the bird
        this->bird = BirdSprite::getInstance();
        this->bird->createBird();
        PhysicsBody *body = PhysicsBody::create();
        body->addShape(PhysicsShapeCircle::create(BIRD_RADIUS));
        body->setCategoryBitmask(ColliderTypeBird);
        body->setCollisionBitmask(ColliderTypeLand & ColliderTypePip | ColliderTypeBall);
        body->setContactTestBitmask(ColliderTypeLand | ColliderTypePip | ColliderTypeBall);
        body->setDynamic(true);
        body->setLinearDamping(0.0f);
        body->setGravityEnable(false);
        this->bird->setPhysicsBody(body);
        this->bird->setPosition(origin.x + visiableSize.width*1/3 - 5,origin.y + visiableSize.height/2 + 5);
        this->bird->idle();
        this->addChild(this->bird);
        
        /*
         //初始化加速粒子
         accelerateParticle = ParticleSystemQuad::create("particleImpact.plist");
         accelerateParticle->setScale(0.5f);
         accelerateParticle->setPosition(0,0);
         addChild(accelerateParticle);
         */
        accelerateParticle = NULL;
        pipSpeed = 2;
        /*
         //闪亮
         Blink *blink = Blink::create(5.0f, 10);
         bird->runAction(blink);
         */
        //Ball
        this->ball = Sprite::create("ball.png");
        PhysicsBody *ballbody = PhysicsBody::create();
        ballbody->addShape(PhysicsShapeCircle::create(BIRD_RADIUS+5));
        
        
        ballbody->setCategoryBitmask(ColliderTypeBall);
        ballbody->setCollisionBitmask(ColliderTypePip | ColliderTypeLand);
        ballbody->setContactTestBitmask(ColliderTypePip | ColliderTypeLand);
        
        
        ballbody->setDynamic(true);
        ballbody->setLinearDamping(0.0f);
        ballbody->setGravityEnable(false);
        ball->setPhysicsBody(ballbody);
        //    static Shaky3D* create(float duration, const Size& gridSize, int range, bool shakeZ);
        
        ball->setPosition(bird->getPositionX(),bird->getPositionY()+30);
        ball->setTag(100);
        //		addChild(ball);
        
        BallisTouch = false;
        
        //BallName
        ballName = Sprite::create("BallWithHoney.png");
        ballName->setPosition(ball->getPositionX(),ball->getPositionY()+40);
        //addChild(ballName);
        
        // Add the ground
        this->groundNode = Node::create();
        float landHeight = BackgroundLayer::getLandHeight();
        auto groundBody = PhysicsBody::create();
        groundBody->addShape(PhysicsShapeBox::create(Size(288, landHeight)));
        groundBody->setDynamic(false);
        groundBody->setLinearDamping(0.0f);
        groundBody->setCategoryBitmask(ColliderTypeLand);
        groundBody->setCollisionBitmask(ColliderTypeBird | ColliderTypeBall);
        groundBody->setContactTestBitmask(ColliderTypeBird | ColliderTypeLand);
        this->groundNode->setPhysicsBody(groundBody);
        this->groundNode->setPosition(144, landHeight/2);
        this->addChild(this->groundNode);
        
        // init land
        this->landSpite1 = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("land"));
        this->landSpite1->setAnchorPoint(Point::ZERO);
        this->landSpite1->setPosition(Point::ZERO);
        this->addChild(this->landSpite1, 30);
        
        this->landSpite2 = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("land"));
        this->landSpite2->setAnchorPoint(Point::ZERO);
        this->landSpite2->setPosition(this->landSpite1->getContentSize().width-2.0f,0);
        this->addChild(this->landSpite2, 30);
        
        shiftLand = schedule_selector(GameLayer::scrollLand);
        this->schedule(shiftLand, 0.01f);
        
        this->scheduleUpdate();
        
        auto contactListener = EventListenerPhysicsContact::create();
        contactListener->onContactBegin = CC_CALLBACK_1(GameLayer::onContactBegin, this);
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
        
        return true;
    }else {
        return false;
    }
}

bool GameLayer::onContactBegin(PhysicsContact& contact) {
    auto b_1 = (Sprite*)contact.getShapeA()->getBody()->getNode();
    auto b_2 = (Sprite*)contact.getShapeB()->getBody()->getNode();
    if (b_1 == ball && b_1->getTag() == 100)
    {
        log("FromBall");
        BallisTouch = true;
    }
    else if(b_2 == ball && b_2->getTag() == 100)
    {
        log("FromBall");
        BallisTouch = true;
        
    }else if (b_1->getPhysicsBody()->getCategoryBitmask()==ColliderTypeBall && b_1->getTag() != 100)
    {
        auto randomColor = rand()%4;
        showStarParticleEffect(randomColor, bird->getPosition(), this);
    }
    else if(b_2->getPhysicsBody()->getCategoryBitmask()==ColliderTypeBall && b_2->getTag() != 100)
    {
        auto randomColor = rand()%4;
        showStarParticleEffect(randomColor, bird->getPosition(), this);
    }
    else
    {
        this->gameOver();
        this->removeChild(accelerateParticle);
        accelerateParticle = NULL;
        bird->changeEffectState(EFFECT_NORMAL);
        unschedule(schedule_selector(GameLayer::addBallObstacle));
    }
    return true;
}
void GameLayer::scrollLand(float dt){
    this->landSpite1->setPositionX(this->landSpite1->getPositionX() - 2.0f);
    this->landSpite2->setPositionX(this->landSpite1->getPositionX() + this->landSpite1->getContentSize().width - 2.0f);
    this->landSpite2->setPositionX(this->landSpite2->getPositionX()-2.0f);
    if(this->landSpite2->getPositionX() == 0) {
        this->landSpite1->setPositionX(0);
        //        this->landSpite2->setPositionX(landSpite1->getContentSize().width-2.0f);
    }
    // move the pips
    for (auto singlePip : this->pips) {
        singlePip->setPositionX(singlePip->getPositionX() - pipSpeed);
        if(singlePip->getPositionX() < -PIP_WIDTH) {
            singlePip->setTag(PIP_NEW);
            Size visibleSize = Director::getInstance()->getVisibleSize();
            singlePip->setPositionX(visibleSize.width);
            singlePip->setPositionY(this->getRandomHeight());
        }
    }
}

void GameLayer::onTouch() {
    if(this->gameStatus == GAME_STATUS_OVER) {
        return;
    }
    
    SimpleAudioEngine::getInstance()->playEffect("sfx_wing.ogg");
    if(this->gameStatus == GAME_STATUS_READY) {
        this->delegator->onGameStart();
        this->bird->fly();
        
        //	ball->getPhysicsBody()->setGravityEnable(true);
        this->gameStatus = GAME_STATUS_START;
        this->createPips();
    }else if(this->gameStatus == GAME_STATUS_START) {
        this->bird->getPhysicsBody()->setVelocity(Vect(0, 260));
        
        /*
         if (!BallisTouch)
         {
         this->ball->getPhysicsBody()->setVelocity(Vect(0,260));
         }
         */
    }
}

void GameLayer::rotateBird() {
    float verticalSpeed = this->bird->getPhysicsBody()->getVelocity().y;
    this->bird->setRotation(- min(max(-90, (verticalSpeed*0.2 + 60)), 30));
}
void GameLayer::update(float delta) {
    if (this->gameStatus == GAME_STATUS_START) {
        this->rotateBird();
        //	ball->setPosition(bird->getPositionX(),bird->getPositionY()+30);
        //		ballName->setPosition(ball->getPositionX(),ball->getPositionY()+40);
        this->checkHit();
        if (bird->curEffectStatus == EFFECT_ACCELERATION) {
            BirdAccelerate();
        }
    }
}
//加速
void GameLayer::BirdAccelerate()
{
    
    if (accelerateParticle == NULL) {
        accelerateParticle = ParticleSystemQuad::create("particleImpact.plist");
        accelerateParticle->setScale(0.3f);
        accelerateParticle->setTotalParticles(20);
        addChild(accelerateParticle);
    }
    accelerateParticle->setPosition(bird->getPositionX()-20, bird->getPositionY());
    if (bird->getPhysicsBody()->getVelocity().x==0) {
        //        bird->getPhysicsBody()->setVelocity(Vec2(150, 0));
        pipSpeed = 5;
    }
    
}
void GameLayer::createPips() {
    // Create the pips
    for (int i = 0; i < PIP_COUNT; i++) {
        Size visibleSize = Director::getInstance()->getVisibleSize();
        Sprite *pipUp = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("pipe_up"));
        Sprite *pipDown = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("pipe_down"));
        Node *singlePip = Node::create();
        
        // bind to pair
        pipDown->setPosition(0, PIP_HEIGHT + PIP_DISTANCE);
        singlePip->addChild(pipDown, 0, DOWN_PIP);
        singlePip->addChild(pipUp, 0, UP_PIP);
        singlePip->setPosition(visibleSize.width + i*PIP_INTERVAL + WAIT_DISTANCE, this->getRandomHeight());
        auto body = PhysicsBody::create();
        auto shapeBoxDown = PhysicsShapeBox::create(pipDown->getContentSize(),PHYSICSSHAPE_MATERIAL_DEFAULT, Point(0, PIP_HEIGHT + PIP_DISTANCE));
        body->addShape(shapeBoxDown);
        body->addShape(PhysicsShapeBox::create(pipUp->getContentSize()));
        body->setDynamic(false);
        body->setCategoryBitmask(ColliderTypePip);
        body->setCollisionBitmask(ColliderTypeBird | ColliderTypeBall);
        body->setContactTestBitmask(ColliderTypeBird | ColliderTypeBall);
        singlePip->setPhysicsBody(body);
        singlePip->setTag(PIP_NEW);
        
        this->addChild(singlePip);
        this->pips.push_back(singlePip);
    }
}

int GameLayer::getRandomHeight() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    return rand()%(int)(2*PIP_HEIGHT + PIP_DISTANCE - visibleSize.height);
}

void GameLayer::checkHit() {
    for(auto pip : this->pips) {
        if (pip->getTag() == PIP_NEW) {
            if (pip->getPositionX() < this->bird->getPositionX()) {
                SimpleAudioEngine::getInstance()->playEffect("sfx_point.ogg");
                this->score ++;
                this->delegator->onGamePlaying(this->score);
                pip->setTag(PIP_PASS);
                switch (score) {
                    case 1:
                    {
                        /*
                         //粒子效果
                         ParticleSystem* particleSystem = ParticleRain::create();
                         particleSystem->setTexture(TextureCache::sharedTextureCache()->addImage("star.png"));
                         addChild(particleSystem);
                         */
                        //读取粒子效果
                        //                        bird->changeEffectState(EFFECT_ACCELERATION);
                        this->indicatordelegator->displayGoodWindow(score);
                    }
                        break;
                    case 2:
                    {
                        //点赞效果
                        //  showComboEffect(0, this);
                        //						accelerateParticle->setTotalParticles(81);
                    }
                        break;
                    case 3:
                    {
                        schedule(schedule_selector(GameLayer::addBallObstacle),1.0f);
                        
                    }
                        break;
                        case 4:
                    {
                        indicatordelegator->displayGoodWindow(score);

                    }
                    default:
                        break;
                }
            }
        }
    }
}
/*
 auto ball = Sprite::create("Ball.png");
 PhysicsBody *ballbody = PhysicsBody::create();
 ballbody->addShape(PhysicsShapeCircle::create(BIRD_RADIUS+5));
 
 
 ballbody->setCategoryBitmask(ColliderTypeBird);
 ballbody->setCollisionBitmask(ColliderTypeLand | ColliderTypePip);
 ballbody->setContactTestBitmask(1);
 ballbody->setDynamic(true);
 ballbody->setLinearDamping(0.0f);
 ballbody->setGravityEnable(true);
 ball->setPhysicsBody(ballbody);
 auto visibleSize = Director::getInstance()->getVisibleSize();
 ball->setPosition(bird->getPositionX()+2*20,visibleSize.height);
 addChild(ball);
 */
void GameLayer::addBallObstacle(float dt)
{
    auto ball = Sprite::create("ball.png");
    PhysicsBody *ballbody = PhysicsBody::create();
    ballbody->addShape(PhysicsShapeCircle::create(BIRD_RADIUS+5));
    
    
    ballbody->setCategoryBitmask(ColliderTypeBall);
    ballbody->setCollisionBitmask(ColliderTypeLand | ColliderTypePip | ColliderTypeBird | ColliderTypeBall);
    ballbody->setContactTestBitmask(ColliderTypeBird);
    ballbody->setDynamic(true);
    ballbody->setLinearDamping(0.0f);
    ballbody->setGravityEnable(true);
    ball->setPhysicsBody(ballbody);
    auto visibleSize = Director::getInstance()->getVisibleSize();
    ball->setPosition(bird->getPositionX()+2*20,visibleSize.height);
    addChild(ball);
}
void GameLayer::gameOver() {
    if(this->gameStatus == GAME_STATUS_OVER) {
        return;
    }
    SimpleAudioEngine::getInstance()->playEffect("sfx_hit.ogg");
    //get the best score
    int bestScore = UserRecord::getInstance()->readIntegerFromUserDefault("best_score");
    //update the best score
    if(this->score > bestScore){
        UserRecord::getInstance()->saveIntegerToUserDefault("best_score",this->score);
    }
    this->delegator->onGameEnd(this->score, bestScore);
    this->unschedule(shiftLand);
    SimpleAudioEngine::getInstance()->playEffect("sfx_die.ogg");
    this->bird->die();
    this->bird->setRotation(-90);
    this->birdSpriteFadeOut();
    this->gameStatus = GAME_STATUS_OVER;
}

void GameLayer::birdSpriteFadeOut(){
    FadeOut* animation = FadeOut::create(1.5);
    CallFunc* animationDone = CallFunc::create(bind(&GameLayer::birdSpriteRemove,this));
    Sequence* sequence = Sequence::createWithTwoActions(animation,animationDone);
    this->bird->stopAllActions();
    this->bird->runAction(sequence);
    //    this->accelerateParticle->runAction(sequence);
}

void GameLayer::birdSpriteRemove(){
    this->bird->setRotation(0);
    this->removeChild(this->bird);
}
