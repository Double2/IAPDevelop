package tw.com.double2;

import android.app.Activity;

import android.app.AlertDialog;
import android.content.Intent;
import android.content.SharedPreferences;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;
import android.os.RemoteException;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;

import com.android.billing.util.*;
import com.android.billing.util.IabHelper.OnIabSetupFinishedListener;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.widget.Toast;
import android.app.AlertDialog;
import android.widget.Button;
import android.content.DialogInterface;

public class GoogleIAB extends Activity
{

	private static Activity me = null;
	private static GLSurfaceView mGLSurfaceView = null;
	private static Inventory me_inventory;
	private static Context me_context = null;
	private static List<String> additionalSkuList = null;
	private static String base64EncodedPublicKey = null;
	private static IabHelper.OnIabSetupFinishedListener meIabSetupFinishedListener = null;
	private static IabHelper.QueryInventoryFinishedListener meGotInventoryListener = null;
	private static IabHelper.OnIabPurchaseFinishedListener mePurchaseFinishedListener = null;
	private static IabHelper.OnConsumeFinishedListener meConsumeFinishedListener = null;
	private static String Consume_Sku_ID = "";
	public static IabHelper mHelper = null;
	public static Handler mUI_Hadnler = null;
	public static Handler mThreadHandler = null;
	public static HandlerThread mThread = null;
	private static final Handler mMainThread = new Handler(Looper.getMainLooper());
	public static boolean flagJava = false;

	private static native void nativeactionFinished(long callback_address, int result);

	// final
	static final String TAG = "GoogleIAB_Java";// Debug tag, for logging
	public static final int RC_REQUEST = 10001; // request code for the purchase
	// flow
	public static final int Func_Sucess = 0;
	public static final int Error__Need_To_Setup_IAB_On_Main_Java = -504;
	public static final int Error__Purchase_Payload_Cannot_Match = -505;
	public static final int Error__mHelper_Need_Set_Up = -507;
	public static final int Error__Need_TO_Set_PublicKey = -516;

	// ************************************************************************************************
	// 建立這class Activity
	// ************************************************************************************************
	public static void setup(Activity activity, Context ct)
	{
		me = activity;
		me_context = ct;
	}

