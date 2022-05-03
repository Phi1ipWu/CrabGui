
#ifndef __CrabGuiMesh_h__
#define __CrabGuiMesh_h__


namespace CrabGui
{

	/// 网格
	class CRABGUIEXPORT Mesh
	{
	public:
		Mesh();
		Mesh(UInt uVertexNum, UInt uTriangleNum);
		virtual ~Mesh();

		/// 清除数据
		void    clear();

		/// 获取点个数
		UInt 	getVertexNum();

		/// 获取
		const PointReal* getVertices();

		/// 设置点个数
		void 	setVertexNum(UInt uVertexNum);

		/// 添加点
		Bool 	setVertex(UInt uVertexIndex, const PointReal* pPoint, const PointReal* pTexPoint1, const PointReal* pTexPoint2);

		/// 设置三角形个数
		void 	setTriangleNum(UInt uTriangleNum);

		/// 设置单个三角形
		Bool 	setTriangle(UInt uTriangleIndex, Int16 sPoint1, Int16 sPoint2, Int16 sPoint3);


	public:

		



	protected:
		UInt  			_uVertexNum;
		PointReal*		_pVertices;
		PointReal*		_pTexVertices;

		UInt 			_uTriangleNum;
		Int16*			_pTriangles;
	};

}

#endif	//	__CrabGuiMesh_h__
