
#include "CrabGuiDefine.h"
#include "CrabGuiButton.h"

#include "CrabGuiSystem.h"
#include "CrabGuiCanvas.h"
#include "CrabGuiEventHandle.h"

#include <string.h>		// memset


namespace CrabGui
{

	Button::Button(System* pSystem, UInt uID)
		: Window(pSystem, uID)
		, _eButtonState(BS_Normal)
		, _ePicAlphaOp(AO_Blend)
		, _uFontSize(14), _eFontAlphaOp(AO_Blend), _eFontAlignType(AT_Center), _uLineHeight(0)
	{
		memset(_cPicColor, 0xFFFFFFFF, sizeof(_cPicColor));
	}

	Button::~Button()
	{
	}


	PCStr Button::TypeName = "Button";


	// 获得类型名
	PCStr Button::getTypeName()
	{
		return TypeName;
	}


	// 设置窗体矩形
	void Button::setPicRect(ButtonState eBS, const Rect& rcPicRect, const Rect& rcPicGrid)
	{
		_rcPicRect[eBS] = rcPicRect;
		_rcPicGrid[eBS] = rcPicGrid;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////
	// 位属性相关


	// 设置是否可用
	void Button::setEnable(Bool isEnable)
	{
		if (isEnable == Window::isEnable())
			return;

		if (isEnable)
		{
			if (_pSystem->getMouseWindow() == this)
			{
				_eButtonState = BS_Hover;
			}
			else
			{
				_eButtonState = BS_Normal;
			}
		}
		else
		{
			_eButtonState = BS_Disable;
		}

		Window::setEnable(isEnable);
	}


	///////////////////////////////////////////////////////////////////////////////////////////////
	// 渲染相关


	// 渲染
	void Button::render(Canvas* pCanvas, const Point& ptBase)
	{
		Point ptCanvasBase = ptBase;
		if (preRender(pCanvas, ptCanvasBase))
		{
			// 渲染底图
			{
				Rect rcPicCanvas;
				getCanvasRect(rcPicCanvas, _rcPicOffset, ptCanvasBase);

				Canvas::drawGridPic(_strPicFile, _rcPicRect[_eButtonState], _rcPicGrid[_eButtonState], rcPicCanvas, 0xFFFFFFFF, _ePicAlphaOp);
			}

			// 渲染文字
			if (_strText.getLength() > 0)
			{
				Rect rcFontCanvas;
				getCanvasRect(rcFontCanvas, _rcFontOffset, ptCanvasBase);

				Canvas::drawText(_strFontName, _uFontSize, _strText, rcFontCanvas, _uLineHeight, _eFontAlignType, Canvas::TFB_TextWrap, _cFontColor[_eButtonState], 0, _eFontAlphaOp);
			}

			postRender(pCanvas, ptCanvasBase);
		}
	}


	///////////////////////////////////////////////////////////////////////////////////////////////
	// 事件相关


	// 事件响应函数
	Bool Button::eventHandle(const EventDesc* pEvent)
	{
		if (pEvent->pWindow == this)
		{
			if (isEnable())
			{
				switch (pEvent->uEvent)
				{
				case Event_HoverChanged:
					{
						_eButtonState = pEvent->nParam[0] ? BS_Hover : BS_Normal;
						setDirtyRect(0);
					}
					break;

				case Event_MouseDown:
					{
						_eButtonState = BS_Pushed;
						setDirtyRect(0);
					}
					break;

				case Event_MouseUp:
					{
						Point ptPos((Int16)pEvent->nParam[0], (Int16)pEvent->nParam[1]);

						_eButtonState = hitTest(ptPos) ? BS_Hover : BS_Normal;
						setDirtyRect(0);
					}
					break;
				}
			}
		}

		return Window::eventHandle(pEvent);
	}


	///////////////////////////////////////////////////////////////////////////////////////////////
	// 提供给编辑器的属性操作

	#define ButtonProperty(name)	Property name[] =										\
	{																						\
		{	"PicFile",				&_strPicFile,				PT_String,		""	},		\
		{	"PicAlphaOp",			&_ePicAlphaOp,				PT_Int,			""	},		\
		{	"NormalPicColor",		&_cPicColor[BS_Normal],		PT_Color,		""	},		\
		{	"NormalPicRect",		&_rcPicRect[BS_Normal],		PT_Rect,		""	},		\
		{	"NormalPicGrid",		&_rcPicGrid[BS_Normal],		PT_Rect,		""	},		\
		{	"HoverPicColor",		&_cPicColor[BS_Hover],		PT_Color,		""	},		\
		{	"HoverPicRect",			&_rcPicRect[BS_Hover],		PT_Rect,		""	},		\
		{	"HoverPicGrid",			&_rcPicGrid[BS_Hover],		PT_Rect,		""	},		\
		{	"PushedPicColor",		&_cPicColor[BS_Pushed],		PT_Color,		""	},		\
		{	"PushedPicRect",		&_rcPicRect[BS_Pushed],		PT_Rect,		""	},		\
		{	"PushedPicGrid",		&_rcPicGrid[BS_Pushed],		PT_Rect,		""	},		\
		{	"DisablePicColor",		&_cPicColor[BS_Disable],	PT_Color,		""	},		\
		{	"DisablePicRect",		&_rcPicRect[BS_Disable],	PT_Rect,		""	},		\
		{	"DisablePicGrid",		&_rcPicGrid[BS_Disable],	PT_Rect,		""	},		\
		{	"PicOffset",			&_rcPicOffset,				PT_Rect,		""	},		\
																							\
		{	"FontName",				&_strFontName,				PT_String,		""	},		\
		{	"FontSize",				&_uFontSize,				PT_UInt,		""	},		\
		{	"NormalFontColor",		&_cFontColor[BS_Normal],	PT_Color,		""	},		\
		{	"HoverFontColor",		&_cFontColor[BS_Hover],		PT_Color,		""	},		\
		{	"PushedFontColor",		&_cFontColor[BS_Pushed],	PT_Color,		""	},		\
		{	"DisableFontColor",		&_cFontColor[BS_Disable],	PT_Color,		""	},		\
		{	"FontAlphaOp",			&_eFontAlphaOp,				PT_Int,			""	},		\
		{	"FontAlignType",		&_eFontAlignType,			PT_Int,			""	},		\
		{	"FontOffset",			&_rcFontOffset,				PT_Rect,		""	},		\
		{	"LineHeight",			&_uLineHeight,				PT_UInt,		""	},		\
	}


	// 获得属性个数
	UInt Button::getNumPropert()
	{
		UInt uNum = Window::getNumPropert();

		ButtonProperty(propList);
		uNum += sizeof(propList) / sizeof(propList[0]);

		return uNum;
	}


	// 设置窗体属性
	Bool Button::setPropert(const String& strKey, const String& strValue)
	{
		if (Window::setPropert(strKey, strValue))
			return True;

		ButtonProperty(propList);
		UInt uNum = sizeof(propList) / sizeof(propList[0]);

		return setPropertyImpl(propList, uNum, strKey, strValue);
	}


	// 获得窗体属性
	Bool Button::getPropert(UInt uIndex, String& strKey, String& strValue)
	{
		if (Window::getPropert(uIndex, strKey, strValue))
			return True;

		ButtonProperty(propList);
		UInt uNum = sizeof(propList) / sizeof(propList[0]);
		UInt uNumWindow = Window::getNumPropert();

		return getPropertyImpl(propList, uNum, uIndex - uNumWindow, strKey, strValue);
	}


}	//	__CrabGuiButton_h__
