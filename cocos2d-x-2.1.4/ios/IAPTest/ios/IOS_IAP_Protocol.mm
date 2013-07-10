//
//  IOS_IAP_Protocol.m
//  Machang
//
//  Created by atwood on 2011/10/8.
//  Copyright 2011年 __Double2__. All rights reserved.
//

#import "IOS_IAP_Protocol.h"
#import "Internet.h"

#if (IAP_PLATFORM == IAP_PLATFORM_IOS)
@implementation IOS_IAP_Protocol

@synthesize ItemNo;

// **************************************************************************************************************
// 購買IAP項目
// **************************************************************************************************************
-(void) CreatPayment:(int)ItemIndex
{
    IAP_Server->IAP_STATUS = IAP_STATUS_WORKING;
    
    ItemNo = ItemIndex;
    
    NSLog(@"-------購買物品-------");
    NSLog(@"ItemNo=%d",ItemNo);
    
    if(ItemNo < InAppPurchase.Num)
    {
        NSString *ProductID = [NSString stringWithUTF8String:InAppPurchase.Info[ItemIndex].SkuID];

        SKPayment *payment = [SKPayment paymentWithProductIdentifier:ProductID];
        [[SKPaymentQueue defaultQueue] addPayment:payment];
        
    }else
    {
        UIAlertView *alerView =  [[UIAlertView alloc]
                                  initWithTitle:@"錯誤"
                                  message:@"查無此項商品" delegate:nil
                                  cancelButtonTitle:NSLocalizedString(@"確定",nil) otherButtonTitles:nil];
        
        [alerView show];
        [alerView release];

        IAP_Server->IAP_STATUS = IAP_STATUS_ERROR;
    }
}
// **************************************************************************************************************
// 警告訊息
// **************************************************************************************************************
-(void) AlertMessage:(int)ErrorCode
{
    switch (ErrorCode)
    {
        case IAP_ERROR_CODE_TIMEOUT:
        {
            UIAlertView *alerView =  [[UIAlertView alloc]
                                      initWithTitle:@"錯誤"
                                      message:@"連線逾時" delegate:nil
                                      cancelButtonTitle:NSLocalizedString(@"確定",nil) otherButtonTitles:nil];
            
            [alerView show];
            [alerView release];
        }
            break;
        default:
            break;
    }
    
}
#pragma mark -----------ProductData-------------

// **************************************************************************************************************
// 檢查IPA物品是否存在
// **************************************************************************************************************
-(void) requestProductData
{
    IAP_Server->IAP_STATUS = IAP_STATUS_WORKING;
    
    NSLog(@"-------要求商品資訊-------");
    NSLog(@"檢查IAP購買許可...");
          
    if ([SKPaymentQueue canMakePayments])
    {
        NSLog(@"允許IAP購買");
        
        //Create a list of product identifiers
        NSMutableArray *ProductIDs = [NSMutableArray arrayWithCapacity:MAX_IAP_NUM];
        for(int a = 0; a < MAX_IAP_NUM; a++)
            [ProductIDs addObject:[NSString stringWithUTF8String:QuerySkuID[a]]];
        NSSet *productIdentifiers =[NSSet setWithArray:ProductIDs];
        
        //Create and initialize a products request object with the above list
        SKProductsRequest *request = [[SKProductsRequest alloc] initWithProductIdentifiers: productIdentifiers]; 
        
        //Attach the request to your delegate
        request.delegate = self;
        
        //Send the request to the App Store
        [request start];
        
    }else
    {
        UIAlertView *alerView =  [[UIAlertView alloc]
                                  initWithTitle:@"錯誤"
                                  message:@"未開啟IAP購買許可" delegate:nil
                                  cancelButtonTitle:NSLocalizedString(@"確定",nil) otherButtonTitles:nil];

        [alerView show];
        [alerView release];
        
        IAP_Server->IAP_STATUS = IAP_STATUS_ERROR;
    }
}

