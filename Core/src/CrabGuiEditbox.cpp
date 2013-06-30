
#include "CrabGuiDefine.h"
#include "CrabGuiEditbox.h"

#include "CrabGuiSystem.h"
#include "CrabGuiCanvas.h"
#include "CrabGuiEventHandle.h"

#include "CrabGuiFont.h"

#include <stdlib.h>


//
//	待优化内容
//
//	文字渲染：不在矩形范围内的，直接跳过，不渲染
//	待测试：_rcFontWindow 不贴边测试
//


namespace CrabGui
{

	Editbox::Editbox(System* pSystem, UInt uID)
		: Window(pSystem, uID)
		, _uBeamPos(0), _uDownPos(0), _uBeamElapse(0)
		, _ePicAlphaOp(AO_Blend)
		, _uFontSize(14), _cFontColor(0xFF000000), _cFontSelectColor(0xFFFFFFFF), _cFontSelectBackColor(0xFF00007F), _eFontAlphaOp(AO_Blend), _eFontAlignType(AT_LeftTop), _uLineHeight(0)
	{
	}

	Editbox::~Editbox()
	{
	}


	PCStr Editbox::TypeName = "Editbox";


	// 获得类型名
	PCStr Editbox::getTypeName()
	{
		return TypeName;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////
	// 渲染相关


	// 光标闪烁间隔（单位：毫秒）
	const UInt Editbox::BeamFlashTime = 500;


	// 逝去时间
	Bool Editbox::update(UInt uElapse)
	{
		if (_pSystem->getFocusWindow() == this)
		{
			// 光标闪烁
			UInt uNewBeamElapse = _uBeamElapse + uElapse;

			if (_uBeamElapse / BeamFlashTime != uNewBeamElapse / BeamFlashTime)
				setDirtyRect(0);

			_uBeamElapse = uNewBeamElapse;
		}

		return Window::update(uElapse);
	}


	// 渲染
	void Editbox::render(Canvas* pCanvas, const Point& ptBase)
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
			{
				Rect rcFontCanvas;
				getCanvasRect(rcFontCanvas, _rcFontOffset, ptCanvasBase);

				if (_pSystem->getFocusWindow() == this || _strText.getWLength() > 0)
				{
					// 计算文字大小
					if (_strText.isWCharPosDirty())
					{
						Point ptFixSize = rcFontCanvas.getSize();

						UInt uTextFormat = 0;
						if (isTextWrap())	// 自动换行
							uTextFormat |= Canvas::TFB_TextWrap;
						//if (0)	// 编辑框不支持转义符
						//	uTextFormat |= Canvas::TFB_Translate;

						Canvas::calcTextPixelSize(_strText, &_rcTextDraw, 0, _strFontName, _uFontSize, _uLineHeight, _eFontAlignType, ptFixSize, uTextFormat);
						_strText.setWCharPosDirty(False);	// 手动处理文字，需要手动设置（默认 Canvas::drawText 内部会有设置）
					}
					// 重新计算光标位置
					_fixScrollPos();

					// 计算选中范围
					UInt uSelectLeft  = _uBeamPos < _uDownPos ? _uBeamPos : _uDownPos;
					UInt uSelectRight = _uBeamPos > _uDownPos ? _uBeamPos : _uDownPos;

					// 渲染文字到画布，只能逐字的画
					UInt uWLength = _strText.getWLength();
					UInt uLineHeight = _uLineHeight > _uFontSize ? _uLineHeight : _uFontSize;
					for (UInt i = 0; i < uWLength + 1; ++i)
					{
						WChar wChar = _strText.getWCharAt(i);
						const String::WCharPos* pWCharPos = _strText.getWCharPos(i);

						// 窗体画布坐标 + 字体位置坐标 - 滚动条偏移坐标
						Point ptFontPos(ptBase.x + pWCharPos->ptPos.x - _ptDrawOffset.x, ptBase.y + pWCharPos->ptPos.y - _ptDrawOffset.y);

						Rect rcWChar(ptFontPos.x, ptFontPos.y, ptFontPos.x + pWCharPos->ptSize.x, ptFontPos.y + _uFontSize);
						if (!rcFontCanvas.isIntersect(rcWChar))
							continue;

						// 渲染单个文字
						if ((pWCharPos->uBitState & String::WCBS_Visible) && i < uWLength)
						{
							if (uSelectLeft <= i && i < uSelectRight)
							{
								// 选中文字
								Rect rcWChar(ptFontPos.x, ptFontPos.y, ptFontPos.x + pWCharPos->ptSize.x, ptFontPos.y + uLineHeight);
								Canvas::fillRect(rcWChar, _cFontSelectBackColor, _eFontAlphaOp);

								Canvas::drawChar(_strFontName, _uFontSize, wChar, ptFontPos, _cFontSelectColor, _eFontAlphaOp);
							}
							else
							{
								// 非选中文字
								Canvas::drawChar(_strFontName, _uFontSize, wChar, ptFontPos, _cFontColor, _eFontAlphaOp);
							}
						}

						// 渲染光标
						if (_uBeamPos == i && _pSystem->getFocusWindow() == this && !((_uBeamElapse / BeamFlashTime) & 1))
						{
							Rect rcBeam(ptFontPos.x, ptFontPos.y, ptFontPos.x + 1, ptFontPos.y + uLineHeight);
							Canvas::fillRect(rcBeam, _cFontColor, _eFontAlphaOp);
						}
					}
				}
				else if (_strEmptyText.getWLength() > 0)
				{
					// 渲染空白时的文字到画布
					Canvas::drawText(_strFontName, _uFontSize, _strEmptyText, rcFontCanvas, _uLineHeight, _eFontAlignType, 0, _cFontEmptyColor, 0, _eFontAlphaOp);
				}
			}

			postRender(pCanvas, ptCanvasBase);
		}
	}


	///////////////////////////////////////////////////////////////////////////////////////////////
	// 事件相关


	// 事件响应函数
	Bool Editbox::eventHandle(const EventDesc* pEvent)
	{
		if (pEvent->pWindow == this)
		{
			switch (pEvent->uEvent)
			{
			case Event_FocusChanged:
				{
					// 获得焦点，失去焦点
					setDirtyRect(0);
				}
				break;

			case Event_MouseMove:
				{
					if (pEvent->nParam[3] & CK_LButton)	// 按住左键移动
					{
						Point ptPos(_rcFontOffset.x1 + _ptDrawOffset.x + (Int16)pEvent->nParam[0], _rcFontOffset.y1 + _ptDrawOffset.y + (Int16)pEvent->nParam[1]);
						UInt uNewBeamPos = _getBeamPos(ptPos);

						if (uNewBeamPos != _uBeamPos)
						{
							_uBeamPos = uNewBeamPos;

							setDirtyRect(0);
							_uBeamElapse = 0;
						}
					}
				}
				break;

			case Event_MouseDown:
				{
					Point ptPos(_rcFontOffset.x1 + _ptDrawOffset.x + (Int16)pEvent->nParam[0], _rcFontOffset.y1 + _ptDrawOffset.y + (Int16)pEvent->nParam[1]);
					_uBeamPos = _getBeamPos(ptPos);
					_uDownPos = _uBeamPos;

					setDirtyRect(0);
					_uBeamElapse = 0;
				}
				break;

			//case Event_MouseUp:
			//	{
			//		//setDirtyRect(0);
			//	}
			//	break;

			case Event_KeyDown:
				{
					switch (pEvent->nParam[0])
					{

					case KB_Backspace:	// 向前删除
						{
							if (isReadOnly())
								break;

							if (_uBeamPos != _uDownPos)
							{
								_deleteSelected();
							}
							else if (_uBeamPos > 0)
							{
								_strText.removeWChar(_uBeamPos - 1, 1);
								_uDownPos = --_uBeamPos;
							}

							setDirtyRect(0);
							_uBeamElapse = 0;
						}
						break;

					case KB_Delete:		// 向后删除
						{
							if (isReadOnly())
								break;

							if (_uBeamPos != _uDownPos)
							{
								_deleteSelected();
							}
							else
							{
								_strText.removeWChar(_uBeamPos, 1);
							}

							setDirtyRect(0);
							_uBeamElapse = 0;
						}
						break;

					case KB_A:		// CTRL + A 全选
						{
							if (pEvent->nParam[3] & CK_Ctrl)
							{
								_uDownPos = 0;
								_uBeamPos = _strText.getWLength();

								setDirtyRect(0);
								_uBeamElapse = 0;
							}
						}
						break;

					case KB_Left:		// 向左移
						{
							if (pEvent->nParam[3] & CK_Shift)
							{
								if (_uBeamPos > 0)
									--_uBeamPos;
							}
							else
							{
								if (_uBeamPos > 0)
									--_uBeamPos;

								_uDownPos = _uBeamPos;
							}
							setDirtyRect(0);
							_uBeamElapse = 0;
						}
						break;

					case KB_Right:		// 向右移
						{
							if (pEvent->nParam[3] & CK_Shift)
							{
								if (_uBeamPos < _strText.getWLength())
									++_uBeamPos;
							}
							else
							{
								if (_uBeamPos < _strText.getWLength())
									++_uBeamPos;

								_uDownPos = _uBeamPos;
							}
							setDirtyRect(0);
							_uBeamElapse = 0;
						}
						break;

					case KB_Up:			// 上移一行
						{
							//Point ptPos; Bool isVisible;
							//if (_strText.getWCharPos(_uBeamPos, ptPos, isVisible))

							const String::WCharPos* pWCharPos = _strText.getWCharPos(_uBeamPos);
							if (pWCharPos)
							{
								Point ptPos = pWCharPos->ptPos;
								ptPos.y -= (Int16)_uLineHeight;

								if (pEvent->nParam[3] & CK_Shift)
								{
									_uBeamPos = _getBeamPos(ptPos);
								}
								else
								{
									_uDownPos = _uBeamPos = _getBeamPos(ptPos);
								}

							}
							setDirtyRect(0);
							_uBeamElapse = 0;
						}
						break;

					case KB_Down:		// 下移一行
						{
							const String::WCharPos* pWCharPos = _strText.getWCharPos(_uBeamPos);
							if (pWCharPos)
							{
								Point ptPos = pWCharPos->ptPos;
								ptPos.y += (Int16)_uLineHeight;

								if (pEvent->nParam[3] & CK_Shift)
								{
									_uBeamPos = _getBeamPos(ptPos);
								}
								else
								{
									_uDownPos = _uBeamPos = _getBeamPos(ptPos);
								}
							}
							setDirtyRect(0);
							_uBeamElapse = 0;
						}
						break;

					case KB_Home:		// 光标移到头
						{
							if (pEvent->nParam[3] & CK_Shift)
							{
								_uBeamPos = 0;
							}
							else
							{
								_uDownPos = _uBeamPos = 0;
							}

							setDirtyRect(0);
							_uBeamElapse = 0;
						}
						break;

					case KB_End:	// 光标移到结尾
						{
							if (pEvent->nParam[3] & CK_Shift)
							{
								_uBeamPos = _strText.getWLength();
							}
							else
							{
								_uDownPos = _uBeamPos = _strText.getWLength();
							}

							setDirtyRect(0);
							_uBeamElapse = 0;
						}
						break;

					case KB_PageUp:		// 向上翻页
						{
							const String::WCharPos* pWCharPos = _strText.getWCharPos(_uBeamPos);
							if (pWCharPos)
							{
								//ScrollInfo stScrollInfo;
								//getVertScroll(stScrollInfo);

								//Point ptPos = pWCharPos->ptPos;
								//ptPos.y -= (Int16)stScrollInfo.uPage;

								//if (pEvent->nParam[3] & CK_Shift)
								//{
								//	_uBeamPos = _getBeamPos(ptPos);
								//}
								//else
								//{
								//	_uDownPos = _uBeamPos = _getBeamPos(ptPos);
								//}
							}
							setDirtyRect(0);
							_uBeamElapse = 0;
						}
						break;

					case KB_PageDown:	// 向下翻页
						{
							const String::WCharPos* pWCharPos = _strText.getWCharPos(_uBeamPos);
							if (pWCharPos)
							{
								//ScrollInfo stScrollInfo;
								//getVertScroll(stScrollInfo);

								//Point ptPos = pWCharPos->ptPos;
								//ptPos.y += (Int16)stScrollInfo.uPage;

								//if (pEvent->nParam[3] & CK_Shift)
								//{
								//	_uBeamPos = _getBeamPos(ptPos);
								//}
								//else
								//{
								//	_uDownPos = _uBeamPos = _getBeamPos(ptPos);
								//}
							}
							setDirtyRect(0);
							_uBeamElapse = 0;
						}
						break;

					} // end switch
				}
				break;

			//case Event_KeyUp:
			//	{
			//	}
			//	break;

			case Event_KeyChar:		// 插入字符
				{
					if (isReadOnly())
						break;

					if (_uBeamPos != _uDownPos)
					{
						_deleteSelected();
					}

					WChar szWChar[2] = {0};

					if (pEvent->nParam[0] == KB_Return)
					{
						if (isMultiLine())
						{
							szWChar[0] = (WChar)('\n');
						}
						else
						{
							break;
						}
					}
					else
					{
						szWChar[0] = (WChar) pEvent->nParam[0];
					}

					_strText.insertWChar(_uBeamPos, szWChar);
					_uDownPos = ++_uBeamPos;

					setDirtyRect(0);
					_uBeamElapse = 0;
				}
				break;
			}
		}

		return Window::eventHandle(pEvent);
	}


	///////////////////////////////////////////////////////////////////////////////////////////////
	// 属性相关


	// 设置行高
	void Editbox::setLineHeight(UInt uLineHeight)
	{
		_uLineHeight = uLineHeight;
	}

	// 获得行高
	UInt Editbox::getLineHeight()
	{
		return _uLineHeight > _uFontSize ? _uLineHeight : _uFontSize;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////
	// 位属性相关


	// 设置只读
	void Editbox::setReadOnly(Bool isReadOnly)
	{
		_setBitState(EBS_ReadOnly, isReadOnly);
	}

	// 是否只读
	Bool Editbox::isReadOnly() const
	{
		return _getBitState(EBS_ReadOnly);
	}


	// 设置多行
	void Editbox::setMultiLine(Bool isMultiLine)
	{
		_setBitState(EBS_MultiLine, isMultiLine);

		setDirtyRect(0);
		_uBeamElapse = 0;
	}

	// 是否多行
	Bool Editbox::isMultiLine() const
	{
		return _getBitState(EBS_MultiLine);
	}


	// 设置自动换行
	void Editbox::setTextWrap(Bool isTextWrap)
	{
		_setBitState(EBS_TextWrap, isTextWrap);

		// 将字符串，强制设成脏，用于重新计算位置
		_strText.setWCharPosDirty(True);

		setDirtyRect(0);
		_uBeamElapse = 0;
	}

	// 是否自动换行
	Bool Editbox::isTextWrap() const
	{
		return _getBitState(EBS_TextWrap);
	}


	///////////////////////////////////////////////////////////////////////////////////////////////
	// 提供给编辑器的属性操作

	#define EditboxProperty(name)	Property name[] =										\
	{																						\
		{	"ReadOnly",				&_uBitState,			PT_Bit24,		""	},			\
		{	"MultiLine",			&_uBitState,			PT_Bit25,		""	},			\
		{	"TextWrap",				&_uBitState,			PT_Bit26,		""	},			\
																							\
		{	"PicFile",				&_strPicFile,			PT_String,		""	},			\
		{	"PicAlphaOp",			&_ePicAlphaOp,			PT_Int,			""	},			\
		{	"PicRect",				&_rcPicRect,			PT_Rect,		""	},			\
		{	"PicGrid",				&_rcPicGrid,			PT_Rect,		""	},			\
		{	"PicOffset",			&_rcPicOffset,			PT_Rect,		""	},			\
																							\
		{	"FontName",				&_strFontName,			PT_String,		""	},			\
		{	"FontSize",				&_uFontSize,			PT_UInt,		""	},			\
		{	"FontColor",			&_cFontColor,			PT_Color,		""	},			\
		{	"FontSelectColor",		&_cFontSelectColor,		PT_Color,		""	},			\
		{	"FontSelectBackColor",	&_cFontSelectBackColor,	PT_Color,		""	},			\
		{	"FontAlphaOp",			&_eFontAlphaOp,			PT_Int,			""	},			\
		{	"FontAlignType",		&_eFontAlignType,		PT_Int,			""	},			\
		{	"FontOffset",			&_rcFontOffset,			PT_Rect,		""	},			\
		{	"LineHeight",			&_uLineHeight,			PT_UInt,		""	},			\
																							\
		{	"FontEmptyText",		&_strEmptyText,			PT_String,		""	},			\
		{	"FontEmptyTextColor",	&_cFontEmptyColor,		PT_Color,		""	},			\
	}


	// 获得属性个数
	UInt Editbox::getNumPropert()
	{
		UInt uNum = Window::getNumPropert();

		EditboxProperty(propList);
		uNum += sizeof(propList) / sizeof(propList[0]);

		return uNum;
	}


	// 设置窗体属性
	Bool Editbox::setPropert(const String& strKey, const String& strValue)
	{
		if (Window::setPropert(strKey, strValue))
			return True;

		EditboxProperty(propList);
		UInt uNum = sizeof(propList) / sizeof(propList[0]);

		return setPropertyImpl(propList, uNum, strKey, strValue);
	}


	// 获得窗体属性
	Bool Editbox::getPropert(UInt uIndex, String& strKey, String& strValue)
	{
		if (Window::getPropert(uIndex, strKey, strValue))
			return True;

		EditboxProperty(propList);
		UInt uNum = sizeof(propList) / sizeof(propList[0]);
		UInt uNumWindow = Window::getNumPropert();

		return getPropertyImpl(propList, uNum, uIndex - uNumWindow, strKey, strValue);
	}


	///////////////////////////////////////////////////////////////////////////////////////////////
	// 辅助函数


	// 根据坐标，获得光标坐标
	UInt Editbox::_getBeamPos(const Point& ptPos)
	{
		UInt uWLength = _strText.getWLength();
		Point ptFixPos = ptPos;

		// 取第一个字，判断顶越界
		const String::WCharPos* pWCharPos = _strText.getWCharPos(0);
		if (ptFixPos.y < pWCharPos->ptPos.y)
			ptFixPos.y = pWCharPos->ptPos.y;

		// 取最后一个 \0 位置，判断底越界
		pWCharPos = _strText.getWCharPos(uWLength);
		if (ptFixPos.y >= pWCharPos->ptPos.y)
		{
			// 直接返回最后一个位置
			if (ptPos.x >= pWCharPos->ptPos.x)
				return uWLength;

			ptFixPos.y = pWCharPos->ptPos.y;
		}

		// 判断距离，取最近的一个
		Bool isSameLine = False;
		UInt uIndex = 0;

		for (UInt i = 0; i < uWLength + 1; ++i)
		{
			pWCharPos = _strText.getWCharPos(i);

			if (pWCharPos->ptPos.y <= ptFixPos.y && ptFixPos.y < pWCharPos->ptPos.y + pWCharPos->ptSize.y)
			{
				isSameLine = True;
				uIndex = i;

				if (ptFixPos.x <= pWCharPos->ptPos.x + (pWCharPos->ptSize.x >> 1))
					return uIndex;

				continue;
			}

			if (isSameLine)	// 超出右的处理
			{
				return uIndex;
			}
		}

		// 没有找到合适位置，返回当前光标位置（如果走到这里，说明算法有问题）
		return _uBeamPos;
	}


	// 删除选中的
	void Editbox::_deleteSelected()
	{
		if (_uDownPos < _uBeamPos)
		{
			_strText.removeWChar(_uDownPos, _uBeamPos - _uDownPos);
			_uBeamPos = _uDownPos;
		}
		else if (_uBeamPos < _uDownPos)
		{
			_strText.removeWChar(_uBeamPos, _uDownPos - _uBeamPos);
			_uDownPos = _uBeamPos;
		}
	}


	// 修正滚动坐标
	void Editbox::_fixScrollPos()
	{
		const String::WCharPos* pWCharPos = _strText.getWCharPos(_uBeamPos);
		Point ptBeamPos = pWCharPos->ptPos;

		// 字体矩形
		Rect rcFontWindow;
		getWindowRect(rcFontWindow);
		rcFontWindow.offset(_rcFontOffset);

		// 横向边界检测
		if (_rcTextDraw.getWidth() <= rcFontWindow.getWidth())
		{
			_ptDrawOffset.x = 0;
		}
		else if (ptBeamPos.x - _ptDrawOffset.x > rcFontWindow.getWidth())	// 超出右边界
		{
			_ptDrawOffset.x = ptBeamPos.x - rcFontWindow.getWidth() * 5 / 6;
		}
		else if (ptBeamPos.x < _ptDrawOffset.x)	// 超出左边界
		{
			_ptDrawOffset.x = ptBeamPos.x - rcFontWindow.getWidth() * 1 / 6;

			if (_ptDrawOffset.x < 0)
				_ptDrawOffset.x = 0;
		}

		// 纵向边界检测
		Int16 nLineHeight = (Int16)getLineHeight();
		if (_rcTextDraw.getHeight() <= rcFontWindow.getHeight())
		{
			_ptDrawOffset.y = 0;
		}
		else if (ptBeamPos.y < _ptDrawOffset.y)		// 超出上边界
		{
			_ptDrawOffset.y = ptBeamPos.y;

			if (_ptDrawOffset.y < 0)
				_ptDrawOffset.y = 0;
		}
		else if (ptBeamPos.y + nLineHeight - _ptDrawOffset.y > rcFontWindow.getHeight())
		{
			_ptDrawOffset.y = ptBeamPos.y + nLineHeight - rcFontWindow.getHeight();
		}
	}

}
