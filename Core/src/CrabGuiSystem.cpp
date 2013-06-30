
#include "CrabGuiDefine.h"
#include "CrabGuiSystem.h"

// 核心类头文件
#include "CrabGuiQueue.h"
#include "CrabGuiRBTree.h"
#include "CrabGuiFontTexture.h"
#include "CrabGuiEventHandle.h"

// 渲染插件头文件
#include "CrabGuiRenderer.h"
#include "CrabGuiTexture.h"

// 解析插件头文件
#include "CrabGuiParser.h"
#include "CrabGuiImage.h"
#include "CrabGuiFont.h"

// 窗体头文件
#include "CrabGuiWindow.h"
#include "CrabGuiStatic.h"
#include "CrabGuiButton.h"
#include "CrabGuiEditbox.h"
#include "CrabGuiFrameWindow.h"
#include "CrabGuiScrollBar.h"
#include "CrabGuiScrollWindow.h"
#include "CrabGuiListbox.h"
#include "CrabGuiTreebox.h"
#include "CrabGuiCombobox.h"

// 调试用
#include "CrabGuiCanvas.h"

#include <string.h>
#include <locale.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>


//
//	待优化内容
//
//	窗体创建：待改成工厂方法，可以让逻辑层创建自己的窗体类型
//


namespace CrabGui
{

	System*	System::s_pSystem = 0;


	System::System(Renderer* pRenderer, Parser* pParser, PCStr pszLocale)
		: _eLogLevel(LL_Disable)
		, _pRenderer(pRenderer), _pParser(pParser)					// 插件接口
		, _pQueueCreate(0), _pQueueDestroy(0)						// 创建销毁队列
		, _pWndRoot(0), _pWndFocus(0), _pWndMouse(0), _pWndDrag(0)	// 快捷窗体
		, _uCtrlKeyState(0), _isEditMode(False)
		, _pFont(0), _pFontTexture(0)
	{
		// assert(s_pSystem);

		s_pSystem = this;

		if (pszLocale && pszLocale[0])
		{
			setlocale(LC_ALL, pszLocale);
		}

		// 创建资源快速索引
		_pRBTreeTexture		= CrabNew(RBTree)();
		_pRBTreePicture		= CrabNew(RBTree)();

		// 窗体创建销毁队列
		_pQueueCreate		= CrabNew(Queue)();
		_pQueueDestroy		= CrabNew(Queue)();
	}

	System::~System(void)
	{
		// 窗体销毁
		destroyWindow(_pWndRoot);

		// 清理窗体创建队列
		_createWindowEvent();
		CrabDelete(_pQueueCreate);
		// 清理窗体销毁队列
		_destroyWindowEvent();
		CrabDelete(_pQueueDestroy);

		// 清理切割图片资源
		destroyAllPicture();
		CrabDelete(_pRBTreePicture);
		// 清理字体
		destroyAllFont();
		// 清理纹理
		destroyAllTexture();
		CrabDelete(_pRBTreeTexture);
	}


	System& System::getSingleton()
	{
		return *s_pSystem;
	}

	System* System::getSingletonPtr()
	{
		return s_pSystem;
	}


	// 获得渲染器
    Renderer* System::getRenderer()
	{
		return _pRenderer;
	}


    // 获得解析器
    Parser* System::getParser()
	{
		return _pParser;
	}


