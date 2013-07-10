#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;


HelloWorld *layerStore;
// IAP
IAP_TYPE *layerIap;

float Rectx = WorkSize_W*0.50f;
float Recty = WorkSize_H*0.50f;

// --------- 商店框 ------------------
float StoreExitx = CAL_StoreBox_X(55);
float StoreExity = CAL_StoreBox_Y(55);
// 項目
float StoreItemx[MAX_IAP_NUM] = {CAL_StoreBox_X(310),CAL_StoreBox_X(310),CAL_StoreBox_X(310),CAL_StoreBox_X(310),CAL_StoreBox_X(310)};
float StoreItemy[MAX_IAP_NUM] = {CAL_StoreBox_Y(188),CAL_StoreBox_Y(288),CAL_StoreBox_Y(388),CAL_StoreBox_Y(488),CAL_StoreBox_Y(588)};
// 項目內容
float ItemContentx[MAX_IAP_NUM] = {CAL_Item_X(282),CAL_Item_X(282),CAL_Item_X(282),CAL_Item_X(282),CAL_Item_X(282)};
float ItemContenty[MAX_IAP_NUM] = {CAL_Item_Y(26),CAL_Item_Y(26),CAL_Item_Y(26),CAL_Item_Y(26),CAL_Item_Y(26)};
int ItemContentFrontSize = 35;
// 項目價錢
float ItemPricex[MAX_IAP_NUM] = {CAL_Item_X(282),CAL_Item_X(282),CAL_Item_X(282),CAL_Item_X(282),CAL_Item_X(282)};
float ItemPricey[MAX_IAP_NUM] = {CAL_Item_Y(63),CAL_Item_Y(63),CAL_Item_Y(63),CAL_Item_Y(63),CAL_Item_Y(63)};
int ItemPriceFrontSize = 35;
// 讀取中
float LoadingBGx = WorkSize_W*0.50f;
float LoadingBGy = WorkSize_H*0.50f;
float Loadingx = WorkSize_W*0.54f;
float Loadingy = WorkSize_H*0.60f;

#if (IAP_PLATFORM == IAP_PLATFORM_GOOGLE)
// Google Public Key
const char *keyd =
		"MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAlQ4YEHml67NlSYh32RC5NpwOa1+tJ7OMbHMb1omnzRiFWQROkUiBHC0n/twAl5kEuID2gc";
const char *keyb =
		"LwwNOzr3g2duobbDwDK4BnyGaWMe8Doz1Txkp7Wq7FUcAvCJ02fM53DxzSdq659zI7TpMNHXJzYvX8SYuIBnA4JClUzM5Q5o4J2eToWgRW50+SGM8c";
const char *keytwo =
		"T2hdOzaISRlbXTO1/ir0LeXdNMXkFTbRvhHj7LakA8G43D1rUgoWfLJ7PzffMkFQ/0IhoUjQS4sZxVK2ZoOJGHYoWj5HTKgTV1ECY+15r6FNCQMBpCUFiANQcImPgZ4r3xDU4QOXsRYXY9mMhRSvFYRteIep8wIDAQAB";
#endif

// 查詢商品ID
char ItemID[MAX_IAP_NUM][50] =
{
    "com.double2.iap.example.item1",
    "com.double2.iap.example.item2",
    "com.double2.iap.example.item3",
    "com.double2.iap.example.item4",
    "com.double2.iap.example.item5"
};

// *************************************************************************************************
// scene
// *************************************************************************************************
CCScene* HelloWorld::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	scene->setScaleX(CCDirector::sharedDirector()->getWinSize().width/1280.0f);
    scene->setScaleY(CCDirector::sharedDirector()->getWinSize().height/720.0f);
    scene->setAnchorPoint(ccp(0,0));
#endif
    
	// 'layer' is an autorelease object
	layerStore = HelloWorld::create();
