//
//  Layer_Store.h
//  Poker7
//
//  Created by David on 13/6/16.
//
//

#ifndef __Poker7__Layer_IOS_IAP__
#define __Poker7__Layer_IOS_IAP__

#include "cocos2d.h"
#include "CCPlatformConfig.h"
#include "Define_IAP.h"
USING_NS_CC;

#if (IAP_PLATFORM == IAP_PLATFORM_IOS)
//********************************************************
// Define區
//********************************************************

#define IAP_TYPE     Layer_IOS_IAP   // IAP類型

enum _PROCESS_STATUS
{
    IAP_Process_Status_None,                  //0
    IAP_Process_Status_Init,                  //1
    IAP_Process_Status_Loading_Info_Wait,     //2
    IAP_Process_Status_Display_Info,          //3
    IAP_Process_Status_Select_Wait,           //4
    IAP_Process_Status_Transaction_Wait,      //5
    IAP_Process_Status_Transaction_Done,      //6
    IAP_Process_Status_Exit,                  //7
    IAP_Process_Status_Error,                 //8
    IAP_Process_Status_Success                //9
};


// IAP資料
typedef struct _IN_APP_PURCHASE
{
    int Num;
    typedef struct _Info_
    {
        char Title[100];
        char SkuID[50];
        char Price[20];
        bool IsPurchase;
        
        int MoneyType; //    00$1000
        int MoneyValue;
    } _Info;
    _Info Info[MAX_IAP_NUM];
} IN_APP_PURCHASE;

//********************************************************
// Class區
//********************************************************
class Layer_IOS_IAP : public cocos2d::CCLayerColor
{
private:
    
    float Delay; // 程序Delay
    int ItemSel; // 記錄項目
    
    // 商店狀態
    int layerIosIapStatus;
    void CloseStore(void); // 關閉商店
    
public:
    
    /*----- 需要實作部分 用IAP_TYPE當作類別實作 -----*/
    virtual void EnableStoreUI(void); // 開啟商店UI
    virtual void DisableStoreUI(void); // 關閉商店UI
    virtual void EnableLoadingUI(void); // 開啟Loading動畫
    virtual void DisableLoadingUI(void); // 關閉Loading動畫
    virtual void UpDateUI(int Type, int Value); // 更新得到的虛擬物品
    
    /*----- 開啟商店 -----*/
    // IAP進入點,呼叫前務必先呼叫SetSkuID方法,否則抓不到商品資訊
    void OpenStore(void);
    
    /*----- 設定商品ID -----*/
    // 將商品ID陣列傳入,使內部設定初始化
    void SetSkuID(const char ID[MAX_IAP_NUM][50]);
    
    /*----- 購買項目 -----*/
    // 將所選擇購買的項目Index傳入
    void BuyItem(int Item); 
    
	/*----- 其他 -----*/ 
	virtual bool init();
	static cocos2d::CCScene* scene();
	void updateGame(ccTime dt);
	void Quit(void);
	void Enable_Touch(void);
	void Disable_Touch(void);
    
	LAYER_NODE_FUNC(Layer_IOS_IAP);
};

//********************************************************
// 提供外部變數
//********************************************************
// 商品ID
extern char QuerySkuID[MAX_IAP_NUM][50];
// IAP資料
extern IN_APP_PURCHASE InAppPurchase;

#endif
#endif /* defined(__Poker7__Layer_IOS_IAP__) */
