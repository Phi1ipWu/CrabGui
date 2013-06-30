
#ifndef __CrabGuiListbox_h__
#define __CrabGuiListbox_h__


#include "CrabGuiWindow.h"


namespace CrabGui
{

	class Queue;


	/// 列表窗体
	class CRABGUIEXPORT Listbox : public Window
	{
	public:
		Listbox(System* pSystem, UInt uID);
		virtual ~Listbox();

		static PCStr	TypeName;

		/// 获得类型名
		virtual PCStr	getTypeName();


	public: // 列表操作相关

		/// 列结构体
		struct ListColumn
		{
			String		strTitle;
			AlignType	eAlignType;

			Int16		nWidth;
			UInt8		isPicture;
			UInt8		uReserve;
		};

		/// 对象结构体
		struct ListItem
		{
			String		strText;
			Color		cColor;
			Rect		rcPicture;

			UInt8		isSelected;
			UInt8		uReserve[3];
		};


		static const Int16		Invalid = -1;


		/// 插入列（有数据时不建议，有效率影响）
		Bool	insertColumn(UInt uColumn, PCStr pszColumn, UInt uWidth, Bool isPicture);

		/// 移除列
		Bool	removeColumn(UInt uColumn);

		/// 移除全部列
		void	removeAllColumn();

		/// 获得列数
		UInt	getNumColumn();

		/// 获得列宽度
		UInt	getColumnWidth(UInt uColumn);

		/// 获得全部列的宽度总和
		UInt	getAllColumnWidth();

		/// 插入行
		Bool	insertItemRow(UInt uItemRow);

		/// 移除行
		Bool	removeItemRow(UInt uItemRow);

		/// 清除全部行
		void	removeAllItemRow();

		/// 获得行数
		UInt	getNumItemRow();

		/// 设置文字
		Bool	setItemText(UInt uItemRow, UInt uColumn, PCStr pszItem);

		/// 设置文字颜色
		Bool	setItemColor(UInt uItemRow, UInt uColumn, Color cColor);

		/// 设置图片
		Bool	setItemPicture(UInt uItemRow, UInt uColumn, PCStr pszPic, const Rect& rcPic);


	public:	// 渲染相关

		/// 渲染
		virtual void	render(Canvas* pCanvas, const Point& ptBase);


	public:	// 事件相关

		/// 事件响应函数
		virtual Bool	eventHandle(const EventDesc* pEvent);


	//public:	// 滚动条相关

	//	/// 获得列表垂直滚动信息
	//	virtual Bool	getVertScroll(ScrollInfo& stScrollInfo);

	//	/// 设置列表垂直滚动条信息
	//	virtual Bool	setVertScroll(UInt uVertPos);

	//	/// 获得列表水平滚动条信息
	//	virtual Bool	getHorzScroll(ScrollInfo& stScrollInfo);

	//	/// 设置列表水平滚动条信息
	//	virtual Bool	setHorzScroll(UInt uHorzPos);


	public:	// 位属性相关

		enum ListboxBitState
		{
			EBS_MultiSelect		= 24,
			EBS_FullRowSelect,
			EBS_DrawGrid,
			EBS_DrawHeader,
		};

		/// 设置多选
		virtual void	setMultiSelect(Bool isMultiSelect);

		/// 是否多选
		virtual Bool	isMultiSelect() const;

		/// 设置整行选中
		virtual void	setFullRowSelect(Bool isFullRowSelect);

		/// 是否整行选中
		virtual Bool	isFullRowSelect() const;

		/// 设置显示网格
		virtual void	setDrawGrid(Bool isDrawGrid);

		/// 是否显示网格
		virtual Bool	isDrawGrid() const;

		/// 设置显示列头
		virtual void	setDrawHeader(Bool isDrawHeader);

		/// 是否显示列头
		virtual Bool	isDrawHeader() const;


	public:	// 提供给编辑器的属性操作

		/// 获得属性个数
		virtual UInt	getNumPropert();

		/// 设置窗体属性
		virtual Bool	setPropert(const String& strKey, const String& strValue);

		/// 获得窗体属性
		virtual Bool	getPropert(UInt uIndex, String& strKey, String& strValue);


	private:

		/// 根据坐标，拾取行列
		Bool		_getItemColumnRow(const Point& ptPos, Point& ptColRow);

		/// 根据行列，获得单元格
		ListItem*	_getItem(const Point& ptColRow);

		/// 设置滚动条坐标
		void		_setScrollBar(Bool isVert, Bool isHorz);


	private:
		Queue*		_pQueueColumn;
		Queue*		_pQueueItem;

		Point		_ptDrawOffset;		// 渲染偏移（用于滚动条）
		Point		_ptSelectColumnRow;	// 选中的行列下标


	private:

		String		_strPicFile;		// 图片文件名
		AlphaOp		_ePicAlphaOp;		// 图片半透混合方式
		Color		_cPicColor;			// 图片颜色
		Rect		_rcPicRect;			// 图片矩形
		Rect		_rcPicGrid;			// 图片九宫格中心矩形
		Rect		_rcPicOffset;		// 图片在窗体区域偏移

		String		_strHeaderPicFile;	// 列头图片文件名
		AlphaOp		_eHeaderPicAlphaOp;	// 列头图片半透混合方式
		Color       _cHeaderPicColor;   // 列头图片颜色
		Rect		_rcHeaderPicRect;	// 列头图片矩形
		Rect		_rcHeaderPicGrid;	// 列头图片九宫格中心矩形

		String		_strSelectPicFile;	// 选中图片文件名
		AlphaOp		_eSelectPicAlphaOp;	// 选中图片半透混合方式
		Color       _cSelectPicColor;   // 选中图片颜色
		Rect		_rcSelectPicRect;	// 选中图片矩形
		Rect		_rcSelectPicGrid;	// 选中图片九宫格中心矩形

		String		_strFontName;		// 字体名
		UInt		_uFontSize;			// 字体大小
		Color		_cFontColor;		// 字体颜色
		Color		_cFontShadowColor;	// 字体阴影颜色
		AlphaOp		_eFontAlphaOp;		// 字体半透混合方式
		AlignType	_eFontAlignType;	// 字体对齐方式

		Rect		_rcListOffset;		// 列表在窗体区域偏移（留空给滚动条）
		UInt16		_uHeaderHeight;		// 列头高度
		UInt16		_uItemHeight;		// 单项高度
	};

}


#endif	//	__CrabGuiListbox_h__
