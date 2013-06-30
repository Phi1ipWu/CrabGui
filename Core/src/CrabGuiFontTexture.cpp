
#include "CrabGuiDefine.h"
#include "CrabGuiFontTexture.h"

#include "CrabGuiSystem.h"
#include "CrabGuiRenderer.h"
#include "CrabGuiTexture.h"
#include "CrabGuiFont.h"
#include "CrabGuiRBTree.h"
#include "CrabGuiQueue.h"

#include <string.h>		// memset
#include <stdio.h>		// fopen


namespace CrabGui
{

	FontTexture::FontTexture(Font* pFont)
		:	_pFont(pFont)
		,	_pBufferCache(0)
		,	_uBufferSize(0)
	{
		_pRBTreeFont	= CrabNew(RBTree);

		memset(_stFontCaches, 0, sizeof(_stFontCaches));
	}

	FontTexture::~FontTexture()
	{
		CrabDeleteArray(_pBufferCache);

		for (UInt i = 0; i < Max_Font_Size; ++i)
		{
			if (0 == _stFontCaches[i].pQueueTexture)
				continue;

			Renderer* pRenderer = System::getSingletonPtr()->getRenderer();
			FontCache* pFontCache = &_stFontCaches[i];
			for (UInt j = 0; j < pFontCache->pQueueTexture->getNum(); ++j)
			{
				Texture* pTexture = (Texture*)pFontCache->pQueueTexture->getAt(j);
				pRenderer->destroyTexture(pTexture);
			}

			CrabDelete(pFontCache->pQueueTexture);
		}

		CrabDelete(_pRBTreeFont);
	}


	// 获得渲染用的字体
	FontTexture::FontChar* FontTexture::getFontChar(UInt uChar, UInt uCharSize, UInt uStyle)
	{
		UInt uKey = _getKey(uChar, uCharSize, uStyle);
		FontChar* pFontChar = (FontChar*)_pRBTreeFont->find(uKey);

		if (pFontChar)
		{
			cacheToTexture(uCharSize);
		}
		else
		{
			//pFontChar = _createFontChar(uChar, uCharSize, uStyle);
			//_pRBTreeFont->insert(uKey, pFontChar);
		}

		return pFontChar;
	}


	// 获得字的逻辑大小
	Bool FontTexture::getFontSize(Point& ptSize, UInt uChar, UInt uCharSize, UInt uStyle)
	{
		UInt uKey = _getKey(uChar, uCharSize, uStyle);
		FontChar* pFontChar = (FontChar*)_pRBTreeFont->find(uKey);

		if (0 == pFontChar)
		{
			pFontChar = _createFontChar(uChar, uCharSize, uStyle);
			_pRBTreeFont->insert(uKey, pFontChar);
		}

		if (pFontChar)
		{
			ptSize = pFontChar->ptCharSize;
			return True;
		}

		ptSize.setZero();
		return False;
	}


