
#include "CrabGuiDefine.h"
#include "CrabGuiFreeImageImage.h"

#include "FreeImage.h"


namespace CrabGui
{

    FreeImageImage::FreeImageImage()
		:	_ptImgSize()
		,	_pImgData(0)
    {
    }

    FreeImageImage::~FreeImageImage()
    {
        unloadImage();
    }


    // 从文件中载入图片
    Bool FreeImageImage::loadFromFile(PCStr pszFileName)
    {
		unloadImage();

        FREE_IMAGE_FORMAT eImgFormat = FIF_UNKNOWN;
		FIBITMAP* pFIBitmap = 0;

        eImgFormat = FreeImage_GetFileType(pszFileName);
        if(eImgFormat == FIF_UNKNOWN)
        {
            eImgFormat = FreeImage_GetFIFFromFilename(pszFileName);
        }
        if(eImgFormat != FIF_UNKNOWN && FreeImage_FIFSupportsReading(eImgFormat))
        {
            pFIBitmap = FreeImage_Load(eImgFormat, pszFileName, 0);
        }

		Bool isSuccess = False;

		if(pFIBitmap)
		{
			// 非 32 位色图片，转换为 32 位
			UInt nBPP = FreeImage_GetBPP(pFIBitmap);
			if(nBPP != 32)
			{
				FIBITMAP* pFIBitmap32 = FreeImage_ConvertTo32Bits(pFIBitmap);
				FreeImage_Unload(pFIBitmap);
				pFIBitmap = pFIBitmap32;
			}
		}

		if(pFIBitmap)
		{
			// 加载到内存数据中
			isSuccess = _load(pFIBitmap);
			FreeImage_Unload(pFIBitmap);

			// 记录下文件名
			_strFileName = pszFileName;
		}

        return isSuccess;
    }


    // 从内存中载入图片
    Bool FreeImageImage::loadFromFileInMemory(PCVoid pData, UInt nDataSize)
    {
		unloadImage();

        FREE_IMAGE_FORMAT eImgFormat = FIF_UNKNOWN;
		FIMEMORY* pFIMemory = FreeImage_OpenMemory((UInt8*) pData, nDataSize);  
		FIBITMAP* pFIBitmap = 0;

        eImgFormat = FreeImage_GetFileTypeFromMemory(pFIMemory);
        if(eImgFormat != FIF_UNKNOWN && FreeImage_FIFSupportsReading(eImgFormat))
        {
			pFIBitmap = FreeImage_LoadFromMemory(eImgFormat, pFIMemory, 0);
        }

		Bool isSuccess = False;

		if(pFIBitmap)
		{
			// 非 32 位色图片，转换为 32 位
			UInt nBPP = FreeImage_GetBPP(pFIBitmap);
			if(nBPP != 32)
			{
				FIBITMAP* pFIBitmap32 = FreeImage_ConvertTo32Bits(pFIBitmap);
				FreeImage_Unload(pFIBitmap);
				pFIBitmap = pFIBitmap32;
			}
		}

		if(pFIBitmap)
		{
			// 加载到内存数据中
			isSuccess = _load(pFIBitmap);
			FreeImage_Unload(pFIBitmap);
		}

		FreeImage_CloseMemory(pFIMemory);
        return isSuccess;
    }


    // 卸载图片
    void FreeImageImage::unloadImage()
    {
		CrabDeleteArray(_pImgData);
		_ptImgSize.setZero();
		_strFileName.setZero();
    }


    // 是否已载入
    Bool FreeImageImage::isLoaded()
    {
        return !!_pImgData;
    }


	// 获得文件名
	PCStr FreeImageImage::getFileName()
	{
		return _strFileName.getStrPtr();
	}


	// 获得图片大小
    Point FreeImageImage::getSize()
    {
        return _ptImgSize;
    }

	// 获取图片原始数据
	Bool FreeImageImage::GetRawData(PCVoid* ppData, UInt* pDataSize)
	{
		if (_pImgData)
		{
			*ppData		= _pImgData;
			*pDataSize	= _ptImgSize.getArea() * 4;
			return True;
		}
		return False;
	}

	// 设置坐标点颜色
	Bool FreeImageImage::setColor(const Point& ptPos, Color cColor)
	{
		if (!_pImgData)
			return False;

		// 图片上下反转
		UInt nIndex = (_ptImgSize.y - 1 - (ptPos.y % _ptImgSize.y)) * _ptImgSize.x + (ptPos.x % _ptImgSize.x);

		if (nIndex >= (UInt)_ptImgSize.getArea())
			return False;

		_pImgData[nIndex] = cColor;
		return True;
	}


