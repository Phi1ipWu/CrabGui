
#ifndef __CrabGuiEditbox_h__
#define __CrabGuiEditbox_h__


#include "CrabGuiWindow.h"


namespace CrabGui
{

	/// 静态窗体
	class CRABGUIEXPORT Editbox : public Window
	{
	public:
		Editbox(System* pSystem, UInt uID);
		virtual ~Editbox();

		static PCStr	TypeName;

		/// 获得类型名
		virtual PCStr	getTypeName();


	public:	// 渲染相关

		/// 光标闪烁间隔（单位：毫秒）
		static const UInt BeamFlashTime;

		/// 逝去时间
		virtual Bool	update(UInt uElapse);

		/// 渲染
		virtual void	render(Canvas* pCanvas, const Point& ptBase);


	public:	// 事件相关

		/// 事件响应函数
		virtual Bool	eventHandle(const EventDesc* pEvent);


	public:	// 属性相关

		/// 设置行高
		virtual void	setLineHeight(UInt uLineHeight);

		/// 获得行高
		virtual UInt	getLineHeight();


	public:	// 位属性相关

		enum EditboxBitState
		{
			EBS_ReadOnly	= 24,
			EBS_MultiLine,
			EBS_TextWrap,
		};

		/// 设置只读
		virtual void	setReadOnly(Bool isReadOnly);

		/// 是否只读
		virtual Bool	isReadOnly() const;

		/// 设置多行
		virtual void	setMultiLine(Bool isMultiLine);

		/// 是否多行
		virtual Bool	isMultiLine() const;

		/// 设置自动换行
		virtual void	setTextWrap(Bool isTextWrap);

		/// 是否自动换行
		virtual Bool	isTextWrap() const;


	public:	// 提供给编辑器的属性操作

		/// 获得属性个数
		virtual UInt	getNumPropert();

		/// 设置窗体属性
		virtual Bool	setPropert(const String& strKey, const String& strValue);

		/// 获得窗体属性
		virtual Bool	getPropert(UInt uIndex, String& strKey, String& strValue);


	private: // 辅助函数

		/// 根据坐标，获得光标坐标
		UInt	_getBeamPos(const Point& ptPos);

		/// 删除选中的
		void	_deleteSelected();

		/// 修正滚动坐标
		void	_fixScrollPos();


	private:

		UInt		_uBeamPos;				// 光标位置
		UInt		_uDownPos;				// 鼠标按下时的位置
		UInt		_uBeamElapse;			// 光标闪烁计时

		Rect		_rcTextDraw;			// 字体渲染矩形（字体对齐后的渲染矩形区域）
		Point		_ptDrawOffset;			// 渲染字体偏移（用于滚动条）


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
		Color		_cFontSelectColor;		// 字体阴影颜色
		Color		_cFontSelectBackColor;	// 字体阴影底颜色
		AlphaOp		_eFontAlphaOp;			// 字体半透混合方式
		AlignType	_eFontAlignType;		// 字体对齐方式
		Rect		_rcFontOffset;			// 字体在窗体区域偏移
		UInt		_uLineHeight;			// 行高

		String		_strEmptyText;			// 空白时文字渲染
		Color		_cFontEmptyColor;		// 空白时文字颜色
	};

}


#endif	//	__CrabGuiEditbox_h__
