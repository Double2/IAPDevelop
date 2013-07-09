/*
 * Google_IAB.cpp
 *
 *  Created on: 2013/6/11
 *      Author: double2
 *
 *      Version 1.00000
 */

#include "Google_IAB.h"
#include "cocos2d.h" 
#include "Define_IAP.h"
#if (IAP_PLATFORM == IAP_PLATFORM_GOOGLE)
#include "platform/android/jni/JniHelper.h"
using namespace cocos2d;
bool Google_IAB::Google_IAB_CallBackFunc = false;
bool Google_IAB::Google_IAB_SetSkuID = false;
bool Google_IAB::Google_IAB_SetupUp = false;
bool Google_IAB::Google_IAB_Inventory = false;
bool Google_IAB::Google_IAB_Init = false;
int Google_IAB::code = Func_Sucess;
int Google_IAB::IAB_Status = IAB_status_Complete;

IN_APP_PURCHASE InAppPurchase;


// ************************************************************************************************
// step:1 設定CallBackFunc
// ************************************************************************************************
void Google_IAB::CallBackFunc(actionFinished callback)
{
	CCLog("%s", __PRETTY_FUNCTION__);

	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, IABCLASS_PATH, "CallBackFunc", "(J)V"))
	{
		jlong arg = (long) (void*) callback;
		t.env->CallStaticVoidMethod(t.classID, t.methodID, arg);
		Google_IAB_CallBackFunc = true;
		t.env->DeleteLocalRef(t.classID);
	}
	else
	{
		code = C_Call_Jave___JavaFunction_Cannot_Find;
		Google_IAB::IAB_Status = IAB_status_Error;
		return;
	}
}
// ************************************************************************************************
// step:2 設定購買商品ID
// ************************************************************************************************
void Google_IAB::SetSkuID(const char* SKU_ID)
{
	CCLog("%s", __PRETTY_FUNCTION__);

	if (SKU_ID == NULL || !SKU_ID[0])
	{
		code = Error__SkuID_is_null;
		Google_IAB::IAB_Status = IAB_status_Error;
		return;
	}

	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, IABCLASS_PATH, "SetSkuID", "(Ljava/lang/String;)V"))
	{
		jstring arg = t.env->NewStringUTF(SKU_ID);
		t.env->CallStaticVoidMethod(t.classID, t.methodID, arg);
		Google_IAB_SetSkuID = true;
		t.env->DeleteLocalRef(arg);
		t.env->DeleteLocalRef(t.classID);
	}
	else
	{
		code = C_Call_Jave___JavaFunction_Cannot_Find;
		Google_IAB::IAB_Status = IAB_status_Error;
		return;
	}
}

// ************************************************************************************************
// step:3 配置c/java環境 參數
// ************************************************************************************************
void Google_IAB::Init(const char* PublicKeyd, const char* PublicKeyb, const char* PublicKeytwo)
{
	CCLog("%s", __PRETTY_FUNCTION__);
	if (PublicKeyd == NULL || PublicKeyb == NULL || PublicKeytwo == NULL || !PublicKeyd[0] || !PublicKeyb[0]
			|| !PublicKeytwo[0])
	{
		code = Error__PublicKey_is_null;
		Google_IAB::IAB_Status = IAB_status_Error;
		return;
	}
	if (Google_IAB_CallBackFunc && Google_IAB_SetSkuID)

	{
		JniMethodInfo t;
		if (JniHelper::getStaticMethodInfo(
				t,
				IABCLASS_PATH,
				"Init",
				"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I"))
		{
			jstring argx = t.env->NewStringUTF(PublicKeyd);
			jstring argy = t.env->NewStringUTF(PublicKeyb);
			jstring argz = t.env->NewStringUTF(PublicKeytwo);
			jint jret = t.env->CallStaticIntMethod(t.classID, t.methodID, argx, argy, argz);
			t.env->DeleteLocalRef(argx);
			t.env->DeleteLocalRef(argy);
			t.env->DeleteLocalRef(argz);
			t.env->DeleteLocalRef(t.classID);
			code = jret;
			if (code == Func_Sucess)
				Google_IAB_Init = true;
		}
		else
		{
			code = C_Call_Jave___JavaFunction_Cannot_Find;
			Google_IAB::IAB_Status = IAB_status_Error;
			return;
		}
	}
	else
	{
//		code = C_Call_Jave___JavaFunction_Cannot_Find;
		Google_IAB::IAB_Status = IAB_status_Error;
		return;
	}

}

