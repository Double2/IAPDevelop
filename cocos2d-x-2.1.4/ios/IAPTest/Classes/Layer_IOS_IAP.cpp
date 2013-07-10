//
//  Layer_Store.cpp
//  Poker7
//
//  Created by David on 13/6/16.
//
//

#include "Layer_IOS_IAP.h"
#include "IOS_IAP_Server.h"

#if (IAP_PLATFORM == IAP_PLATFORM_IOS)
//********************************************************
// 提供外部變數
//********************************************************

// 查詢商品ID
char QuerySkuID[MAX_IAP_NUM][50];

// IAP資料結構
IN_APP_PURCHASE InAppPurchase;

//********************************************************
// 自用 Global 變數
//********************************************************
// Loading 延遲時間上限
float DelayTime = 30.0f;

// *************************************************************************************************
// Layer 初始化
// *************************************************************************************************
bool Layer_IOS_IAP::init()
{
    
#ifdef TRACE_MODE
	CCLog("%s",__PRETTY_FUNCTION__);
#endif
    
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}
    
	// 設定為不可觸控
	this->setIsTouchEnabled(false);
	this->setIsKeypadEnabled(false);
    
    // LayerColor 初始化
    this->initWithColor(ccc4f(0,0,0,0));
    this->setIsVisible(true);
    
    // 程序狀態初始化
    layerIosIapStatus = IAP_Process_Status_None;
    
     // 主程序
	this->schedule(schedule_selector(Layer_IOS_IAP::updateGame));
    
	return true;
}
// *************************************************************************************************
// Layer 主程序
// *************************************************************************************************
void Layer_IOS_IAP::updateGame(ccTime dt)
{
    Delay += dt;
    
    switch (layerIosIapStatus)
    {
        // 啓動IAP
        case IAP_Process_Status_Init:
            {
                EnableLoadingUI(); // 開啟Loading動畫
                
                if(IAP_Server->EnableIAP())
                    layerIosIapStatus++;
                else
                    layerIosIapStatus = IAP_Process_Status_Exit;
            }
            break;
        // 等待商品資訊
        case IAP_Process_Status_Loading_Info_Wait:
            {
                
                if(IAP_Server->IAP_STATUS == IAP_STATUS_SUCCESS)
                {
                    layerIosIapStatus++;
                }else if(Delay >= DelayTime ||  IAP_Server->IAP_STATUS == IAP_STATUS_ERROR)
                {
                    DisableLoadingUI(); // 關閉Loading動畫
                    
                    EnableStoreUI(); // 開啟商店UI
                    
                    // 連線逾時警告
                    if(Delay >= DelayTime)
                    {
//                        IAP_Server->ErrorMessage(IAP_ERROR_CODE_TIMEOUT);
                    }
                    
                    layerIosIapStatus = IAP_Process_Status_Exit;
                }
            }
            break;
        // 顯示商品資訊
        case IAP_Process_Status_Display_Info:
            {
                DisableLoadingUI(); // 關閉Loading動畫

                EnableStoreUI(); // 開啟商店UI
                layerIosIapStatus++;
            }
            break;
            
        // 遊戲內購買選單 (IAP) - 等待玩家選擇
        case IAP_Process_Status_Select_Wait:
            {
                // Do nothing!
            }
            break;
            
        // 等待IAP回應
        case IAP_Process_Status_Transaction_Wait:
        {
            
            if(IAP_Server->IAP_STATUS == IAP_STATUS_SUCCESS)
            {
                layerIosIapStatus++;
            }else if(Delay >= DelayTime || IAP_Server->IAP_STATUS == IAP_STATUS_ERROR)
            {
                // 連線逾時警告
                if(Delay >= DelayTime)
                {
//                    IAP_Server->ErrorMessage(IAP_ERROR_CODE_TIMEOUT);
                }
                
                layerIosIapStatus = IAP_Process_Status_Exit;
            }
        }
            break;
        // 完成購買
        case IAP_Process_Status_Transaction_Done:
            {                                   
                DisableLoadingUI(); // 關閉Loading動畫
                
                // 更新得到虛擬物品
                UpDateUI(InAppPurchase.Info[ItemSel].MoneyType, InAppPurchase.Info[ItemSel].MoneyValue); 
                
                layerIosIapStatus++;
            }
            break;
        // 離開 - 遊戲內購買選單 (IAP)
        case IAP_Process_Status_Exit:
            {
                CloseStore();
            }
            break;

        default:
            break;
    }
}
//******************************************************************************************************
// Layer 結束
//******************************************************************************************************
void Layer_IOS_IAP::Quit(void)
{
	
	CCLog("%s",__PRETTY_FUNCTION__);
    
	this->removeAllChildrenWithCleanup(true);
	
}
// *************************************************************************************************
// 設定為可觸控
// *************************************************************************************************
void Layer_IOS_IAP::Enable_Touch(void)
{
	this->setIsTouchEnabled(true);
	this->setIsKeypadEnabled(true);
}

// *************************************************************************************************
// 設定為可觸控
// *************************************************************************************************
void Layer_IOS_IAP::Disable_Touch(void)
{
	this->setIsTouchEnabled(false);
	this->setIsKeypadEnabled(false);
}

//***************************************************************************************************************
// 開啓商店
//***************************************************************************************************************
void Layer_IOS_IAP::OpenStore(void)
{
    CCLog("%s",__PRETTY_FUNCTION__);
    
    
    Delay = 0.0f;
    // 開始進入程序
    layerIosIapStatus = IAP_Process_Status_Init;
}
//***************************************************************************************************************
// 關閉商店
//***************************************************************************************************************
void Layer_IOS_IAP::CloseStore(void)
{
    CCLog("%s",__PRETTY_FUNCTION__);
    
    DisableLoadingUI();// 關閉Loading動畫
    
    // 程序結束
    if(IAP_Server->IAP_STATUS == IAP_STATUS_SUCCESS)
        layerIosIapStatus = IAP_Process_Status_Success;
    else
        layerIosIapStatus = IAP_Process_Status_Error;
    
    // 關閉IAP
    IAP_Server->DisableIAP();
}

// *************************************************************************************************
// 設定商品ID
// *************************************************************************************************
void Layer_IOS_IAP::SetSkuID(const char ID[MAX_IAP_NUM][50])
{
    for(int a = 0; a < MAX_IAP_NUM; a++)
        strcpy(QuerySkuID[a], ID[a]);
}
// *************************************************************************************************
// 購買項目
// *************************************************************************************************
void Layer_IOS_IAP::BuyItem(int Item)
{
    EnableLoadingUI(); // 開啟Loading動畫
    
    ItemSel = Item; // 記錄所選項目
    IAP_Server->BuyItem(Item); // 購買項目
    
    Delay = 0.0f;
    layerIosIapStatus = IAP_Process_Status_Transaction_Wait;
}

#endif
