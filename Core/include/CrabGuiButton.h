
#ifndef __CrabGuiButton_h__
#define __CrabGuiButton_h__


#include "CrabGuiWindow.h"


namespace CrabGui
{

	enum ButtonState
	{
		BS_Normal = 0,
		BS_Hover,
		BS_Pushed,
		BS_Disable,

		BS_Size,
	};


	/// 按钮
	class CRABGUIEXPORT Button : public Window
	{
	public:
		Button(System* pSystem, UInt uID);
		virtual ~Button();

		static PCStr	TypeName;

		/// 获得类型名
		virtual PCStr	getTypeName();

		/// 设置窗体矩形
		void	setPicRect(ButtonState eBS, const Rect& rcPicRect, const Rect& rcPicGrid);


	public:	// 位属性相关

		/// 设置是否可用
		virtual void	setEnable(Bool isEnable);


	public:	// 渲染相关

		/// 渲染
		virtual void	render(Canvas* pCanvas, const Point& ptBase);


	public:	// 事件相关

		/// 事件响应函数
		virtual Bool	eventHandle(const EventDesc* pEvent);


	public:	// 提供给编辑器的属性操作

		/// 获得属性个数
		virtual UInt	getNumPropert();

		/// 设置窗体属性
		virtual Bool	setPropert(const String& strKey, const String& strValue);

		/// 获得窗体属性
		virtual Bool	getPropert(UInt uIndex, String& strKey, String& strValue);


	private:

		ButtonState	_eButtonState;


	private:	// 编辑器属性

		String		_strPicFile;			// 图片文件名
		AlphaOp		_ePicAlphaOp;			// 图片半透混合方式
		Color		_cPicColor[BS_Size];	// 图片颜色
		Rect		_rcPicRect[BS_Size];	// 图片矩形
		Rect		_rcPicGrid[BS_Size];	// 图片九宫格中心矩形
		Rect		_rcPicOffset;			// 图片在窗体区域偏移

		String		_strFontName;			// 字体名
		UInt		_uFontSize;				// 字体大小
		Color		_cFontColor[BS_Size];	// 字体颜色
		AlphaOp		_eFontAlphaOp;			// 字体半透混合方式
		AlignType	_eFontAlignType;		// 字体对齐方式
		Rect		_rcFontOffset;			// 字体在窗体区域偏移
		UInt		_uLineHeight;			// 行高
	};

}


#endif	//	__CrabGuiButton_h__
