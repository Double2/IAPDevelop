/*
 * Layer_GoogleIAB.cpp
 *
 *  Created on: 2013/6/24
 *      Author: double2
 */

#include "Layer_GoogleIAB.h"
#include "Google_IAB.h"
#include "Define_IAP.h"
#if (IAP_PLATFORM == IAP_PLATFORM_GOOGLE)
// *************************************************************************************************
// on "init" you need to initialize your instance
// *************************************************************************************************

bool Layer_GoogleIAB::init()
{
	CCLog("%s", __PRETTY_FUNCTION__);

	//////////////////////////////
	// 1. super init first
	if (!CCLayer::init())
	{
		return false;
	}

	//Set CallBackFunc
	Google_IAB::CallBackFunc(Google_IAB::CallBackFunction);

	Work_Status = Waiting_Job;
	this->schedule(schedule_selector(Layer_GoogleIAB::updateGame));
	return true;
}
// ************************************************************************************************
// func 1 : 設定SetPublicKey
// ************************************************************************************************
void Layer_GoogleIAB::SetPublicKey(const char *keyd, const char *keyb, const char *keytwo)
{
	sprintf(d, "%s", keyd);
	sprintf(b, "%s", keyb);
	sprintf(two, "%s", keytwo);
}
// ************************************************************************************************
// func 2 : 設定PutSkuID
// ************************************************************************************************
void Layer_GoogleIAB::SetSkuID(const char ID[MAX_IAP_NUM][50])
{
	InAppPurchase.Num = MAX_IAP_NUM;
	for (int n = 0; n < InAppPurchase.Num; n++)
	{
		if (ID[n][0])
		{
			sprintf(InAppPurchase.Info[n].SkuID, "%s", ID[n]);
			Google_IAB::SetSkuID(InAppPurchase.Info[n].SkuID);
		}
	}
}
// ************************************************************************************************
// func 3 : 連線googleIAB 查詢資料 關掉googleIAB
// ************************************************************************************************
void Layer_GoogleIAB::OpenStore()
{
	Work_Status = OpenStore_0;
}
// ************************************************************************************************
// 如果func 3 有庫存 消耗掉
// ************************************************************************************************
void Layer_GoogleIAB::ConsumeItem(int num)
{
	ConsumItemNum = num;
	Work_Status = ConsumeItem_0;

}

// ************************************************************************************************
// func 4 : 購買
// ************************************************************************************************
void Layer_GoogleIAB::BuyItem(int num)
{
	ConsumItemNum = num;
	Work_Status = BuySomthing_0;
}

