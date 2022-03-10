
#include "CrabGuiDefine.h"
#include "CrabGuiStbImageImage.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


namespace CrabGui
{

    StbImageImage::StbImageImage()
		:	_ptImgSize()
		,	_pImgData(0)
    {
    }

    StbImageImage::~StbImageImage()
    {
        unloadImage();
    }


    // 从文件中载入图片
    Bool StbImageImage::loadFromFile(PCStr pszFileName)
    {
		unloadImage();
		Bool isSuccess = False;
		Int w, h, n;

		_pImgData = (stbi_uc *)stbi_load(pszFileName, &w, &h, &n, 4);

        return !!_pImgData;
    }


    // 从内存中载入图片
    Bool StbImageImage::loadFromMemory(PCVoid pData, UInt nDataSize)
    {
		unloadImage();
		Bool isSuccess = False;
		Int w, h, n;

		_pImgData = (stbi_uc *)stbi_load_from_memory((const stbi_uc *)pData, nDataSize, &w, &h, &n, 4);
		_ptImgSize.setPoint(w, h);

        return !!_pImgData;
    }


    // 卸载图片
    void StbImageImage::unloadImage()
    {
		stbi_image_free(_pImgData);
		_pImgData = 0;

		_ptImgSize.setZero();
		_strFileName.setZero();
    }


    // 是否已载入
    Bool StbImageImage::isLoaded()
    {
        return !!_pImgData;
    }


	// 获得文件名
	PCStr StbImageImage::getFileName()
	{
		return _strFileName.getStrPtr();
	}


	// 获得图片大小
    Point StbImageImage::getSize()
    {
        return _ptImgSize;
    }


	// 设置坐标点颜色
	Bool StbImageImage::setColor(const Point& ptPos, Color cColor)
	{
		return False;
	}


    // 获得坐标点的颜色
    Color StbImageImage::getColor(const Point& ptPos)
    {
		return 0x0;
    }


    // 获得插值颜色
    Color StbImageImage::getBlurColor(float x, float y)
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
	Bool StbImageImage::saveToFile(ImageFileFormat eFileFormat, PCStr pszFileName, Rect* pRectClip, Point* pResize)
	{
		if (!isLoaded())
			return False;

		Bool isSuccess = False;
		return isSuccess;
	}
}