// ************************************************************************************************
// step:4 連線Goole  會有callBackFunction
// ************************************************************************************************
void Google_IAB::startSetup(void)
{
	CCLog("%s", __PRETTY_FUNCTION__);
	if (!Google_IAB_Init)
	{
		code = Error__Need_to_Init_Google;
		Google_IAB::IAB_Status = IAB_status_Error;
		return;
	}
	if (!Google_IAB::checkInternet)
	{
		code = Error__Internet_Connection_Drops_off;
		Google_IAB::IAB_Status = IAB_status_Error;
		return;
	}
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, IABCLASS_PATH, "startSetup", "()V"))
	{
		Google_IAB::IAB_Status = IAB_status_StartSetup;
		Google_IAB_SetupUp = true;
		t.env->CallStaticIntMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
	else
	{
		code = C_Call_Jave___JavaFunction_Cannot_Find;
		Google_IAB::IAB_Status = IAB_status_Error;
		return;
	}
}

// ************************************************************************************************
// step:5 從Google取得商品資料 會有callBackFunction
// ************************************************************************************************
void Google_IAB::QueryInventory(void)
{
	CCLog("%s", __PRETTY_FUNCTION__);

	if (!Google_IAB_SetupUp)
	{
		code = Error__Google_IAB_SetupUp_NEED_Complete;
		Google_IAB::IAB_Status = IAB_status_Error;
		return;
	}
	if (!Google_IAB::checkInternet)
	{
		code = Error__Internet_Connection_Drops_off;
		Google_IAB::IAB_Status = IAB_status_Error;
		return;
	}
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, IABCLASS_PATH, "QueryInventory", "()V"))
	{
		Google_IAB::IAB_Status = IAB_status_QueryInventory;
		t.env->CallStaticIntMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
		Google_IAB_Inventory = true;
	}
	else
	{
		code = C_Call_Jave___JavaFunction_Cannot_Find;
		Google_IAB::IAB_Status = IAB_status_Error;
		return;
	}
}

