//
//  IOS_IAP_Server.h
//  Poker7
//
//  Created by David on 13/6/24.
//
//

#ifndef __Poker7__IOS_IAP_SERVER__
#define __Poker7__IOS_IAP_SERVER__

#include "cocos2d.h"
#include "Define_IAP.h"
USING_NS_CC;

#if (IAP_PLATFORM == IAP_PLATFORM_IOS)
#include "Layer_IOS_IAP.h"

//********************************************************
// Define區
//********************************************************
// IAP Protocol內部運作狀態
enum _IAP_STATUS_ENUM
{
    IAP_STATUS_WORKING, // 工作中
    IAP_STATUS_ERROR,   // 錯誤
    IAP_STATUS_SUCCESS  // 成功
};
// IAP Protocol內部ErrorCode
enum _IAP_ERROR_CODE
{
    IAP_ERROR_CODE_TIMEOUT //
};
//********************************************************
// Class區
//********************************************************
// IAP服務類別
class IOS_IAP_Server : CCNode
{
private:

    
public:
    
    int IAP_STATUS; // IAP Protocol內部運作狀態
    
    void Init(void); // IAP服務初始化
    void Quit(void); // IAP服務結束
    
    bool EnableIAP(void); // 啓動IAP服務
    void DisableIAP(void); // 關閉IAP服務
    
    // 要求商品資訊
    void RequestProductInfo(void);
    // IAP 購買選項
    void BuyItem(int ItemIndex);
    // 警告訊息
    void ErrorMessage(int ErrorCode);
};

//********************************************************
// 提供外部變數
//********************************************************
// IAP服務
extern IOS_IAP_Server *IAP_Server;

#endif
#endif /* defined(__Poker7__IOS_IAP_SERVER__) */
