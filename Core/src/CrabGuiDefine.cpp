
#include "CrabGuiDefine.h"

#include <stdio.h>
#include <stdlib.h>		// mbstowcs
#include <string.h>		// strcpy
#include <stdarg.h>		// va_list
#include <wchar.h>      // wcslen


namespace CrabGui
{

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// 点


	Point::Point()
		:	x(0), y(0)
	{
	}

	Point::Point(Int16 x_, Int16 y_)
		:	x(x_), y(y_)
	{
	}
	
	Point::Point(Int x_, Int y_)
		:	x((Int16)x_), y((Int16)y_)
	{
	}

	// 设置点
	void Point::setPoint(Int16 x_, Int16 y_)
	{
		x = x_;
		y = y_;
	}

    // 获得面积
	Int Point::getArea() const
	{
		return x * y;
	}

    // 是否是零点
	Bool Point::isZero() const
	{
		return !(x || y);
	}

	// 点设零
	void Point::setZero()
	{
		x = y = 0;
	}

	// 点偏移
	void Point::offset(const Point& pt)
	{
		x += pt.x;
		y += pt.y;
	}

	
	// 相等操作
	Point& Point::operator=(const Point& pt)
	{
		x = pt.x;
		y = pt.y;

		return *this;
	}


	// 点相加操作
	Point& Point::operator+=(const Point& pt)
	{
		x += pt.x;
		y += pt.y;

		return *this;
	}


	// 点相等比较操作
	Bool Point::operator==(const Point& pt) const
	{
		return (x == pt.x && y == pt.y);
	}


