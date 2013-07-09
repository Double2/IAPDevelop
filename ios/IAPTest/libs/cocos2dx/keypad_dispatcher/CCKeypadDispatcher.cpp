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

#include "CCKeypadDispatcher.h"
#include "support/data_support/ccCArray.h"

namespace   cocos2d {
	
	static CCKeypadDispatcher* s_KeypadDispatcher = NULL;
	//------------------------------------------------------------------
	//
	// CCKeypadDispatcher
	//
	//------------------------------------------------------------------
	CCKeypadDispatcher::CCKeypadDispatcher()
	: m_bLocked(false)
	, m_bToAdd(false)
	, m_bToRemove(false)
	{
    m_pDelegates = new KeypadDelegateArray;
		
    m_pHandlersToAdd    = ccCArrayNew(8);
    m_pHandlersToRemove = ccCArrayNew(8);
	}
	
	CCKeypadDispatcher::~CCKeypadDispatcher()
	{
		CC_SAFE_RELEASE(m_pDelegates);
		if (m_pHandlersToAdd)
		{
			ccCArrayFree(m_pHandlersToAdd);
		}
    
		if (m_pHandlersToRemove)
		{
			ccCArrayFree(m_pHandlersToRemove);
		}
	}
	
	CCKeypadDispatcher* CCKeypadDispatcher::sharedDispatcher()
	{
    if (! s_KeypadDispatcher)
    {
			s_KeypadDispatcher = new CCKeypadDispatcher;
    }
		
    return s_KeypadDispatcher;
	}
	
	void CCKeypadDispatcher::purgeSharedDispatcher()
	{
    if (s_KeypadDispatcher)
    {
			s_KeypadDispatcher->release();
			s_KeypadDispatcher = NULL;
    }
	}
	
	void CCKeypadDispatcher::removeDelegate(CCKeypadDelegate* pDelegate)
	{
    if (!pDelegate)
    {
			return;
    }
    if (! m_bLocked)
    {
			forceRemoveDelegate(pDelegate);
    }
    else
    {
			ccCArrayAppendValue(m_pHandlersToRemove, pDelegate);
			m_bToRemove = true;
    }
	}
	
	void CCKeypadDispatcher::addDelegate(CCKeypadDelegate* pDelegate)
	{
    if (!pDelegate)
    {
			return;
    }
		
    if (! m_bLocked)
    {
			forceAddDelegate(pDelegate);
    }
    else
    {
			ccCArrayAppendValue(m_pHandlersToAdd, pDelegate);
			m_bToAdd = true;
    }
	}
	
	void CCKeypadDispatcher::forceAddDelegate(CCKeypadDelegate* pDelegate)
	{
    CCKeypadHandler* pHandler = CCKeypadHandler::handlerWithDelegate(pDelegate);
		
    if (pHandler)
    {
			m_pDelegates->addObject(pHandler);
    }
	}
	
	void CCKeypadDispatcher::forceRemoveDelegate(CCKeypadDelegate* pDelegate)
	{
    CCKeypadHandler  *pHandler;
    CCMutableArray<CCKeypadHandler*>::CCMutableArrayIterator  iter;
		
    for (iter = m_pDelegates->begin(); iter != m_pDelegates->end(); ++iter)
    {
			pHandler = *iter;
			if (pHandler && pHandler->getDelegate() == pDelegate)
			{
				m_pDelegates->removeObject(pHandler);
				break;
			}
    }
	}
	
