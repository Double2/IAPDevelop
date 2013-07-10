/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

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

#ifndef __CCKEYPAD_DELEGATE_H__
#define __CCKEYPAD_DELEGATE_H__


#include "CCObject.h"

namespace   cocos2d {

class CC_DLL CCKeypadDelegate
{
public:
	// The back key clicked
  virtual void keyBackClicked() {}

  // The menu key clicked. only avialble on wophone & android
  virtual void keyMenuClicked() {};
	
	// The back key clicked
  virtual void keyUpClicked() {}
  
  // The back key clicked
  virtual void keyDownClicked() {}
  
  // The back key clicked
  virtual void keyLeftClicked() {}
  
  // The back key clicked
  virtual void keyRightClicked() {}
  
  // The back key clicked
  virtual void keyCenterClicked() {}
	
	// The back key clicked
  virtual void keyChannelUpClicked() {}
	
	// The back key clicked
  virtual void keyChannelDownClicked() {}

	// The back key clicked
  virtual void keyNum0Clicked() {}

	// The back key clicked
  virtual void keyNum1Clicked() {}
	
	// The back key clicked
  virtual void keyNum2Clicked() {}
	
	// The back key clicked
  virtual void keyNum3Clicked() {}
	
	// The back key clicked
  virtual void keyNum4Clicked() {}
	
	// The back key clicked
  virtual void keyNum5Clicked() {}
	
	// The back key clicked
  virtual void keyNum6Clicked() {}
	
	// The back key clicked
  virtual void keyNum7Clicked() {}
	
	// The back key clicked
  virtual void keyNum8Clicked() {}
	
	// The back key clicked
  virtual void keyNum9Clicked() {}
	
	
	// The back key clicked
  virtual void keyBackReleased() {}
	
  // The menu key clicked. only avialble on wophone & android
  virtual void keyMenuReleased() {};
	
	// The back key clicked
  virtual void keyUpReleased() {}
  
  // The back key clicked
  virtual void keyDownReleased() {}
  
  // The back key clicked
  virtual void keyLeftReleased() {}
  
  // The back key clicked
  virtual void keyRightReleased() {}
  
  // The back key clicked
  virtual void keyCenterReleased() {}
	
	// The back key clicked
  virtual void keyChannelUpReleased() {}
	
	// The back key clicked
  virtual void keyChannelDownReleased() {}
	
	// The back key clicked
  virtual void keyNum0Released() {}
	
	// The back key clicked
  virtual void keyNum1Released() {}
	
	// The back key clicked
  virtual void keyNum2Released() {}
	
	// The back key clicked
  virtual void keyNum3Released() {}
	
	// The back key clicked
  virtual void keyNum4Released() {}
	
	// The back key clicked
  virtual void keyNum5Released() {}
	
	// The back key clicked
  virtual void keyNum6Released() {}
	
	// The back key clicked
  virtual void keyNum7Released() {}
	
	// The back key clicked
  virtual void keyNum8Released() {}
	
	// The back key clicked
  virtual void keyNum9Released() {}
};

/**
@brief
CCKeypadHandler
Object than contains the CCKeypadDelegate.
*/
class CC_DLL CCKeypadHandler : public CCObject
{
public:
    virtual ~CCKeypadHandler(void);

    /** delegate */
    CCKeypadDelegate* getDelegate();
    void setDelegate(CCKeypadDelegate *pDelegate);

    /** initializes a CCKeypadHandler with a delegate */
    virtual bool initWithDelegate(CCKeypadDelegate *pDelegate);

public:
    /** allocates a CCKeypadHandler with a delegate */
    static CCKeypadHandler* handlerWithDelegate(CCKeypadDelegate *pDelegate);

protected:
    CCKeypadDelegate* m_pDelegate;
};

} //namespace   cocos2d 

#endif // __CCKEYPAD_DELEGATE_H__