	// 点不等比较操作
	Bool Point::operator!=(const Point& pt) const
	{
		return (x != pt.x || y != pt.y);
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////
	// 点（浮点数）

/*
	PointReal::PointReal()
		: x(0.0f), y(0.0f)
	{
	}

	PointReal::PointReal(Int16 x_, Int16 y_)
		: x(Real(x_)), y(Real(y_))
	{
	}

	void PointReal::SetPoint(Real x_, Real y_)
	{
		x = x_;
		y = y_;
	}
*/

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// 矩形


	Rect::Rect()
		:	x1(0), y1(0), x2(0), y2(0)
	{
	}

	Rect::Rect(Int16 x1_, Int16 y1_, Int16 x2_, Int16 y2_)
		:	x1(x1_), y1(y1_), x2(x2_), y2(y2_)
	{
	}

	Rect::Rect(const Rect& rc, const Point& ptOffset)
	{
		*this = rc;
		offset(ptOffset);
	}


	// 设置矩型
	void Rect::setRect(Int16 x1_, Int16 y1_, Int16 x2_, Int16 y2_)
	{
		x1 = x1_;
		y1 = y1_;
		x2 = x2_;
		y2 = y2_;
	}

	// 获得宽度
	Int Rect::getWidth() const
	{
		return x2 - x1;
	}

	// 获得高度
	Int Rect::getHeight() const
	{
		return y2 - y1;
	}

    // 获得大小
	Point Rect::getSize() const
	{
		return Point(x2 - x1, y2 - y1);
	}

	// 获得面积
	Int Rect::getArea() const
	{
		return getSize().getArea();
	}

	// 是否点在矩形内
	Bool Rect::isInRect(const Point& ptPos)
	{
		return (x1 <= ptPos.x && ptPos.x < x2 && y1 <= ptPos.y && ptPos.y < y2);
	}

	// 是否矩形相交（待修改）
	Bool Rect::isIntersect(const Rect& rc) const
	{
		return !(x2 <= rc.x1 || x1 >= rc.x2 || y2 <= rc.y1 || y1 >= rc.y2);
	}

	// 是否是零矩形
    Bool Rect::isZero() const
    {
		return !(x1 || y1 || x2 || y2);
	}

	// 矩型设零
	void Rect::setZero()
	{
		x1 = y1 = x2 = y2 = 0;
	}

	// 矩形偏移
	void Rect::offset(const Point& ptOffset)
	{
		x1 += ptOffset.x;
		y1 += ptOffset.y;
		x2 += ptOffset.x;
		y2 += ptOffset.y;
	}

	// 矩形偏移
	void Rect::offset(const Rect& rcOffset)
	{
		x1 += rcOffset.x1;
		y1 += rcOffset.y1;
		x2 += rcOffset.x2;
		y2 += rcOffset.y2;
	}

    // 矩形合并
	Rect& Rect::merge(const Rect& rc)
	{
		if (rc.x1 < x1)	x1 = rc.x1;
		if (rc.y1 < y1)	y1 = rc.y1;
		if (rc.x2 > x2)	x2 = rc.x2;
		if (rc.y2 > y2)	y2 = rc.y2;

		return *this;
	}


	// 相交合并
	Rect& Rect::intersect(const Rect& rc)
	{
		if (rc.x1 > x1)	x1 = rc.x1;
		if (rc.y1 > y1)	y1 = rc.y1;
		if (rc.x2 < x2)	x2 = rc.x2;
		if (rc.y2 < y2) y2 = rc.y2;

		return *this;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////
	// 字符串


	String::String()
		: _pChar(0), _uLength(0), _uSize(0)
		, _pWChar(0), _uWLength(0), _uWSize(0), _pWCharPos(0)
		, _isCharDirty(False), _isWCharDirty(False), _isWCharPosDirty(False)
		, _isReserve(0)
	{
	}

	String::String(PCStr pszText)
		: _pChar(0), _uLength(0), _uSize(0)
		, _pWChar(0), _uWLength(0), _uWSize(0), _pWCharPos(0)
		, _isCharDirty(False), _isWCharDirty(False), _isWCharPosDirty(False)
		, _isReserve(0)
	{
		operator=(pszText);
	}

	String::~String()
	{
		setZero();
	}


	// 获得字符串长度
	UInt String::getLength() const
	{
		getStrPtr();
		return _uLength;
	}


	// 获得宽字符串长度
	UInt String::getWLength() const
	{
		getWStrPtr();
		return _uWLength;
	}


	// 获得字符串指针
	PCStr String::getStrPtr() const
	{
		if (_isCharDirty)
		{
			// 需要转换
			if (_uSize < _uWLength * 2 + 1)
			{
				_resizeChar(_uWLength * 2 + 1);
			}

			_uLength = (UInt) wcstombs(_pChar, _pWChar, _uWLength * 2);
			_uLength = (UInt) strlen(_pChar);

			_isCharDirty = False;
		}

		return _pChar ? _pChar : "";
	}


	// 获得字符
	Char String::getCharAt(UInt uIndex) const
	{
		PCStr pszStr = getStrPtr();

		if (uIndex < getLength())
			return pszStr[uIndex];

		return 0;
	}


	// 获得宽字符串指针
	PCWStr String::getWStrPtr() const
	{
		if (_isWCharDirty)
		{
			// 需要转换
			if (_uWSize < _uLength + 1)
			{
				_resizeWChar(_uLength + 1);
			}

			_uWLength = (UInt) mbstowcs(_pWChar, _pChar, _uLength);
			_uWLength = (UInt) wcslen(_pWChar);

			_isWCharDirty = False;
		}

		return _pWChar ? _pWChar : L"";
	}


	// 获得宽字符
	WChar String::getWCharAt(UInt uIndex) const
	{
		PCWStr pszWStr = getWStrPtr();

		if (uIndex < getWLength())
			return pszWStr[uIndex];

		return 0;
	}


	// 是否宽字符脏了（即ANSI有更改）
	Bool String::isWCharDirty() const
	{
		return _isWCharDirty;
	}


	// 是否宽字符坐标位置脏了（需要新计算字的位置）
	Bool String::isWCharPosDirty() const
	{
		return _isWCharPosDirty;
	}

	// 设置宽字符坐标脏了（一般是排版方式变更）
	void String::setWCharPosDirty(Bool isDirty)
	{
		_isWCharPosDirty = isDirty;
	}

	// 字符串格式化
	void String::format(PCStr pszFmt, ...)
	{
		// 注：据牛人 jampe 反映，这种写法，在linux上会当机
		va_list argptr;
		va_start(argptr, pszFmt);
		{
			UInt uSize = vsnprintf(0, 0, pszFmt, argptr) + 1;
			_resizeChar(uSize);

			_uLength = vsnprintf(_pChar, _uSize, pszFmt, argptr);
		}
		va_end(argptr);

		_isWCharDirty    = True;
		_isWCharPosDirty = True;
	}


	// 字符串清空
	void String::setZero()
	{
		CrabDeleteArray(_pChar);
		_uLength = 0;
		_uSize   = 0;

		CrabDeleteArray(_pWChar);
		_uWLength = 0;
		_uWSize   = 0;

		_isCharDirty     = False;
		_isWCharDirty    = True;	// WChar 空字符串也需要保存位置信息
		_isWCharPosDirty = True;

		CrabDeleteArray(_pWCharPos);
	}


	// 复制子串
	void String::copySubstr(const String& strFull, UInt uStart, UInt uLength)
	{
		if (uStart >= strFull.getLength())
			return;

		if (uStart + uLength >= strFull.getLength())
			uLength = strFull.getLength() - uStart;

		_resizeChar(uLength + 1);
		strncpy(_pChar, strFull.getStrPtr() + uStart, uLength);
		_pChar[uLength] = 0;

		_uLength = uLength;
		_isWCharDirty = True;
		_isWCharPosDirty = True;
	}


	// 插入宽字符串（适用于 Editbox）
	void String::insertWChar(UInt uPos, PCWStr pszWChar)
	{
		UInt uWLength = getWLength();
		UInt uWLengthInsert = (UInt) wcslen(pszWChar);

		if (_uWSize < uWLength + uWLengthInsert + 1)
		{
			// 需要扩充内存
			_resizeWChar(uWLength + uWLengthInsert + 1);
		}

		if (uPos > uWLength)
			uPos = uWLength;

		// 老的向后移
		for (Int i = uWLength; i >= (Int)uPos; --i)
		{
			_pWChar[i + uWLengthInsert] = _pWChar[i];
		}

		// 插入新的字符串
		for (UInt i = 0; i < uWLengthInsert; ++i)
		{
			_pWChar[i + uPos] = pszWChar[i];
		}

		_uWLength += uWLengthInsert;

		_isCharDirty     = True;
		_isWCharPosDirty = True;
	}


	// 移除宽字符串（适用于 Editbox）
	void String::removeWChar(UInt uPos, UInt uSize)
	{
		UInt uWLength = getWLength();
		if (uPos > uWLength)
			return;

		if (uPos + uSize > uWLength)
			uSize = uWLength - uPos;

		for (UInt i = uPos + uSize; i < uWLength + 1; ++i)
		{
			_pWChar[i - uSize] = _pWChar[i];
		}

		_uWLength -= uSize;
		_isCharDirty = True;
		_isWCharPosDirty = True;
	}


	// 转换为整数
	Int String::toInt() const
	{
		return atoi(getStrPtr());
	}


	// 转换为整数
	UInt String::toUInt() const
	{
		UInt uNum = 0;
		sscanf(getStrPtr(), "%u", &uNum);

		return uNum;
	}


	// 哈希码
	UInt String::hashCode() const
	{
		//	http://blog.csdn.net/eaglex/article/details/6310727
		//	http://blog.csai.cn/user3/50125/archives/2009/35638.html	// 感谢牛人Jampe提供

		PCStr pszStr = getStrPtr();

		UInt uSeed = 131; // 31 131 1313 13131 131313 etc..
		UInt uHash = 0;

		while (*pszStr)
		{
			uHash = uHash * uSeed + (*pszStr++);
		}

		return uHash;
		//return (uHash & 0x7FFFFFFF);
	}


	// 查找
	UInt String::find(PCStr pszFind, UInt uStart) const
	{
		if (uStart == NullPos || uStart >= getLength())
			return NullPos;

		const char* pszResult = strstr(getStrPtr() + uStart, pszFind);
		if(0 == pszResult)
			return NullPos;

		return (UInt)(pszResult - getStrPtr());
	}


	// 查找字符
	UInt String::find(Char cFind, UInt uStart) const
	{
		Char szTmp[2] = { cFind, 0 };
		return find(szTmp, uStart);
	}


	///////////////////////////////////////////////////////////////////////////////////////////////


	// 设置单个字的坐标
	Bool String::setWCharPos(UInt uIndex, const WCharPos& stWCharPos)
	{
		getWStrPtr();
		if (uIndex < _uWLength + 1)
		{
			_pWCharPos[uIndex] = stWCharPos;
			return True;
		}

		return False;
	}


	// 获得单个字的坐标
	const String::WCharPos* String::getWCharPos(UInt uIndex) const
	{
		getWStrPtr();
		if (uIndex < _uWSize)
		{
			return &(_pWCharPos[uIndex]);
		}

		return 0;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////


	// 字符串赋值
	String& String::operator=(const String& strText)
	{
		return operator=(strText.getStrPtr());
	}


	// 字符串赋值操作
	String& String::operator=(PCStr pszText)
	{
		UInt uLength = (UInt) strlen(pszText);

		if (uLength)
		{
			_resizeChar(uLength + 1);
			strcpy(_pChar, pszText);

			_uLength = uLength;
			_isWCharDirty = True;
			_isWCharPosDirty = True;
		}
		else
		{
			setZero();
		}

		return *this;
	}


	// 字符串赋值操作
	String& String::operator=(PCWStr pszText)
	{
		UInt uWLength = (UInt) wcslen(pszText);

		if (uWLength)
		{
			_resizeWChar(uWLength + 1);
			wcscpy(_pWChar, pszText);

			_uWLength = uWLength;
			_isCharDirty = True;
			_isWCharPosDirty = True;
		}
		else
		{
			setZero();
		}

		return *this;
	}


	//// 字符串添加操作
	//String& String::operator+(PCStr pszText)
	//{
	//	UInt uLength = (UInt) strlen(pszText);

	//	if (uLength)
	//	{
	//		_resizeChar(_uLength + uLength + 1);
	//		strcpy(_pChar + _uLength, pszText);

	//		_uLength += uLength;
	//		_isWCharDirty = True;
	//		_isWCharPosDirty = True;
	//	}

	//	return *this;
	//}


	// 字符串添加操作
	String& String::operator+=(const String& strText)
	{
		UInt uLength = strText.getLength();

		if (uLength)
		{
			getStrPtr();

			_resizeChar(_uLength + uLength + 1);
			strcpy(_pChar + _uLength, strText.getStrPtr());

			_uLength += uLength;
			_isWCharDirty = True;
			_isWCharPosDirty = True;
		}

		return *this;
	}


	// 字符串添加操作
	String& String::operator+=(Char cChar)
	{
		UInt uLength = 1;

		if (uLength)
		{
			getStrPtr();

			_resizeChar(_uLength + uLength + 1);
			_pChar[_uLength] = cChar;
			_pChar[_uLength + 1] = 0;

			_uLength += uLength;
			_isWCharDirty = True;
			_isWCharPosDirty = True;
		}

		return *this;
	}


	// 字符串与字符串比较操作
	Bool String::operator==(String& strText) const
	{
		return operator==(strText.getStrPtr());
	}


	// 字符串与字符指针比较操作
	Bool String::operator==(PCStr pszText) const
	{
		return (0 == strcmp(getStrPtr(), pszText));
	}


	// 字符串比较
	Bool String::operator!=(PCStr pszText) const
	{
		return (0 != strcmp(getStrPtr(), pszText));
	}


	// 字符串小于比较
	Bool String::operator<(const String& strText) const
	{
		return strcmp(getStrPtr(), strText.getStrPtr()) < 0;
	}


	// 字符串大于比较
	Bool String::operator>(const String& strText) const
	{
		return strcmp(getStrPtr(), strText.getStrPtr()) > 0;
	}


	// 字符串取值
	Char& String::operator[](UInt uIndex)
	{
		return _pChar[uIndex];
	}


	// 默认增速
	const UInt String::DefaultIncrease = 32;


	// 字符串大小重设
	void String::_resizeChar(UInt uSize) const
	{
		if (uSize > _uSize)
		{
			UInt uNewSize = ((uSize / DefaultIncrease) + 1) * DefaultIncrease;
			PStr pNewChar = CrabNewArray(Char, uNewSize);

			if (_pChar)
			{
				strncpy(pNewChar, _pChar, _uLength);
				CrabDeleteArray(_pChar);
			}
			else
			{
				memset(pNewChar, 0, sizeof(Char) * uNewSize);
			}

			_uSize = uNewSize;
			_pChar = pNewChar;
		}
	}


	// 宽字符串大小重设
	void String::_resizeWChar(UInt uWSize) const
	{
		if (uWSize > _uWSize)
		{
			// 宽字符存储空间
			uWSize = ((uWSize / DefaultIncrease) + 1) * DefaultIncrease;
			WChar* pWChar = CrabNewArray(WChar, uWSize);

			if (_pWChar)
			{
				wcscpy(pWChar, _pWChar);
				CrabDeleteArray(_pWChar);
			}
			else
			{
				memset(pWChar, 0, sizeof(WChar) * uWSize);
			}

			_pWChar = pWChar;
			_uWSize = uWSize;

			// 宽字符字体大小空间
			CrabDeleteArray(_pWCharPos);
			_pWCharPos = CrabNewArray(WCharPos, _uWSize);
		}
	}

}
