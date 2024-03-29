﻿
#ifndef __CrabGuiFreeImageImage_h__
#define __CrabGuiFreeImageImage_h__


#include "CrabGuiImage.h"


namespace CrabGui
{

    class CRABGUIEXPORT StbImageImage : public Image
    {
	public:
		StbImageImage();
		virtual ~StbImageImage();

		/// 从文件中载入图片
		virtual Bool	loadFromFile(PCStr pszFileName);

		/// 从内存中载入图片
		virtual Bool	loadFromFileInMemory(PCVoid pData, UInt nDataSize);

        /// 卸载图片
        virtual void    unloadImage();

		/// 是否已载入
		virtual Bool	isLoaded();

		/// 获得文件名
		virtual PCStr	getFileName();

		/// 获得图片大小
		virtual Point	getSize();

		/// 获取图片原始数据
		virtual Bool 	GetRawData(PCVoid* ppData, UInt* pDataSize);

		/// 设置坐标点颜色
		virtual Bool	setColor(const Point& ptPos, Color cColor);

		/// 获得坐标点的颜色
		virtual Color	getColor(const Point& ptPos);

		/// 获得插值颜色
		virtual Color	getBlurColor(float x, float y);

		/// 图片另存为
		virtual Bool	saveToFile(ImageFileFormat eFileFormat, PCStr pszFileName, Rect* pRectClip, Point* pResize);


	private:
		void	_convertBGRAtoRGBA();


    private:
		String	_strFileName;
        Point   _ptImgSize;
		UInt8*	_pImgData;
		Int		_nImgComp;
    };

}


#endif  //  __CrabGuiFreeImageImage_h__