	// ************************************************************************************************
	// 設callback
	// **
	// * 以下是跟google各連線後的callBackFunction
	// * 建立連線 meIabSetupFinishedListener
	// * 詢問商品 meGotInventoryListener
	// * 購買商品 mePurchaseFinishedListener
	// * 消耗商品 meConsumeFinishedListener
	// */
	// ************************************************************************************************
	public static void CallBackFunc(final long callback_address)
	{

		// 建立連線 callbackfunction
		if (meIabSetupFinishedListener == null)
		{
			meIabSetupFinishedListener = new IabHelper.OnIabSetupFinishedListener()
			{
				public void onIabSetupFinished(IabResult result)
				{
					Log.d(TAG, "Setup finished.");
					if (mHelper == null)
					{
						MsgAlertDialog("Error__mHelper_Need_Set_Up");
						nativeactionFinished(callback_address, Error__mHelper_Need_Set_Up);
						return;
					}

					if (result.isFailure())
					{
						MsgAlertDialog(result.getMessage());
						Log.d(TAG, "Setup Failure.");
						nativeactionFinished(callback_address, result.getResponse());
						return;
					}

					// IAB is fully set up. Now, let's get an inventory of stuff
					// we own.
					Log.d(TAG, "Setup successful. Querying inventory.");
					nativeactionFinished(callback_address, result.getResponse());
				}
			};
		}
		// 商品info callbackfunction
		if (meGotInventoryListener == null)
		{
			meGotInventoryListener = new IabHelper.QueryInventoryFinishedListener()
			{
				public void onQueryInventoryFinished(IabResult result, Inventory inventory)
				{
					Log.d(TAG, "Query inventory finished.");
					if (mHelper == null)
					{
						MsgAlertDialog("Error__mHelper_Need_Set_Up");
						nativeactionFinished(callback_address, Error__mHelper_Need_Set_Up);
						return;
					}
					if (result.isFailure())
					{
						MsgAlertDialog(result.getMessage());
						Log.d(TAG, "Query inventory was Failure.");
						nativeactionFinished(callback_address, result.getResponse());
						return;
					}
					Log.d(TAG, "Query inventory was successful.");
					me_inventory = inventory;
					Log.d(TAG, "Initial inventory query finished");
					// Log.d(TAG, "" +
					// inventory.getSkuDetails("test_ser_01").getPrice());
					nativeactionFinished(callback_address, result.getResponse());
				}
			};
		}
		// 購買 callbackfunction
		if (mePurchaseFinishedListener == null)
		{
			mePurchaseFinishedListener = new IabHelper.OnIabPurchaseFinishedListener()
			{
				public void onIabPurchaseFinished(IabResult result, Purchase purchase)
				{
					Log.d(TAG, "Purchase finished: " + result + ", purchase: " + purchase);
					if (mHelper == null)
					{
						MsgAlertDialog("Error__mHelper_Need_Set_Up");
						nativeactionFinished(callback_address, Error__mHelper_Need_Set_Up);
						return;
					}

					if (result.isFailure())
					{
						// MsgAlertDialog(result.getMessage(), true);
						Log.d(TAG, "Purchase Failure.");
						nativeactionFinished(callback_address, result.getResponse());
						// setWaitScreen(false);
						return;
					}
					if (result.getResponse() == IabHelper.BILLING_RESPONSE_RESULT_ITEM_ALREADY_OWNED)
					{
						// MsgAlertDialog(result.getMessage(), true);
						Log.d(TAG, "Already purchased");
						nativeactionFinished(callback_address, result.getResponse());
						return;
					}
					if (!purchase.getDeveloperPayload().equals(android.provider.Settings.Secure.ANDROID_ID))
					{
						// MsgAlertDialog("Error__Purchase_Payload_Cannot_Match", true);
						Log.d(TAG, "Purchase Failure.");
						nativeactionFinished(callback_address, Error__Purchase_Payload_Cannot_Match);
						return;
					}
					Log.d(TAG, "Purchase Successful.");
					nativeactionFinished(callback_address, result.getResponse());
				}
			};
		}
		// 消耗 callbackfunction
		if (meConsumeFinishedListener == null)
		{
			meConsumeFinishedListener = new IabHelper.OnConsumeFinishedListener()
			{
				public void onConsumeFinished(Purchase purchase, IabResult result)
				{
					Log.d(TAG, "Consumption finished. Purchase: " + purchase + ", result: " + result);
					if (mHelper == null)
					{
						MsgAlertDialog("Error__mHelper_Need_Set_Up");
						nativeactionFinished(callback_address, Error__mHelper_Need_Set_Up);
						return;
					}
					if (result.isFailure())
					{
						MsgAlertDialog(result.getMessage());
						Log.d(TAG, "Purchase Failure.");
						nativeactionFinished(callback_address, result.getResponse());
						return;
					}

					String[] iab_msgs1 = me_inventory.getSkuDetails(Consume_Sku_ID).getDescription().split("\\$");
					String[] iab_msgs2 = iab_msgs1[1].split("#");
					if (Integer.parseInt(iab_msgs1[0]) == 0)
						MsgAlertDialog("增加金幣 : " + iab_msgs2[0]);
					else if (Integer.parseInt(iab_msgs1[0]) == 1)
						MsgAlertDialog("增加遊戲幣 : " + iab_msgs2[0]);
					Log.d(TAG, "Consumption successful. Provisioning.");
					nativeactionFinished(callback_address, result.getResponse());
				}
			};
		}
	}

	// ************************************************************************************************
	// 增加查詢商品的ID
	// ************************************************************************************************
	public static void SetSkuID(final String SKU_ID)
	{
		if (additionalSkuList == null)
		{
			additionalSkuList = new ArrayList<String>();
		}
		Log.d(TAG, SKU_ID);
		additionalSkuList.add(SKU_ID);

	}

	// ************************************************************************************************
	// 1.檢查Activity
	// 2.檢查base64EncodedPublicKey
	// 3.建立mHelper
	// ************************************************************************************************
	public static int Init(final String PublicKeyd, final String PublicKeyb, final String PublicKeytwo)
	{
		if (me == null)
		{
			return Error__Need_To_Setup_IAB_On_Main_Java;
		}
		if (PublicKeyd == null || PublicKeyb == null || PublicKeytwo == null)
		{
			return Error__Need_TO_Set_PublicKey;
		}
		if (mHelper == null)
		{
			mHelper = new IabHelper(me, PublicKeyd + PublicKeyb + PublicKeytwo);
		}
		// Is_debug
		mHelper.enableDebugLogging(true);

		return Func_Sucess;
	}

	// ************************************************************************************************
	// 1.建立和商店系統的連線。
	// 2.確認手機系統是否支援應用程式內付費機制。
	// 3.執行完會call c funtion
	// ************************************************************************************************
	public static void startSetup()
	{
		mHelper.startSetup(meIabSetupFinishedListener);
	}