// **************************************************************************************************************
// 回傳 - 檢查物品是否存在
// **************************************************************************************************************
-(void) productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
    IAP_Server->IAP_STATUS = IAP_STATUS_WORKING;
    
    NSLog(@"-------回應商品資訊-------");
    
    NSArray *myProduct = response.products;
    
    InAppPurchase.Num = [myProduct count];
    if([myProduct count] != 0)
    {
        // populate UI
        for(int i=0;i<[myProduct count];i++)
        {
            SKProduct *product = [myProduct objectAtIndex:i];
            NSLog(@"Name: %@ - Price: %f",[product localizedTitle],[[product price] doubleValue]);
            NSLog(@"Product identifier: %@",[product productIdentifier]);
            NSLog(@"Product description: %@" , product.localizedDescription);
            // 轉換貨幣
            NSNumberFormatter *priceFormatter = [[NSNumberFormatter alloc] init];
            [priceFormatter setNumberStyle:NSNumberFormatterCurrencyStyle];
            [priceFormatter setLocale:product.priceLocale];
            NSString *CurrencyString = [priceFormatter stringFromNumber:product.price];
            
            if (i <= MAX_IAP_NUM)
            {
                strcpy(InAppPurchase.Info[i].Title, [[product localizedTitle] UTF8String]);
                strcpy(InAppPurchase.Info[i].SkuID, [[product productIdentifier] UTF8String]);
                strcpy(InAppPurchase.Info[i].Price, [CurrencyString UTF8String]);
                
                NSCharacterSet *charSet = [NSCharacterSet characterSetWithCharactersInString:@"$#"];
                NSArray *stringSet = [product.localizedDescription componentsSeparatedByCharactersInSet:charSet];
                
                InAppPurchase.Info[i].MoneyType = [[stringSet objectAtIndex:0] intValue];
                InAppPurchase.Info[i].MoneyValue = [[stringSet objectAtIndex:1] intValue];
            }
            else
                NSLog(@"商品資訊[%d]已超過 MAX_IAP_NUM 定義數量", i+1);
            
            [priceFormatter release];
        }
        
        IAP_Server->IAP_STATUS = IAP_STATUS_SUCCESS;
    }else
    {
        NSArray *invalidProudcts = response.invalidProductIdentifiers;
        if ([invalidProudcts count] != 0) 
        {
            NSLog(@"不應該出現的錯誤");
            NSLog(@"物品不存在");
            
            // 先取消購買選項
            // 顯示錯誤訊息
            UIAlertView *alertView = [[UIAlertView alloc]
                                      initWithTitle:@"錯誤"
                                      message:@"連線發生問題,請稍候再試..." delegate:self
                                      cancelButtonTitle:@"OK" otherButtonTitles:nil];
            [alertView show];
            [alertView release];
            
            
            IAP_Server->IAP_STATUS = IAP_STATUS_ERROR;
        }
    }
    //[request autorelease];
    [request release];
}

#pragma mark -----------MyObserver-------------

// **************************************************************************************************************
// MyObserver初始化
// **************************************************************************************************************
- (void) ObserverInit
{
    IAP_Server->IAP_STATUS = IAP_STATUS_WORKING;
    
    NSLog(@"-------[ObserverInit]-------");
    
    // 建立IAP交易序列監控
    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
    
}
// **************************************************************************************************************
// MyObserver離開
// **************************************************************************************************************
- (void) ObserverExit
{
    IAP_Server->IAP_STATUS = IAP_STATUS_WORKING;
    
    NSLog(@"-------[ObserverExit]-------");
    
    // 移除IAP交易序列監控
    [[SKPaymentQueue defaultQueue] removeTransactionObserver:self];
}
// **************************************************************************************************************
// 交易序列回應
// **************************************************************************************************************
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    IAP_Server->IAP_STATUS = IAP_STATUS_WORKING;
    
    for (SKPaymentTransaction *transaction in transactions)
    {
        switch (transaction.transactionState)
        {
            case SKPaymentTransactionStatePurchasing:
            {
                NSLog(@"-------交易進行中...-------");
            }
                break;
            case SKPaymentTransactionStatePurchased:
            {
                [self completeTransaction:transaction];
            }
                break;
            case SKPaymentTransactionStateFailed:
            {
                [self failedTransaction:transaction];
            }
                break;
            case SKPaymentTransactionStateRestored:
            {
                [self restoreTransaction:transaction];
            }
                break;
            default: break;
        }
    }
}

// **************************************************************************************************************
// 交易成功
// **************************************************************************************************************
- (void) completeTransaction: (SKPaymentTransaction *)transaction
{

        NSLog(@"-------交易成功-------");
        NSLog(@"選擇方案%d",ItemNo+1);
        NSLog(@"Product ID: %@", transaction.payment.productIdentifier);
        NSLog(@"%@", transaction.transactionReceipt); // 交易收據
//        NSLog(@"交易前金額: %d",Record.Gold);    
//        NSLog(@"交易後金額: %d",Record.Gold);

        NSString *Message = [NSString stringWithFormat:@"金幣增加$%d", InAppPurchase.Info[ItemNo].MoneyValue];
        // 顯示購買成功訊息
        UIAlertView *alertView = [[UIAlertView alloc]
                                  initWithTitle:@"購買成功"
                                  message:Message delegate:self
                                  cancelButtonTitle:@"確定" otherButtonTitles:nil];
        [alertView show];
        [alertView release];
    
        [[SKPaymentQueue defaultQueue] finishTransaction: transaction]; // 交易結束
    
    
        IAP_Server->IAP_STATUS = IAP_STATUS_SUCCESS;
}