    // 获得坐标点的颜色
    Color FreeImageImage::getColor(const Point& ptPos)
    {
		if (!_pImgData || _ptImgSize.isZero())
			return 0x0;

		// 图片上下反转
		UInt nIndex = (_ptImgSize.y - 1 - (ptPos.y % _ptImgSize.y)) * _ptImgSize.x + (ptPos.x % _ptImgSize.x);

		return _pImgData[nIndex];
    }


    // 获得插值颜色
    Color FreeImageImage::getBlurColor(float x, float y)
    {
		static Bool isHQ = False;
		if(isHQ)
		{
			//Int nx = (Int) x;
			//Int ny = (Int) y;

			//Point ptPos;
			//Color nColor[4] = 0;

			//ptPos.setPoint(nx,	   ny);			nColor[0] = getColor(ptPos);
			//ptPos.setPoint(nx + 1, ny);			nColor[1] = getColor(ptPos);
			//ptPos.setPoint(nx,     ny + 1);		nColor[2] = getColor(ptPos);
			//ptPos.setPoint(nx + 1, ny + 1);		nColor[3] = getColor(ptPos);

			//Color nColorRet = nColor[0] 
		}

		Point ptPos((Int16)x, (Int16)y);
		return getColor(ptPos);
    }


	/// 图片另存为
	Bool FreeImageImage::saveToFile(ImageFileFormat eFileFormat, PCStr pszFileName, Rect* pRectClip, Point* pResize)
	{
		if (!isLoaded())
			return False;

		FREE_IMAGE_FORMAT eFreeImageFormat;
		switch (eFileFormat)
		{
		case IFF_BMP:
			eFreeImageFormat = FIF_BMP;
			break;
		case IFF_PNG:
			eFreeImageFormat = FIF_PNG;
			break;
		case IFF_JPG:
			eFreeImageFormat = FIF_JPEG;
			break;

		default:
			return False;
		}

		Point ptLeftTop(0, 0);
		Point ptSize(_ptImgSize.x, _ptImgSize.y);

		// 有裁减矩形
		if (pRectClip)
		{
			ptLeftTop.setPoint(pRectClip->x1, pRectClip->y1);
			ptSize = pRectClip->getSize();

			if (ptLeftTop.x + ptSize.x > _ptImgSize.x ||
				ptLeftTop.y + ptSize.y > _ptImgSize.y)
				return False;
		}

		FIBITMAP* pFIBitmap = FreeImage_Allocate(ptSize.x, ptSize.y, 32, 8, 8, 8);

		for(Int y = 0; y < ptSize.y; ++y)
		{
			// 需要考虑到 图片倒转
			Int nSrcFlipY = _ptImgSize.y - y - 1 - ptLeftTop.y;
			Int nDstFlipY = ptSize.y - y - 1;

			Color* pColorSrc = &_pImgData[_ptImgSize.x * nSrcFlipY + ptLeftTop.x];
			Color* pColorDst = (Color*)FreeImage_GetScanLine(pFIBitmap, nDstFlipY);

			memcpy(pColorDst, pColorSrc, sizeof(Color) * ptSize.x);
		}

		if (pResize)
		{
			if (0 == pResize->x && 0 < pResize->y)
				pResize->x = (Int16)((Real)pResize->y / (Real)ptSize.y * (Real)ptSize.x);

			if (0 == pResize->y && 0 < pResize->x)
				pResize->y = (Int16)((Real)pResize->x / (Real)ptSize.x * (Real)ptSize.y);

			pFIBitmap = FreeImage_Rescale(pFIBitmap, pResize->x, pResize->y, FILTER_BILINEAR);
		}

		Bool isSuccess = FreeImage_Save(eFreeImageFormat, pFIBitmap, pszFileName, PNG_DEFAULT);
		FreeImage_Unload(pFIBitmap);

		return isSuccess;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////


	Bool FreeImageImage::_load(FIBITMAP* pFIBitmap)
	{
		BITMAPINFO* pBitmapInfo = FreeImage_GetInfo(pFIBitmap);

		_ptImgSize.x = (Int16) pBitmapInfo->bmiHeader.biWidth;
		_ptImgSize.y = (Int16) pBitmapInfo->bmiHeader.biHeight;

		Color* pImgData = (Color*) FreeImage_GetBits(pFIBitmap);
		//RGBQUAD* pPalette = FreeImage_GetPalette(pFIBitmap);

		_pImgData = CrabNewArray(Color, _ptImgSize.getArea());
		memcpy(_pImgData, pImgData, sizeof(Color) * _ptImgSize.getArea());

		return True;
	}

}
