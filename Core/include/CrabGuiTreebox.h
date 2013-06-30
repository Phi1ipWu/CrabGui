
#ifndef __CrabGuiTreebox_h__
#define __CrabGuiTreebox_h__


#include "CrabGuiWindow.h"


namespace CrabGui
{

	class RBTree;


	/// 树窗体
	class CRABGUIEXPORT Treebox : public Window
	{
	public:
		/// 对象结构体
		struct TreeItem
		{
			UInt		uID;
			TreeItem*	pParent;		// 父结点
			Queue*		pChilds;

			String		strText;		// 文字内容
			Color		cTextColor;		// 文字颜色
			String		strPicFileName;
			Rect		rcPicRect;

			UInt8		isSelected;     // 是否选中
			UInt8       isExtended;     // 是否展开
			UInt8		uReserve[2];

			TreeItem();
		};


	public:
		Treebox(System* pSystem, UInt uID);
		virtual ~Treebox();

		static PCStr	TypeName;

		/// 获得类型名
		virtual PCStr	getTypeName();


	public: // 树操作相关

		static const UInt Invalid = 0xFFFFFFFF;

		/// 插入结点
		UInt			insertItem(UInt uParentID);

		/// 插入带文字图片结点
		UInt			insertItem(UInt uParentID, PCStr pszText, PCStr pszPicFileName, const Rect& rcPicRect);

		/// 删除结点
		Bool			removeItem(UInt uItemID);

		/// 移除全部结点
		void			removeAllItems();

		/// 设置文字
		Bool			setItemText(UInt uItemID, PCStr pszText);

		/// 设置文字颜色
		Bool			setItemColor(UInt uItemID, Color cItemColor);


	public:	// 位属性相关

		enum TreeboxBitState
		{
			EBS_OnlyLeafSelect	= 24,
			EBS_DrawRoot,
		};

		/// 设置仅叶子选中
		virtual void	setOnlyLeafSelect(Bool isOnlyLeafSelect);

		/// 是否仅叶子选中
		virtual Bool	isOnlyLeafSelect() const;

		/// 设置显示根结点
		virtual void	setDrawRoot(Bool isDrawRoot);

		/// 是否显示根结点
		virtual Bool	isDrawRoot() const;


	private:

		/// 寻找结点
		TreeItem*		_getItem(UInt uItemID);

		/// 获得结点高度（自动计算展开高度）
		UInt			_getItemHeight(TreeItem* pItem);


	public:	// 提供给编辑器的属性操作

		/// 获得属性个数
		virtual UInt	getNumPropert();

		/// 设置窗体属性
		virtual Bool	setPropert(const String& strKey, const String& strValue);

		/// 获得窗体属性
		virtual Bool	getPropert(UInt uIndex, String& strKey, String& strValue);


    private:

		RBTree*		_pRBTreeItem;
		TreeItem*	_pRootItem;         // 根结点
		UInt		_uNextItemID;


	private:	// 提供编辑器的属性

		String		_strPicFile;		// 图片文件名
		AlphaOp		_ePicAlphaOp;		// 图片半透混合方式
		Color		_cPicColor;			// 图片颜色
		Rect		_rcPicRect;			// 图片矩形
		Rect		_rcPicGrid;			// 图片九宫格中心矩形
		Rect		_rcPicOffset;		// 图片在窗体区域偏移

		String		_strSelectPicFile;	// 选中图片文件名
		AlphaOp		_eSelectPicAlphaOp;	// 选中图片半透混合方式
		Color       _cSelectPicColor;   // 选中图片颜色
		Rect		_rcSelectPicRect;	// 选中图片矩形
		Rect		_rcSelectPicGrid;	// 选中图片九宫格中心矩形

		String		_strAddPicFile;		// 加号图片文件名
		AlphaOp		_eAddPicAlphaOp;	// 加号图片半透混合方式
		Rect		_rcAddPicRect;		// 加号图片矩形

		String		_strMinusPicFile;	// 减号图片文件名
		AlphaOp		_eMinusPicAlphaOp;	// 减号图片半透混合方式
		Rect		_rcMinusPicRect;	// 减号图片矩形

		String		_strFontName;		// 字体名
		UInt		_uFontSize;			// 字体大小
		Color		_cFontColor;		// 字体颜色
		Color		_cFontShadowColor;	// 字体阴影颜色
		AlphaOp		_eFontAlphaOp;		// 字体半透混合方式
		//AlignType	_eFontAlignType;	// 字体对齐方式

		Rect		_rcTreeOffset;		// 树在窗体区域偏移（留空给滚动条）
		UInt16		_uItemHeight;		// 单项高度
		Int16		_nReserve;

	};
}


#endif  //  __CrabGuiTreebox_h__
