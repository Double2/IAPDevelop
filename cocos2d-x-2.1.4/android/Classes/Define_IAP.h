/*
 * Define_IAP.h
 *
 *  Created on: 2013/6/28
 *      Author: double2
 */

#ifndef DEFINE_IAP_H_
#define DEFINE_IAP_H_

#include "cocos2d.h"
#define MAX_IAP_NUM 10

#define IAP_PLATFORM_IOS 0
#define IAP_PLATFORM_GOOGLE 1
#define IAP_PLATFORM_DEAK 2


#define IAP_PLATFORM 1

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#undef IAP_PLATFORM
#define IAP_PLATFORM IAP_PLATFORM_IOS
#endif


#if (IAP_PLATFORM == IAP_PLATFORM_GOOGLE)
#include "Layer_GoogleIAB.h"
#define IAP_TYPE Layer_GoogleIAB
#elif (IAP_PLATFORM == IAP_PLATFORM_IOS)
#include "Layer_IOS_IAP.h"
#include "IOS_IAP_Server.h"
#define IAP_TYPE Layer_IOS_IAP

#endif

#endif /* DEFINE_IAP_H_ */
