
#include "CrabGuiDefine.h"
#include "CrabGuiListbox.h"

#include "CrabGuiSystem.h"
#include "CrabGuiCanvas.h"
#include "CrabGuiEventHandle.h"

#include "CrabGuiQueue.h"


namespace CrabGui
{

	Listbox::Listbox(System* pSystem, UInt uID)
		: Window(pSystem, uID), _ptSelectColumnRow(Invalid, Invalid)
		, _ePicAlphaOp(AO_Blend), _cPicColor(0xFFFFFFFF)
		, _eHeaderPicAlphaOp(AO_Blend), _cHeaderPicColor(0xFFFFFFFF)
		, _eSelectPicAlphaOp(AO_Blend), _cSelectPicColor(0xFFFFFFFF)
		, _uFontSize(16), _cFontColor(0xFF000000), _cFontShadowColor(0x0), _eFontAlphaOp(AO_Blend), _eFontAlignType(AT_Center)
		, _uHeaderHeight(20), _uItemHeight(16)
	{
		_pQueueColumn = CrabNew(Queue)(8);
		_pQueueItem   = CrabNew(Queue)(64);
	}

	Listbox::~Listbox(void)
	{
		removeAllColumn();

		CrabDelete(_pQueueColumn);
		CrabDelete(_pQueueItem);
	}


	PCStr Listbox::TypeName = "Listbox";


	// 获得类型名
	PCStr Listbox::getTypeName()
	{
		return TypeName;
	}


    ///////////////////////////////////////////////////////////////////////////////////////////////
    // 列表操作相关


	// 插入列（有数据时不建议，有效率影响）
	Bool Listbox::insertColumn(UInt uColumn, PCStr pszColumn, UInt uWidth, Bool isPicture)
	{
		UInt uNumColumn = getNumColumn();

		// 插入一列
		if (_pQueueColumn->insert(uColumn, 0))
		{
			ListColumn* pColumn = CrabNew(ListColumn);
			_pQueueColumn->setAt(uColumn, pColumn);

			pColumn->strTitle	= pszColumn;
			pColumn->eAlignType	= _eFontAlignType;
			pColumn->isPicture	= (UInt8)isPicture;
			pColumn->nWidth		= (Int16)uWidth;

			// 原来 Item 位置变更
			for (UInt uItem = uColumn; uItem < _pQueueItem->getNum(); uItem += uNumColumn)
			{
				if (_pQueueItem->insertEmpty(uItem, 1))
				{
					ListItem* pItem = CrabNew(ListItem);
					pItem->cColor = _cFontColor;
					pItem->isSelected = False;
					_pQueueItem->setAt(uItem, pItem);
				}
			}

			_setScrollBar(False, True);
			return True;
		}

		return False;
	}


	// 移除列
	Bool Listbox::removeColumn(UInt uColumn)
	{
		UInt uNumColumn = getNumColumn();
		ListColumn* pColumn = (ListColumn*)_pQueueColumn->remove(uColumn);

		if (pColumn)
		{
			CrabDelete(pColumn);

			// 原来 Item 位置变更
			for (UInt uItem = uColumn; uItem < _pQueueItem->getNum(); uItem += uNumColumn)
			{
				ListItem* pItem = (ListItem*)_pQueueItem->remove(uItem);
				CrabDelete(pItem);
			}

			_setScrollBar(False, True);
			return True;
		}

		return False;
	}


	// 移除全部列
	void Listbox::removeAllColumn()
	{
		// 先移除全部行
		removeAllItemRow();

		for (UInt uColumn = 0; uColumn < _pQueueColumn->getNum(); ++uColumn)
		{
			ListColumn* pColumn = (ListColumn*)_pQueueColumn->getAt(uColumn);
			CrabDelete(pColumn);
		}
		_pQueueColumn->clear();

		_setScrollBar(False, True);
	}


	// 获得列数
	UInt Listbox::getNumColumn()
	{
		return _pQueueColumn->getNum();
	}


	// 获得列宽度
	UInt Listbox::getColumnWidth(UInt uColumn)
	{
		ListColumn* pColumn = (ListColumn*)_pQueueColumn->getAt(uColumn);
		return pColumn ? pColumn->nWidth : 0;
	}


	// 获得全部列的宽度总和
	UInt Listbox::getAllColumnWidth()
	{
		UInt uWidth = 0;
		for (UInt i = 0; i < getNumColumn(); ++i)
		{
			uWidth += getColumnWidth(i);
		}

		return uWidth;
	}


