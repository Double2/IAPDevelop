#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;
#include "Define_IAP.h"

#define WorkSize_W 1280
#define WorkSize_H 720

#define TOUCH_MODE
#define fcp(x,y) ccp((x),(WorkSize_H-(y)))

#define StoreBox_W 620
#define StoreBox_H 670
#define CAL_StoreBox_X(x) ((float)(x))
#define CAL_StoreBox_Y(y) ((float)(StoreBox_H-(y)))
#define Item_W 550
#define Item_H 85
#define CAL_Item_X(x) ((float)(x))
#define CAL_Item_Y(y) ((float)(Item_H-(y)))

class HelloWorld : public cocos2d::CCLayerColor
{
private:
    CCSprite *Work; // 繪圖
    CCLabelTTF *Text; // 系統文字
    char ResourceName[291]; // StringBuffer
	float Basicx; // 基準坐標 - x
	float Basicy; // 基準坐標 - y
    int CorrectScene; // 判斷場景
    int Depth; // 圖層深度
    
public:
    // 商店介面
    CCSprite *spriteLoadingBG;  // Loading背景
    CCSprite *spriteLoading;    // Loading文字
    CCSprite *spriteStoreBG;    // 商店選單背景
    CCSprite *spriteStoreItems[MAX_IAP_NUM]; // 商品項目背景
    CCSpriteFrame *frameStoreItems[2]; // 商品項目素材
    CCLabelTTF *labelItemContent[MAX_IAP_NUM]; // 商品價錢
    CCLabelTTF *labelItemPrice[MAX_IAP_NUM]; // 商品價錢
    
    CCMenu* StoreMenu;
    
    void Enable_Touch(void);
    void Disable_Touch(void);
    virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    
    
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();
    
	// a selector callback
	virtual void menuCloseCallback(CCObject* pSender);
    // a selector callback
	virtual void StoreButtonCallback(CCObject* pSender);
    // a selector callback
	virtual void StoreExitBtnCallback(CCObject* pSender);

	// implement the "static node()" method manually
	LAYER_NODE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
