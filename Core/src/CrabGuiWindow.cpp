
#include "CrabGuiDefine.h"
#include "CrabGuiWindow.h"

#include "CrabGuiSystem.h"
#include "CrabGuiRenderer.h"
#include "CrabGuiQueue.h"
#include "CrabGuiCanvas.h"
#include "CrabGuiEventHandle.h"

#include <string.h>		// memset
#include <stdlib.h>		// atoi


namespace CrabGui
{

	Window::Window(System* pSystem, UInt uID)
		: _pSystem(pSystem), _uID(uID)
		, _pCanvas(0), _pQueueChilds(0), _pWndParent(0), _pEventHandle(0)
		, _uBitState(0), _u8ZLayer(ZLayerDefault), _u8TabStop(0)
	{
		setVisible(True);
		setEnable(True);
	}

	Window::~Window()
	{
		CrabDelete(_pQueueChilds);
		CrabDelete(_pCanvas);

		_pSystem->log(LL_High, "DestroyWindow: %s", _strName.getStrPtr());
	}


	PCStr Window::TypeName = "Window";


	// 获得类型名
	PCStr Window::getTypeName()
	{
		return TypeName;
	}


	// 获得创建此窗体的系统
	System* Window::getSystem()
	{
		return _pSystem;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////
	// 核心接口（慎用）


	// 设置窗体名字（允许重名）
	void Window::setName(PCStr pszName)
	{
		_strName = pszName;
	}


	// 获取窗体名字
	PCStr Window::getName()
	{
		return _strName.getStrPtr();
	}


	// 脏矩型
	void Window::setDirtyRect(const Rect* pDirtyRect)
	{
		Rect rcDirty;

		if (pDirtyRect)
		{
			// 使用逻辑层指定的脏矩形
			rcDirty = *pDirtyRect;
		}
		else
		{
			// 不指定矩型，整个窗体区域都是脏矩型
			rcDirty.setRect(0, 0, _ptSize.x, _ptSize.y);

			if (rcDirty.isZero())
				return;
		}

		if (_pCanvas)
		{
			// 当前窗体使用画布，脏矩形防越界（会当机，屏蔽下）
			if (rcDirty.x1 < 0)
				rcDirty.x1 = 0;
			if (rcDirty.x2 > _ptSize.x)
				rcDirty.x2 = _ptSize.x;
			if (rcDirty.y1 < 0)
				rcDirty.y1 = 0;
			if (rcDirty.y2 > _ptSize.y)
				rcDirty.y2 = _ptSize.y;

			_pCanvas->mergeDirtyRect(rcDirty);
		}
		else
		{
			// 当前窗体不使用画布，继续向上一层推送
			if (_pWndParent)
			{
				rcDirty.x1 += _ptPos.x;
				rcDirty.y1 += _ptPos.y;
				rcDirty.x2 += _ptPos.x;
				rcDirty.y2 += _ptPos.y;

				_pWndParent->setDirtyRect(& rcDirty);
			}
		}
	}


	// 设置是否使用画布（不得乱调用）
	void Window::setUseCanvas(Bool isTrue)
	{
		if (_ptSize.isZero() || isUseCanvas() == isTrue)
			return;

		if (isTrue)
		{
			_pCanvas = CrabNew(Canvas)();
			setDirtyRect(0);
		}
		else
		{
			CrabDelete(_pCanvas);
		}
	}


	// 是否使用画布
	Bool Window::isUseCanvas() const
	{
		return !!_pCanvas;
	}


	// 获得画布矩形窗体（用于渲染图片，文字）
	void Window::getCanvasRect(Rect& rcClient, const Rect& rcWindowOffset, const Point& ptCanvasBase)
	{
		rcClient.setRect(0, 0, _ptSize.x, _ptSize.y);
		rcClient.offset(rcWindowOffset);
		rcClient.offset(ptCanvasBase);
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////
	// 渲染相关


	// 获得鼠标指针
	UInt Window::getMouseCursor() const
	{
		return MC_Arrow;
	}


	// 获得窗体矩形
	void Window::getWindowRect(Rect& rcWindow) const
	{
		rcWindow.setRect(0, 0, _ptSize.x, _ptSize.y);
	}


	// 逝去时间
	Bool Window::update(UInt uElapse)
	{
		for (UInt i = 0; i < getNumChild(); ++i)
		{
			Window* pWndChild = getChild(i);
			pWndChild->update(uElapse);
		}

		return True;
	}


	// 窗体裁减
	void Window::setClipRect(Canvas* pCanvas, const Point& ptBase, const Rect& rcWindowClip)
	{
		Rect rcScissor(rcWindowClip, ptBase);

		if (rcWindowClip.isZero())
		{
			getWindowRect(rcScissor);
			rcScissor.offset(ptBase);
		}

		if (pCanvas)
		{
			const Rect rcDirty = pCanvas->getDirtyRect();

			rcScissor.x1 = Max(rcScissor.x1, rcDirty.x1);
			rcScissor.y1 = Max(rcScissor.y1, rcDirty.y1);
			rcScissor.x2 = Min(rcScissor.x2, rcDirty.x2);
			rcScissor.y2 = Min(rcScissor.y2, rcDirty.y2);
		}

		_pSystem->getRenderer()->setScissorRect(True, rcScissor);
	}


	// 准备渲染
	Bool Window::preRender(Canvas*& pCanvas, Point& ptBase)
	{
		if (0 == pCanvas && _pCanvas)
		{
			_pCanvas->setSize(isVisible(), _ptSize);
		}

		if (!isVisible())
			return False;

		// 当前窗体是画布窗体，切换到画布原点坐标
		if (0 == pCanvas && _pCanvas)
		{
			if (_pCanvas->getDirtyRect().isZero())
			{
				// 画布不脏，渲染完即结束
				_pCanvas->render(_ptPos, 0);
				return False;
			}

			// 脏矩形区域清空
			_pCanvas->bind();

			pCanvas = _pCanvas;
			ptBase.setZero();
		}

		// 根据窗体区域矩形，转换到画布上的矩形
		Rect rcWindowClip;
		getWindowRect(rcWindowClip);
		//rcWindow.offset(ptBase);
		setClipRect(pCanvas, ptBase, rcWindowClip);

		//if (pCanvas)
		//{
		//	 设置当前窗体裁减
		//	pCanvas->setClipRect(rcWindow);
		//}

		return True;
	}


	// 结束渲染
	void Window::postRender(Canvas* pCanvas, const Point& ptBase)
	{
		UInt uNumChild = getNumChild();
		for (UInt i = uNumChild; i > 0; --i)
		{
			Window* pWndChild = getChild(i - 1);

			// 获得子窗体矩形，转换到画布上的矩形
			// PS：不能使用客户区矩形，有可能客户区矩形外还有子窗体，会导致一些问题
			Rect rcChild;
			rcChild.x1 = ptBase.x + pWndChild->getPos().x;
			rcChild.y1 = ptBase.y + pWndChild->getPos().y;
			rcChild.x2 = rcChild.x1 + pWndChild->getSize().x;
			rcChild.y2 = rcChild.y1 + pWndChild->getSize().y;

			// 是否与脏矩形有交集
			if (pCanvas && !rcChild.isIntersect(pCanvas->getDirtyRect()))
				continue;

			// 渲染子窗体
			Point ptChildBase;
			ptChildBase.setPoint(ptBase.x + pWndChild->getPos().x, ptBase.y + pWndChild->getPos().y);
			pWndChild->render(pCanvas, ptChildBase);
		}

		if (_pCanvas && !_pCanvas->getDirtyRect().isZero())
		{
			_pCanvas->unbind();

			// 画布脏，先更新画布再渲染
			_pCanvas->render(_ptPos, 0);

			// 渲染完成，脏矩形清空
			_pCanvas->setDirtyRectZero();
		}
	}


	// 渲染
	void Window::render(Canvas* pCanvas, const Point& ptBase)
	{
		Point ptNewBase = ptBase;
		if (preRender(pCanvas, ptNewBase))
		{
			postRender(pCanvas, ptNewBase);
		}
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////
	// 窗体事件处理


	// 事件响应函数
	Bool Window::eventHandle(const EventDesc* pEvent)
	{
		// 逻辑层消息回调
		if (_pEventHandle && _pEventHandle->eventHandle(pEvent))
		{
			return True;
		}

		// 处理自己的消息
		if (pEvent->pWindow == this)
		{
			switch (pEvent->uEvent)
			{
			case Event_KeyDown:
			case Event_KeyUp:
			case Event_KeyChar:
				return !isKeyToParent();
			}
		}

		// 没有处理，转给父窗口处理
		if (_pWndParent && _pWndParent->eventHandle(pEvent))
		{
			return True;
		}

		// 无窗体响应消息
		return False;
	}


	// 设置逻辑层事件回调
	void Window::setEventHandle(EventHandle* pEventHandle)
	{
		_pEventHandle = pEventHandle;
	}


	// 获得逻辑层事件回调
	EventHandle* Window::getEventHandle() const
	{
		return _pEventHandle;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////
	// 窗体父子层级相关


	// 设置父窗体
	Bool Window::setParent(Window* pWndParent)
	{
		if (_pWndParent == pWndParent)
			return False;

		// 先从原先的 父窗体移除
		if (_pWndParent)
		{
			if (!_pWndParent->_removeChild(this))
				return False;

			setDirtyRect(0);
		}

		// 设置父窗体
		_pWndParent = pWndParent;

		// 添加到子窗体
		if (_pWndParent)
		{
			if (!_pWndParent->_insertChild(this))
				return False;

			setDirtyRect(0);
		}

		return True;
	}


	// 获取父窗体
	Window* Window::getParent() const
	{
		return _pWndParent;
	}


	// 获得子窗体个数
	UInt Window::getNumChild() const
	{
		return _pQueueChilds ? _pQueueChilds->getNum() : 0;
	}


	// 获得子窗体
	Window* Window::getChild(UInt uIndex) const
	{
		return _pQueueChilds ? (Window*)_pQueueChilds->getAt(uIndex) : 0;
	}


	// 获得子窗体
	Window* Window::getChild(PCStr pszName) const
	{
		String strName(pszName);

		if (_pQueueChilds)
		{
			for (UInt i = 0; i < _pQueueChilds->getNum(); ++i)
			{
				Window* pWndChild = (Window*)_pQueueChilds->getAt(i);
				if(strName == pWndChild->getName())
				{
					return pWndChild;
				}
			}
		} // end if

		return 0;
	}


	// 获得邻居窗体
	Window* Window::getSibling(PCStr pszName) const
	{
		if (_pWndParent)
		{
			return _pWndParent->getChild(pszName);
		}

		return 0;
	}


	// 激活窗体
	void Window::setFocus()
	{
		_pSystem->focusWindow(this);
	}


	// 移到同一层的顶端
	void Window::moveTop()
	{
		if (_pWndParent)
		{
			if (isZChangeable())
			{
				_pWndParent->_removeChild(this);
				_pWndParent->_insertChild(this);

				if (!isUseCanvas())
					setDirtyRect(0);
			}

			_pWndParent->moveTop();
		}
	}


	// 添加子窗体
	Bool Window::_insertChild(Window* pWndChild)
	{
		if (!_pQueueChilds)
		{
			_pQueueChilds = CrabNew(Queue)();
		}

        // 添加子窗体
		UInt uNumChild = getNumChild();
        for (UInt i = 0; i < uNumChild; ++i)
        {
			// 数字越大，层级越低
			if(pWndChild->getZLayer() > getChild(i)->getZLayer())
				continue;

			// 当前位置插入窗体
			return _pQueueChilds->insert(i, pWndChild);
        }

		// 直接插入在最后一个
		return _pQueueChilds->insert(uNumChild, pWndChild);
	}


	// 移除子窗体
	Bool Window::_removeChild(Window* pWndChild)
	{
		if (_pQueueChilds)
		{
			UInt uNumChild = getNumChild();
			for (UInt i = 0; i < uNumChild; ++i)
			{
				if(pWndChild == getChild(i))
				{
					return !!_pQueueChilds->remove(i);
				}
			}
		} // end if

		return False;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////
	// 属性相关


	// 获得窗体层级
	void Window::setZLayer(UInt8 u8ZLayer)
	{
		if(_u8ZLayer == u8ZLayer)
			return;

		_u8ZLayer = u8ZLayer;

		if(_pWndParent)
		{
			_pWndParent->_removeChild(this);
			_pWndParent->_insertChild(this);
		}
	}

    // 设置窗体层级
    UInt8 Window::getZLayer() const
	{
		return _u8ZLayer;
	}


	// 设置窗体位置
	void Window::setPos(const Point& ptPos)
	{
		if (ptPos.x == _ptPos.x && ptPos.y == _ptPos.y)
			return;

		if (_pCanvas)
		{
			// 直接变更坐标，无需重刷画布
			_ptPos = ptPos;
		}
		else
		{
			// 老位置需要重绘
			setDirtyRect(0);

			// 设置新坐标
			_ptPos = ptPos;

			// 新的位置需要重绘
			setDirtyRect(0);
		}
	}

	// 获得窗体位置
	const Point& Window::getPos() const
	{
		return _ptPos;
	}


	// 设置窗体大小
	void Window::setSize(const Point& ptSize)
	{
		if (ptSize.x == _ptSize.x && ptSize.y == _ptSize.y)
			return;

		if (_pCanvas)
		{
			// 画布重绘
			setDirtyRect(0);
		}
		else
		{
			// 老位置需要重绘
			setDirtyRect(0);

			// 设置新大小
			_ptSize = ptSize;

			// 字符位置脏，需要重新计算（涉及对齐方式）
			_strText.setWCharPosDirty(True);

			// 新的位置需要重绘
			setDirtyRect(0);
		}
	}

	// 获得窗体大小
	const Point& Window::getSize() const
	{
		return _ptSize;
	}


	// 设置文字
	void Window::setText(PCStr pszText)
	{
		_strText = pszText;
		setDirtyRect(0);
	}


	// 获取文字
	PCStr Window::getText() const
	{
		return _strText.getStrPtr();
	}


	// 鼠标命中测试 发送父窗体（用于非矩形窗体）
	Bool Window::hitTest(const Point& ptPos) const
	{
		// 窗体不显示，鼠标漏过，不命中
		if (!isVisible() || isMouseToParent())
			return False;

		// 如果是模态，必定命中，不漏过
		if (getZLayer() == ZLayerModal)
			return True;

		// 如果不在矩形范围内
		Rect rcWindow;
		getWindowRect(rcWindow);
		if (!rcWindow.isInRect(ptPos))
			return False;

		//// 画布窗体，全透漏过
		//if (_pCanvas)
		//{
		//	// 如果有画布，判断是否透明漏过，可以修改阀值
		//	Color nColor = _pCanvas->getColor(ptPos);
		//	if ((nColor & 0xFF000000) <= 0)
		//	{
		//		return False;
		//	}
		//}

		return True;
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////
	//// 滚动条相关（即将废除）


	//// 获得垂直滚动信息
	//Bool Window::getVertScroll(ScrollInfo& stScrollInfo)
	//{
	//	memset(&stScrollInfo, 0, sizeof(stScrollInfo));
	//	return False;
	//}

	//// 设置垂直滚动条信息
	//Bool Window::setVertScroll(UInt uVertPos)
	//{
	//	return False;
	//}


	//// 获得水平滚动条信息
	//Bool Window::getHorzScroll(ScrollInfo& stScrollInfo)
	//{
	//	memset(&stScrollInfo, 0, sizeof(stScrollInfo));
	//	return False;
	//}

	//// 设置水平滚动条信息
	//Bool Window::setHorzScroll(UInt uHorzPos)
	//{
	//	return False;
	//}


	//////////////////////////////////////////////////////////////////////////////////////////////////
	// 滚动条相关


	// 设置垂直滚动条
	Bool Window::setVertScroll(const ScrollInfo& stScroll)
	{
		UInt uNumChild = getNumChild();

		for (UInt i = 0; i < uNumChild; ++i)
		{
			Window* pWndChild = getChild(i);

			if (pWndChild->onParentSetVertScroll(stScroll))
			{
				return True;
			}
		}

		return False;
	}


	// 设置水平滚动条
	Bool Window::setHorzScroll(const ScrollInfo& stScroll)
	{
		UInt uNumChild = getNumChild();

		for (UInt i = 0; i < uNumChild; ++i)
		{
			Window* pWndChild = getChild(i);
			
			if (pWndChild->onParentSetHorzScroll(stScroll))
			{
				return True;
			}
		}

		return False;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////
	// 滚动条响应相关


	// 父窗体垂直滚动设置事件
	Bool Window::onParentSetVertScroll(const ScrollInfo& stScroll)
	{
		// ScrollBar 负责实现，默认不响应
		return False;
	}


	// 父窗体水平滚动设置事件
	Bool Window::onParentSetHorzScroll(const ScrollInfo& stScroll)
	{
		// ScrollBar 负责实现，默认不响应
		return False;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////
	// 位属性相关


	// 设置是否可见
	void Window::setVisible(Bool isVisible)
	{
		_setBitState(BS_Visible, isVisible);
		setDirtyRect(0);
	}

	// 设置是否可用
	void Window::setEnable(Bool isEnable)
	{
		_setBitState(BS_Enable, isEnable);
		setDirtyRect(0);
	}

	// 设置是否可移动
	void Window::setMovable(Bool isMovable)
	{
		_setBitState(BS_Movable, isMovable);
	}

	// 设置是否可拖曳
	void Window::setDragable(Bool isDragable)
	{
		_setBitState(BS_Dragable, isDragable);
	}

	// 设置是否可缩放
	void Window::setResizable(Bool isResizable)
	{
		_setBitState(BS_Resizable, isResizable);
	}

	// 设置是否允许变换层次
	void Window::setZChangeable(Bool isZChangeable)
	{
		_setBitState(BS_ZChangeable, isZChangeable);
	}

	// 设置是否按键事件 发送父窗体
	void Window::setKeyToParent(Bool isKeyToParent)
	{
		_setBitState(BS_KeyToParent, isKeyToParent);
	}

	// 设置是否鼠标事件 发送父窗体
	void Window::setMouseToParent(Bool isMouseToParent)
	{
		_setBitState(BS_MouseToParent, isMouseToParent);
	}


	// 是否可见
	Bool Window::isVisible() const
	{
		return _getBitState(BS_Visible);
	}

	// 是否可用
	Bool Window::isEnable() const
	{
		return _getBitState(BS_Enable);
	}

	// 是否可移动
	Bool Window::isMovable() const
	{
		return _getBitState(BS_Movable);
	}

	// 是否可拖曳
	Bool Window::isDragable() const
	{
		return _getBitState(BS_Dragable);
	}

	// 是否可缩放
	Bool Window::isResizable() const
	{
		return _getBitState(BS_Resizable);
	}

	// 是否允许变换层次
	Bool Window::isZChangeable() const
	{
		return _getBitState(BS_ZChangeable);
	}

	// 是否按键事件 发送父窗体
	Bool Window::isKeyToParent() const
	{
		return _getBitState(BS_KeyToParent);
	}

	// 是否鼠标事件 发送父窗体
	Bool Window::isMouseToParent() const
	{
		return _getBitState(BS_MouseToParent);
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////


	// 获得位状态属性
	Bool Window::_getBitState(UInt uBit) const
	{
		return !!(_uBitState & (1 << uBit));
	}

	// 设置位状态属性
	void Window::_setBitState(UInt uBit, Bool isTrue)
	{
		if (_getBitState(uBit) != isTrue)
		{
			if (isTrue)
			{
				_uBitState |= (1 << uBit);
			}
			else
			{
				_uBitState &= ~(1 << uBit);
			}

			//// 触发位状态变化事件
			//EventDesc evt;
			//evt.pWindow		= this;
			//evt.uEvent		= Event_StateChanged;
			//evt.nParam[0]	= isTrue;
			//eventHandle(&evt);
		}
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////
	// 提供给编辑器的属性操作


	#define WindowProperty(name)	Property name[] =								\
	{																				\
		{	"Name",				&_strName,			PT_String,		""	},			\
		{	"Pos",				&_ptPos,			PT_Point,		""	},			\
		{	"Size",				&_ptSize,			PT_Point,		""	},			\
																					\
		{	"Visible",			&_uBitState,		PT_Bit0,		""	},			\
		{	"Enable",			&_uBitState,		PT_Bit1,		""	},			\
		{	"Movable",			&_uBitState,		PT_Bit2,		""	},			\
		{	"Dragable",			&_uBitState,		PT_Bit3,		""	},			\
		{	"Resizable",		&_uBitState,		PT_Bit4,		""	},			\
		{	"ZChangeable",		&_uBitState,		PT_Bit5,		""	},			\
		{	"KeyToParent",		&_uBitState,		PT_Bit6,		""	},			\
		{	"MouseToParent",	&_uBitState,		PT_Bit7,		""	},			\
		{	"AcceptDrop",		&_uBitState,		PT_Bit8,		""	},			\
																					\
		{	"ZLayer",			&_u8ZLayer,			PT_UInt8,		""	},			\
		{	"TabStop",			&_u8TabStop,		PT_UInt8,		""	},			\
																					\
		{	"Text",				&_strText,			PT_String,		""	},			\
	}


	// 获得属性个数
	UInt Window::getNumPropert()
	{
		WindowProperty(propList);

		UInt uNum = sizeof(propList) / sizeof(propList[0]);
		return uNum;
	}


	// 设置窗体属性
	Bool Window::setPropert(const String& strKey, const String& strValue)
	{
		WindowProperty(propList);
		UInt uNum = sizeof(propList) / sizeof(propList[0]);

		Bool isSuccess = setPropertyImpl(propList, uNum, strKey, strValue);
		return isSuccess;
	}


	// 获得窗体属性
	Bool Window::getPropert(UInt uIndex, String& strKey, String& strValue)
	{
		WindowProperty(propList);
		UInt uNum = sizeof(propList) / sizeof(propList[0]);

		return getPropertyImpl(propList, uNum, uIndex, strKey, strValue);
	}


	static const Char PROPERTY_SPLIT = ',';


	// 根据属性数组 设置属性值
	Bool Window::setPropertyImpl(Property* pPropList, UInt uNumProp, const String& strKey, const String& strValue)
	{
		for (UInt i = 0; i < uNumProp; ++i)
		{
			if (strKey != pPropList[i].szKey)
				continue;

			Property* pProperty = &pPropList[i];
			switch (pProperty->eType)
			{
			case PT_String:
				{
					String* pString = (String*)(pProperty->pData);
					*pString = strValue.getStrPtr();
					return True;
				}

			case PT_Int8:
				{
					Int8* pInt8 = (Int8*)(pProperty->pData);
					*pInt8 = (Int8) strValue.toInt();
					return True;
				}

			case PT_UInt8:
				{
					UInt8* pUInt8 = (UInt8*)(pProperty->pData);
					*pUInt8 = (UInt8) strValue.toInt();
					return True;
				}

			case PT_Int16:
				{
					Int16* pInt16 = (Int16*)(pProperty->pData);
					*pInt16 = (Int16) strValue.toInt();
					return True;
				}

			case PT_UInt16:
				{
					UInt16* pUInt16 = (UInt16*)(pProperty->pData);
					*pUInt16 = (UInt16) strValue.toInt();
					return True;
				}

			case PT_Int:
				{
					Int* pInt = (Int*)(pProperty->pData);
					*pInt = strValue.toInt();
					return True;
				}

			case PT_UInt:
			case PT_Color:
				{
					UInt* pUInt = (UInt*)(pProperty->pData);
					*pUInt = (UInt) strValue.toUInt();
					return True;
				}

			case PT_Point:
				{
					Window::splitString(strValue, PROPERTY_SPLIT, (Int16*)(Point*)(pProperty->pData), 2);
					return True;
				}

			case PT_Rect:
				{
					Window::splitString(strValue, PROPERTY_SPLIT, (Int16*)(Rect*)(pProperty->pData), 4);
					return True;
				}

			case PT_Bit0:
			case PT_Bit1:
			case PT_Bit2:
			case PT_Bit3:
			case PT_Bit4:
			case PT_Bit5:
			case PT_Bit6:
			case PT_Bit7:
			case PT_Bit8:
			case PT_Bit9:
			case PT_Bit10:
			case PT_Bit11:
			case PT_Bit12:
			case PT_Bit13:
			case PT_Bit14:
			case PT_Bit15:
			case PT_Bit16:
			case PT_Bit17:
			case PT_Bit18:
			case PT_Bit19:
			case PT_Bit20:
			case PT_Bit21:
			case PT_Bit22:
			case PT_Bit23:
			case PT_Bit24:
			case PT_Bit25:
			case PT_Bit26:
			case PT_Bit27:
			case PT_Bit28:
			case PT_Bit29:
			case PT_Bit30:
			case PT_Bit31:
				{
					UInt* pBitState = (UInt*)(pProperty->pData);

					if (strValue.toInt())	// 位或1
					{
						*pBitState |= (1 << (pProperty->eType - PT_Bit0));
					}
					else	// 位与0
					{
						*pBitState &= ~(1 << (pProperty->eType - PT_Bit0));
					}
					return True;
				}
                    
            default:
                break;
			}
		}

		return False;
	}


	// 根据属性数组 获得属性值
	Bool Window::getPropertyImpl(Property* pPropList, UInt uNumProp, UInt uIndex, String& strKey, String& strValue)
	{
		if (uIndex >= uNumProp)
			return False;

		Property* pProperty = &pPropList[uIndex];

		// 属性KEY字符串
		strKey = pProperty->szKey;

		// 属性值字符串
		switch (pProperty->eType)
		{
		case PT_String:
			{
				strValue = *(String*)(pProperty->pData);
				return True;
			}

		case PT_Int8:
			{
				Int8* pInt8 = (Int8*)(pProperty->pData);
				strValue.format("%d", *pInt8);
				return True;
			}

		case PT_UInt8:
			{
				UInt8* pUInt8 = (UInt8*)(pProperty->pData);
				strValue.format("%u", *pUInt8);
				return True;
			}

		case PT_Int16:
			{
				Int16* pInt16 = (Int16*)(pProperty->pData);
				strValue.format("%d", *pInt16);
				return True;
			}

		case PT_UInt16:
			{
				UInt16* pUInt16 = (UInt16*)(pProperty->pData);
				strValue.format("%u", *pUInt16);
				return True;
			}

		case PT_Int:
			{
				Int* pInt = (Int*)(pProperty->pData);
				strValue.format("%d", *pInt);
				return True;
			}

		case PT_UInt:
		case PT_Color:
			{
				UInt* pUInt = (UInt*)(pProperty->pData);
				strValue.format("%u", *pUInt);
				return True;
			}

		case PT_Point:
			{
				Point* pPoint = (Point*)(pProperty->pData);
				strValue.format("%d%c%d", pPoint->x, PROPERTY_SPLIT, pPoint->y);
				return True;
			}

		case PT_Rect:
			{
				Rect* pRect = (Rect*)(pProperty->pData);
				strValue.format("%d%c%d%c%d%c%d", pRect->x1, PROPERTY_SPLIT, pRect->y1, PROPERTY_SPLIT, pRect->x2, PROPERTY_SPLIT, pRect->y2);
				return True;
			}

		case PT_Bit0:
		case PT_Bit1:
		case PT_Bit2:
		case PT_Bit3:
		case PT_Bit4:
		case PT_Bit5:
		case PT_Bit6:
		case PT_Bit7:
		case PT_Bit8:
		case PT_Bit9:
		case PT_Bit10:
		case PT_Bit11:
		case PT_Bit12:
		case PT_Bit13:
		case PT_Bit14:
		case PT_Bit15:
		case PT_Bit16:
		case PT_Bit17:
		case PT_Bit18:
		case PT_Bit19:
		case PT_Bit20:
		case PT_Bit21:
		case PT_Bit22:
		case PT_Bit23:
		case PT_Bit24:
		case PT_Bit25:
		case PT_Bit26:
		case PT_Bit27:
		case PT_Bit28:
		case PT_Bit29:
		case PT_Bit30:
		case PT_Bit31:
			{
				UInt* pBitState = (UInt*)(pProperty->pData);
				strValue = (*pBitState & (1 << (pProperty->eType - PT_Bit0))) ? "1" : "0";
				return True;
			}
                
        default:
            break;
		}

		return False;
	}


	// 分割
	void Window::splitString(const String& strText, Char cSplit, Int16* pArray, UInt uArraySize)
	{
		UInt uPos1 = 0;
		PCStr pszText = strText.getStrPtr();

		pArray[0] = atoi(pszText);
		uPos1 = strText.find(cSplit, uPos1);

		for (UInt i = 1; i < uArraySize; ++i)
		{
			if (uPos1 != String::NullPos)
			{
				pArray[i] = atoi(pszText + uPos1 + 1);
				uPos1 = strText.find(cSplit, uPos1 + 1);
			}
			else
			{
				pArray[i] = 0;
			}
		}
	}

}
