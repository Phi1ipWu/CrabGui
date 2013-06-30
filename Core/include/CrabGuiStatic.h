
#ifndef __CrabGuiStatic_h__
#define __CrabGuiStatic_h__


#include "CrabGuiWindow.h"


namespace CrabGui
{

	/// 静态窗体
	class CRABGUIEXPORT Static : public Window
	{
	public:
		Static(System* pSystem, UInt uID);
		virtual ~Static();

		static PCStr	TypeName;

		/// 获得类型名
		virtual PCStr	getTypeName();


	public:	// 属性相关

		/// 获得字体对齐方式
		AlignType		getTextAlignType() const;

		/// 设置字体对齐方式
		void			setTextAlignType(AlignType eAlignType);


	public:	// 渲染相关

		/// 渲染
		virtual void	render(Canvas* pCanvas, const Point& ptBase);


	public:	// 提供给编辑器的属性操作

		/// 获得属性个数
		virtual UInt	getNumPropert();

		/// 设置窗体属性
		virtual Bool	setPropert(const String& strKey, const String& strValue);

		/// 获得窗体属性
		virtual Bool	getPropert(UInt uIndex, String& strKey, String& strValue);


	private:	// 编辑器属性

		String		_strPicFile;			// 图片文件名
		AlphaOp		_ePicAlphaOp;			// 图片半透混合方式
		Color		_cPicColor;				// 图片颜色
		Rect		_rcPicRect;				// 图片矩形
		Rect		_rcPicGrid;				// 图片九宫格中心矩形
		Rect		_rcPicOffset;			// 图片在窗体区域偏移

		String		_strFontName;			// 字体名
		UInt		_uFontSize;				// 字体大小
		Color		_cFontColor;			// 字体颜色
		Color		_cFontShadowColor;		// 字体阴影颜色
		AlphaOp		_eFontAlphaOp;			// 字体半透混合方式
		AlignType	_eFontAlignType;		// 字体对齐方式
		Rect		_rcFontOffset;			// 字体在窗体区域偏移
		UInt		_uLineHeight;			// 行高
	};

}


#endif	//	__CrabGuiStatic_h__