//    layerIap = IAP_TYPE::create();

	// add layer as a child to scene
	scene->addChild(layerStore);
    scene->addChild(layerIap);

	// return the scene
	return scene;
}

// *************************************************************************************************
// Layer Init
// *************************************************************************************************
// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}
  this->setTouchEnabled(true);
  
    this->setKeypadEnabled(true);
    
    Depth = 0;
    // LayerColor 初始化
//    this->initWithColor(ccc4f(0,0,0,150));
    this->setContentSize(CCSizeMake(WorkSize_W, WorkSize_H));
    this->setVisible(true);
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Common.plist");
	CCSpriteBatchNode *spriteCommon = CCSpriteBatchNode::create("Common.png");
	spriteCommon->getTextureAtlas()->resizeCapacity(50);
	addChild(spriteCommon);
    
	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
										"CloseNormal.png",
										"CloseSelected.png",
										this,
										menu_selector(HelloWorld::menuCloseCallback) );
	pCloseItem->setPosition( ccp(WorkSize_W - 20, 20) );
	CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
	pMenu->setPosition( CCPointZero );
	this->addChild(pMenu);
    

	CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Thonburi", 50);
	pLabel->setPosition( ccp(WorkSize_W/ 2, WorkSize_H - 20) );
	this->addChild(pLabel);

    // 商店進入鈕
    CCMenuItemImage *StoreItem = CCMenuItemImage::create(
                                                   "HelloWorld.png",
                                                   "HelloWorld.png",
                                                   this,
                                                   menu_selector(HelloWorld::StoreButtonCallback));
	StoreItem->setPosition( ccp(WorkSize_W/2, WorkSize_H/2) );
	StoreMenu = CCMenu::create(StoreItem, NULL);
	StoreMenu->setPosition( CCPointZero );
	this->addChild(StoreMenu);
    
    // 商店介面
    spriteStoreBG = CCSprite::createWithSpriteFrameName("ui_shop_01.png");
    spriteStoreBG->setPosition(fcp(Rectx, Recty));
    spriteStoreBG->setVisible(false);
    this->addChild(spriteStoreBG, Depth++);
    
    
    // 商店離開鈕
    CCMenuItem *StoreExitItem = CCMenuItemSprite::create(
                                   CCSprite::createWithSpriteFrameName("ui_shop_04.png"), CCSprite::createWithSpriteFrameName("ui_shop_04.png"), this, menu_selector(HelloWorld::StoreExitBtnCallback));
    CCMenu *StoreExitMenu = CCMenu::create(StoreExitItem);
    StoreExitMenu->setPosition(ccp(StoreExitx, StoreExity));
    spriteStoreBG->addChild(StoreExitMenu);
    
    // 準備商店項目素材
    frameStoreItems[0] = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ui_shop_02.png"); // 金幣
    frameStoreItems[1] = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ui_shop_03.png"); // 籌碼
    // 商店項目
    for(int a = 0; a < MAX_IAP_NUM; a++)
    {
        // 商品項目
        spriteStoreItems[a] = CCSprite::createWithSpriteFrame(frameStoreItems[0]);
        spriteStoreItems[a]->setPosition(ccp(StoreItemx[a], StoreItemy[a]));
        spriteStoreItems[a]->setOpacity(255);
        spriteStoreItems[a]->setVisible(false);
        spriteStoreBG->addChild(spriteStoreItems[a], a);
        
        // 商品內容
        labelItemContent[a] = CCLabelTTF::create("------", "arial", ItemContentFrontSize);
        labelItemContent[a]->setColor(ccWHITE);
        labelItemContent[a]->setPosition(ccp(ItemContentx[a],ItemContenty[a]));
        spriteStoreItems[a]->addChild(labelItemContent[a]);
        
        // 商項目購買金額初始化
        labelItemPrice[a] = CCLabelTTF::create("------", "arial", ItemPriceFrontSize);
        labelItemPrice[a]->setColor(ccWHITE);
        labelItemPrice[a]->setPosition(ccp(ItemPricex[a],ItemPricey[a]));
        spriteStoreItems[a]->addChild(labelItemPrice[a]);
    }
    
    // 商店Loading背景
    spriteLoadingBG = CCSprite::createWithSpriteFrameName("loading10.png");
    spriteLoadingBG->setPosition(fcp(LoadingBGx, LoadingBGy));
    spriteLoadingBG->setVisible(false);
    spriteLoadingBG->setScale(0.3f);
    this->addChild(spriteLoadingBG, Depth++);
    // 商店Loading背景動畫
