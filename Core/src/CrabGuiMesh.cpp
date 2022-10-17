﻿
#include "CrabGuiDefine.h"
#include "CrabGuiMesh.h"


namespace CrabGui
{

	Mesh::Mesh()
		: _uVertexSize(0), _pVertices(0), _pTexVertices(0)
		, _uTriangleSize(0), _pTriangles(0)
	{
	}

	Mesh::Mesh(UInt uVertexSize, UInt uTriangleSize)
	{
		setVertexSize(uVertexSize);
		setTriangleSize(uTriangleSize);
	}

	Mesh::~Mesh()
	{
		clear();
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////
	// 顶点数据相关


	// 清除数据
	void Mesh::clear()
	{
		CrabDeleteArray(_pVertices);
		CrabDeleteArray(_pTexVertices);
		CrabDeleteArray(_pTexVertices);
	}


	// 设置点个数
	void Mesh::setVertexSize(UInt uVertexSize)
	{
		if (!_pVertices || !_pTexVertices || _uVertexSize != uVertexSize)
		{
			CrabDeleteArray(_pVertices);
			_pVertices = CrabNewArray(PointReal, uVertexSize);

			CrabDeleteArray(_pTexVertices); 
			_pTexVertices = CrabNewArray(PointReal, uVertexSize);

			_uVertexSize = uVertexSize;
		}
	}


	// 添加点
	Bool Mesh::setVertex(UInt uVertexIndex, const PointReal* pVertex, const PointReal* pTexVertex1, const PointReal* pTexVertex2)
	{
		if (uVertexIndex < _uVertexSize)
		{
			_pVertices[uVertexIndex]	= *pVertex;
			_pTexVertices[uVertexIndex] = *pTexVertex1;

			return True;
		}
		return False;
	}


	// 获取点个数
	UInt Mesh::getVertexSize() const
	{
		return _uVertexSize;
	}


	// 获取顶点
	const PointReal* Mesh::getVertices() const
	{
		return _pVertices;
	}


	// 获取所有纹理UV
	const PointReal* Mesh::getTexVertices() const
	{
		return _pTexVertices;
	}


	// 设置三角形个数
	void Mesh::setTriangleSize(UInt uTriangleSize)
	{
		if (!_pTriangles || _uTriangleSize != uTriangleSize)
		{
			CrabDeleteArray(_pTriangles);
			_pTriangles = CrabNewArray(Int16, uTriangleSize * 3);
			_uTriangleSize = uTriangleSize;
		}
	}


	// 设置单个三角形
	Bool Mesh::setTriangle(UInt uTriangleIndex, Int16 sIndex1, Int16 sIndex2, Int16 sIndex3)
	{
		if (uTriangleIndex < _uTriangleSize)
		{
			Int16* pTriangles = &_pTriangles[uTriangleIndex * 3];
			*_pTriangles++ = sIndex1;
			*_pTriangles++ = sIndex2;
			*_pTriangles++ = sIndex3;
			return True;
		}
		return False;
	}


	// 获取三角形个数
	UInt Mesh::getTriangleSize() const
	{
		return _uTriangleSize;
	}


	// 获取三角形
	const Int16* Mesh::getTriangles() const
	{
		return _pTriangles;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////

	
	// 设置平面
	void Mesh::setPlane(Point ptGridSize, Point ptPos, Point ptSize)
	{
		UInt uVertexSize = ptGridSize.getArea();
		UInt uRectSize = (ptGridSize.y - 1) * (ptGridSize.x - 1);
		setVertexSize(uVertexSize);
		setTriangleSize(uRectSize * 2);

		for (UInt i = 0; i < uVertexSize; ++i)
		{
			PointReal prVertex, prTexVertex;
			getPlaneVertex(ptGridSize, ptPos, ptSize, i, &prVertex, &prTexVertex);
			setVertex(i, &prVertex, &prTexVertex, 0);
		}

		for (UInt i = 0; i < uRectSize; ++i)
		{
			UInt uIndexLT = 0;
			UInt uIndexRT = 1;
			UInt uIndexLB = ptGridSize.x;
			UInt uIndexRB = ptGridSize.x + 1;
			setTriangle(i * 2 + 0, uIndexLT, uIndexRT, uIndexLB);
			setTriangle(i * 2 + 1, uIndexLB, uIndexRT, uIndexRB);
		}
	}

	
	// 获取平面顶点
	Bool Mesh::getPlaneVertex(Point ptGridSize, Point ptPos, Point ptSize, UInt uIndex, PointReal* pVertex, PointReal* pTexVertex)
	{
		if (uIndex >= ptGridSize.getArea())
			return False;

		UInt uGridX = uIndex % ptSize.y;
		UInt uGridY = uIndex / ptSize.y;
		Real rGridStepX = (Real)ptSize.x / ptGridSize.x;
		Real rGridStepY = (Real)ptSize.y / ptGridSize.y;

		if (pVertex)
		{
			pVertex->x = ptPos.x + uGridX * rGridStepX;
			pVertex->y = ptPos.y + uGridY * rGridStepY;
		}
		if (pTexVertex)
		{
			pTexVertex->x = rGridStepX * uGridX;
			pTexVertex->y = rGridStepY * uGridY;
		}
		return True;
	}

	
	// 设置圆
	void Mesh::setCircle(UInt uFanSize, Point ptPos, Point ptSize)
	{
	}

}
