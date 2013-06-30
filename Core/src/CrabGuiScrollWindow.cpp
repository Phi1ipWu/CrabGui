
#include "CrabGuiDefine.h"
#include "CrabGuiScrollWindow.h"

#include "CrabGuiSystem.h"


namespace CrabGui
{

	ScrollWindow::ScrollWindow(System* pSystem, UInt uID)
		: Window(pSystem, uID)
	{
	}

	ScrollWindow::~ScrollWindow()
	{
	}


	PCStr ScrollWindow::TypeName = "ScrollWindow";


	// 获得类型名
	PCStr ScrollWindow::getTypeName()
	{
		return TypeName;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////
	// 渲染相关



	//////////////////////////////////////////////////////////////////////////////////////////////////
	// 滚动条相关



	//////////////////////////////////////////////////////////////////////////////////////////////////
	// 提供给编辑器的属性操作
	
	//#define ScrollWindowProperty(name)	Property name[] =									\
	//{																					\
	//	{	"VScroll.Min",		&_stVertScroll.uMin,		PT_UInt,	""	},			\
	//	{	"VScroll.Max",		&_stVertScroll.uMax,		PT_UInt,	""	},			\
	//	{	"VScroll.Page",		&_stVertScroll.uPage,		PT_UInt,	""	},			\
	//	{	"VScroll.Step",		&_stVertScroll.uStep,		PT_UInt,	""	},			\
	//	{	"VScroll.TrackPos",	&_stVertScroll.uTrackPos,	PT_UInt,	""	},			\
	//																					\
	//	{	"HScroll.Min",		&_stHorzScroll.uMin,		PT_UInt,	""	},			\
	//	{	"HScroll.Max",		&_stHorzScroll.uMax,		PT_UInt,	""	},			\
	//	{	"HScroll.Page",		&_stHorzScroll.uPage,		PT_UInt,	""	},			\
	//	{	"HScroll.Step",		&_stHorzScroll.uStep,		PT_UInt,	""	},			\
	//	{	"HScroll.TrackPos",	&_stHorzScroll.uTrackPos,	PT_UInt,	""	},			\
	//}


	// 获得属性个数
	UInt ScrollWindow::getNumPropert()
	{
		UInt uNum = Window::getNumPropert();

		//ScrollWindowProperty(propList);
		//uNum += sizeof(propList) / sizeof(propList[0]);

		return uNum;
	}


	// 设置窗体属性
	Bool ScrollWindow::setPropert(const String& strKey, const String& strValue)
	{
		//if (Window::setPropert(strKey, strValue))
		//	return True;

		//ScrollWindowProperty(propList);
		//UInt uNum = sizeof(propList) / sizeof(propList[0]);

		//return setPropertyImpl(propList, uNum, strKey, strValue);
		return Window::setPropert(strKey, strValue);
	}


	// 获得窗体属性
	Bool ScrollWindow::getPropert(UInt uIndex, String& strKey, String& strValue)
	{
		//if (Window::getPropert(uIndex, strKey, strValue))
		//	return True;

		//ScrollWindowProperty(propList);
		//UInt uNum = sizeof(propList) / sizeof(propList[0]);
		//UInt uNumWindow = Window::getNumPropert();

		//return getPropertyImpl(propList, uNum, uIndex - uNumWindow, strKey, strValue);

		return Window::getPropert(uIndex, strKey, strValue);
	}

}
