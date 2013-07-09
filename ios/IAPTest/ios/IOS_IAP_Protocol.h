//
//  IOS_IAP_Protocol.h
//  Machang
//
//  Created by atwood on 2011/10/8.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "UIKit/UIKit.h"
#import "StoreKit/StoreKit.h"
#include "IOS_IAP_Server.h"

#if (IAP_PLATFORM == IAP_PLATFORM_IOS)

//********************************************************
// Define區
//********************************************************

//********************************************************
// Interface 區
//********************************************************
@interface IOS_IAP_Protocol : NSObject  <SKPaymentTransactionObserver, SKProductsRequestDelegate>
{  

}

@property int ItemNo;
@property(nonatomic, retain) SKProduct *product;

// 其他 *************
- (void) CreatPayment:(int)ItemIndex;// IAP 購買選項
- (void) AlertMessage:(int)ErrorCode;// 警告

// 商品資訊 *************
- (void) requestProductData; // 要求商品資訊
- (void) productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response; // 回應商品資訊

// 交易序列監控 *************
- (void) ObserverInit; // MyObserver初始化
- (void) ObserverExit; // MyObserver離開
- (void) paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions; // 交易序列回應
- (void) completeTransaction: (SKPaymentTransaction *)transaction; // 交易成功
- (void) failedTransaction: (SKPaymentTransaction *)transaction; // 交易失敗
- (void) restoreTransaction: (SKPaymentTransaction *)transaction; // 重複購買選項 (無作用中)
- (void) recordTransaction:(SKPaymentTransaction *)transaction; // 交易記錄 (無作用中)

#endif
@end
