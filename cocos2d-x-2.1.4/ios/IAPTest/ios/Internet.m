//
//  Internet.m
//  Poker7
//
//  Created by David on 13/6/25.
//
//

#import "Internet.h"
#import <SystemConfiguration/SystemConfiguration.h>
#import <netinet/in.h>

@implementation Internet

// 檢查是否有連接 Internet
+(BOOL) checkConnection :(BOOL) ShowErrorFlag
{
	BOOL connectionFlag = FALSE;
	
    struct sockaddr_in Addr;
    bzero(&Addr, sizeof(Addr));
    Addr.sin_len    = sizeof(Addr);
    Addr.sin_family = AF_INET;
    
    SCNetworkReachabilityRef target = SCNetworkReachabilityCreateWithAddress(NULL, (struct sockaddr *)&Addr);
    SCNetworkReachabilityFlags flags;
    SCNetworkReachabilityGetFlags(target, &flags);
    
    if(flags & kSCNetworkFlagsReachable)
    {
//#ifdef DEBUG_MODE
        NSLog(@"-------網路:正常連線-------");
//#endif
		// 目前連線中
		connectionFlag = TRUE;
    }
    else
    {
//#ifdef DEBUG_MODE
        NSLog(@"-------網路:無法連線-------");
//#endif
        
        // 如果要顯示錯誤訊息
		if(ShowErrorFlag==TRUE)
		{
			// 顯示錯誤訊息
			UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"網路錯誤" message:@"無法連接網路" delegate:self cancelButtonTitle:@"確定" otherButtonTitles:nil];
			[alertView show];
			[alertView release];
		}
    }
    
    return connectionFlag;
}
/*
 // 檢查是否有連接 Internet
 +(BOOL) checkConnection :(BOOL) ShowErrorFlag
 {
 BOOL connectionFlag = FALSE;
 
 NSError *error = nil;
 // 先檢查網路是否連線
 NSString *connectionString = [[NSString alloc] initWithContentsOfURL:[NSURL URLWithString:@"http://www.google.com"] encoding:NSUTF8StringEncoding error:&error];
 
 // 沒有連線
 if([connectionString length ] == 0)
 {
 // 如果要顯示錯誤訊息
 if(ShowErrorFlag==TRUE)
 {
 // 顯示錯誤訊息
 UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"Error" message:@"You do not appear to connected the internet" delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil];
 [alertView show];
 [alertView release];
 }
 }
 else
 {
 #ifdef DEBUG_MODE
 NSLog(@"目前連線中");
 #endif
 // 目前連線中
 connectionFlag = TRUE;
 }
 
 [connectionString release];
 
 return connectionFlag;
 }
 */

@end

