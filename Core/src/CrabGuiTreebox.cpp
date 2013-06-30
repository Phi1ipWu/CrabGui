
#include "CrabGuiDefine.h"
#include "CrabGuiTreebox.h"

#include "CrabGuiSystem.h"
#include "CrabGuiQueue.h"
#include "CrabGuiRBTree.h"
#include "CrabGuiCanvas.h"
#include "CrabGuiEventHandle.h"


namespace CrabGui
{

	Treebox::TreeItem::TreeItem()
	{
		uID			= Invalid;
		pParent		= 0;
		pChilds		= 0;

		cTextColor	= 0xFFFFFFFF;
		rcPicRect.setZero();

		isSelected	= False;
		isExtended	= False;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////


	Treebox::Treebox(System* pSystem, UInt uID)
		: Window(pSystem, uID)
		, _pRBTreeItem(0), _pRootItem(0), _uNextItemID(0)
		, _uFontSize(16)
		, _uItemHeight(16)
	{
		_pRootItem = CrabNew(TreeItem)();
		_pRootItem->uID = _uNextItemID++;

		_pRBTreeItem = CrabNew(RBTree)();
	}

	Treebox::~Treebox(void)
	{
		CrabDelete(_pRBTreeItem);
		CrabDelete(_pRootItem);
	}


	PCStr Treebox::TypeName = "Treebox";


	// 获得类型名
	PCStr Treebox::getTypeName()
	{
		return TypeName;
	}


    ///////////////////////////////////////////////////////////////////////////////////////////////
    // 树操作相关


	// 插入结点
	UInt Treebox::insertItem(UInt uParentID)
    {
		TreeItem* pParentItem = _getItem(uParentID);
		if (0 == pParentItem)
			return Invalid;

		if (0 == pParentItem->pChilds)
		{
			pParentItem->pChilds = CrabNew(Queue)();
		}

		// 创建新结点
		TreeItem* pItem = CrabNew(TreeItem);
		pItem->uID = _uNextItemID++;

		UInt uIndex = pParentItem->pChilds->getNum();
		pParentItem->pChilds->insert(uIndex, pItem);

		return pItem->uID;
    }


	// 插入带图片结点
	UInt Treebox::insertItem(UInt uParentID, PCStr pszText, PCStr pszPicFileName, const Rect& rcPicRect)
    {
        return Invalid;
    }


    // 删除结点
    Bool Treebox::removeItem(UInt uItemID)
    {
        return False;
    }


	// 移除全部结点
	void Treebox::removeAllItems()
	{
	}


    // 设置文字
    Bool Treebox::setItemText(UInt uItemID, PCStr pszText)
    {
        return False;
    }


    // 设置文字颜色
    Bool Treebox::setItemColor(UInt uItemID, Color cItemColor)
    {
        return False;
    }


	///////////////////////////////////////////////////////////////////////////////////////////////


	// 设置仅叶子选中
	void Treebox::setOnlyLeafSelect(Bool isOnlyLeafSelect)
	{
		_setBitState(EBS_OnlyLeafSelect, isOnlyLeafSelect);
	}

	// 是否仅叶子选中
	Bool Treebox::isOnlyLeafSelect() const
	{
		return _getBitState(EBS_OnlyLeafSelect);
	}


	// 设置显示根结点
	void Treebox::setDrawRoot(Bool isDrawRoot)
	{
		_setBitState(EBS_DrawRoot, isDrawRoot);
	}

	// 是否显示根结点
	Bool Treebox::isDrawRoot() const
	{
		return _getBitState(EBS_DrawRoot);
	}


	///////////////////////////////////////////////////////////////////////////////////////////////


	// 寻找结点
	Treebox::TreeItem* Treebox::_getItem(UInt uItemID)
	{
		return 0;
	}


	// 获得结点高度（自动计算展开高度）
	UInt Treebox::_getItemHeight(Treebox::TreeItem* pItem)
	{
		return 0;
	}


    ///////////////////////////////////////////////////////////////////////////////////////////////
	// 提供给编辑器的属性操作


	#define TreeboxProperty(name)	Property name[] =										\
	{																						\
		{	"OnlyLeafSelect",		&_uBitState,			PT_Bit24,		""	},			\
		{	"DrawRoot",				&_uBitState,			PT_Bit25,		""	},			\
																							\
		{	"PicFile",				&_strPicFile,			PT_String,		""	},			\
		{	"PicAlphaOp",			&_ePicAlphaOp,			PT_Int,			""	},			\
		{	"PicColor",				&_cPicColor,			PT_Color,		""	},			\
		{	"PicRect",				&_rcPicRect,			PT_Rect,		""	},			\
		{	"PicGrid",				&_rcPicGrid,			PT_Rect,		""	},			\
		{	"PicOffset",			&_rcPicOffset,			PT_Rect,		""	},			\
																							\
		{	"SelectPicFile",		&_strSelectPicFile,		PT_String,		""	},			\
		{	"SelectPicAlphaOp",		&_eSelectPicAlphaOp,	PT_Int,			""	},			\
		{   "SelectPicColor",		&_cSelectPicColor,		PT_Color,		""  },          \
		{	"SelectPicRect",		&_rcSelectPicRect,		PT_Rect,		""	},			\
		{	"SelectPicGrid",		&_rcSelectPicGrid,		PT_Rect,		""	},			\
																							\
		{	"FontName",				&_strFontName,			PT_String,		""	},			\
		{	"FontSize",				&_uFontSize,			PT_UInt,		""	},			\
		{	"FontColor",			&_cFontColor,			PT_Color,		""	},			\
		{	"FontShadowColor",		&_cFontShadowColor,		PT_Color,		""	},			\
		{	"FontAlphaOp",			&_eFontAlphaOp,			PT_Int,			""	},			\
		/*{	"FontAlignType",		&_eFontAlignType,		PT_Int,			""	},*/		\
																							\
		{	"TreeOffset",			&_rcTreeOffset,			PT_Rect,		""	},			\
		{	"ItemHeight",			&_uItemHeight,			PT_UInt16,		""	},			\
	}


	// 获得属性个数
	UInt Treebox::getNumPropert()
	{
		UInt uNum = Window::getNumPropert();

		TreeboxProperty(propTree);
		uNum += sizeof(propTree) / sizeof(propTree[0]);

		return uNum;
	}


	// 设置窗体属性
	Bool Treebox::setPropert(const String& strKey, const String& strValue)
	{
		if (Window::setPropert(strKey, strValue))
			return True;

		TreeboxProperty(propTree);
		UInt uNum = sizeof(propTree) / sizeof(propTree[0]);

		return setPropertyImpl(propTree, uNum, strKey, strValue);
	}


	// 获得窗体属性
	Bool Treebox::getPropert(UInt uIndex, String& strKey, String& strValue)
	{
		if (Window::getPropert(uIndex, strKey, strValue))
			return True;

		TreeboxProperty(propTree);
		UInt uNum = sizeof(propTree) / sizeof(propTree[0]);
		UInt uNumWindow = Window::getNumPropert();

		return getPropertyImpl(propTree, uNum, uIndex - uNumWindow, strKey, strValue);
	}

}