// ************************************************************************************************
// step:6-0 從step5資料中抓得商品 "價格"
// ************************************************************************************************
void Google_IAB::GetSkuPrice(const char* SKU_ID, char *SkuPrice)
{
	CCLog("%s", __PRETTY_FUNCTION__);

	if (SKU_ID == NULL || !SKU_ID[0])
	{
		code = Error__SkuID_is_null;
		Google_IAB::IAB_Status = IAB_status_Error;
		return;
	}
	if (SkuPrice == NULL)
	{
		code = Error__SkuPrice_is_null;
		Google_IAB::IAB_Status = IAB_status_Error;
		return;
	}

	if (!Google_IAB_Inventory)
	{
		code = Error__Google_QueryInventory_Need_Set_Up;
		Google_IAB::IAB_Status = IAB_status_Error;
		return;
	}
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, IABCLASS_PATH, "GetSkuPrice", "(Ljava/lang/String;)Ljava/lang/String;"))
	{
		jstring Jarg = t.env->NewStringUTF(SKU_ID);
		jstring Jret = (jstring) t.env->CallStaticObjectMethod(t.classID, t.methodID, Jarg);

		t.env->DeleteLocalRef(Jarg);
		t.env->DeleteLocalRef(t.classID);
		if (Jret)
		{
			const char *Cstr = t.env->GetStringUTFChars(Jret, NULL); //isCopy==NULL
			if (!Cstr)
			{
				code = C_Call_Jave___Out_Of_Memory;
				Google_IAB::IAB_Status = IAB_status_Error;
				return;
			}
			strcpy(SkuPrice, Cstr);
			t.env->ReleaseStringUTFChars(Jret, Cstr);			//free
			t.env->DeleteLocalRef(Jret);
		}
		else
		{
			strcpy(SkuPrice, "");
			t.env->DeleteLocalRef(Jret);
			return;
		}
	}
	else
	{
		code = C_Call_Jave___JavaFunction_Cannot_Find;
		Google_IAB::IAB_Status = IAB_status_Error;
		return;
	}

}
// ************************************************************************************************
// step:6-1 從step5資料中抓得商品 "是否已購買"
// ************************************************************************************************
void Google_IAB::isPurchased(const char* SKU_ID, bool &isPurz)
{
	CCLog("%s", __PRETTY_FUNCTION__);

	if (SKU_ID == NULL || !SKU_ID[0])
	{
		code = Error__SkuID_is_null;
		Google_IAB::IAB_Status = IAB_status_Error;
		return;
	}
	if (!Google_IAB_Inventory)
	{
		code = Error__Google_QueryInventory_Need_Set_Up;
		Google_IAB::IAB_Status = IAB_status_Error;
		return;
	}
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, IABCLASS_PATH, "isPurchased", "(Ljava/lang/String;)Z"))
	{
		jstring Jarg = t.env->NewStringUTF(SKU_ID);
		jboolean jret = t.env->CallStaticIntMethod(t.classID, t.methodID, Jarg);
		isPurz = jret == JNI_TRUE ? true : false;
		t.env->DeleteLocalRef(Jarg);
		t.env->DeleteLocalRef(t.classID);
	}
	else
	{
		code = C_Call_Jave___JavaFunction_Cannot_Find;
		Google_IAB::IAB_Status = IAB_status_Error;
		return;
	}

}
// ************************************************************************************************
// step:6-2 從step5資料中抓得商品 "描述"
// ************************************************************************************************
void Google_IAB::GetDescription(const char* SKU_ID, char *SkuDetail)
{
	CCLog("%s", __PRETTY_FUNCTION__);

	if (SKU_ID == NULL || !SKU_ID[0])
	{
		code = Error__SkuID_is_null;
		Google_IAB::IAB_Status = IAB_status_Error;
		return;
	}
	if (SkuDetail == NULL)
	{
		code = Error__SkuDetail_is_null;
		Google_IAB::IAB_Status = IAB_status_Error;
		return;
	}
	if (!Google_IAB_Inventory)
	{
		code = Error__Google_QueryInventory_Need_Set_Up;
		Google_IAB::IAB_Status = IAB_status_Error;
		return;
	}
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, IABCLASS_PATH, "GetDescription", "(Ljava/lang/String;)Ljava/lang/String;"))
	{
		jstring Jarg = t.env->NewStringUTF(SKU_ID);
		jstring Jret = (jstring) t.env->CallStaticObjectMethod(t.classID, t.methodID, Jarg);

		t.env->DeleteLocalRef(Jarg);
		t.env->DeleteLocalRef(t.classID);
		if (Jret)
		{
			const char *Cstr = t.env->GetStringUTFChars(Jret, NULL); //isCopy==NULL
			if (!Cstr)
			{
				code = C_Call_Jave___Out_Of_Memory;
				Google_IAB::IAB_Status = IAB_status_Error;
				return;
			}
			strcpy(SkuDetail, Cstr);
			t.env->ReleaseStringUTFChars(Jret, Cstr);			//free
			t.env->DeleteLocalRef(Jret);

		}
		else
		{
			strcpy(SkuDetail, "");
			t.env->DeleteLocalRef(Jret);
			return;
		}
	}
	else
	{
		code = C_Call_Jave___JavaFunction_Cannot_Find;
		Google_IAB::IAB_Status = IAB_status_Error;
		return;
	}

}
// ************************************************************************************************
// step:7 購買商品 會有callBackFunction
// ************************************************************************************************
void Google_IAB::Purchase(const char* SKU_ID)
{
	CCLog("%s", __PRETTY_FUNCTION__);

	if (SKU_ID == NULL || !SKU_ID[0])
	{
		code = Error__SkuID_is_null;
		Google_IAB::IAB_Status = IAB_status_Error;
		return;
	}
	if (!Google_IAB_SetupUp)
	{
		code = Error__Google_IAB_SetupUp_NEED_Complete;
		Google_IAB::IAB_Status = IAB_status_Error;
		return;
	}
	if (!Google_IAB::checkInternet)
	{
		code = Error__Internet_Connection_Drops_off;
		Google_IAB::IAB_Status = IAB_status_Error;
		return;
	}
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, IABCLASS_PATH, "Purchase", "(Ljava/lang/String;)V"))
	{
		Google_IAB::IAB_Status = IAB_status_Purchase;
		jstring arg = t.env->NewStringUTF(SKU_ID);
		t.env->CallStaticIntMethod(t.classID, t.methodID, arg);
		t.env->DeleteLocalRef(arg);
		t.env->DeleteLocalRef(t.classID);
		return;
	}
	else
	{
		code = C_Call_Jave___JavaFunction_Cannot_Find;
		Google_IAB::IAB_Status = IAB_status_Error;
		return;
	}

}
// ************************************************************************************************
// step:8 消耗商品 會有callBackFunction
// ************************************************************************************************

