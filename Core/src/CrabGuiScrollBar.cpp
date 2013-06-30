
#include "CrabGuiDefine.h"
#include "CrabGuiScrollBar.h"

#include "CrabGuiSystem.h"
#include "CrabGuiCanvas.h"
#include "CrabGuiEventHandle.h"

#include <string.h>		// memset


namespace CrabGui
{

	ScrollBar::ScrollBar(System* pSystem, UInt uID)
		: Window(pSystem, uID)
		, _pWndUp(0), _pWndBar(0), _pWndDown(0)
	{
		setVertical(True);

		_strNameUp	 = "up";
		_strNameBar	 = "bar";
		_strNameDown = "down";

		memset(&_stSI, 0, sizeof(_stSI));
	}

	ScrollBar::~ScrollBar(void)
	{
	}


	PCStr ScrollBar::TypeName = "ScrollBar";


	// 获得类型名
	PCStr ScrollBar::getTypeName()
	{
		return TypeName;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////
	// 渲染相关


	// 逝去时间
	Bool ScrollBar::update(UInt uElapse)
	{
		return Window::update(uElapse);
	}


	///////////////////////////////////////////////////////////////////////////////////////////////
	// 事件相关


	// 事件响应函数
	Bool ScrollBar::eventHandle(const EventDesc* pEvent)
	{
		if (pEvent->pWindow->getParent() == this)
		{
			switch (pEvent->uEvent)
			{
			case Event_MouseMove:
				{
					if (_pWndBar == pEvent->pWindow && pEvent->nParam[3] & CK_LButton &&	// 按住左键移动滑块
						_stSI.uDocSize > _stSI.uPageSize)
					{
						// 移动滑块
						if (isVertical())
						{
							Point ptBarPos = _ptBarDownPos;
							ptBarPos.y += _pSystem->getScreenMousePos().y - _ptScrMouseDownPos.y;

							Int16 nMin = _pWndUp->getPos().y   + _pWndUp->getSize().y;
							Int16 nMax = _pWndDown->getPos().y - _pWndBar->getSize().y;
							if (ptBarPos.y < nMin) ptBarPos.y = nMin;
							if (ptBarPos.y > nMax) ptBarPos.y = nMax;

							if (ptBarPos.y != _pWndBar->getPos().y)
							{
								_pWndBar->setPos(ptBarPos);

								Real rScale = (Real)(ptBarPos.y - nMin) / (Real)(nMax - nMin);
								_stSI.uPos = (UInt)(rScale * (_stSI.uDocSize - _stSI.uPageSize));
								_updateParent();
							}
						}
						else
						{
						}
					}
				}
				break;

			case Event_MouseDown:
				{
					if (_pWndBar == pEvent->pWindow)	// 开始移动滑块
					{
						_ptBarDownPos		= _pWndBar->getPos();				// 鼠标点击时，保存窗体坐标
						_ptScrMouseDownPos	= _pSystem->getScreenMousePos();	// 鼠标点击时，保存屏幕坐标
					}
				}
				break;

			case Event_MouseUp:
				{
					if (_pWndBar == pEvent->pWindow)	// 结束移动滑块
					{
					}
				}
				break;

			case Event_MouseClick:
				{
					if (_pWndUp == pEvent->pWindow)			// 点击向上滚动
					{
						_stSI.uPos = (_stSI.uPos > _stSI.uStep) ? (_stSI.uPos - _stSI.uStep) : 0;

						_updateBar();
						_updateParent();
					}
					else if (_pWndDown == pEvent->pWindow)	// 点击向下滚动
					{
						_stSI.uPos += _stSI.uStep;

						_updateBar();
						_updateParent();
					}
				}
			}
		}
		else if (pEvent->pWindow == this)
		{
			switch (pEvent->uEvent)
			{
			case Event_MouseDown:	// 点击在空白位置，滑块直接移动到点击位置
				{
					setDirtyRect(0);
				}
				break;
			}
		}

		return Window::eventHandle(pEvent);
	}


	///////////////////////////////////////////////////////////////////////////////////////////////
	// 窗体父子层级相关


	// 添加子窗体
	Bool ScrollBar::_insertChild(Window* pWndChild)
	{
		if (!Window::_insertChild(pWndChild))
		{
			return False;
		}

		if (_strNameUp == pWndChild->getName())
		{
			_pWndUp = pWndChild;
		}
		else if (_strNameBar == pWndChild->getName())
		{
			_pWndBar = pWndChild;
		}
		else if (_strNameDown == pWndChild->getName())
		{
			_pWndDown = pWndChild;
		}

		return True;
	}


	// 移除子窗体
	Bool ScrollBar::_removeChild(Window* pWndChild)
	{
		if (!Window::_removeChild(pWndChild))
		{
			return False;
		}

		if (_pWndUp == pWndChild)
		{
			_pWndUp = 0;
		}
		else if (_pWndBar == pWndChild)
		{
			_pWndBar = 0;
		}
		else if (_pWndDown == pWndChild)
		{
			_pWndDown = 0;
		}

		return True;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////
	// 滚动条响应相关


	// 父窗体垂直滚动设置事件
	Bool ScrollBar::onParentSetVertScroll(const ScrollInfo& stScroll)
	{
		if (isVertical())
		{
			_stSI = stScroll;

			if (_stSI.uPageSize >= _stSI.uDocSize)
			{
				memset(&_stSI, 0, sizeof(_stSI));
			}

			_updateBar();
			_updateParent();

			return True;
		}

		return False;
	}


	// 父窗体水平滚动设置事件
	Bool ScrollBar::onParentSetHorzScroll(const ScrollInfo& stScroll)
	{
		if (!isVertical())
		{
			_stSI = stScroll;

			// ... 

			return True;
		}

		return True;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////
	// 位属性相关


	// 设置垂直滚动条
	void ScrollBar::setVertical(Bool isVertical)
	{
		_setBitState(SBS_Vertical, isVertical);
		setDirtyRect(0);
	}

	// 是否垂直滚动条
	Bool ScrollBar::isVertical() const
	{
		return _getBitState(SBS_Vertical);
	}


	// 设置手动控制
	void ScrollBar::setManual(Bool isManual)
	{
		_setBitState(SBS_Manual, isManual);
	}

	// 是否手动控制
	Bool ScrollBar::isManual()
	{
		return _getBitState(SBS_Manual);
	}


	///////////////////////////////////////////////////////////////////////////////////////////////
	// 提供给编辑器的属性操作

	#define ScrollBarProperty(name)	Property name[] =								\
	{																				\
		{	"Vertical",			&_uBitState,		PT_Bit24,		""	},			\
		{	"Manual",			&_uBitState,		PT_Bit25,		""	},			\
	}


	// 获得属性个数
	UInt ScrollBar::getNumPropert()
	{
		UInt uNum = Window::getNumPropert();

		ScrollBarProperty(propList);
		uNum += sizeof(propList) / sizeof(propList[0]);

		return uNum;
	}


	// 设置窗体属性
	Bool ScrollBar::setPropert(const String& strKey, const String& strValue)
	{
		if (Window::setPropert(strKey, strValue))
			return True;

		ScrollBarProperty(propList);
		UInt uNum = sizeof(propList) / sizeof(propList[0]);

		return setPropertyImpl(propList, uNum, strKey, strValue);
	}


	// 获得窗体属性
	Bool ScrollBar::getPropert(UInt uIndex, String& strKey, String& strValue)
	{
		if (Window::getPropert(uIndex, strKey, strValue))
			return True;

		ScrollBarProperty(propList);
		UInt uNum = sizeof(propList) / sizeof(propList[0]);
		UInt uNumWindow = Window::getNumPropert();

		return getPropertyImpl(propList, uNum, uIndex - uNumWindow, strKey, strValue);
	}


	///////////////////////////////////////////////////////////////////////////////////////////////
	// 内部函数

	void ScrollBar::_updateBar()
	{
		if (isVertical())
		{
			// 计算滚动条高度
			Int16 nY1 = 0, nY2 = getSize().y;
			if (_pWndUp)	nY1 = _pWndUp->getPos().y + _pWndUp->getSize().y;
			if (_pWndDown)	nY2 = _pWndDown->getPos().y;

			Point ptPos  = _pWndBar->getPos();
			Point ptSize = _pWndBar->getSize();
			if (_stSI.uDocSize > _stSI.uPageSize)
			{
				if (_stSI.uPos > _stSI.uDocSize - _stSI.uPageSize)
					_stSI.uPos = _stSI.uDocSize - _stSI.uPageSize;

				Real rSizeScale = (Real)_stSI.uPageSize / (Real)_stSI.uDocSize;
				ptSize.y = (Int16)(rSizeScale * (nY2 - nY1));

				Real rPosScale  = (Real)_stSI.uPos / (Real)(_stSI.uDocSize - _stSI.uPageSize);
				ptPos.y  = (Int16)(rPosScale  * (nY2 - nY1 - ptSize.y)) + nY1;
			}
			else
			{
				ptPos.y  = nY1;
				ptSize.y = nY2 - nY1;
			}
			_pWndBar->setPos(ptPos);
			_pWndBar->setSize(ptSize);

			setDirtyRect(0);
		}
		else
		{
		}
	}


	void ScrollBar::_updateParent()
	{
		if (isVertical())
		{
			// 垂直滚动条事件
			EventDesc evt;
			evt.pWindow		= this;
			evt.uEvent		= Event_VertScroll;
			evt.nParam[0]	= (Int)_stSI.uPos;
			eventHandle(&evt);
		}
		else
		{
			// 水平滚动条事件
			EventDesc evt;
			evt.pWindow		= this;
			evt.uEvent		= Event_HorzScroll;
			evt.nParam[0]	= (Int)_stSI.uPos;
			eventHandle(&evt);
		}
	}
}
