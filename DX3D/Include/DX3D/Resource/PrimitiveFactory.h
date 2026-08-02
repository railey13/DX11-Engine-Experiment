#pragma once
#include <DX3D/Prerequisites.h>
#include <DX3D/Math/VertexMesh.h>

class PrimitiveFactory {
private:
	static void setTangentBinormal(VertexMesh* faceVerts, unsigned char count);
	static void setTangentBinormal(VertexMesh& v, f32 theta);
	static void reverseWinding(ui32* indices, size_t count);
public:
	static void createCube(ResourceManager* manager, GameObject* obj);
	static void createPlane(ResourceManager* manager, GameObject* obj);
	static void createSphere(ResourceManager* manager, GameObject* obj);
	static void createCapsule(ResourceManager* manager, GameObject* obj);
};