void Google_IAB::ConsumeItem(const char* SKU_ID)
{
	CCLog("%s", __PRETTY_FUNCTION__);

	if (SKU_ID == NULL || !SKU_ID[0])
	{
		code = Error__SkuID_is_null;
		Google_IAB::IAB_Status = IAB_status_Error;
		return;
	}
	if (!Google_IAB_Inventory)
	{
		code = Error__Google_QueryInventory_Need_Set_Up;
		Google_IAB::IAB_Status = IAB_status_Error;
		return;
	}
	if (!Google_IAB::checkInternet)
	{
		code = Error__Internet_Connection_Drops_off;
		Google_IAB::IAB_Status = IAB_status_Error;
		return;
	}
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, IABCLASS_PATH, "ConsumeItem", "(Ljava/lang/String;)Z"))
	{
		Google_IAB::IAB_Status = IAB_status_ConsumeItem;
		jstring arg = t.env->NewStringUTF(SKU_ID);
		jboolean jret = t.env->CallStaticIntMethod(t.classID, t.methodID, arg);
		if (jret == JNI_TRUE)
		{
			t.env->DeleteLocalRef(arg);
			t.env->DeleteLocalRef(t.classID);
		}
		else
		{
			code = Error__No_Item_Consume;
			Google_IAB::IAB_Status = IAB_status_Error;
			t.env->DeleteLocalRef(arg);
			t.env->DeleteLocalRef(t.classID);
			return;
		}
	}
	else
	{
		code = C_Call_Jave___JavaFunction_Cannot_Find;
		Google_IAB::IAB_Status = IAB_status_Error;
		return;
	}

}
// ************************************************************************************************
// step:9 關閉連線 ->開啓一次後 重新啓動可從step 3開始(step1,step2 已紀錄)
// ************************************************************************************************
void Google_IAB::DestroyStore(void)
{
	CCLog("%s", __PRETTY_FUNCTION__);
	if (Google_IAB_Init)
	{
		JniMethodInfo t;
		if (JniHelper::getStaticMethodInfo(t, IABCLASS_PATH, "DestroyStore", "()V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
			Google_IAB_SetupUp = false;
			Google_IAB_Inventory = false;
			Google_IAB_Init = false;
			return;
		}
		else
		{
			code = C_Call_Jave___JavaFunction_Cannot_Find;
			Google_IAB::IAB_Status = IAB_status_Error;
			return;
		}
	}

}

// ************************************************************************************************
// Java call C 的callBackFunction
// ************************************************************************************************
void Google_IAB::CallBackFunction(int msg)
{
	if (msg == Func_Sucess)
	{
		Google_IAB::IAB_Status = IAB_status_Complete;
	}
	else
	{
		Google_IAB::IAB_Status = IAB_status_Error;
	}
	Google_IAB::code = msg;
}

// ************************************************************************************************
// 檢查網路
// ************************************************************************************************
bool Google_IAB::checkInternet(void)
{
	CCLog("%s", __PRETTY_FUNCTION__);
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, IABCLASS_PATH, "checkInternet", "()Z"))
	{
		jboolean jret = t.env->CallStaticIntMethod(t.classID, t.methodID);
		if (jret == JNI_TRUE)
		{
			t.env->DeleteLocalRef(t.classID);
			return true;
		}
		else
		{
			t.env->DeleteLocalRef(t.classID);
			return false;
		}
	}
	else
	{
		code = C_Call_Jave___JavaFunction_Cannot_Find;
		Google_IAB::IAB_Status = IAB_status_Error;
		return false;
	}
}

