/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/
package tw.com.double2.iap;

import org.cocos2dx.lib.*;
import tw.com.double2.GoogleIAB;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

public class IapVersion2 extends Cocos2dxActivity {
	private Cocos2dxGLSurfaceView mGLView;

	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		// 增加這行

		mGLView = new Cocos2dxGLSurfaceView(this);
		mGLView.setEGLContextClientVersion(2);
		mGLView.setCocos2dxRenderer(new Cocos2dxRenderer());
		GoogleIAB.setup(this, getApplicationContext());
		// end 增加這行
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		if (GoogleIAB.mHelper == null)
			return;
		Log.d("ActivityResult", "onActivityResult(" + requestCode + ","
				+ resultCode + "," + data);

		if (!GoogleIAB.mHelper.handleActivityResult(requestCode, resultCode,
				data)) {
			// not handled, so handle it ourselves (here's where you'd
			// perform any handling of activity results not related to in-app
			// billing...
			super.onActivityResult(requestCode, resultCode, data);
		} else {
			Log.d("ActivityResult", "onActivityResult handled by IABUtil.");
		}
	}

	
	@Override
	protected void onPause() {
		super.onPause();
		mGLView.onPause();
	}

	@Override
	protected void onResume() {
		super.onResume();
		mGLView.onResume();
	}
	static {
		System.loadLibrary("game");
	}

}