// **************************************************************************************************************
// 交易失敗
// **************************************************************************************************************
- (void) failedTransaction: (SKPaymentTransaction *)transaction
{
    NSLog(@"-------交易失敗-------");
    NSString *Title = [[NSString alloc] init];
    
    // 檢查錯誤碼
    if(transaction.error.code == SKErrorUnknown)
    {
        NSLog(@">>> SKErrorUnknown");
        NSLog(@"Unknown Error (%d), product: %@", (int)transaction.error.code, transaction.payment.productIdentifier);
        Title = [NSString stringWithFormat:@"未知錯誤"];
    }
    else if(transaction.error.code == SKErrorClientInvalid)
    {
        NSLog(@">>> SKErrorClientInvalid");
        Title = [NSString stringWithFormat:@"用戶無效"];
    }
    else if(transaction.error.code == SKErrorPaymentCancelled)
    {
        NSLog(@">>> SKErrorPaymentCancelled");
    }
    else if(transaction.error.code == SKErrorPaymentInvalid)
    {
        NSLog(@">>> SKErrorPaymentInvalid");
        Title = [NSString stringWithFormat:@"交易無效"];
    }
    else if(transaction.error.code == SKErrorPaymentNotAllowed)
    {
        NSLog(@">>> SKErrorPaymentNotAllowed");
        Title = [NSString stringWithFormat:@"交易被拒"];
    }
    else
    {
        NSLog(@"Unknowed Error....transaction.error.code=%d", transaction.error.code);
        Title = [NSString stringWithFormat:@"未知錯誤"];
    }
    
    // 發生錯誤
    if (transaction.error.code != SKErrorPaymentCancelled)
    {
        // 顯示錯誤訊息
        UIAlertView *alertView = [[UIAlertView alloc]
                                  initWithTitle:Title
                                  message:@"已取消本次交易,請稍候再試..." delegate:self
                                  cancelButtonTitle:@"確定" otherButtonTitles:nil];
        [alertView show];
        [alertView release];
        
    }
    // 取消交易 (不用顯示錯誤)
    else
    {

    }
    
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction]; // 交易結束
    IAP_Server->IAP_STATUS = IAP_STATUS_ERROR;
    
    [Title release];
    
}

// **************************************************************************************************************
// 重複購買選項 (無作用中)
// **************************************************************************************************************
-(void) restoreTransaction: (SKPaymentTransaction *)transaction
{
//    if(IAPTradeFlag==0)
//    {
        NSLog(@"未購買成功,重新給玩家金幣 選擇方案%d",ItemNo);
//        IAPTradeFlag = 1;
        
#ifdef LOG_RECORD_MODE
        NSLog(@"交易成功 選擇方案%d",ItemNo+1);
        NSLog(@"%@", transaction.transactionReceipt);
        NSLog(@"%@", transaction.payment.productIdentifier);
        NSLog(@"交易前金額 %d",Record.Gold);
#endif
        //        switch(ItemNo)
        //        {
        //            case 0:  Record.Gold+=200;   break;  // 200
        //        }
#ifdef LOG_RECORD_MODE
        NSLog(@"交易後金額 %d",Record.Gold);
#endif
    
        // 顯示重複交易訊息
        UIAlertView *alertView = [[UIAlertView alloc]
                                  initWithTitle:@"重複交易"
                                  message:@"已取消本次交易,請稍候再試..." delegate:self
                                  cancelButtonTitle:@"確定" otherButtonTitles:nil];
        [alertView show];
        [alertView release];
    
        [[SKPaymentQueue defaultQueue] finishTransaction: transaction]; // 交易結束
    
        IAP_Server->IAP_STATUS = IAP_STATUS_ERROR;
//    }
}

// **************************************************************************************************************
// 交易記錄 (無作用中)
// **************************************************************************************************************
- (void)recordTransaction:(SKPaymentTransaction *)transaction
{
    NSLog(@"recordTransaction");
}

#endif
@end