	// 插入行
	Bool Listbox::insertItemRow(UInt uItemRow)
	{
		// 允许行数越界一行，作为尾插
		UInt uNumColumn = getNumColumn();
		if (0 == uNumColumn || uItemRow > getNumItemRow())
			return False;

		if (!_pQueueItem->insertEmpty(uItemRow * uNumColumn, uNumColumn))
			return False;

		for (UInt uCol = 0; uCol < uNumColumn; ++uCol)
		{
			ListItem* pItem = CrabNew(ListItem);
			pItem->cColor = _cFontColor;
			pItem->isSelected = False;
			_pQueueItem->setAt(uItemRow * uNumColumn + uCol, pItem);
		}

		_setScrollBar(True, False);

		return True;
	}


	// 移除行
	Bool Listbox::removeItemRow(UInt uItemRow)
	{
		if (uItemRow >= getNumItemRow())
			return False;

		UInt uNumColumn = getNumColumn();
		for (UInt uItem = uItemRow * uNumColumn; uItem < (uItemRow + 1) * uNumColumn; ++uItem)
		{
			ListItem* pItem = (ListItem*)_pQueueItem->remove(uItem);
			CrabDelete(pItem);
		}

		_setScrollBar(True, False);

		return True;
	}


	// 清除全部行
	void Listbox::removeAllItemRow()
	{
		for (UInt uItem = 0; uItem < _pQueueItem->getNum(); ++uItem)
		{
			ListItem* pItem = (ListItem*)_pQueueItem->getAt(uItem);
			CrabDelete(pItem);
		}

		_pQueueItem->clear();

		_setScrollBar(True, False);
	}


	// 获得行数
	UInt Listbox::getNumItemRow()
	{
		return _pQueueItem->getNum() / _pQueueColumn->getNum();
	}


	// 设置文字
	Bool Listbox::setItemText(UInt uItemRow, UInt uColumn, PCStr pszItem)
	{
		ListItem* pItem = (ListItem*)_pQueueItem->getAt(uItemRow * getNumColumn() + uColumn);
		if (pItem)
		{
			pItem->cColor = _cFontColor;
			pItem->strText = pszItem;

			// 待优化
			setDirtyRect(0);
			return True;
		}

		return False;
	}


