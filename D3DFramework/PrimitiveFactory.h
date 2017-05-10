//#pragma once
//
//namespace DirectX
//{
//	struct XMFLOAT4;
//	struct XMFLOAT3;
//	struct XMFLOAT2;
//}
//
//class PrimitiveFactory
//{
//public:
//	// Take center, size, and orientation of primitive to be created
//	void CreateTriangle( const ModelSpecs_L &Specs );
//	void CreatePlane( const ModelSpecs_L &Specs );
//	void CreatePlaneNM( const ModelSpecs_L &Specs );
//	void CreateCube( const ModelSpecs_L &Specs );
//	void CreateSphereNM( const ModelSpecs_L & Specs, const float radiusGlobe = 50.0f );
//	void CreateCubeNM( const ModelSpecs_L &Specs );
//	void CreateMeshNM( const std::wstring&Filename );
//	void CreateColor( float R, float G, float B, float A );
//	void CreateMesh( const std::wstring& fileName );
//
//	// ACCESSOR FUNCTIONS
//	std::vector<DirectX::XMFLOAT3> GetVertices();
//	std::vector<DirectX::XMFLOAT3> GetNormals();
//	std::vector<DirectX::XMFLOAT2> GetUVs();
//	std::vector<DirectX::XMFLOAT3> GetTangent();
//	std::vector<DirectX::XMFLOAT3> GetBiTangent();
//	std::vector<DWORD> GetIndices();
//	DirectX::XMFLOAT4 GetColor();
//
//private:
//	DirectX::XMFLOAT3 CalculateTangent(
//		const DirectX::XMFLOAT3 &Edge10,
//		const DirectX::XMFLOAT2& tEdge10 );
//	DirectX::XMFLOAT3 CalculateNormal(
//		const DirectX::XMFLOAT3 &Edge10,
//		const DirectX::XMFLOAT3 &Edge20 );
//	DirectX::XMFLOAT3 CalculateBiNormal(
//		const DirectX::XMFLOAT3 &Tangent,
//		const DirectX::XMFLOAT3 &Normal );
//	void Common( const ModelSpecs_L &Specs );
//	void ClearAllBuffers();
//private:
//	std::vector<DirectX::XMFLOAT3> vertices;
//	std::vector<DirectX::XMFLOAT3> normals;
//	std::vector<DirectX::XMFLOAT2> uvs;
//	// Tangent runs parallel to surface in the U coord of texture 
//	// coordinates UV
//	static std::vector<DirectX::XMFLOAT3> tangents;
//	// Suppose to be called binormal, runs parallel to surface in the V
//	// coord of the texture coordinates UV
//	static std::vector<DirectX::XMFLOAT3> binormals;
//
//	static std::vector<DWORD> indices;
//	static DirectX::XMFLOAT4 color;
//};
//
