
#include "CrabGuiDefine.h"
#include "CrabGuiCombobox.h"

#include "CrabGuiSystem.h"
#include "CrabGuiCanvas.h"
#include "CrabGuiEventHandle.h"


namespace CrabGui
{

	Combobox::Combobox(System* pSystem, UInt uID)
		: Window(pSystem, uID)
	{
	}

	Combobox::~Combobox(void)
	{
	}


	PCStr Combobox::TypeName = "Combobox";


	// 获得类型名
	PCStr Combobox::getTypeName()
	{
		return TypeName;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////
	// 事件相关


	// 事件响应函数
	Bool Combobox::eventHandle(const EventDesc* pEvent)
	{
		if (pEvent->pWindow->getParent() == this)
		{
			switch (pEvent->uEvent)
			{
			case Event_MouseClick:
				{
					if (_pWndButton == pEvent->pWindow)			// 打开下拉列表
					{
					}
				}
				break;
			}
		}

		return Window::eventHandle(pEvent);
	}


	///////////////////////////////////////////////////////////////////////////////////////////////
	// 窗体父子层级相关


	// 添加子窗体
	Bool Combobox::_insertChild(Window* pWndChild)
	{
		if (!Window::_insertChild(pWndChild))
		{
			return False;
		}

		//if (_strNameUp == pWndChild->getName())
		//{
		//	_pWndUp = pWndChild;
		//}
		//else if (_strNameBar == pWndChild->getName())
		//{
		//	_pWndBar = pWndChild;
		//}
		//else if (_strNameDown == pWndChild->getName())
		//{
		//	_pWndDown = pWndChild;
		//}

		return True;
	}


	// 移除子窗体
	Bool Combobox::_removeChild(Window* pWndChild)
	{
		if (!Window::_removeChild(pWndChild))
		{
			return False;
		}

		//if (_pWndUp == pWndChild)
		//{
		//	_pWndUp = 0;
		//}
		//else if (_pWndBar == pWndChild)
		//{
		//	_pWndBar = 0;
		//}
		//else if (_pWndDown == pWndChild)
		//{
		//	_pWndDown = 0;
		//}

		return True;
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////
	//// 提供给编辑器的属性操作

	//#define ComboboxProperty(name)	Property name[] =										\
	//{																						\
	//	{	"PicFile",			&_strPicFile,		PT_String,		""	},					\
	//	{	"PicAlphaOp",		&_ePicAlphaOp,		PT_Int,			""	},					\
	//	{	"PicColor",			&_cPicColor,		PT_Color,		""	},					\
	//	{	"PicRect",			&_rcPicRect,		PT_Rect,		""	},					\
	//	{	"PicGrid",			&_rcPicGrid,		PT_Rect,		""	},					\
	//	{	"PicOffset",		&_rcPicOffset,		PT_Rect,		""	},					\
	//																						\
	//	{	"FontName",			&_strFontName,		PT_String,		""	},					\
	//	{	"FontSize",			&_uFontSize,		PT_UInt,		""	},					\
	//	{	"FontColor",		&_cFontColor,		PT_Color,		""	},					\
	//	{	"FontShadowColor",	&_cFontShadowColor,	PT_Color,		""	},					\
	//	{	"FontAlphaOp",		&_eFontAlphaOp,		PT_Int,			""	},					\
	//	{	"FontAlignType",	&_eFontAlignType,	PT_Int,			""	},					\
	//	{	"FontOffset",		&_rcFontOffset,		PT_Rect,		""	},					\
	//}


	//// 获得属性个数
	//UInt Combobox::getNumPropert()
	//{
	//	UInt uNum = Window::getNumPropert();

	//	ComboboxProperty(propList);
	//	uNum += sizeof(propList) / sizeof(propList[0]);

	//	return uNum;
	//}


	//// 设置窗体属性
	//Bool Combobox::setPropert(const String& strKey, const String& strValue)
	//{
	//	if (Window::setPropert(strKey, strValue))
	//		return True;

	//	ComboboxProperty(propList);
	//	UInt uNum = sizeof(propList) / sizeof(propList[0]);

	//	return setPropertyImpl(propList, uNum, strKey, strValue);
	//}


	//// 获得窗体属性
	//Bool Combobox::getPropert(UInt uIndex, String& strKey, String& strValue)
	//{
	//	if (Window::getPropert(uIndex, strKey, strValue))
	//		return True;

	//	ComboboxProperty(propList);
	//	UInt uNum = sizeof(propList) / sizeof(propList[0]);
	//	UInt uNumWindow = Window::getNumPropert();

	//	return getPropertyImpl(propList, uNum, uIndex - uNumWindow, strKey, strValue);
	//}

}