	// 日志记录
	void System::log(LogLevel eLogLevel, PCStr pszLogFmt, ...)
	{
		if (eLogLevel > _eLogLevel)
			return;

		FILE* fp = fopen("CrabGui.log", "a+");
		if (fp)
		{
			Char szLog[256] = {0};

			// 时间
			struct tm *ptm = NULL;
			time_t tme;
			tme = time(0);
			ptm = localtime(&tme);
			sprintf(szLog, "[%d-%02d-%02d %02d:%02d:%02d] ", (ptm->tm_year + 1900),
					ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
			fwrite(szLog, strlen(szLog), sizeof(Char), fp);

			// 内容
			va_list argptr;
			va_start(argptr, pszLogFmt);
			UInt uLength = vsnprintf(szLog, 256 - 1, pszLogFmt, argptr);
			va_end(argptr);

			fseek(fp, 0, SEEK_END);

			fwrite(szLog, uLength, sizeof(Char), fp);
			fwrite("\n", strlen("\n"), sizeof(Char), fp);

			fflush(fp);
			fclose(fp);
		}
	}


	// 打印调试信息
	void System::print(PCStr pszDbgFmt, ...)
	{
		Char szDbg[256] = {0};

		// 内容
		va_list argptr;
		va_start(argptr, pszDbgFmt);
		vsnprintf(szDbg, 256 - 1, pszDbgFmt, argptr);
		va_end(argptr);

		_strDebug = szDbg;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////
	// 资源相关

	//
	//	以下待修改，改成资源引用计数
	//

	// 获得图片
	Image* System::acquireImage(const String& strName)
	{
		if (0 == strName.getLength())
			return 0;

		Image* pImage = _pParser->createImage();
		if (!pImage->loadFromFile(strName.getStrPtr()))
		{
			_pParser->destroyImage(pImage);
			pImage = 0;
		}

		return pImage;
	}


	// 释放图片
	void System::releaseImage(Image* pImage)
	{
		_pParser->destroyImage(pImage);
	}


	// 获得纹理
	Texture* System::acquireTexture(const String& strName)
	{
		// 待修改，资源读取效率化
		UInt uHash = strName.hashCode();
		Texture* pTexture = (Texture*)_pRBTreeTexture->find(uHash);

		if (pTexture)
		{
			// 已经创建的直接返回
			return pTexture;
		}

		pTexture = _pRenderer->createTexture();
		if (!pTexture->loadFromFile(strName.getStrPtr()))
		{
			// ...
			// write log

			_pRenderer->destroyTexture(pTexture);
			pTexture = 0;
		}

		_pRBTreeTexture->insert(uHash, pTexture);
		return pTexture;
	}


	// 释放纹理
	void System::releaseTexture(Texture* pTexture)
	{
		// ...
	}


	/// 销毁所有纹理
	void System::destroyAllTexture()
	{
		for (Texture* pTexture = (Texture*)_pRBTreeTexture->seekFirst(0); pTexture; pTexture = (Texture*)_pRBTreeTexture->seekNext(0))
		{
			_pRenderer->destroyTexture(pTexture);
		}

		_pRBTreeTexture->clear();
	}


	// 获得字体
	FontTexture* System::acquireFont(const String& strName)
	{
		// 默认字体
		if (0 == strName.getLength())
			return _pFontTexture;

		return _pFontTexture;
	}


	// 释放字体
	void System::releaseFont(FontTexture* pFont)
	{
		// ...
	}


	// 创建字体
	FontTexture* System::createFont(PCStr pszFileName)
	{
		if (!_pFont && !_pFontTexture)
		{
			_pFont = _pParser->createFont();
			_pFont->loadFont(pszFileName);

			_pFontTexture = CrabNew(FontTexture)(_pFont);
		}

		return _pFontTexture;
	}


	// 销毁所有字体
	void System::destroyAllFont()
	{
		CrabDelete(_pFontTexture);
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////
	// 矩形图片资源


	// 获得矩形图片
	Picture* System::acquirePicture(const String& strName)
	{
		UInt uHashCode = strName.hashCode();
		Picture* pPicture = (Picture*)_pRBTreePicture->find(uHashCode);

		return pPicture;
	}


	// 释放矩形图片
	void System::releasePicture(const String& strName)
	{
		// ...
	}


	// 创建矩形图片
	Picture* System::createPicture(const String& strName, const Rect& rcPicRect, const Rect& rcPicGrid, Color cPicColor)
	{
		UInt uHashCode = strName.hashCode();
		Picture* pPicture = (Picture*)_pRBTreePicture->find(uHashCode);

		if (pPicture)
			return pPicture;

		pPicture = CrabNew(Picture);
		pPicture->strName	= strName;
		pPicture->rcPicRect	= rcPicRect;
		pPicture->rcPicGrid	= rcPicGrid;
		pPicture->cPicColor	= cPicColor;
		
		_pRBTreePicture->insert(uHashCode, pPicture);

		return pPicture;
	}


	// 销毁矩形图片
	void System::destroyAllPicture()
	{
		Picture* pPicture = 0;
		for (pPicture = (Picture*)_pRBTreePicture->seekFirst(0); pPicture; pPicture = (Picture*)_pRBTreePicture->seekNext(0))
		{
			CrabDelete(pPicture);
		}
		_pRBTreePicture->clear();
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////
	// 外部事件传入


	// 鼠标坐标移动事件
	Bool System::injectMousePosition(const Point& ptPos)
	{
		// 记录屏幕坐标
		_ptScrMousePos = ptPos;

		Point ptBase = ptPos, ptChild = ptPos;
		Window* pWndMouse = 0;
		if (_pWndMouse && (_uCtrlKeyState & CK_LButton))
		{
			// 左键按住，以按下的窗体计算
			for (pWndMouse = _pWndMouse; pWndMouse; pWndMouse = pWndMouse->getParent())
			{
				ptChild.x -= pWndMouse->getPos().x;
				ptChild.y -= pWndMouse->getPos().y;
			}
			pWndMouse = _pWndMouse;

			// 可移动窗体
			if (_pWndMouse->isMovable())
			{
				Point ptNewPos = _ptWndDownPos;
				ptNewPos.x += ptPos.x - _ptScrMouseDownPos.x;
				ptNewPos.y += ptPos.y - _ptScrMouseDownPos.y;
				_pWndMouse->setPos(ptNewPos);
			}
			// 可拖曳窗体
			else if (_pWndMouse->isDragable())
			{
				// _pDragCanvas
			}
		}
		else if (!(_uCtrlKeyState & CK_LButton))
		{
			// 鼠标拾取
			pWndMouse = _pWndRoot;

			while (pWndMouse)
			{
				ptBase.x -= pWndMouse->getPos().x;
				ptBase.y -= pWndMouse->getPos().y;

				Window* pWndTemp = 0;

				for (UInt i = 0; i < pWndMouse->getNumChild(); ++i)
				{
					Window* pWndChild = pWndMouse->getChild(i);

					ptChild.setPoint(ptBase.x - pWndChild->getPos().x, ptBase.y - pWndChild->getPos().y);
					if (pWndChild->hitTest(ptChild))
					{
						pWndTemp = pWndChild;
						break;
					}
				}

				if (pWndTemp)
				{
					// 继续子窗体遍历
					pWndMouse = pWndTemp;
				}
				else
				{
					// 当前已经是命中窗体，无需再遍历
					break;
				}
			}

			// 根窗体，特殊处理
			if (_pWndRoot && pWndMouse == _pWndRoot && _pWndRoot->isMouseToParent())
			{
				pWndMouse = 0;
			}
		}

		// 当前帧和上一帧，比较拾取窗体
		if (pWndMouse != _pWndMouse)
		{
			if (_pWndMouse)
			{
				// 鼠标离开事件
				EventDesc evt;
				evt.pWindow		= _pWndMouse;
				evt.uEvent		= Event_HoverChanged;
				evt.nParam[0]	= False;
				_pWndMouse->eventHandle(&evt);
			}

			_pWndMouse = pWndMouse;

			if (_pWndMouse)
			{
				// 鼠标进入事件
				EventDesc evt;
				evt.pWindow		= _pWndMouse;
				evt.uEvent		= Event_HoverChanged;
				evt.nParam[0]	= True;
				_pWndMouse->eventHandle(&evt);
			}
		}

		if (_pWndMouse)
		{
			_ptWndMousePos = ptChild;

			// 鼠标移动事件
			EventDesc evt;
			evt.pWindow		= _pWndMouse;
			evt.uEvent		= Event_MouseMove;
			evt.nParam[0]	= _ptWndMousePos.x;
			evt.nParam[1]	= _ptWndMousePos.y;
			evt.nParam[3]	= _uCtrlKeyState;
			return _pWndMouse->eventHandle(&evt);
		}

		return False;
	}


	// 鼠标按键按下事件
	Bool System::injectMouseKeyDown(UInt uMouseKey)
	{
		switch (uMouseKey)
		{
		case MB_Left:		_uCtrlKeyState |= CK_LButton;		break;
		case MB_Right:		_uCtrlKeyState |= CK_RButton;		break;
		case MB_Middle:		_uCtrlKeyState |= CK_MButton;		break;
		}

		// 当前窗体获得焦点（可能是0）
		focusWindow(_pWndMouse);

		if (_pWndMouse)
		{
			if (_uCtrlKeyState & CK_LButton)
			{
				// 开始拖动，拖曳
				_ptWndDownPos		= _pWndMouse->getPos();
				_ptScrMouseDownPos	= _ptScrMousePos;
				_ptWndMouseDownPos	= _ptWndMousePos;
			}

			// 鼠标按下事件
			EventDesc evt;
			evt.pWindow		= _pWndMouse;
			evt.uEvent		= Event_MouseDown;
			evt.nParam[0]	= _ptWndMousePos.x;
			evt.nParam[1]	= _ptWndMousePos.y;
			evt.nParam[2]	= uMouseKey;
			evt.nParam[3]	= _uCtrlKeyState;
			_pWndFocus->eventHandle(&evt);

			return True;
		}

		return False;
	}


	// 鼠标按键抬起事件
	Bool System::injectMouseKeyUp(UInt uMouseKey)
	{
		switch (uMouseKey)
		{
		case MB_Left:		_uCtrlKeyState &= ~CK_LButton;		break;
		case MB_Right:		_uCtrlKeyState &= ~CK_RButton;		break;
		case MB_Middle:		_uCtrlKeyState &= ~CK_MButton;		break;
		}

		if (_pWndFocus)
		{
			// 鼠标抬起事件
			EventDesc evt;
			evt.pWindow		= _pWndFocus;
			evt.uEvent		= Event_MouseUp;
			evt.nParam[0]	= _ptWndMousePos.x;
			evt.nParam[1]	= _ptWndMousePos.y;
			evt.nParam[2]	= uMouseKey;
			evt.nParam[3]	= _uCtrlKeyState;
			_pWndFocus->eventHandle(&evt);

			Rect rcWnd;
			rcWnd.setRect(0, 0, _pWndFocus->getSize().x, _pWndFocus->getSize().y);

			if (_pWndFocus->isEnable() && rcWnd.isInRect(_ptWndMousePos))	// 如果是在窗体范围内，触发点击事件
			{
				// 点击事件
				evt.uEvent = Event_MouseClick;
				_pWndFocus->eventHandle(&evt);
			}

			return True;
		}

		return False;
	}


	// 鼠标滚轮事件
	Bool System::injectMouseWheel(const Point& ptScroll)
	{
		if (_pWndFocus)
		{
			// 滚轮事件
			EventDesc evt;
			evt.pWindow		= _pWndFocus;
			evt.uEvent		= Event_MouseScroll;
			evt.nParam[0]	= ptScroll.x;
			evt.nParam[1]	= ptScroll.y;
			evt.nParam[3]	= _uCtrlKeyState;
			return _pWndFocus->eventHandle(&evt);
		}

		return False;
	}


	// 键盘按键按下事件
	Bool System::injectKeyDown(UInt uKey)
	{
		switch (uKey)
		{
		case KB_Ctrl:		_uCtrlKeyState |= CK_Ctrl;		break;
		case KB_Alt:		_uCtrlKeyState |= CK_Alt;		break;
		case KB_Shift:		_uCtrlKeyState |= CK_Shift;		break;
		}

		if (_pWndFocus)
		{
			// 按键按下事件
			EventDesc evt;
			evt.pWindow		= _pWndFocus;
			evt.uEvent		= Event_KeyDown;
			evt.nParam[0]	= uKey;
			evt.nParam[3]	= _uCtrlKeyState;
			return _pWndFocus->eventHandle(&evt);
		}

		return False;
	}


	// 键盘按键抬起事件
	Bool System::injectKeyUp(UInt uKey)
	{
		switch (uKey)
		{
		case KB_Ctrl:		_uCtrlKeyState &= ~CK_Ctrl;		break;
		case KB_Alt:		_uCtrlKeyState &= ~CK_Alt;		break;
		case KB_Shift:		_uCtrlKeyState &= ~CK_Shift;	break;
		}

		if (_pWndFocus)
		{
			// 按键按下事件
			EventDesc evt;
			evt.pWindow		= _pWndFocus;
			evt.uEvent		= Event_KeyUp;
			evt.nParam[0]	= uKey;
			evt.nParam[3]	= _uCtrlKeyState;
			return _pWndFocus->eventHandle(&evt);
		}

		return False;
	}


	// 键盘字符事件
	Bool System::injectKeyChar(UInt uKeyChar)
	{
		if (_pWndFocus)
		{
			// 按键按下事件
			EventDesc evt;
			evt.pWindow		= _pWndFocus;
			evt.uEvent		= Event_KeyChar;
			evt.nParam[0]	= uKeyChar;
			evt.nParam[3]	= _uCtrlKeyState;
			return _pWndFocus->eventHandle(&evt);
		}

		return False;
	}


	// 窗体大小变化事件
	Bool System::injectResize(const Point& ptSize)
	{
		if (_pWndRoot)
		{
			_pWndRoot->setSize(ptSize);

			return True;
		}

		return False;
	}


	// 时间流逝事件（单位：毫秒）
	Bool System::injectTimeElapse(UInt uElapse)
	{
		_createWindowEvent();
		_destroyWindowEvent();

		if (_pWndRoot)
		{
			_pWndRoot->update(uElapse);
			return True;
		}

		return False;
	}


	// 渲染GUI
	Bool System::renderGui()
	{
		_pRenderer->beginRender();

		if (_pWndRoot)
		{
			Point ptBase = _pWndRoot->getPos();
			_pWndRoot->render(0, ptBase);
		}

		if (_strDebug.getLength() > 0)
		{
			// 控制台渲染
			Rect rcText;
			_pWndRoot->getWindowRect(rcText);

			Canvas::drawText("", 16, _strDebug, rcText, 16, AT_LeftTop, 0, 0xFFFFFFFF, 0, AOP_Default);
		}

		_pRenderer->endRender();

		return True;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////
	// 窗体相关


	// 创建窗体
	Window* System::createWindow(PCStr pszWndType)
	{
		static UInt uWndID = 0;
		++ uWndID;

		Window* pWindow = 0;

		if (0 == strcmp(pszWndType, Window::TypeName))
		{
			pWindow = CrabNew(Window)(this, uWndID);
		}
		else if (0 == strcmp(pszWndType, Static::TypeName))
		{
			pWindow = CrabNew(Static)(this, uWndID);
		}
		else if (0 == strcmp(pszWndType, Button::TypeName))
		{
			pWindow = CrabNew(Button)(this, uWndID);
		}
		else if (0 == strcmp(pszWndType, Editbox::TypeName))
		{
			pWindow = CrabNew(Editbox)(this, uWndID);
		}
		else if (0 == strcmp(pszWndType, FrameWindow::TypeName))
		{
			pWindow = CrabNew(FrameWindow)(this, uWndID);
		}
		else if (0 == strcmp(pszWndType, ScrollBar::TypeName))
		{
			pWindow = CrabNew(ScrollBar)(this, uWndID);
		}
		else if (0 == strcmp(pszWndType, ScrollWindow::TypeName))
		{
			pWindow = CrabNew(ScrollWindow)(this, uWndID);
		}
		else if (0 == strcmp(pszWndType, Listbox::TypeName))
		{
			pWindow = CrabNew(Listbox)(this, uWndID);
		}
		else if (0 == strcmp(pszWndType, Treebox::TypeName))
		{
			pWindow = CrabNew(Treebox)(this, uWndID);
		}
		else if (0 == strcmp(pszWndType, Combobox::TypeName))
		{
			pWindow = CrabNew(Combobox)(this, uWndID);
		}

		// 加入创建队列，下一帧统一消息派发
		if (pWindow)
		{
			_pQueueCreate->insertEnd(pWindow);
		}

		return pWindow;
	}


	// 销毁窗体
	void System::destroyWindow(Window* pWindow)
	{
		for (UInt i = pWindow->getNumChild(); i > 0; --i)
		{
			Window* pWndChild = pWindow->getChild(i - 1);
			destroyWindow(pWndChild);
		}

		pWindow->setParent(0);
		_pQueueDestroy->insertEnd(pWindow);
	}


	// 更改窗体类型（重新创建一个新的）
	Window* System::changeWindowType(Window* pWnd, PCStr pszWndType)
	{
		Window* pWndNew = createWindow(pszWndType);
		if (0 == pWndNew)
			return 0;

		// 复制相同的名字和父窗体
		pWndNew->setName(pWnd->getName());
		pWndNew->setParent(pWnd->getParent());

		// 复制属性
		String strKey, strValue;
		for (UInt i = 0; i < pWnd->getNumPropert(); ++i)
		{
			if (pWnd->getPropert(i, strKey, strValue))
			{
				pWndNew->setPropert(strKey, strValue);
			}
		}

		// 子窗体挂到新的窗体上
		for (UInt i = pWnd->getNumChild(); i > 0; --i)
		{
			Window* pWndChild = pWnd->getChild(i - 1);
			pWndChild->setParent(pWndNew);
		}

		// 原窗体销毁
		destroyWindow(pWnd);

		return pWndNew;
	}


	// 激活窗体
	void System::focusWindow(Window* pWndFocus)
	{
		if (_pWndFocus == pWndFocus)
			return;

		if (_pWndFocus)
		{
			// 失焦事件
			EventDesc evt;
			evt.pWindow		= _pWndFocus;
			evt.uEvent		= Event_FocusChanged;
			evt.nParam[0]	= False;
			_pWndFocus->eventHandle(&evt);
		}

		_pWndFocus = pWndFocus;

		if (_pWndFocus)
		{
			_pWndFocus->moveTop();

			// 聚焦事件
			EventDesc evt;
			evt.pWindow		= _pWndFocus;
			evt.uEvent		= Event_FocusChanged;
			evt.nParam[0]	= True;
			_pWndFocus->eventHandle(&evt);
		}
	}


	// 设置根窗体
	void System::setRootWindow(Window* pWndRoot)
	{
		_pWndRoot = pWndRoot;
	}


	// 获得根窗体
	Window* System::getRootWindow()
	{
		return _pWndRoot;
	}


	// 获得当前激活窗体
	Window* System::getFocusWindow()
	{
		return _pWndFocus;
	}


	// 获得鼠标拾取窗体
	Window* System::getMouseWindow()
	{
		return _pWndMouse;
	}


	// 获得拖拽窗体
	Window* System::getDragWindow()
	{
		return _pWndDrag;
	}


	// 获得鼠标指针
	MouseCursor System::getMouseCursor()
	{
		return MC_Arrow;
	}


	// 复制窗体属性
	UInt System::copyProperty(Window* pWndDst, Window* pWndSrc)
	{
		String strKey, strValue;
		UInt uNum = pWndSrc->getNumPropert();

		for (UInt i = 0; i < uNum; ++i)
		{
			pWndSrc->getPropert(i, strKey, strValue);
			pWndDst->setPropert(strKey, strValue);
		}

		return uNum;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////
	// 属性相关


	// 创建窗体事件
	void System::_createWindowEvent()
	{
		for (UInt i = 0; i < _pQueueCreate->getNum(); ++i)
		{
			Window* pWindow = (Window*)_pQueueCreate->getAt(i);

			// 鼠标离开事件
			EventDesc evt;
			evt.pWindow		= pWindow;
			evt.uEvent		= Event_WindowCreate;
			pWindow->eventHandle(&evt);
		}

		_pQueueCreate->clear();
	}


	// 销毁窗体事件
	void System::_destroyWindowEvent()
	{
		for (UInt i = 0; i < _pQueueDestroy->getNum(); ++i)
		{
			Window* pWindow = (Window*)_pQueueDestroy->getAt(i);

			// 鼠标离开事件
			EventDesc evt;
			evt.pWindow		= pWindow;
			evt.uEvent		= Event_WindowDestroy;
			pWindow->eventHandle(&evt);

			CrabDelete(pWindow);
			_pQueueDestroy->setAt(i, 0);
		}

		_pQueueDestroy->clear();
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////
	// 属性相关


	const Point& System::getScreenMousePos() const
	{
		return _ptScrMousePos;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////
	// 编辑器相关


	// 设置编辑器模式
	void System::setEditMode(Bool isEditMode)
	{
		_isEditMode = isEditMode;
	}


	// 是否是编辑器模式
	Bool System::isEditMode()
	{
		return _isEditMode;
	}

}
