/*
 * Layer_GoogleIAB.h
 *
 *  Created on: 2013/6/24
 *      Author: double2
 */

#ifndef Layer_GoogleIAB_H_
#define Layer_GoogleIAB_H_

#include "cocos2d.h"
#include "Define_IAP.h"
#if (IAP_PLATFORM == IAP_PLATFORM_GOOGLE)
#include "Google_IAB.h"
USING_NS_CC;
class Layer_GoogleIAB: public cocos2d::CCLayer
{
	public:

	//需實做以下
	virtual void EnableStoreUI(void); // 開啟商店UI
	virtual void DisableStoreUI(void);// 關閉商店UI
	virtual void EnableLoadingUI(void);// 開啟Loading動畫
	virtual void DisableLoadingUI(void);// 關閉Loading動畫
	virtual void UpDateUI(int Type, int Value); //購買成功後 更新遊戲畫面
	//end 需實做以下



	//1.設定key (only for Google Billing)
	//          (每個Key限制char 200以內)
	void SetPublicKey(const char *keyd, const char *keyb, const char *keytwo);

	//2.設定SkuID
	void SetSkuID(const char ID[MAX_IAP_NUM][50]);

	//3.查詢商品資訊 並打開商店UI
	//流程:
	//	1.呼叫EnableLoadingUI()
	//	2.查詢 SetSkuID()所設SkuID 並將SkuID回傳結果儲存在 Global_var InAppPurchase
	//		3-1.如果有 "未消耗商品"   呼叫 DisableLoadingUI()=>EnableStoreUI()=>UpDateUI()
	//		3-2.如果沒有 "未消耗商品" 呼叫 DisableLoadingUI()=>EnableStoreUI()
	//	4.中間任何地方有錯誤        呼叫 DisableLoadingUI()=>EnableStoreUI()
	//
	// ps:使用 OpenStore() 之前需先呼叫 Function 1 & 2
	void OpenStore(void);

	//4.購買商品(num)
	//流程:
	//	1.呼叫EnableLoadingUI()
	//		2-1.如果購買   	 呼叫 DisableLoadingUI()=>UpDateUI()
	//		2-2.沒購買     	 呼叫 DisableLoadingUI()
	//	3.中間任何地方有錯誤 呼叫 DisableLoadingUI()
	void BuyItem(int num);

	//5.檢查網路
	//回傳true=>有網路
	bool CheckInternet();

	//說明/介紹消耗流程(不需要呼叫)
	void ConsumeItem(int num);

	//工作狀態 0:等待工作 1:購買/消耗成功 -1:有錯誤
	int Work_Status;
	virtual bool init();
	void updateGame(ccTime dt);
	LAYER_NODE_FUNC(Layer_GoogleIAB);

	private:
	char d[200];
	char b[200];
	char two[200];
	int ConsumItemNum;
	int AutoConsumItemNum;
	int AutoConsumItemNum_Data[MAX_IAP_NUM];
	int Itme_Num;
	char tmpID[128];
	enum __IAB_status__
	{
		Something_Error = -1,
		Waiting_Job = 0,
		Succes_ConsumItem_Need_TO_Add_Money = 1,
		OpenStore_0,
		OpenStore_1,
		OpenStore_2,
		OpenStore_3,
		OpenStore_4,
		OpenStore_5,
		OpenStore_6,
		BuySomthing_0,
		BuySomthing_1,
		BuySomthing_2,
		BuySomthing_3,
		BuySomthing_4,
		BuySomthing_5,
		ConsumeItem_0,
		ConsumeItem_1,
		ConsumeItem_2,
		ConsumeItem_3,

	};

};
#endif
#endif /* Layer_GoogleIAB_H_ */