	// 缓存到纹理
	void FontTexture::cacheToTexture(UInt uCharSize)
	{
		FontCache* pFontCache = &_stFontCaches[uCharSize - 1];
		if (!pFontCache->isDirty)
			return;

		pFontCache->isDirty = False;

		UInt uIndex = pFontCache->pQueueTexture->getNum() - 1;
		Texture* pTexture = (Texture*)pFontCache->pQueueTexture->getAt(uIndex);

		Color* pData = 0;
		UInt uPitch  = 0;
		if (pTexture->lock((void*&)pData, uPitch))
		{
			for (UInt i = 0; i < Font_TexSize * Font_TexSize; ++i)
			{
				UInt8 uAlpha = pFontCache->szTextureData[i];
				pData[i] = (uAlpha << 24) | 0x00FFFFFF;
			}

			pTexture->unlock();
		}
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////


	// 根据字体大小获得 KEY
	UInt FontTexture::_getKey(UInt uChar, UInt uCharSize, UInt uStyle)
	{
		if (uCharSize < Min_Font_Size)
			uCharSize = Min_Font_Size;
		if (uCharSize > Max_Font_Size)
			uCharSize = Max_Font_Size;

		UInt uKey = (uStyle << 24) | (uCharSize << 16) | (uChar & 0xFFFF);
		return uKey;
	}


	// 获得渲染用的字体
	FontTexture::FontChar* FontTexture::_createFontChar(UInt uChar, UInt uCharSize, UInt uStyle)
	{
		if (uCharSize < Min_Font_Size)
			uCharSize = Min_Font_Size;
		if (uCharSize > Max_Font_Size)
			uCharSize = Max_Font_Size;

		Point ptDataSize, ptOffset, ptCharSize;
		UInt uBufferSize = _pFont->getCharInfo(_pBufferCache, _uBufferSize, &ptDataSize, &ptOffset, &ptCharSize, uChar, uCharSize, uStyle);

		if (0 == uBufferSize)
			return 0;

		if (uBufferSize > _uBufferSize)
		{
			// 单个字体的缓存扩大
			CrabDeleteArray(_pBufferCache);

			_uBufferSize = uBufferSize;
			_pBufferCache = CrabNewArray(UInt8, _uBufferSize);

			_pFont->getCharInfo(_pBufferCache, _uBufferSize, &ptDataSize, &ptOffset, &ptCharSize, uChar, uCharSize, uStyle);
		}

		// 保存纹理缓存
		Texture* pTexture = 0;

		FontCache* pFontCache = &_stFontCaches[uCharSize - 1];
		if (0 == pFontCache->pQueueTexture)
		{
			pFontCache->pQueueTexture = CrabNew(Queue)();

			Renderer* pRenderer = System::getSingletonPtr()->getRenderer();
			pTexture = pRenderer->createTexture();

			Point ptTexSize(Font_TexSize, Font_TexSize);
			pTexture->setSize(ptTexSize);
			pFontCache->pQueueTexture->insertEnd(pTexture);

			pFontCache->ptCurrPos.setZero();
			pFontCache->ptNextRow.setZero();

			memset(pFontCache->szTextureData, 0, sizeof(pFontCache->szTextureData));
		}
		else if (pFontCache->ptCurrPos.x + ptDataSize.x + Font_EdgeSpace * 2 > Font_TexSize)
		{
			if (pFontCache->ptCurrPos.y + ptDataSize.y + Font_EdgeSpace * 2 > Font_TexSize)
			{
				// 当前纹理写满，另起一个新纹理
				cacheToTexture(uCharSize);

				Renderer* pRenderer = System::getSingletonPtr()->getRenderer();
				pTexture = pRenderer->createTexture();

				Point ptTexSize(Font_TexSize, Font_TexSize);
				pTexture->setSize(ptTexSize);
				pFontCache->pQueueTexture->insertEnd(pTexture);

				pFontCache->ptCurrPos.setZero();
				pFontCache->ptNextRow.setZero();

				memset(pFontCache->szTextureData, 0, sizeof(pFontCache->szTextureData));
			}
			else
			{
				// 当前行满，写入下一行
				pFontCache->ptCurrPos = pFontCache->ptNextRow;
				pTexture = (Texture*)pFontCache->pQueueTexture->getAt(pFontCache->pQueueTexture->getNum() - 1);
			}
		}
		else
		{
			pTexture = (Texture*)pFontCache->pQueueTexture->getAt(pFontCache->pQueueTexture->getNum() - 1);
		}

		// 是否一行已经写满
		Int16 nNextRow = pFontCache->ptCurrPos.y + ptDataSize.y + Font_EdgeSpace * 2;
		if (pFontCache->ptNextRow.y < nNextRow)
			pFontCache->ptNextRow.y = nNextRow;

		pFontCache->isDirty = True;


		// 写入缓存
		FontChar* pFontChar		= CrabNew(FontChar);
		pFontChar->pTexture		= pTexture;
		pFontChar->rcTexture.x1 = pFontCache->ptCurrPos.x + Font_EdgeSpace;
		pFontChar->rcTexture.y1 = pFontCache->ptCurrPos.y + Font_EdgeSpace;
		pFontChar->rcTexture.x2 = pFontChar->rcTexture.x1 + ptDataSize.x;
		pFontChar->rcTexture.y2 = pFontChar->rcTexture.y1 + ptDataSize.y;
		pFontChar->ptOffset		= ptOffset;
		pFontChar->ptCharSize	= ptCharSize;

		for (Int y = 0; y < ptDataSize.y; ++y)
		{
			for (Int x = 0; x < ptDataSize.x; ++x)
			{
				Int nSrcIndex = y * ptDataSize.x + x;
				Int nDstIndex = (pFontChar->rcTexture.y1 + y) * Font_TexSize + pFontChar->rcTexture.x1 + x;

				pFontCache->szTextureData[nDstIndex] = _pBufferCache[nSrcIndex];
			}
		}

		// 移到下一位置
		pFontCache->ptCurrPos.x += ptDataSize.x + Font_EdgeSpace * 2;

		return pFontChar;
	}

}