	// ************************************************************************************************
	// 1.向Google IAB服務取得指定商品的資訊
	// 2.執行完會call c funtion
	// ************************************************************************************************
	public static void QueryInventory()
	{
		mHelper.queryInventoryAsync(true, additionalSkuList, meGotInventoryListener);
	}

	// ************************************************************************************************
	// 1.檢查是否有這SKUID 資訊
	// 2.回傳 價格
	// ************************************************************************************************
	public static String GetSkuPrice(final String SKU_ID)
	{
		return me_inventory.hasDetails(SKU_ID) ? me_inventory.getSkuDetails(SKU_ID).getPrice() : "";
	}

	// ************************************************************************************************
	// 1.檢查是否有這SKUID 資訊
	// 2.回傳 描述
	// ************************************************************************************************
	public static String GetDescription(final String SKU_ID)
	{
		return me_inventory.hasDetails(SKU_ID) ? me_inventory.getSkuDetails(SKU_ID).getDescription() : "";
	}

	// ************************************************************************************************
	// 1.檢查是否有這SKUID 資訊
	// 2.回傳 是否已買
	// ************************************************************************************************
	public static boolean isPurchased(final String SKU_ID)
	{
		return me_inventory.hasDetails(SKU_ID) && me_inventory.hasPurchase(SKU_ID);
	}

	// ************************************************************************************************
	// 1.購買商品
	// 2.執行完會call c funtion
	// ************************************************************************************************
	public static void Purchase(final String SKU_ID)
	{
		Log.d(TAG, "Purchase Start ");
		final String id = android.provider.Settings.Secure.ANDROID_ID;

		me.runOnUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				mHelper.launchPurchaseFlow(me, SKU_ID, RC_REQUEST, mePurchaseFinishedListener, id);
			}
		});
	}

	// ************************************************************************************************
	// 1.檢查是否有這SKUID 資訊
	// 2.消耗商品
	// 3.執行完會call c funtion
	// ************************************************************************************************
	public static boolean ConsumeItem(final String SKU_ID)
	{
		if (me_inventory.hasPurchase(SKU_ID))
		{
			Consume_Sku_ID = SKU_ID;
			mHelper.consumeAsync(me_inventory.getPurchase(SKU_ID), meConsumeFinishedListener);
			return true;
		} else
		{
			return false;
		}
	}

	// ************************************************************************************************
	// 連線停止+free mHelper
	// ************************************************************************************************
	public static void DestroyStore()
	{
		Log.d(TAG, "Destroying helper.");
		if (mHelper != null)
		{
			mHelper.dispose();
			mHelper = null;
		}
	}

	// ************************************************************************************************
	// 檢查網路
	// ************************************************************************************************
	public static boolean checkInternet()
	{
		// get the ConnectivityManager for handling management of network
		// connections.
		ConnectivityManager cm = (ConnectivityManager) me_context.getSystemService(Context.CONNECTIVITY_SERVICE);
		return cm.getActiveNetworkInfo() != null && cm.getActiveNetworkInfo().isConnectedOrConnecting();

		// For 3G check
		// connecting.getNetworkInfo(ConnectivityManager.TYPE_MOBILE).isConnectedOrConnecting();
		// For WiFi Check
		// connecting.getNetworkInfo(ConnectivityManager.TYPE_WIFI).isConnectedOrConnecting();
	}

	// ************************************************************************************************
	// 顯示訊息(AlertDialog)
	// ************************************************************************************************
	public static void MsgAlertDialog(final String msg)
	{
		AlertDialog alertDialog = new AlertDialog.Builder(me).create();
		// alertDialog.setTitle("Title");
		alertDialog.setMessage(msg);
		// alertDialog.setIcon(R.drawable.tick);
		alertDialog.setCancelMessage(null);
		alertDialog.setButton("確定", new DialogInterface.OnClickListener()
		{
			public void onClick(DialogInterface dialog, int which)
			{
				// Write your code here to execute after dialog closed
				// Toast.makeText(me, "You clicked on OK", Toast.LENGTH_SHORT).show();
			}
		});
		// Showing Alert Message
		alertDialog.show();
	}

	// ************************************************************************************************
	// 顯示訊息(Toast)
	// ************************************************************************************************
	public static void MsgToast(final String msg, final boolean display_time)
	{
		final Handler handler = new Handler(Looper.getMainLooper());
		handler.post(new Runnable()
		{
			@Override
			public void run()
			{
				if (display_time)
					Toast.makeText(me_context, msg, Toast.LENGTH_LONG).show();
				else
					Toast.makeText(me_context, msg, Toast.LENGTH_SHORT).show();
			}
		});
	}
}
