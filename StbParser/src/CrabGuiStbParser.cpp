
#include "CrabGuiDefine.h"
#include "CrabGuiStbParser.h"

#include "CrabGuiStbImageImage.h"
#include "CrabGuiStbTrueTypeFont.h"

#include "CrabGuiSystem.h"
#include "CrabGuiWindow.h"


namespace CrabGui
{

    StbParser::StbParser()
    {
    }

    StbParser::~StbParser()
    {
    }


    // 从文件中创建
    Window* StbParser::createFromFile(Window* pWndParent, PCStr pszFileName)
    {
		_fp = fopen(pszFileName, "r");
		if (0 == _fp)
			return False;

		System* pSystem = pWndParent->getSystem();
		Window* pWindow = 0;
		Window* pWndLast = 0;
		UInt uLayer = 0, uLayerLast = 0;

		Char szLine[2048] = {0};
		String strLine;
		for (; fgets(szLine, 2048, _fp);)
		{
			strLine = szLine;
			Window* pNewWindow = _importWindow(uLayer, pSystem, strLine);
			if (0 == pNewWindow)
				break;

			if (0 == uLayer)	// 最底层窗体
			{
				pNewWindow->setParent(pWndParent);
				pWindow = pNewWindow;
			}
			else if (uLayer == uLayerLast + 1)	// 下一层子窗体
			{
				pNewWindow->setParent(pWndLast);
			}
			else if (uLayer <= uLayerLast)		// 同级/上级窗体
			{
				for (UInt i = 0; i < uLayerLast - uLayer + 1; ++i)
				{
					pWndLast = pWndLast->getParent();
				}

				pNewWindow->setParent(pWndLast);
			}
			else	// 无法层次归属，销毁刚创建的窗体
			{
				pSystem->destroyWindow(pNewWindow);
				pNewWindow = 0;

				break;
			}

			pWndLast	= pNewWindow;
			uLayerLast	= uLayer;
		}

		fclose(_fp);
		_fp = 0;

        return pWindow;
    }


    // 从内存中创建
    Window* StbParser::createFromMemory(Window* pWndParent, void* pData, UInt nDataSize)
    {
		Window* pWindow = 0;

		return pWindow;
    }


    // 保存到文件
    Bool StbParser::saveToFile(Window* pWindow, PCStr pszFileName)
    {
		_fp = fopen(pszFileName, "w+");
		if (0 == _fp)
			return False;

		_exportWindow(pWindow, 0);

		fclose(_fp);
		_fp = 0;

        return True;
    }


	//////////////////////////////////////////////////////////////////////////////////////////////////


    // 创建图片接口
    Image* StbParser::createImage()
    {
		Image* pImage = CrabNew(StbImageImage)();

        return pImage;
    }


    // 销毁图片接口
    void StbParser::destroyImage(Image* pImage)
    {
		CrabDelete(pImage);
    }


    // 创建字体接口
    Font* StbParser::createFont()
    {
		Font* pFont = CrabNew(StbTrueTypeFont)();
		
        return pFont;
    }


    // 销毁图片接口
    void StbParser::destroyFont(Font* pFont)
    {
		CrabDelete(pFont);
    }


	//////////////////////////////////////////////////////////////////////////////////////////////////


	static const Char PARSER_SPLIT	= ';';


	// 从字符串导入单个窗体（文件中一行即一个窗体的全部信息）
	Window* StbParser::_importWindow(UInt& uLayer, System* pSystem, const String& strLine)
	{
		Window* pWindow = 0;
		uLayer = 0;

		String strKey, strValue;

		UInt uPos1 = 0, uPos2 = 0, uPos3 = 0;
		for(UInt i = 0; True; ++i)
		{
			uPos1 = uPos3;

			uPos2 = strLine.find(PARSER_SPLIT, uPos1);
			if (uPos2 == String::NullPos)
				return pWindow;

			++uPos2;
			uPos3 = strLine.find(PARSER_SPLIT, uPos2);
			if (uPos3 == String::NullPos)
				return pWindow;

			++uPos3;

			strKey.copySubstr(strLine, uPos1, uPos2 - uPos1 - 1);
			strValue.copySubstr(strLine, uPos2, uPos3 - uPos2 - 1);

			if (i == 0)
			{
				uLayer  = strKey.toUInt();
				pWindow = pSystem->createWindow(strValue.getStrPtr());

				if (0 == pWindow)
					return 0;
			}
			else
			{
				pWindow->setPropert(strKey, strValue);
			}
		}

		return pWindow;
	}


	// 从窗体导出字符串
	void StbParser::_exportWindow(Window* pWindow, UInt uLayer)
	{
		// 导出当前窗体
		String strLine, strKey, strValue;
		UInt uNumProperty = pWindow->getNumPropert();

		strLine.format("%u%c%s%c", uLayer, PARSER_SPLIT, pWindow->getTypeName(), PARSER_SPLIT);

		for (UInt i = 0; i < uNumProperty; ++i)
		{
			pWindow->getPropert(i, strKey, strValue);

			strLine += strKey;
			strLine += PARSER_SPLIT;
			strLine += strValue;
			strLine += PARSER_SPLIT;
		}

		strLine += "\n";
		fwrite(strLine.getStrPtr(), strLine.getLength(), 1, _fp);

		// 递归导出子窗体
		for (UInt i = 0; i < pWindow->getNumChild(); ++i)
		{
			_exportWindow(pWindow->getChild(i), uLayer + 1);
		}
	}

}