// ************************************************************************************************
// func 5 : 檢查網路連線
// ************************************************************************************************
bool Layer_GoogleIAB::CheckInternet()
{
	return Google_IAB::checkInternet();
}
// ************************************************************************************************
// ************************************************************************************************
void Layer_GoogleIAB::updateGame(float dt)
{
	switch (Work_Status)
	{
		case OpenStore_0:
		{
			EnableLoadingUI();
			Google_IAB::Init(d, b, two);
			//初始成功
			if (Google_IAB::code == Google_IAB::Func_Sucess)
			{
				Google_IAB::startSetup();
				AutoConsumItemNum = 0;
				Work_Status = OpenStore_1;
			}
			//初始失敗
			else
			{
				InAppPurchase.Num = 0;
				DisableLoadingUI();
				EnableStoreUI();
				Work_Status = Something_Error;
			}
		}
			break;
		case OpenStore_1:
			switch (Google_IAB::IAB_Status)
			{
				//建立成功
				case Google_IAB::IAB_status_Complete:
					Google_IAB::QueryInventory();
					Work_Status = OpenStore_2;
					break;
					//建立失敗
				case Google_IAB::IAB_status_Error:
					InAppPurchase.Num = 0;
					DisableLoadingUI();
					EnableStoreUI();
					Work_Status = Something_Error;
					break;
			}
			break;
		case OpenStore_2:
			switch (Google_IAB::IAB_Status)
			{
				//查詢成功
				case Google_IAB::IAB_status_Complete:
				{
					int count = 0;
					for (int n = 0; n < InAppPurchase.Num; n++)
					{
						char tmp[256];
						char *ptr;

						if (InAppPurchase.Info[n].SkuID[0])
						{
							Google_IAB::GetSkuPrice(InAppPurchase.Info[n].SkuID, tmp);
							if (tmp[0])
							{
								sprintf(InAppPurchase.Info[count].Price, "%s", tmp);
								sprintf(InAppPurchase.Info[count].SkuID, "%s", InAppPurchase.Info[n].SkuID);
//								strtok_r(tmp, "$", &ptr);
//								sprintf(InAppPurchase.Info[count].Price, "%s", ptr);
								Google_IAB::isPurchased(InAppPurchase.Info[count].SkuID, InAppPurchase.Info[count].IsPurchase);
								Google_IAB::GetDescription(InAppPurchase.Info[count].SkuID, tmp);
								if (tmp[0])
								{
									strtok_r(tmp, "$", &ptr);
									InAppPurchase.Info[count].MoneyType = atoi(tmp);
									if (ptr != NULL && ptr[0])
									{
										sprintf(tmp, "%s", ptr);
										strtok_r(tmp, "#", &ptr);
										InAppPurchase.Info[count].MoneyValue = atoi(tmp);
									}
								}
								count++;
							}
						}
					}
					InAppPurchase.Num = count;
					Work_Status = OpenStore_3;
				}
					break;
					//查詢失敗
				case Google_IAB::IAB_status_Error:
					InAppPurchase.Num = 0;
					DisableLoadingUI();
					EnableStoreUI();
					Work_Status = Something_Error;
					break;
			}
			break;
		case OpenStore_3:
			Work_Status = OpenStore_4;
			//檢查是否有 未消耗物品 ? 有 消耗掉並回OpenStore_1 : 沒有去OpenStore_4
			for (int n = 0; n < InAppPurchase.Num; n++)
			{
				if (InAppPurchase.Info[n].IsPurchase)
				{
					AutoConsumItemNum_Data[AutoConsumItemNum] = n;
					AutoConsumItemNum++;
					Google_IAB::ConsumeItem(InAppPurchase.Info[n].SkuID);
					Work_Status = OpenStore_1;
					break;
				}
			}
			break;
		case OpenStore_4:
			Google_IAB::DestroyStore();
			DisableLoadingUI();
			EnableStoreUI();
			Work_Status = AutoConsumItemNum == 0 ? Waiting_Job : Succes_ConsumItem_Need_TO_Add_Money;
			break;

			//購買開始
		case BuySomthing_0:
			EnableLoadingUI();
			Google_IAB::Init(d, b, two);
			//初始成功
			if (Google_IAB::code == Google_IAB::Func_Sucess)
			{
				Google_IAB::startSetup();
				Work_Status = BuySomthing_1;
			}
			//初始失敗
			else
			{
				Work_Status = Something_Error;
			}
			break;
		case BuySomthing_1:
			switch (Google_IAB::IAB_Status)
			{
				//建立成功
				case Google_IAB::IAB_status_Complete:
					Google_IAB::Purchase(InAppPurchase.Info[ConsumItemNum].SkuID);
					Work_Status = BuySomthing_2;
					break;
					//建立失敗
				case Google_IAB::IAB_status_Error:
					Work_Status = Something_Error;
					break;
			}
			break;
		case BuySomthing_2:
			switch (Google_IAB::IAB_Status)
			{
				//購買成功
				case Google_IAB::IAB_status_Complete:
					Google_IAB::QueryInventory();
					Work_Status = BuySomthing_3;
					break;
					//購買失敗
				case Google_IAB::IAB_status_Error:
					Work_Status = Something_Error;
					break;
			}
			break;
		case BuySomthing_3:
			switch (Google_IAB::IAB_Status)
			{
				//查詢成功
				case Google_IAB::IAB_status_Complete:
					Google_IAB::ConsumeItem(InAppPurchase.Info[ConsumItemNum].SkuID);
					Work_Status = BuySomthing_4;
					break;
					//查詢失敗
				case Google_IAB::IAB_status_Error:
					Work_Status = Something_Error;
					break;
			}
			break;
		case BuySomthing_4:
			switch (Google_IAB::IAB_Status)
			{
				//消耗成功
				case Google_IAB::IAB_status_Complete:
					Google_IAB::DestroyStore();
					Work_Status = Succes_ConsumItem_Need_TO_Add_Money;
					break;
					//消耗失敗
				case Google_IAB::IAB_status_Error:
					Work_Status = Something_Error;
					break;
			}
			break;
			//消耗流程
		case ConsumeItem_0:
		{
			EnableLoadingUI();
			Google_IAB::Init(d, b, two);
			if (Google_IAB::code == Google_IAB::Func_Sucess)
			{
				Google_IAB::startSetup();
				Work_Status = ConsumeItem_1;

			}
			else
			{
				Work_Status = Something_Error;
			}
		}
			break;
		case ConsumeItem_1:
			switch (Google_IAB::IAB_Status)
			{
				case Google_IAB::IAB_status_Complete:
					Google_IAB::QueryInventory();
					Work_Status = ConsumeItem_2;
					break;
				case Google_IAB::IAB_status_Error:
					Work_Status = Something_Error;
					break;
			}
			break;
		case ConsumeItem_2:
			switch (Google_IAB::IAB_Status)
			{
				case Google_IAB::IAB_status_Complete:
					Google_IAB::ConsumeItem(InAppPurchase.Info[ConsumItemNum].SkuID);
					Work_Status = ConsumeItem_3;
					break;
				case Google_IAB::IAB_status_Error:
					Work_Status = Something_Error;
					break;
			}
			break;
		case ConsumeItem_3:
			switch (Google_IAB::IAB_Status)
			{
				case Google_IAB::IAB_status_Complete:
					Google_IAB::DestroyStore();
					Work_Status = Succes_ConsumItem_Need_TO_Add_Money;
					break;
				case Google_IAB::IAB_status_Error:
					Work_Status = Something_Error;
					break;
			}
			break;
		case Something_Error:
			Google_IAB::DestroyStore();
			DisableLoadingUI();
			switch (Google_IAB::code)
			{
				//User Cancelled
				case -1005:
				case 1:
					Work_Status = Waiting_Job;
					break;
				case 7:
					Work_Status = ConsumeItem_0;
					break;
					//網路斷
				case 2:
				case 6:
					Work_Status = Waiting_Job;
					break;
				default:
					Work_Status = Waiting_Job;
					break;
			}
			Google_IAB::IAB_Status = Google_IAB::IAB_status_Complete;
			break;
		case Succes_ConsumItem_Need_TO_Add_Money:
		{
			DisableLoadingUI();
			if (AutoConsumItemNum == 0)
				UpDateUI(InAppPurchase.Info[ConsumItemNum].MoneyType, InAppPurchase.Info[ConsumItemNum].MoneyValue);
			else
			{
				int sum = 0;
				for (int n = 0; n < AutoConsumItemNum; n++)
				{
					sum += InAppPurchase.Info[n].MoneyValue;
				}
				UpDateUI(InAppPurchase.Info[0].MoneyType, sum);//如果type種類不同 就會有錯誤
			}
			Work_Status = Waiting_Job;
		}
			break;
	}	//switch (Work_Status)
}
#endif