	bool CCKeypadDispatcher::dispatchKeypadMSG(ccKeypadMSGType nMsgType)
	{
    CCKeypadHandler  *pHandler;
    CCKeypadDelegate *pDelegate;
    CCMutableArray<CCKeypadHandler*>::CCMutableArrayIterator  iter;
		
    m_bLocked = true;
		
    if (m_pDelegates->count() > 0)
    {
			for (iter = m_pDelegates->begin(); iter != m_pDelegates->end(); ++iter)
			{
				CC_BREAK_IF(!(*iter));
				
				pHandler = *iter;
				pDelegate = pHandler->getDelegate();
				
				CCLog("dispatchKeypadMSG=%d",nMsgType);
				
				switch (nMsgType)
				{
					case kTypeBackClicked:
						pDelegate->keyBackClicked();
						break;
					case kTypeMenuClicked:
						pDelegate->keyMenuClicked();
						break;
					case kTypeUpClicked:
						pDelegate->keyUpClicked();
						break;
					case kTypeDownClicked:
						pDelegate->keyDownClicked();
						break;
					case kTypeLeftClicked:
						pDelegate->keyLeftClicked();
						break;
					case kTypeRightClicked:
						pDelegate->keyRightClicked();
						break;
					case kTypeCenterClicked:
						pDelegate->keyCenterClicked();
						break;
					case kTypeChannelUpClicked:
						pDelegate->keyChannelUpClicked();
						break;
					case kTypeChannelDownClicked:
						pDelegate->keyChannelDownClicked();
						break;
					case kTypeNum0Clicked:
						pDelegate->keyNum0Clicked();
						break;
					case kTypeNum1Clicked:
						pDelegate->keyNum1Clicked();
						break;
					case kTypeNum2Clicked:
						pDelegate->keyNum2Clicked();
						break;
					case kTypeNum3Clicked:
						pDelegate->keyNum3Clicked();
						break;
					case kTypeNum4Clicked:
						pDelegate->keyNum4Clicked();
						break;
					case kTypeNum5Clicked:
						pDelegate->keyNum5Clicked();
						break;
					case kTypeNum6Clicked:
						pDelegate->keyNum6Clicked();
						break;
					case kTypeNum7Clicked:
						pDelegate->keyNum7Clicked();
						break;
					case kTypeNum8Clicked:
						pDelegate->keyNum8Clicked();
						break;
					case kTypeNum9Clicked:
						pDelegate->keyNum9Clicked();
						break;
						
					case kTypeBackReleased:
						pDelegate->keyBackReleased();
						break;
					case kTypeMenuReleased:
						pDelegate->keyMenuReleased();
						break;
					case kTypeUpReleased:
						pDelegate->keyUpReleased();
						break;
					case kTypeDownReleased:
						pDelegate->keyDownReleased();
						break;
					case kTypeLeftReleased:
						pDelegate->keyLeftReleased();
						break;
					case kTypeRightReleased:
						pDelegate->keyRightReleased();
						break;
					case kTypeCenterReleased:
						pDelegate->keyCenterReleased();
						break;
					case kTypeChannelUpReleased:
						pDelegate->keyChannelUpReleased();
						break;
					case kTypeChannelDownReleased:
						pDelegate->keyChannelDownReleased();
						break;
					case kTypeNum0Released:
						pDelegate->keyNum0Released();
						break;
					case kTypeNum1Released:
						pDelegate->keyNum1Released();
						break;
					case kTypeNum2Released:
						pDelegate->keyNum2Released();
						break;
					case kTypeNum3Released:
						pDelegate->keyNum3Released();
						break;
					case kTypeNum4Released:
						pDelegate->keyNum4Released();
						break;
					case kTypeNum5Released:
						pDelegate->keyNum5Released();
						break;
					case kTypeNum6Released:
						pDelegate->keyNum6Released();
						break;
					case kTypeNum7Released:
						pDelegate->keyNum7Released();
						break;
					case kTypeNum8Released:
						pDelegate->keyNum8Released();
						break;
					case kTypeNum9Released:
						pDelegate->keyNum9Released();
						break;
						
					default:
						break;
				}
			}
    }
		
    m_bLocked = false;
    if (m_bToRemove)
    {
			m_bToRemove = false;
			for (unsigned int i = 0; i < m_pHandlersToRemove->num; ++i)
			{
				forceRemoveDelegate((CCKeypadDelegate*)m_pHandlersToRemove->arr[i]);
			}
			ccCArrayRemoveAllValues(m_pHandlersToRemove);
    }
		
    if (m_bToAdd)
    {
			m_bToAdd = false;
			for (unsigned int i = 0; i < m_pHandlersToAdd->num; ++i)
			{
				forceAddDelegate((CCKeypadDelegate*)m_pHandlersToAdd->arr[i]);
			}
			ccCArrayRemoveAllValues(m_pHandlersToAdd);
    }
		
    return true;
	}
	
}
