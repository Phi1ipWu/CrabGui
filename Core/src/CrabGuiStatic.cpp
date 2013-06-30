
#include "CrabGuiDefine.h"
#include "CrabGuiStatic.h"

#include "CrabGuiSystem.h"
#include "CrabGuiCanvas.h"


namespace CrabGui
{

	Static::Static(System* pSystem, UInt uID)
		: Window(pSystem, uID)
		, _ePicAlphaOp(AO_Blend), _cPicColor(0xFFFFFFFF)
		, _uFontSize(14), _cFontColor(0xFF000000), _cFontShadowColor(0x0), _eFontAlphaOp(AO_Blend), _eFontAlignType(AT_Center), _uLineHeight(0)
	{
	}

	Static::~Static(void)
	{
	}


	PCStr Static::TypeName = "Static";


	// 获得类型名
	PCStr Static::getTypeName()
	{
		return TypeName;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////
	// 属性相关


	AlignType Static::getTextAlignType() const
	{
		return _eFontAlignType;
	}


	void Static::setTextAlignType(AlignType eAlignType)
	{
		if (_eFontAlignType != eAlignType)
		{
			_eFontAlignType = eAlignType;

			// 设置文字位置脏，重新按排序方式排
			_strText.setWCharPosDirty(True);
			setDirtyRect(0);
		}
	}


	///////////////////////////////////////////////////////////////////////////////////////////////
	// 渲染相关


	// 渲染
	void Static::render(Canvas* pCanvas, const Point& ptBase)
	{
		Point ptCanvasBase = ptBase;
		if (preRender(pCanvas, ptCanvasBase))
		{
			// 渲染底图
			{
				Rect rcPicCanvas;
				getCanvasRect(rcPicCanvas, _rcPicOffset, ptCanvasBase);

				Canvas::drawGridPic(_strPicFile, _rcPicRect, _rcPicGrid, rcPicCanvas, _cPicColor, _ePicAlphaOp);
			}

			// 渲染文字
			if (_strText.getLength() > 0)
			{
				Rect rcFontCanvas;
				getCanvasRect(rcFontCanvas, _rcFontOffset, ptCanvasBase);

				UInt uTextFormat = Canvas::TFB_TextWrap | Canvas::TFB_Translate;
				Canvas::drawText(_strFontName, _uFontSize, _strText, rcFontCanvas, _uLineHeight, _eFontAlignType, uTextFormat, _cFontColor, _cFontShadowColor, _eFontAlphaOp);
			}

			postRender(pCanvas, ptCanvasBase);
		}
	}


	///////////////////////////////////////////////////////////////////////////////////////////////
	// 提供给编辑器的属性操作

	#define StaticProperty(name)	Property name[] =										\
	{																						\
		{	"PicFile",				&_strPicFile,			PT_String,		""	},			\
		{	"PicAlphaOp",			&_ePicAlphaOp,			PT_Int,			""	},			\
		{	"PicColor",				&_cPicColor,			PT_Color,		""	},			\
		{	"PicRect",				&_rcPicRect,			PT_Rect,		""	},			\
		{	"PicGrid",				&_rcPicGrid,			PT_Rect,		""	},			\
		{	"PicOffset",			&_rcPicOffset,			PT_Rect,		""	},			\
																							\
		{	"FontName",				&_strFontName,			PT_String,		""	},			\
		{	"FontSize",				&_uFontSize,			PT_UInt,		""	},			\
		{	"FontColor",			&_cFontColor,			PT_Color,		""	},			\
		{	"FontShadowColor",		&_cFontShadowColor,		PT_Color,		""	},			\
		{	"FontAlphaOp",			&_eFontAlphaOp,			PT_Int,			""	},			\
		{	"FontAlignType",		&_eFontAlignType,		PT_Int,			""	},			\
		{	"FontOffset",			&_rcFontOffset,			PT_Rect,		""	},			\
		{	"LineHeight",			&_uLineHeight,			PT_UInt,		""	},			\
	}


	// 获得属性个数
	UInt Static::getNumPropert()
	{
		UInt uNum = Window::getNumPropert();

		StaticProperty(propList);
		uNum += sizeof(propList) / sizeof(propList[0]);

		return uNum;
	}


	// 设置窗体属性
	Bool Static::setPropert(const String& strKey, const String& strValue)
	{
		if (Window::setPropert(strKey, strValue))
			return True;

		StaticProperty(propList);
		UInt uNum = sizeof(propList) / sizeof(propList[0]);

		return setPropertyImpl(propList, uNum, strKey, strValue);
	}


	// 获得窗体属性
	Bool Static::getPropert(UInt uIndex, String& strKey, String& strValue)
	{
		if (Window::getPropert(uIndex, strKey, strValue))
			return True;

		StaticProperty(propList);
		UInt uNum = sizeof(propList) / sizeof(propList[0]);
		UInt uNumWindow = Window::getNumPropert();

		return getPropertyImpl(propList, uNum, uIndex - uNumWindow, strKey, strValue);
	}

}
