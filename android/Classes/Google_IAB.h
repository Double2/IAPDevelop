/*
 * Google_IAB.h
 *
 *  Created on: 2013/6/11
 *      Author: double2
 */

#ifndef GOOGLE_IAB_H_
#define GOOGLE_IAB_H_
#include "cocos2d.h"
#include "Define_IAP.h"

#if (IAP_PLATFORM == IAP_PLATFORM_GOOGLE)

#include "platform/android/jni/JniHelper.h"
#define IABCLASS_PATH "tw/com/double2/GoogleIAB"
using namespace std;
typedef void (*actionFinished)(int);

class Google_IAB
{
		friend class Layer_GoogleIAB;
	public:
		//初始function
		static void CallBackFunc(actionFinished callback);
		static void SetSkuID(const char* SKU_ID);
		static void Init(const char* PublicKeyd, const char* PublicKeyb, const char* PublicKeytwo);
		//主要function
		static void startSetup(void);
		static void QueryInventory(void);
		static void GetSkuPrice(const char* SKU_ID, char *SkuPrice);
		static void GetDescription(const char* SKU_ID, char *SkuDetail);
		static void isPurchased(const char* SKU_ID, bool &isPurz);
		static void Purchase(const char* SKU_ID);
		static void ConsumeItem(const char* SKU_ID);
		static void DestroyStore(void);
		//支援function
		static bool checkInternet(void);
		static void MsgToast(const char* msg, bool display_time);
		static void MsgAlertDialog(const char* msg);
		static void CallBackFunction(int msg);
		static const char *Error_Code(int code);

		static int IAB_Status;
		static int code;
		static bool Google_IAB_CallBackFunc, Google_IAB_SetupUp, Google_IAB_Inventory, Google_IAB_Init, Google_IAB_SetSkuID;

	private:
		enum _error_IAB_
		{
			Func_Sucess = 0,
			C_Call_Jave___JavaFunction_Cannot_Find = -501,
			C_Call_Jave___Return_Failure = -502,
			C_Call_Jave___Out_Of_Memory = -503,
			Error__Need_To_Setup_IAB_On_Main_Java = -504,
			Error__Purchase_Payload_Cannot_Match = -505,
			Error__Google_IAB_SetupUp_NEED_Complete = -506,
			Error__mHelper_Need_Set_Up = -507,
			Error__Google_QueryInventory_Need_Set_Up = -508,
			Error__No_Item_Consume = -509,
			Error__Need_to_Init_Google = -510,
			Error__SkuID_is_null = -511,
			Error__PublicKey_is_null = -512,
			Error__SkuPrice_is_null = -513,
			Error__SkuDetail_is_null = -514,
			Error__Internet_Connection_Drops_off = -515,
			Error__Need_TO_Set_PublicKey = -516,

		};
		enum _IAB_status_
		{
			IAB_status_Complete = 0,
			IAB_status_StartSetup = 1,
			IAB_status_QueryInventory = 2,
			IAB_status_Purchase = 3,
			IAB_status_ConsumeItem = 4,
			IAB_status_Error = 5,

		};

};

typedef struct _IN_APP_PURCHASE
{
		int Num;
		typedef struct _Info_
		{
				char Title[100];
				char SkuID[50];
				char Price[50];
				bool IsPurchase;

				int MoneyType; //    00$1000
				int MoneyValue;
		} _Info;
		_Info Info[MAX_IAP_NUM];
} IN_APP_PURCHASE;

extern IN_APP_PURCHASE InAppPurchase;
#endif
#endif /* GOOGLE_IAB_H_ */