// ************************************************************************************************
// 訊息顯示(Toast) true=顯示時間長
// ************************************************************************************************
void Google_IAB::MsgToast(const char* msg, bool display_time)
{
	CCLog("%s", __PRETTY_FUNCTION__);

	if (msg == NULL || !msg[0])
	{
		code = Error__SkuID_is_null;
		Google_IAB::IAB_Status = IAB_status_Error;
		return;
	}

	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, IABCLASS_PATH, "MsgToast", "(Ljava/lang/String;Z)V"))
	{
		jstring arg = t.env->NewStringUTF(msg);
		jboolean arg1 = (unsigned char) display_time;
		t.env->CallStaticVoidMethod(t.classID, t.methodID, arg, arg1);
		t.env->DeleteLocalRef(arg);
		t.env->DeleteLocalRef(t.classID);
	}
	else
	{
		code = C_Call_Jave___JavaFunction_Cannot_Find;
		Google_IAB::IAB_Status = IAB_status_Error;
		return;
	}
}
// ************************************************************************************************
// 訊息顯示(AlertDialog)
// ************************************************************************************************
void Google_IAB::MsgAlertDialog(const char* msg)
{
	CCLog("%s", __PRETTY_FUNCTION__);

	if (msg == NULL || !msg[0])
	{
		code = Error__SkuID_is_null;
		Google_IAB::IAB_Status = IAB_status_Error;
		return;
	}

	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, IABCLASS_PATH, "MsgAlertDialog", "(Ljava/lang/String;)V"))
	{
		jstring arg = t.env->NewStringUTF(msg);
		t.env->CallStaticVoidMethod(t.classID, t.methodID, arg);
		t.env->DeleteLocalRef(arg);
		t.env->DeleteLocalRef(t.classID);
	}
	else
	{
		code = C_Call_Jave___JavaFunction_Cannot_Find;
		Google_IAB::IAB_Status = IAB_status_Error;
		return;
	}
}

// ************************************************************************************************
// Error 查詢
// ************************************************************************************************
const char *Google_IAB::Error_Code(int code)
{
	const char *mess[10] =
	{ "0:OK", //
			"1:User Canceled", //
			"2:Unknown", //
			"3:Billing Unavailable", //
			"4:Item unavailable", //
			"5:Developer Error", //
			"6:Error", //
			"7:Item Already Owned", //
			"8:Item not owned" };
	const char *mess1000[13] =
	{ "0:OK", //
			"-1001:Remote exception during initialization", //
			"-1002:Bad response received", //
			"-1003:Purchase signature verification failed", //
			"-1004:Send intent failed", //
			"-1005:User Cancelled", //
			"-1006:Unknown purchase response", //
			"-1007:Missing token", //
			"-1008:Unknown error", //
			"-1009:Subscriptions not available", //
			"-1010:Invalid consumption attempt" };

	const char *mess500[17] =
	{ "0:OK",			//
			"-501:JavaFunction Cannot Find",			//
			"-502:Java Return Failure",			//
			"-503:OutOfMemory",			//
			"-504:Need To Setup IAB On Main Java",			//
			"-505:Error__Purchase_Payload_Cannot_Match",			//
			"-506:Error__Google_IAB_SetupUp_NEED_Complete",			//
			"-507:Error__mHelper_Need_Set_Up",			//
			"-508:Error__Google_QueryInventory_Need_Set_Up",			//
			"-509:Error__No_Item_Consume",			//
			"-510:Error__Need_to_Init_Google_IAB(on C)",			//
			"-511:Error__SkuID_is_null",			//
			"-512:Error__PublicKey_is_null",			//
			"-513:Error__SkuPrice_is_null",			//
			"-514:Error__SkuDetail_is_null",			//
			"-515:Error__Internet_Connection_Drops_off",			//
			"-516:Error__Need_TO_Set_PublicKey",			//
			};

	if (code < -1000)
	{
		return mess1000[-1000 - code];
	}
	else if (code < -500)
	{
		return mess500[-500 - code];
	}
	else
	{
		return mess[code];
	}
	return NULL;
}
// ************************************************************************************************
// Java Call C
// ************************************************************************************************
extern "C"
{
void Java_tw_com_double2_GoogleIAB_nativeactionFinished(
		JNIEnv* env,
		jobject thiz,
		jlong callback_address,
		jint error_code)
{
	if (callback_address)
		(*reinterpret_cast<actionFinished>(callback_address))(error_code);
}
}
#endif