//    spriteLoadingBG->runAction(CCRepeatForever::actionWithAction(CCRotateBy::actionWithDuration(1.0f, 360.0f)));
//    
//    // 商店Loading文字(讀取中...)
//    spriteLoading = CCSprite::createWithSpriteFrameName("loading01.png");
//    spriteLoading->setPosition(fcp(Loadingx, Loadingy));
//    spriteLoading->setVisible(false);
//    this->addChild(spriteLoading, Depth++);
//    // 商店Loading文字字(讀取中...)動畫
//    CCAnimation* animation = CCAnimation::animation();
//    animation->setDelay(0.1f);
//    for(int a = 0; a < 8; a++)
//    {
//        a < 1 ?
//        sprintf(ResourceName, "loading%02d.png", 1):
//        sprintf(ResourceName, "loading%02d.png", a);
//        
//        animation->addFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(ResourceName));
//        spriteLoading->runAction(CCRepeatForever::actionWithAction(CCAnimate::actionWithAnimation(animation, true)));
//    }

	return true;
}
// *************************************************************************************************
// menuCloseCallback
// *************************************************************************************************
void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCLog("%s",__PRETTY_FUNCTION__);
    if(this->isKeypadEnabled())
	{
        CCDirector::sharedDirector()->end();

        
#if (IAP_PLATFORM == IAP_PLATFORM_IOS)
        // 移除IOS_IAP監控
        IAP_Server->Quit();
#endif
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
#endif
    }
}
// *************************************************************************************************
// StoreButtonCallback
// *************************************************************************************************
void HelloWorld::StoreButtonCallback(CCObject* pSender)
{
    CCLog("%s",__PRETTY_FUNCTION__);
    if(this->isKeypadEnabled())
    {
        StoreMenu->setTouchEnabled(false); // 關閉商店進入按鈕觸碰
#if (IAP_PLATFORM == IAP_PLATFORM_GOOGLE)
        layerIap->SetPublicKey(keyd, keyb, keytwo); // 設定金鑰
#endif
        layerIap->SetSkuID(ItemID); // 設定商品ID
        layerIap->OpenStore(); // 開啓商店
    }
}
// *************************************************************************************************
// StoreExitBtnCallback
// *************************************************************************************************
void HelloWorld::StoreExitBtnCallback(CCObject* pSender)
{
    CCLog("%s",__PRETTY_FUNCTION__);
    if(this->isKeypadEnabled())
	{
        StoreMenu->setTouchEnabled(true); // 開啟商店進入按鈕觸碰
        layerIap->DisableStoreUI();// 關閉商店選單
        layerIap->DisableLoadingUI();// 關閉Loading動畫
    }
}
// *************************************************************************************************
// 開啟觸碰
// *************************************************************************************************
void HelloWorld::Enable_Touch(void)
{
    this->setTouchEnabled(true);
    this->setKeypadEnabled(true);
}
// *************************************************************************************************
// 關閉觸碰
// *************************************************************************************************
void HelloWorld::Disable_Touch(void)
{
    this->setTouchEnabled(false);
    this->setKeypadEnabled(false);
}
// *************************************************************************************************
// 觸控﹣按下
// *************************************************************************************************
void HelloWorld::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    CCLog("%s", __PRETTY_FUNCTION__);
    if(this->isKeypadEnabled())
    {
        CCSetIterator it = pTouches->begin();
        CCTouch *touch = (CCTouch*)(*it);
        
        // 获取点在视图中的坐标（左上角为原点）
        CCPoint touchLocation = touch->getLocationInView();
        // 把点的坐标转换成OpenGL坐标（左下角为原点）
        touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
        
        for(int a = 0; a < InAppPurchase.Num; a++)
        {
            // 把OpenGL的坐標轉換成CCSprite的坐標
            CCPoint local = spriteStoreItems[a]->convertToNodeSpace(touchLocation);
            CCLOG("local x= %f y = %f",local.x,local.y);
            
            CCRect rect = spriteStoreItems[a]->getTextureRect();
            rect.origin = CCPointZero;
            
            if(rect.containsPoint(local))
            {
                layerIap->BuyItem(a); // 購買項目
                break;
            }
        }
    }
}
// *************************************************************************************************
// 開啟商店UI
// *************************************************************************************************
void IAP_TYPE::EnableStoreUI(void)
{
    // 顯示商店選單
    layerStore->spriteStoreBG->setVisible(true);
    
    for(int a = 0; a < InAppPurchase.Num; a++)
    {
        char TempString[291];
        
        // 顯示價格
        layerStore->labelItemPrice[a]->setString(InAppPurchase.Info[a].Price);
        
        // 顯示商品項目
        switch(InAppPurchase.Info[a].MoneyType)
        {
                // 虛擬物品代碼 - 金幣
            case 00:
            {
                // 顯示產品內容
                sprintf(TempString, "%d 金幣", InAppPurchase.Info[a].MoneyValue);
                layerStore->labelItemContent[a]->setString(TempString);
                layerStore->spriteStoreItems[a]->setDisplayFrame(layerStore->frameStoreItems[0]);
            }
                break;
                // 虛擬物品代碼 - 籌碼
            case 01:
            {
                // 顯示產品內容
                sprintf(TempString, "%d 籌碼", InAppPurchase.Info[a].MoneyValue);
                layerStore->labelItemContent[a]->setString(TempString);
                layerStore->spriteStoreItems[a]->setDisplayFrame(layerStore->frameStoreItems[1]);
            }
                break;
            default:
                break;
        }
        
        layerStore->spriteStoreItems[a]->setVisible(true);
        
    }
}

