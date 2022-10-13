
#ifndef __CrabGuiMesh_h__
#define __CrabGuiMesh_h__


namespace CrabGui
{

	/// 网格
	class CRABGUIEXPORT Mesh
	{
	public:
		Mesh();
		Mesh(UInt uVertexSize, UInt uTriangleSize);
		virtual ~Mesh();

		/// 清除数据
		void    clear();

		/// 设置顶点个数
		void 	setVertexSize(UInt uVertexSize);

		/// 设置顶点
		Bool 	setVertex(UInt uVertexIndex, const PointReal* pVertex, const PointReal* pTexVertex1, const PointReal* pTexVertex2);

		/// 获取顶点个数
		UInt 	getVertexSize();

		/// 获取顶点
		const PointReal* getVertices();

		/// 获取所有纹理UV
		const PointReal* getTexVertices();

		/// 设置三角形个数
		void 	setTriangleSize(UInt uTriangleSize);
		
		/// 设置单个三角形
		Bool 	setTriangle(UInt uTriangleIndex, Int16 sIndex1, Int16 sIndex2, Int16 sIndex3);

		/// 获取三角形个数
		UInt	getTriangleSize();

		/// 获取三角形
		const Int16* getTriangles();


	protected:
		UInt  			_uVertexSize;
		PointReal*		_pVertices;
		PointReal*		_pTexVertices;

		UInt 			_uTriangleSize;
		Int16*			_pTriangles;
	};

}

#endif	//	__CrabGuiMesh_h__