	// 设置文字颜色
	Bool Listbox::setItemColor(UInt uItemRow, UInt uColumn, Color cColor)
	{
		ListItem* pItem = (ListItem*)_pQueueItem->getAt(uItemRow * getNumColumn() + uColumn);
		if (pItem)
		{
			pItem->cColor = cColor;

			// 待优化
			setDirtyRect(0);
			return True;
		}

		return False;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////
	// 渲染相关


	// 渲染
	void Listbox::render(Canvas* pCanvas, const Point& ptBase)
	{
		Point ptCanvasBase = ptBase;
		if (preRender(pCanvas, ptCanvasBase))
		{
			// 渲染底图
			{
				Rect rcPicCanvas;
				getCanvasRect(rcPicCanvas, _rcPicOffset, ptBase);

				Canvas::drawGridPic(_strPicFile,  _rcPicRect, _rcPicGrid, rcPicCanvas, _cPicColor, _ePicAlphaOp);
			}

			// 渲染网格
			if (isDrawGrid())
			{

			}

			// 渲染文字
			{
				// 转换到画布上的客户区矩形坐标
				Rect rcListCanvas;
				getCanvasRect(rcListCanvas, _rcListOffset, ptBase);

				// 渲染列头
				if (isDrawHeader())
				{
					Rect rcHeaderPicWindow;
					rcHeaderPicWindow.y1 = _rcListOffset.y1 + ptBase.y;
					rcHeaderPicWindow.y2 = rcHeaderPicWindow.y1 + _uHeaderHeight;

					UInt uColumnWidth = 0;
					for (UInt uColumn = 0; uColumn < getNumColumn(); ++uColumn)
					{
						ListColumn* pColumn = (ListColumn*)_pQueueColumn->getAt(uColumn);

						rcHeaderPicWindow.x1 = _rcListOffset.x1 + uColumnWidth - _ptDrawOffset.x + ptBase.x;
						rcHeaderPicWindow.x2 = rcHeaderPicWindow.x1 + pColumn->nWidth;

						uColumnWidth += pColumn->nWidth;

						// 判断优化，整列剔除
						// ...

						Canvas::drawGridPic(_strHeaderPicFile, _rcHeaderPicRect, _rcHeaderPicGrid, rcHeaderPicWindow, _cHeaderPicColor, _eHeaderPicAlphaOp);

						// 每次强制计算坐标，再渲染到画布
						//pColumn->strTitle.setWCharPosDirty(True);
						Canvas::drawText(_strFontName, _uFontSize, pColumn->strTitle, rcHeaderPicWindow, _uItemHeight, pColumn->eAlignType, 0, _cFontColor, _cFontShadowColor, _eFontAlphaOp);
					}
				}


				// 如果渲染了列头，偏移列头高度
				if (isDrawHeader())
					rcListCanvas.y1 += (Int16)_uHeaderHeight;

				// 设置当前窗体裁减矩形（此操作慎用）
				setClipRect(pCanvas, Point(), rcListCanvas);

				// 转换到画布上的坐标原点
				Point ptClientBase = ptBase;
				ptClientBase.x += _rcListOffset.x1;
				ptClientBase.y += _rcListOffset.y1;

				// 列遍历
				UInt uColumnWidth = 0;
				for (UInt uColumn = 0; uColumn < getNumColumn(); ++uColumn)
				{
					ListColumn* pColumn = (ListColumn*)_pQueueColumn->getAt(uColumn);

					// 判断优化，整列剔除
					// ...

					// 行遍历
					for (UInt uItem = uColumn, uItemRow = 0; uItem < _pQueueItem->getNum(); uItem += getNumColumn(), ++uItemRow)
					{
						// 计算 Item 的渲染矩形（带滚条动偏移）
						Rect rcItem;
						rcItem.x1 = uColumnWidth - _ptDrawOffset.x;
						rcItem.x2 = rcItem.x1 + pColumn->nWidth;
						rcItem.y1 = uItemRow * _uItemHeight - _ptDrawOffset.y + (isDrawHeader() ? _uHeaderHeight : 0);
						rcItem.y2 = rcItem.y1 + _uItemHeight;
						rcItem.offset(ptClientBase);

						// 如果不在有效范围，跳过
						if (!rcListCanvas.isIntersect(rcItem))
							continue;

						ListItem* pItem = (ListItem*)_pQueueItem->getAt(uItem);

						// 每次强制计算坐标，再渲染到画布
						pItem->strText.setWCharPosDirty(True);
						Canvas::drawText(_strFontName, _uFontSize, pItem->strText, rcItem, _uItemHeight, pColumn->eAlignType, 0, pItem->cColor, _cFontShadowColor, _eFontAlphaOp);
					}

					uColumnWidth += pColumn->nWidth;
				}
			}

			postRender(pCanvas, ptCanvasBase);
		}
	}


	///////////////////////////////////////////////////////////////////////////////////////////////
	// 事件相关


	// 事件响应函数
	Bool Listbox::eventHandle(const EventDesc* pEvent)
	{
		if (pEvent->pWindow == this)
		{
			switch (pEvent->uEvent)
			{
			case Event_WindowCreate:
				{
					// 设置滚动条
					_setScrollBar(True, True);
				}
				break;

			case Event_WindowDestroy:
				{
					// ...
				}
				break;

			case Event_MouseDown:
				{
					Point ptPos((Int16)pEvent->nParam[0], (Int16)pEvent->nParam[1]);

					Point ptColRow;
					_getItemColumnRow(ptPos, ptColRow);
					ListItem* pItem = _getItem(ptColRow);

					if (pItem)
					{
						pItem->isSelected = True;	// !pItem->isSelected;

						if (pItem->isSelected)
						{
							_ptSelectColumnRow = ptColRow;
						}
						else
						{
							_ptSelectColumnRow .setPoint(Invalid, Invalid);
						}
					}

					// 行选中
					setDirtyRect(0);
				}
				break;
			}
		}
		else if (pEvent->pWindow->getParent() == this)	// 滚动条
		{
			switch (pEvent->uEvent)
			{
			case Event_VertScroll:
				{
					_ptDrawOffset.y = pEvent->nParam[0];
					setDirtyRect(0);
				}
				break;

			case Event_HorzScroll:
				{
					_ptDrawOffset.x = pEvent->nParam[0];
					setDirtyRect(0);
				}
				break;
			}
		}

		return Window::eventHandle(pEvent);
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////
	//// 滚动条相关


	//// 获得列表垂直滚动信息
	//Bool Listbox::getVertScroll(ScrollInfo& stScrollInfo)
	//{
	//	UInt uAllItemHeight	= _uItemHeight * getNumItemRow();
	//	UInt uDrawHeight	= _ptSize.y - _rcListOffset.getSize().y;

	//	if (isDrawHeader())	// 如果有列头显示，减去相应高度
	//		uDrawHeight -= _uHeaderHeight;

	//	if (uDrawHeight < uAllItemHeight)
	//	{
	//		// 计算滚动条
	//		stScrollInfo.uMin		= 0;
	//		stScrollInfo.uMax		= uAllItemHeight - uDrawHeight;
	//		stScrollInfo.uPage		= uDrawHeight;
	//		stScrollInfo.uStep		= _uItemHeight;
	//		stScrollInfo.uTrackPos	= _ptDrawOffset.y;

	//		return True;
	//	}

	//	return False;
	//}

	//// 设置列表垂直滚动条信息
	//Bool Listbox::setVertScroll(UInt uVertPos)
	//{
	//	_ptDrawOffset.y = (Int16)uVertPos;
	//	setDirtyRect(0);

	//	return True;
	//}


	//// 获得列表水平滚动条信息
	//Bool Listbox::getHorzScroll(ScrollInfo& stScrollInfo)
	//{
	//	return True;
	//}

	//// 设置列表水平滚动条信息
	//Bool Listbox::setHorzScroll(UInt uHorzPos)
	//{
	//	_ptDrawOffset.x = (Int16)uHorzPos;
	//	setDirtyRect(0);

	//	return True;
	//}


	///////////////////////////////////////////////////////////////////////////////////////////////
	// 位属性相关


	// 设置多选
	void Listbox::setMultiSelect(Bool isMultiSelect)
	{
		_setBitState(EBS_MultiSelect, isMultiSelect);
	}

	// 是否多选
	Bool Listbox::isMultiSelect() const
	{
		return _getBitState(EBS_MultiSelect);
	}


	// 设置整行选中
	void Listbox::setFullRowSelect(Bool isFullRowSelect)
	{
		_setBitState(EBS_FullRowSelect, isFullRowSelect);
	}

	// 是否整行选中
	Bool Listbox::isFullRowSelect() const
	{
		return _getBitState(EBS_FullRowSelect);
	}


	// 设置显示网格
	void Listbox::setDrawGrid(Bool isDrawGrid)
	{
		if (_getBitState(EBS_DrawGrid) == isDrawGrid)
			return;

		_setBitState(EBS_DrawGrid, isDrawGrid);
		setDirtyRect(0);
	}

	// 是否显示网格
	Bool Listbox::isDrawGrid() const
	{
		return _getBitState(EBS_DrawGrid);
	}


	// 设置显示列头
	void Listbox::setDrawHeader(Bool isDrawHeader)
	{
		if (_getBitState(EBS_DrawHeader) == isDrawHeader)
			return;

		_setBitState(EBS_DrawHeader, isDrawHeader);
		_setScrollBar(True, False);
	}

	// 是否显示列头
	Bool Listbox::isDrawHeader() const
	{
		return _getBitState(EBS_DrawHeader);
	}


	///////////////////////////////////////////////////////////////////////////////////////////////
	// 提供给编辑器的属性操作


	#define ListboxProperty(name)	Property name[] =										\
	{																						\
		{	"MultiSelect",			&_uBitState,			PT_Bit24,		""	},			\
		{	"FullRowSelect",		&_uBitState,			PT_Bit25,		""	},			\
		{	"DrawGrid",				&_uBitState,			PT_Bit26,		""	},			\
		{	"DrawHeader",			&_uBitState,			PT_Bit27,		""	},			\
																							\
		{	"PicFile",				&_strPicFile,			PT_String,		""	},			\
		{	"PicAlphaOp",			&_ePicAlphaOp,			PT_Int,			""	},			\
		{	"PicColor",				&_cPicColor,			PT_Color,		""	},			\
		{	"PicRect",				&_rcPicRect,			PT_Rect,		""	},			\
		{	"PicGrid",				&_rcPicGrid,			PT_Rect,		""	},			\
		{	"PicOffset",			&_rcPicOffset,			PT_Rect,		""	},			\
																							\
		{	"HeaderPicFile",		&_strHeaderPicFile,		PT_String,		""	},			\
		{	"HeaderPicAlphaOp",		&_eHeaderPicAlphaOp,	PT_Int,			""	},			\
		{   "HeaderPicColor",		&_cHeaderPicColor,		PT_Color,		""  },          \
		{	"HeaderPicRect",		&_rcHeaderPicRect,		PT_Rect,		""	},			\
		{	"HeaderPicGrid",		&_rcHeaderPicGrid,		PT_Rect,		""	},			\
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
		{	"FontAlignType",		&_eFontAlignType,		PT_Int,			""	},			\
																							\
		{	"ListOffset",			&_rcListOffset,			PT_Rect,		""	},			\
		{	"HeaderHeight",			&_uHeaderHeight,		PT_UInt16,		""	},			\
		{	"ItemHeight",			&_uItemHeight,			PT_UInt16,		""	},			\
	}


	// 获得属性个数
	UInt Listbox::getNumPropert()
	{
		UInt uNum = Window::getNumPropert();

		ListboxProperty(propList);
		uNum += sizeof(propList) / sizeof(propList[0]);

		return uNum;
	}


	// 设置窗体属性
	Bool Listbox::setPropert(const String& strKey, const String& strValue)
	{
		if (Window::setPropert(strKey, strValue))
			return True;

		ListboxProperty(propList);
		UInt uNum = sizeof(propList) / sizeof(propList[0]);

		return setPropertyImpl(propList, uNum, strKey, strValue);
	}


	// 获得窗体属性
	Bool Listbox::getPropert(UInt uIndex, String& strKey, String& strValue)
	{
		if (Window::getPropert(uIndex, strKey, strValue))
			return True;

		ListboxProperty(propList);
		UInt uNum = sizeof(propList) / sizeof(propList[0]);
		UInt uNumWindow = Window::getNumPropert();

		return getPropertyImpl(propList, uNum, uIndex - uNumWindow, strKey, strValue);
	}


	///////////////////////////////////////////////////////////////////////////////////////////////


	// 根据坐标，拾取行列
	Bool Listbox::_getItemColumnRow(const Point& ptPos, Point& ptColRow)
	{
		if (ptPos.x < 0 || ptPos.y < 0)	// 超出左上边界
		{
			ptColRow.setPoint(Invalid, Invalid);
			return 0;
		}

		// 转换到左上第一个 Item 的原点坐标
		UInt uX = ptPos.x + _ptDrawOffset.x + _rcListOffset.x1;
		UInt uY = ptPos.y + _ptDrawOffset.y + _rcListOffset.y1;
		if (isDrawHeader())
			uY -= _uHeaderHeight;

		UInt uColumn = 0, uWidth = 0;
		for (; uColumn < getNumColumn(); ++uColumn)
		{
			uWidth += getColumnWidth(uColumn);

			if (uX < uWidth)
				break;
		}

		if (uColumn >= getNumColumn())	// 超出右边界
		{
			ptColRow.setPoint(Invalid, Invalid);
			return False;
		}

		UInt uIndex = (uY / _uItemHeight) * getNumColumn() + uColumn;
		ListItem* pItem = (ListItem*)_pQueueItem->getAt(uIndex);

		if (0 == pItem)	// 超出下边界
		{
			ptColRow.setPoint(Invalid, Invalid);
			return False;
		}

		ptColRow.x = (Int16)uColumn;
		ptColRow.y = (Int16)(uY / _uItemHeight);
		return True;
	}


	// 根据行列，获得单元格
	Listbox::ListItem* Listbox::_getItem(const Point& ptColRow)
	{
		if (Invalid == ptColRow.x || Invalid == ptColRow.y)
			return 0;

		UInt uIndex = ptColRow.y * getNumColumn() + ptColRow.x;
		ListItem* pItem = (ListItem*)_pQueueItem->getAt(uIndex);

		return pItem;
	}


	// 设置滚动条坐标
	void Listbox::_setScrollBar(Bool isVert, Bool isHorz)
	{
		ScrollInfo si;

		if (isVert)
		{
			// 垂直
			si.uPageSize = getSize().y - _rcListOffset.getSize().y - (isDrawHeader() ? _uHeaderHeight : 0);
			si.uDocSize	 = _uItemHeight * getNumItemRow();
			si.uPos		 = _ptDrawOffset.y;
			si.uStep	 = _uItemHeight;
			setVertScroll(si);
		}

		if (isHorz)
		{
			// 水平
			si.uPageSize = getSize().x - _rcListOffset.getSize().x;
			si.uDocSize	 = getAllColumnWidth();
			si.uPos		 = _ptDrawOffset.x;
			si.uStep	 = _rcListOffset.getSize().x / 4;
			setHorzScroll(si);
		}

		setDirtyRect(0);
	}

}
