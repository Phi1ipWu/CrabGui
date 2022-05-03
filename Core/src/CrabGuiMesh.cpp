
#include "CrabGuiDefine.h"
#include "CrabGuiMesh.h"


namespace CrabGui
{

	Mesh::Mesh()
		: _uVertexNum(0), _pVertices(0), _pTexVertices(0)
		, _uTriangleNum(0), _pTriangles(0)
	{
	}

	Mesh::Mesh(UInt uVertexNum, UInt uTriangleNum)
	{
		setVertexNum(uVertexNum);
		setTriangleNum(uTriangleNum);
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

	}


	// 获取点个数
	UInt Mesh::getVertexNum()
	{
		return _uVertexNum;
	}


	// 设置点个数
	void Mesh::setVertexNum(UInt uVertexNum)
	{

	}


	// 添加点
	Bool Mesh::setVertex(UInt uVertexIndex, const PointReal* pPoint, const PointReal* pTexPoint1, const PointReal* pTexPoint2)
	{
		return False;
	}


	// 设置三角形个数
	void Mesh::setTriangleNum(UInt uTriangleNum)
	{

	}


	// 设置单个三角形
	Bool Mesh::setTriangle(UInt uTriangleIndex, Int16 sPoint1, Int16 sPoint2, Int16 sPoint3)
	{
		return False;
	}

}
