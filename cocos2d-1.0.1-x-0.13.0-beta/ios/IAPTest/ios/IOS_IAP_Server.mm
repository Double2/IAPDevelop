//
//  IOS_IAP_Server.cpp
//  Poker7
//
//  Created by David on 13/6/24.
//
//
#import <Foundation/Foundation.h>
#import "Internet.h"

#import "IOS_IAP_Protocol.h"
#include "IOS_IAP_Server.h"

#if (IAP_PLATFORM == IAP_PLATFORM_IOS)
//********************************************************
// 提供外部變數
//********************************************************
// IAP服務
IOS_IAP_Server *IAP_Server;
//********************************************************
// 自用 Global 變數
//********************************************************
// IAP協定
IOS_IAP_Protocol *IAP_Protocol;
// *************************************************************************************************
// 啓動IAP服務
// *************************************************************************************************
bool IOS_IAP_Server::EnableIAP(void)
{
	CCLog("%s",__PRETTY_FUNCTION__);
    
    // 先檢查網路連線
    if([Internet checkConnection:TRUE])
    {
        RequestProductInfo(); // 要求商品資訊
        return true;
    }else
        return false;
}

// *************************************************************************************************
// 關閉IAP服務
// *************************************************************************************************
void IOS_IAP_Server::DisableIAP(void)
{
	CCLog("%s",__PRETTY_FUNCTION__);
}

// *************************************************************************************************
// IAP服務初始化
// *************************************************************************************************
void IOS_IAP_Server::Init(void)
{
	CCLog("%s",__PRETTY_FUNCTION__);

    IAP_Server = new IOS_IAP_Server;
    
    IAP_Protocol = [[IOS_IAP_Protocol alloc] init]; // IAP Protocol初始化
    [IAP_Protocol ObserverInit]; // 建立IAP監控
}
// *************************************************************************************************
// IAP服務結束
// *************************************************************************************************
void IOS_IAP_Server::Quit(void)
{
	CCLog("%s",__PRETTY_FUNCTION__);

    IAP_Server->release();
    [IAP_Protocol ObserverExit]; // 移除IAP監控
}

// *************************************************************************************************
// 購買IAP項目
// *************************************************************************************************
void IOS_IAP_Server::BuyItem(int ItemIndex)
{
	CCLog("%s",__PRETTY_FUNCTION__);
    
    // 購買IAP項目
    [IAP_Protocol CreatPayment:ItemIndex];
}

// *************************************************************************************************
// 要求商品資訊
// *************************************************************************************************
void IOS_IAP_Server::RequestProductInfo(void)
{
	CCLog("%s",__PRETTY_FUNCTION__);
    
    // 要求商品資訊
    [IAP_Protocol requestProductData];
    
}
// **************************************************************************************************************
// 警告訊息
// **************************************************************************************************************
void IOS_IAP_Server::ErrorMessage(int ErrorCode)
{
    CCLog("%s",__PRETTY_FUNCTION__);
    
    // 要求商品資訊
    [IAP_Protocol AlertMessage:IAP_ERROR_CODE_TIMEOUT];
}
#endif