// *************************************************************************************************
// 關閉商店UI
// *************************************************************************************************
void IAP_TYPE::DisableStoreUI(void)
{
    // 顯示商店選單
    layerStore->spriteStoreBG->setVisible(false);
}
// *************************************************************************************************
// 開啟Loading動畫
// *************************************************************************************************
void IAP_TYPE::EnableLoadingUI(void)
{
    // 關閉觸碰
    layerStore->Disable_Touch();
    
    layerStore->spriteLoadingBG->setVisible(true);
    layerStore->spriteLoading->setVisible(true);
    
}
// *************************************************************************************************
// 關閉Loading動畫
// *************************************************************************************************
void IAP_TYPE::DisableLoadingUI(void)
{
    // 啓動觸碰
    layerStore->Enable_Touch();
    
    layerStore->spriteLoadingBG->setVisible(false);
    layerStore->spriteLoading->setVisible(false);
    
}

// *************************************************************************************************
// 更新得到虛擬物品
// *************************************************************************************************
void IAP_TYPE::UpDateUI(int Type, int Value)
{
//    // 音效 - 加錢
//    MySound::PlayWave("fruit_money", "wave", Record.Sound);
    
    // 更新虛擬物品
    switch(Type)
    {
            // 虛擬物品代碼 - 金幣
        case 00:
        {
//            Record.Gold += InAppPurchase.Info[0].MoneyValue;
//            PlayerData[0].G_money = Record.Gold;
//            // 更新金幣
//            Game_Preferences::Update_Gold();
        }
            break;
            // 虛擬物品代碼 - 籌碼
        case 01:
        {
            
        }
            break;
        default:
            break;
    }
}
