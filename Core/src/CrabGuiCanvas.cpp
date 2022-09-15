
#include "CrabGuiDefine.h"
#include "CrabGuiCanvas.h"

#include "CrabGuiSystem.h"
#include "CrabGuiQueue.h"
#include "CrabGuiRenderer.h"
#include "CrabGuiTexture.h"
#include "CrabGuiRenderTarget.h"
#include "CrabGuiFontTexture.h"

#include <string.h>


//
//	待优化内容
//
//	图片渲染：自动截减矩形
//	文字渲染：不在脏矩形范围内的，直接跳过，不处理
//


namespace CrabGui
{

	Canvas::Canvas()
		: _pRenderTarget(0)
		, _pGridVertices(0), _pGridColors(0), _pGridTexPoints(0), _pGridTriangles(0)
	{
	}

	Canvas::~Canvas(void)
	{
		setSize(False, Point(), Point());
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////
	// 渲染相关


	// 渲染（可以传入效果）
	void Canvas::render(const Point& ptPos, void* pFadeout)
	{
		if (_pRenderTarget)
		{
			Point ptSize = _pRenderTarget->getRenderSize();
			if (ptSize.isZero())
				return;

			Real rScale = 1.0f;
			ptSize.x *= rScale;
			ptSize.y *= rScale;

			if (_pGridVertices && _pGridTexPoints && _pGridTriangles)
			{
				Real r = 0.0f;
				Real rSpring = 0.005f;
				r = (Real)ptPos.x + 0;			_pGridVertices[0].x += (r - _pGridVertices[0].x) * rSpring;
				r = (Real)ptPos.y + 0;			_pGridVertices[0].y += (r - _pGridVertices[0].y) * rSpring;
				r = (Real)ptPos.x + ptSize.x;	_pGridVertices[1].x += (r - _pGridVertices[1].x) * rSpring;
				r = (Real)ptPos.y + 0;			_pGridVertices[1].y += (r - _pGridVertices[1].y) * rSpring;
				r = (Real)ptPos.x + 0;			_pGridVertices[2].x += (r - _pGridVertices[2].x) * rSpring;
				r = (Real)ptPos.y + ptSize.y;	_pGridVertices[2].y += (r - _pGridVertices[2].y) * rSpring;
				r = (Real)ptPos.x + ptSize.x;	_pGridVertices[3].x += (r - _pGridVertices[3].x) * rSpring;
				r = (Real)ptPos.y + ptSize.y;	_pGridVertices[3].y += (r - _pGridVertices[3].y) * rSpring;

				System::getSingletonPtr()->getRenderer()->renderRenderTargetVertices(_pRenderTarget, 4, _pGridVertices, 0, _pGridTexPoints, 6, _pGridTriangles);
			}
			else
			{
				Rect rcTex(0, 0, ptSize.x, ptSize.y);
				Rect rcScreen(ptPos.x, ptPos.y, ptPos.x + ptSize.x, ptPos.y + ptSize.y);
				System::getSingletonPtr()->getRenderer()->renderRenderTarget(_pRenderTarget, 0xFFFFFFFF, rcTex, rcScreen);
			}
		}
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////
	// 矩形裁减相关


	// 合并脏矩型
	void Canvas::mergeDirtyRect(const Rect& rcDirty)
	{
		if(_rcDirty.isZero())
		{
			_rcDirty = rcDirty;
		}
		else
		{
			_rcDirty.merge(rcDirty);
		}
	}


	// 获得脏矩型
	const Rect& Canvas::getDirtyRect() const
	{
		return _rcDirty;
	}


	// 脏矩形清空
	void Canvas::setDirtyRectZero()
	{
		_rcDirty.setZero();
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////
	// 画布属性相关


	// 重设大小
	void Canvas::setSize(Bool isVisible, Point ptSize, Point ptGridSize)
	{
		if (isVisible && ptSize.getArea() > 0)
		{
			if (0 == _pRenderTarget)
			{
				_pRenderTarget = System::getSingletonPtr()->getRenderer()->createRenderTarget();
			}

			_pRenderTarget->setRenderSize(ptSize);
		}
		else
		{
			if (_pRenderTarget)
			{
				System::getSingletonPtr()->getRenderer()->destroyRenderTarget(_pRenderTarget);
				_pRenderTarget = 0;
			}
		}

		if (ptGridSize != _ptGridSize)
		{
			_ptGridSize.setZero();
			CrabDeleteArray(_pGridVertices);
			CrabDeleteArray(_pGridColors);
			CrabDeleteArray(_pGridTexPoints);
			CrabDeleteArray(_pGridTriangles);

			if (ptGridSize.x > 1 || ptGridSize.y > 1)
			{
				_ptGridSize.setPoint(ptGridSize.x, ptGridSize.y);

				Int nVertexSize		= _ptGridSize.getArea();
				Int nTriangleSize	= (_ptGridSize.x - 1) * (_ptGridSize.y - 1) * 2 * 3;	// rectrangle => 2triangles, triangle => 3points

				if (!_pGridVertices)
					_pGridVertices = CrabNewArray(PointReal, nVertexSize);
				//if (!_pGridColors)
				//	_pGridColors = CrabNewArray(PointReal, nVertexSize);
				if (!_pGridTexPoints)
					_pGridTexPoints = CrabNewArray(PointReal, nVertexSize);
				if (!_pGridTriangles)
					_pGridTriangles = CrabNewArray(Int16, nTriangleSize);
	/*
				Int nVertexIndex = 0;
				Int nTriangleIndex = 0;
				for (Int i = 0; i < nVertexSize; ++i)
				{
					_pGridVertices[nVertexIndex].x = 0;
					_pGridVertices[nVertexIndex].y = 0;

					++nVertexIndex;
				}
	*/
				_pGridVertices[0].x = 0,	_pGridVertices[0].y = 0;
				_pGridVertices[1].x = 0,	_pGridVertices[1].y = 0;
				_pGridVertices[2].x = 0,	_pGridVertices[2].y = 0;
				_pGridVertices[3].x = 0,	_pGridVertices[3].y = 0;

				_pGridTexPoints[0].x = 0.0f,	_pGridTexPoints[0].y = 0.0f;
				_pGridTexPoints[1].x = 1.0f,	_pGridTexPoints[1].y = 0.0f;
				_pGridTexPoints[2].x = 0.0f,	_pGridTexPoints[2].y = 1.0f;
				_pGridTexPoints[3].x = 1.0f,	_pGridTexPoints[3].y = 1.0f;

				_pGridTriangles[0] = 0;
				_pGridTriangles[1] = 1;
				_pGridTriangles[2] = 2;
				_pGridTriangles[3] = 1;
				_pGridTriangles[4] = 3;
				_pGridTriangles[5] = 2;
			}
		}
	}


	// 绑定渲染目标
	void Canvas::bind()
	{
		_pRenderTarget->bind(_rcDirty, 0xFFFFFFFF);
	}


	// 解绑渲染目标
	void Canvas::unbind()
	{
		_pRenderTarget->unbind();
	}


	///////////////////////////////////////////////////////////////////////////////////////////////
	// 纯色渲染


	// 画线
	void Canvas::lineTo(const Point& ptPos1, const Point ptPos2, Color cLine, AlphaOp eAlphaOp)
	{
		//Rect rcTex(0, 0, 1, 1);
		//Rect rcCanvas(ptPos1.x, ptPo1
		//Renderer* pRenderer = System::getSingletonPtr()->getRenderer();
		//pRenderer->renderTexture(0, cRect, rcTex, rcCanvas);
	}


	// 颜色填充
	void Canvas::fillRect(const Rect& rcCanvas, Color cRect, AlphaOp eAlphaOp)
	{
		Rect rcTex(0, 0, 1, 1);
		Renderer* pRenderer = System::getSingletonPtr()->getRenderer();
		pRenderer->renderTexture(0, cRect, rcTex, rcCanvas);
	}


	///////////////////////////////////////////////////////////////////////////////////////////////
	// 图片渲染


	// 简单图片渲染
	void Canvas::drawPic(const String& strName, const Rect& rcPic, const Rect& rcCanvas, Color cPic, AlphaOp eAlphaOp)
	{
		// 获取图片文件资源
		Texture* pTexture = System::getSingletonPtr()->acquireTexture(strName);
		if (0 == pTexture)
			return;

		Renderer* pRenderer = System::getSingletonPtr()->getRenderer();
		pRenderer->renderTexture(pTexture, cPic, rcPic, rcCanvas);

		System::getSingletonPtr()->releaseTexture(pTexture);
	}


	// 九宫格图片渲染
	void Canvas::drawGridPic(const String& strName, const Rect& rcPic, const Rect& rcGrid, const Rect& rcCanvas, Color cPic, AlphaOp eAlphaOp)
	{
		// 获取图片文件资源
		Texture* pTexture = System::getSingletonPtr()->acquireTexture(strName);
		if (0 == pTexture)
			return;

		Renderer* pRenderer = System::getSingletonPtr()->getRenderer();

		if (rcGrid.isZero())
		{
			// 不切割，直接绘源图
			pRenderer->renderTexture(pTexture, cPic, rcPic, rcCanvas);
		}
		else
		{
			// 切出图片 四条分割线
			Int16 nSrc_x[4] = { rcPic.x1, rcGrid.x1, rcGrid.x2, rcPic.x2 };
			Int16 nSrc_y[4] = { rcPic.y1, rcGrid.y1, rcGrid.y2, rcPic.y2 };

			// 切出画布 四条分割线
			Int16 nDst_x[4] = { rcCanvas.x1, rcCanvas.x1 + (rcGrid.x1 - rcPic.x1), rcCanvas.x2 - (rcPic.x2 - rcGrid.x2), rcCanvas.x2 };
			Int16 nDst_y[4] = { rcCanvas.y1, rcCanvas.y1 + (rcGrid.y1 - rcPic.y1), rcCanvas.y2 - (rcPic.y2 - rcGrid.y2), rcCanvas.y2 };

			Rect rcSrc, rcDst;
			for (Int y = 0; y < 3; ++y)
			{
				for (Int x = 0; x < 3; ++x)
				{
					rcSrc.setRect(nSrc_x[x], nSrc_y[y], nSrc_x[x + 1], nSrc_y[y + 1]);
					rcDst.setRect(nDst_x[x], nDst_y[y], nDst_x[x + 1], nDst_y[y + 1]);

					if (rcSrc.getWidth() > 0 && rcSrc.getHeight() > 0)
					{
						pRenderer->renderTexture(pTexture, cPic, rcSrc, rcDst);
					}
				}
			}
		}

		System::getSingletonPtr()->releaseTexture(pTexture);
	}


	///////////////////////////////////////////////////////////////////////////////////////////////
	// 字体渲染


	// 简单文字渲染
	void Canvas::drawChar(const String& strFontName, UInt uFontSize, UInt uCharCode, Point ptPos, Color cText, AlphaOp eAlphaOp)
	{
		FontTexture* pFontTexture = System::getSingletonPtr()->acquireFont(strFontName);
		FontTexture::FontChar* pFontChar = pFontTexture->getFontChar(uCharCode, uFontSize, 0);
		
		if (0 == pFontChar)
			return;

		Renderer* pRenderer = System::getSingletonPtr()->getRenderer();
		Rect rcDst;
		rcDst.x1 = ptPos.x + pFontChar->ptOffset.x;
		rcDst.y1 = ptPos.y + pFontChar->ptOffset.y;
		rcDst.x2 = rcDst.x1 + pFontChar->rcTexture.getWidth();
		rcDst.y2 = rcDst.y1 + pFontChar->rcTexture.getHeight();

		pRenderer->renderTexture(pFontChar->pTexture, cText, pFontChar->rcTexture, rcDst);
	}


	// 字符串文字渲染
	void Canvas::drawText(const String& strFontName, UInt uFontSize, String& strText, const Rect& rcText, UInt uLineHeight, AlignType eAlignType, UInt uTextFormat, Color cText, Color cShadow, AlphaOp eAlphaOp)
	{
		if (strText.isWCharPosDirty())
		{
			Point ptFixSize = rcText.getSize();
			Canvas::calcTextPixelSize(strText, 0, 0, strFontName, uFontSize, uLineHeight, eAlignType, ptFixSize, uTextFormat);
			strText.setWCharPosDirty(False);
		}

		UInt uWLength = strText.getWLength();
		for (UInt i = 0; i < uWLength; ++i)
		{
			WChar wChar = strText.getWCharAt(i);
			const String::WCharPos* pWCharPos = strText.getWCharPos(i);

			if (pWCharPos->uBitState & String::WCBS_Visible)
			{
				Point ptPos(rcText.x1 + pWCharPos->ptPos.x, rcText.y1 + pWCharPos->ptPos.y);

				if (cShadow)
				{
					Point ptPosShadow(ptPos.x + 1, ptPos.y + 1);
					drawChar(strFontName, uFontSize, wChar, ptPosShadow, cShadow, eAlphaOp);		// 先渲染影子
				}

				drawChar(strFontName, uFontSize, wChar, ptPos, cText, eAlphaOp);		// 再渲染字体
			}
		} // end for
	}


	///////////////////////////////////////////////////////////////////////////////////////////////
	// 工具相关函数


	// 计算文字像素大小（根据宽度，pTextRect 返回字符串文字的宽度和高度，pColRowSize最大返回行数和列数）
	void Canvas::calcTextPixelSize(String& strText, Rect* pTextRect, Point* pColRowSize, const String& strFontName, UInt uFontSize, UInt uLineHeight, AlignType eAlignType, const Point& ptFixSize, UInt uTextFormat)
	{
		Point ptPos, ptPixelSize;
		//UInt uRowBegin = 0;

		String::WCharPos stWCharPos;

		FontTexture* pFontTexture = System::getSingletonPtr()->acquireFont(strFontName);

		if (uLineHeight < uFontSize)
			uLineHeight = uFontSize;

		//
		// 计算每个字的位置，默认左上对齐
		//
		UInt uWLength = strText.getWLength();
		for (UInt i = 0; i < uWLength; )
		{
			UInt uCharSize = 1;

			stWCharPos.ptPos = ptPos;
			stWCharPos.ptSize.setPoint(0, (Int16)uFontSize);
			stWCharPos.uBitState = 0;

			WChar wChar = strText.getWCharAt(i);

			// 转义符处理，二个字的，需要特殊处理
			if ((uTextFormat & TFB_Translate) && wChar == (WChar)('\\') && i < uWLength - 1)
			{
				WChar wCharNext = strText.getWCharAt(i + 1);

				if (wCharNext == (WChar)('n'))
				{
					uCharSize = 2;

					// 第一个字是软回车
					stWCharPos.uBitState |= String::WCBS_Return;
					strText.setWCharPos(i,     stWCharPos);

					stWCharPos.uBitState = 0;
					strText.setWCharPos(i + 1, stWCharPos);

					// 整体宽度
					if (ptPixelSize.x < ptPos.x)
						ptPixelSize.x = ptPos.x;

					ptPos.x = 0;
					ptPos.y += uLineHeight;

					i += uCharSize;
					continue;
				}
				else if (wCharNext == (WChar)('t'))
				{
					uCharSize = 2;

					// Tab 相当于二倍全角
					stWCharPos.ptSize.x = (Int16)uFontSize * 2;
					stWCharPos.uBitState = 0;
					strText.setWCharPos(i,     stWCharPos);
					strText.setWCharPos(i + 1, stWCharPos);
				}
				else if (wCharNext == (WChar)('\\'))
				{
					uCharSize = 2;

					// 读取单个字的大小
					pFontTexture->getFontSize(stWCharPos.ptSize, wCharNext, uFontSize, 0);

					// 第一个可见
					stWCharPos.uBitState |= String::WCBS_Visible;
					strText.setWCharPos(i,     stWCharPos);
					// 第二个不可见
					stWCharPos.uBitState = 0;
					strText.setWCharPos(i + 1, stWCharPos);
				}
				else
				{
					uCharSize = 2;

					stWCharPos.uBitState |= String::WCBS_Visible;
					strText.setWCharPos(i,     stWCharPos);
					strText.setWCharPos(i + 1, stWCharPos);
				}
			}
			else if (wChar == (WChar)('\n'))
			{
				// 整体宽度
				if (ptPixelSize.x < ptPos.x)
					ptPixelSize.x = ptPos.x;

				ptPos.x = 0;
				ptPos.y += uLineHeight;

				stWCharPos.uBitState |= String::WCBS_Return;
				strText.setWCharPos(i, stWCharPos);

				++i;
				continue;
			}
			else if (wChar == (WChar)('\t'))
			{
				stWCharPos.ptSize.x = (Int16)uFontSize * 2;	// Tab 相当于二倍全角
				strText.setWCharPos(i, stWCharPos);
			}
			else	// 未被处理过的字符，按默认处理
			{
				// 读取单个字的大小
				pFontTexture->getFontSize(stWCharPos.ptSize, wChar, uFontSize, 0);

				stWCharPos.uBitState |= String::WCBS_Visible;
				strText.setWCharPos(i, stWCharPos);
			}

			// 自动换行
			if ((uTextFormat & TFB_TextWrap) && (ptPos.x + stWCharPos.ptSize.x > ptFixSize.x - 1))	// -1 考虑阴影位置
			{
				// 整体宽度
				if (ptPixelSize.x < ptPos.x)
					ptPixelSize.x = ptPos.x;

				// 开始新的一行，重设坐标，虚拟回车
				ptPos.x = 0;
				ptPos.y += uLineHeight;

				stWCharPos.ptPos = ptPos;
				stWCharPos.uBitState |= String::WCBS_Return;
				strText.setWCharPos(i, stWCharPos);
			}

			ptPos.x += stWCharPos.ptSize.x;
			i += uCharSize;
		}
		// 最后一个 \0 也有坐标
		stWCharPos.ptPos = ptPos;
		stWCharPos.ptSize.setPoint(0, (Int16)uFontSize);
		stWCharPos.uBitState = String::WCBS_Visible | String::WCBS_Eof;
		strText.setWCharPos(uWLength, stWCharPos);

		// 整体宽度（对齐计算用）
		if (ptPixelSize.x < ptPos.x)
			ptPixelSize.x = ptPos.x;
		// 整体高度（对齐计算用）
		ptPixelSize.y = ptPos.y + uLineHeight;

		//
		// 排版操作
		//
		Point ptOffset(0, 0);

		// 上下排版计算
		if (ptFixSize.y > ptPixelSize.y)
		{
			switch (eAlignType)
			{
			case AT_Center:		// 上下居中
			case AT_Left:
			case AT_Right:
				ptOffset.y = (ptFixSize.y - ptPixelSize.y) >> 1;
				break;

			case AT_Bottom:		// 贴底
			case AT_LeftBottom:
			case AT_RightBottom:
			case AT_BottomLeft:
			case AT_BottomRight:
				ptOffset.y = (ptFixSize.y - ptPixelSize.y);
				break;
                    
            default:
                break;
			}
		}

		// 左右排版计算
		UInt  uLineStart = 0;
		Int16 nLineWidth = 0;
		for (UInt i = 0; i <= uWLength; ++i)
		{
			const String::WCharPos* pWCharPos = strText.getWCharPos(i);
			if (0 == (pWCharPos->uBitState & String::WCBS_Visible))
				continue;

			if (pWCharPos->ptPos.x != 0 && i != uWLength)
			{
				nLineWidth = pWCharPos->ptPos.x + pWCharPos->ptSize.x;
				continue;
			}

			switch (eAlignType)
			{
			case AT_Center:			// 左右居中
			case AT_Top:
			case AT_Bottom:
				ptOffset.x = (ptFixSize.x - nLineWidth) >> 1;
				break;

			case AT_Right:			// 贴右
			case AT_RightTop:
			case AT_RightBottom:
			case AT_TopRight:
			case AT_BottomRight:
				ptOffset.x = ptFixSize.x - nLineWidth;
				break;
			}

			String::WCharPos stWCharPos;
			for (UInt j = uLineStart; j < i || j == uWLength ; ++j)
			{
				stWCharPos = *strText.getWCharPos(j);
				stWCharPos.ptPos.offset(ptOffset);
				strText.setWCharPos(j, stWCharPos);
			}

			// 重设行开始位置
			uLineStart = i;
			// 防止最后一个字一行，会被漏算的情况
			nLineWidth = pWCharPos->ptPos.x + pWCharPos->ptSize.x;
		}


		// 整体矩形位置返回，根据整体宽度重新计算偏移（上下排版不用再次计算）
		if (pTextRect)
		{
			pTextRect->setRect(0, 0, ptPixelSize.x + 1, ptPixelSize.y + 1);	// +1 考虑阴影位置

			ptOffset.x = 0;
			if (ptFixSize.y > ptPixelSize.y)
			{
				switch (eAlignType)
				{
				case AT_Center:			// 左右居中
				case AT_Top:
				case AT_Bottom:
					ptOffset.x = (ptFixSize.x - ptPixelSize.x) >> 1;
					break;

				case AT_Right:			// 贴右
				case AT_RightTop:
				case AT_RightBottom:
					ptOffset.x = ptFixSize.x - ptPixelSize.x;
					break;
                        
                default:
                    break;
				}
			}

			pTextRect->offset(ptOffset);
		}

		System::getSingletonPtr()->releaseFont(pFontTexture);
	}


	///////////////////////////////////////////////////////////////////////////////////////////////


	//void Canvas::_dataToTexture()
	//{
	//	if (_rcDirty.isZero())
	//		return;

	//	// 刷新需要刷新的纹理
	//	for (Int ty = 0; ty < _ptTextureSize.y; ++ty)
	//	{
	//		for (Int tx = 0; tx < _ptTextureSize.x; ++tx)
	//		{
	//			Int nTexIndex = ty * _ptTextureSize.x + tx;
	//			Texture* pTexture = _ppTextures[nTexIndex];

	//			Point ptTexSize = pTexture->getSize();
	//			Point ptDataPos(tx * ptTexSize.x, ty * ptTexSize.y);	// 内存数据对应的，左上位置

	//			// 纹理渲染矩形是否与脏矩型有交集
	//			Rect rcTex(ptTexSize.x * tx, ptTexSize.y * ty, ptTexSize.x * (tx + 1), ptTexSize.y * (ty + 1));
	//			if (!_rcDirty.isIntersect(rcTex))
	//			{
	//				continue;
	//			}

	//			// 锁定纹理，刷新像素点
	//			UInt8* pTexData = 0;
	//			UInt nPitch = 0;
	//			PixelFmt ePixelFmt = _ppTextures[nTexIndex]->getPixelFormat();

	//			if (PF_ARGB == ePixelFmt)	// ARGB 格式
	//			{
	//				if (pTexture->lock((void*&) pTexData, nPitch))
	//				{
	//					// 计算要复制的行数
	//					UInt nRowSize = _ptDataSize.y - ptTexSize.y * ty;
	//					nRowSize = (nRowSize > (UInt)ptTexSize.y ? ptTexSize.y : nRowSize);

	//					for (UInt y = 0; y < nRowSize; ++y)
	//					{
	//						UInt nDataIndex = (ptDataPos.y + y) * _ptDataSize.x + ptDataPos.x;	// 内存数据行开始下标
	//						UInt nTexIndex  = y * ptTexSize.x;									// 纹理数据行开始下标

	//						nDataIndex <<= 2;	// 内存中一个像素占四个字节
	//						nTexIndex  = nPitch * y;

	//						// 计算一行要复制的像素数
	//						UInt nColSize = _ptDataSize.x - ptTexSize.x * tx;
	//						nColSize = (nColSize > (UInt)ptTexSize.x ? ptTexSize.x : nColSize);

	//						// 从内存中复制到纹理中，均为字节单位
	//						memcpy(pTexData + nTexIndex, ((UInt8*)_pData) + nDataIndex, nColSize << 2);
	//					}

	//					pTexture->unlock();
	//				}
	//			}
	//			else if (PF_RGB == ePixelFmt)	// RGB 格式，可能会有问题
	//			{
	//				if (pTexture->lock((void*&) pTexData, nPitch))
	//				{
	//					// 计算要复制的行数
	//					UInt nRowSize = _ptDataSize.y - ptTexSize.y * ty;
	//					nRowSize = (nRowSize > (UInt)ptTexSize.y ? ptTexSize.y : nRowSize);

	//					for (UInt y = 0; y < nRowSize; ++y)
	//					{
	//						UInt nDataIndex = (ptDataPos.y + y) * _ptDataSize.x + ptDataPos.x;	// 内存数据行开始下标
	//						UInt nTexIndex  = y * ptTexSize.x;									// 纹理数据行开始下标

	//						nDataIndex <<= 2;	// 内存中一个像素占四个字节
	//						nTexIndex  = nPitch * y;

	//						// 计算一行要复制的像素个数
	//						UInt nArraySize = _ptDataSize.x - ptTexSize.x * tx;
	//						nArraySize = (nArraySize > (UInt)ptTexSize.x ? ptTexSize.x : nArraySize);

	//						for (UInt x = 0; x < nArraySize; ++x)
	//						{
	//							nDataIndex++;	// Alpha 位扔掉
	//							pTexData[nTexIndex++] = ((UInt8*)_pData)[nDataIndex++];
	//							pTexData[nTexIndex++] = ((UInt8*)_pData)[nDataIndex++];
	//							pTexData[nTexIndex++] = ((UInt8*)_pData)[nDataIndex++];
	//						}
	//					}

	//					pTexture->unlock();
	//				}
	//			} // end of  else if(PF_RGB == ePixelFmt)
	//		}
	//	}
	//}

}
