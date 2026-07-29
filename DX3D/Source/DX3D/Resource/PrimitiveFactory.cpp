#include <DX3D/Resource/PrimitiveFactory.h>
#include <DX3D/Resource/Mesh.h>
#include <DX3D/Math/Vector3D.h>
#include <DX3D/Math/Vector2D.h>

#include <DX3D/GameObject/GameObject.h>
#include <DX3D/GameObject/TransformComponent.h>
#include <DX3D/GameObject/MeshComponent.h>

#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>

void PrimitiveFactory::setTangentBinormal(VertexMesh* faceVerts, unsigned char count) {
	if (count < 3) return;

	Vector3D tangent, binormal;
	Mesh::computeTangents(
		faceVerts[0].m_position, faceVerts[1].m_position, faceVerts[2].m_position,
		faceVerts[0].m_texcoord, faceVerts[1].m_texcoord, faceVerts[2].m_texcoord,
		tangent, binormal
	);

	for (unsigned char i = 0; i < count; i++) {
		faceVerts[i].m_tangent = tangent;
		faceVerts[i].m_binormal = binormal;
	}
}

void PrimitiveFactory::setTangentBinormal(VertexMesh& v, f32 theta) {
	Vector3D tangent = Vector3D(-sinf(theta), 0.0f, cosf(theta));
	f32 len = tangent.magnitude();
	tangent = len ? (tangent * (1.0f / len)) : Vector3D(1, 0, 0);

	v.m_tangent = tangent;
	v.m_binormal = v.m_normal.vectorProduct(tangent);
}

void PrimitiveFactory::reverseWinding(ui32* indices, size_t count) {
	for (size_t i = 0; i + 2 < count; i += 3) {
		std::swap(indices[i + 1], indices[i + 2]);
	}
}

void PrimitiveFactory::createCube(ResourceManager* manager, GameObject* obj) {
	Vector3D position_list[] = {
		Vector3D(-0.2f, -0.2f, -0.2f), // POS1
		Vector3D(-0.2f, 0.2f, -0.2f),  // POS2
		Vector3D(0.2f, 0.2f, -0.2f),   // POS3 
		Vector3D(0.2f, -0.2f, -0.2f),  // POS4

		Vector3D(0.2f, -0.2f, 0.2f),
		Vector3D(0.2f, 0.2f, 0.2f),
		Vector3D(-0.2f, 0.2f, 0.2f),
		Vector3D(-0.2f, -0.2f, 0.2f),
	};

	Vector2D texcoord_list[] = {
		Vector2D(0.0f, 0.0f),
		Vector2D(0.0f, 1.0f),
		Vector2D(1.0f, 0.0f),
		Vector2D(1.0f, 1.0f)
	};

	Vector3D normal_front = Vector3D(0, 0, -1);
	Vector3D normal_back = Vector3D(0, 0, 1);
	Vector3D normal_top = Vector3D(0, 1, 0);
	Vector3D normal_bottom = Vector3D(0, -1, 0);
	Vector3D normal_right = Vector3D(1, 0, 0);
	Vector3D normal_left = Vector3D(-1, 0, 0);

	VertexMesh vertices[] = {
		// FRONT SIDE
		VertexMesh(position_list[0], texcoord_list[1], normal_front),
		VertexMesh(position_list[1], texcoord_list[0], normal_front),
		VertexMesh(position_list[2], texcoord_list[2], normal_front),
		VertexMesh(position_list[3], texcoord_list[3], normal_front),
		// BACK SIDE
		VertexMesh(position_list[4], texcoord_list[1], normal_back),
		VertexMesh(position_list[5], texcoord_list[0], normal_back),
		VertexMesh(position_list[6], texcoord_list[2], normal_back),
		VertexMesh(position_list[7], texcoord_list[3], normal_back),
		// TOP SIDE
		VertexMesh(position_list[1], texcoord_list[1], normal_top),
		VertexMesh(position_list[6], texcoord_list[0], normal_top),
		VertexMesh(position_list[5], texcoord_list[2], normal_top),
		VertexMesh(position_list[2], texcoord_list[3], normal_top),
		// BOTTOM SIDE
		VertexMesh(position_list[7], texcoord_list[1], normal_bottom),
		VertexMesh(position_list[0], texcoord_list[0], normal_bottom),
		VertexMesh(position_list[3], texcoord_list[2], normal_bottom),
		VertexMesh(position_list[4], texcoord_list[3], normal_bottom),
		// RIGHT SIDE
		VertexMesh(position_list[3], texcoord_list[1], normal_right),
		VertexMesh(position_list[2], texcoord_list[0], normal_right),
		VertexMesh(position_list[5], texcoord_list[2], normal_right),
		VertexMesh(position_list[4], texcoord_list[3], normal_right),
		// LEFT SIDE
		VertexMesh(position_list[7], texcoord_list[1], normal_left),
		VertexMesh(position_list[6], texcoord_list[0], normal_left),
		VertexMesh(position_list[1], texcoord_list[2], normal_left),
		VertexMesh(position_list[0], texcoord_list[3], normal_left),
	};

	ui32 size_list = ARRAYSIZE(vertices);

	for (ui32 face = 0; face < size_list; face += 4) {
		setTangentBinormal(&vertices[face], 4);
	}

	ui32 index_list[] = {
		// FRONT SIDE
		0,1,2,
		2,3,0,
		// BACK SIDE
		4,5,6,
		6,7,4,
		// TOP SIDE
		8,9,10,
		10,11,8,
		// BOTTOM SIDE
		12,13,14,
		14,15,12,
		// RIGHT SIDE
		16,17,18,
		18,19,16,
		// LEFT SIDE
		20,21,22,
		22,23,20
	};

	ui32 size_index_list = ARRAYSIZE(index_list);
	reverseWinding(index_list, size_index_list);
	MaterialSlot slot;
	slot.start_index = 0;
	slot.num_indices = size_index_list;
	slot.maetrial_id = 0;

	auto mesh =  std::make_shared<Mesh>(
		vertices, size_list,
		index_list, size_index_list,
		&slot, 1,
		manager
	);

	obj->setMeshData(mesh);
	obj->setName("Cube");
}

void PrimitiveFactory::createPlane(ResourceManager* manager, GameObject* obj) {
	Vector3D position_list[] = {
		Vector3D(-1, 0, -1), // POS1
		Vector3D(-1, 0,  1), // POS2
		Vector3D(1,  0,  1), // POS3 
		Vector3D(1,  0, -1), // POS4
	};

	Vector2D texcoord_list[] = {
		Vector2D(0.0f, 0.0f),
		Vector2D(0.0f, 1.0f),
		Vector2D(1.0f, 0.0f),
		Vector2D(1.0f, 1.0f)
	};

	Vector3D normal = Vector3D(0, 1, 0);

	VertexMesh vertices[] = {
		VertexMesh(position_list[0], texcoord_list[1], normal),
		VertexMesh(position_list[1], texcoord_list[0], normal),
		VertexMesh(position_list[2], texcoord_list[2], normal),
		VertexMesh(position_list[3], texcoord_list[3], normal),
	};

	ui32 size_list = ARRAYSIZE(vertices);

	setTangentBinormal(vertices, (unsigned char)size_list);

	ui32 index_list[] = {
		0,1,2,
		2,3,0,
	};

	ui32 size_index_list = ARRAYSIZE(index_list);
	reverseWinding(index_list, size_index_list); 
	MaterialSlot slot;
	slot.start_index = 0;
	slot.num_indices = size_index_list;
	slot.maetrial_id = 0;

	auto mesh = std::make_shared<Mesh>(
		vertices, size_list,
		index_list, size_index_list,
		&slot, 1,
		manager
	);

	obj->setMeshData(mesh);
	obj->setName("Plane");
}

void PrimitiveFactory::createSphere(ResourceManager* manager, GameObject* obj) {
	std::vector<VertexMesh> verts;
	std::vector<ui32> indices;

	ui32 m_stackCount = 16;
	ui32 m_sliceCount = 24;
	f32 radius = 0.3f;

	VertexMesh top;
	top.m_position = Vector3D(0, radius, 0);
	top.m_texcoord = Vector2D(0.5f, 0.0f);
	top.m_normal = Vector3D(0,1,0);
	setTangentBinormal(top, 0.0f);
	verts.push_back(top);

	f32 phiStep = (f32)M_PI / m_stackCount;
	f32 thetaStep = 2.0f * (f32)M_PI / m_sliceCount;

	for (ui32 i = 1; i <= m_stackCount - 1; i++) {
		f32 phi = i * phiStep;
		for (ui32 j = 0; j <= m_sliceCount; j++) {
			f32 theta = j * thetaStep;

			f32 cosPhi = cosf(phi);
			f32 cosTheta = cosf(theta);
			f32 sinPhi = sinf(phi);
			f32 sinTheta = sinf(theta);

			Vector3D p = Vector3D(
				radius * sinPhi * cosTheta,
				radius * cosPhi,
				radius * sinPhi * sinTheta
			);

			VertexMesh v;
			v.m_position = p;
			v.m_texcoord = Vector2D(
				(f32)j / (f32)m_sliceCount,
				(f32)i / (f32)m_stackCount
			);
			v.m_normal = p.normalize();
			setTangentBinormal(v, theta);
			verts.push_back(v);
		}
	}

	VertexMesh bottom;
	bottom.m_position = Vector3D(0, -radius, 0);
	bottom.m_texcoord = Vector2D(0.5f, 1.0f); 
	bottom.m_normal = Vector3D(0, -1, 0);
	setTangentBinormal(bottom, 0.0f);
	verts.push_back(bottom);

	for (ui32 i = 1; i <= m_sliceCount; i++) {
		indices.push_back(0);
		indices.push_back(i + 1);
		indices.push_back(i);
	}

	ui32 baseIndex = 1;
	ui32 ringVertexCount = m_sliceCount + 1;

	for (ui32 i = 0; i < m_stackCount - 2; i++) {
		for (ui32 j = 0; j < m_sliceCount; j++) {
			indices.push_back(baseIndex + i * ringVertexCount + j);
			indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

			indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
			indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
		}
	}

	ui32 southPoleIndex = (ui32)verts.size() - 1;
	baseIndex = southPoleIndex - ringVertexCount;

	for (ui32 i = 0; i < m_sliceCount; i++) {
		indices.push_back(southPoleIndex);
		indices.push_back(baseIndex + i);
		indices.push_back(baseIndex + i + 1);
	}
	
	reverseWinding(indices.data(), indices.size());

	MaterialSlot slot;
	slot.start_index = 0;
	slot.num_indices = (ui32)indices.size();
	slot.maetrial_id = 0;

	auto mesh = std::make_shared<Mesh>(
		verts.data(), (ui32)verts.size(),
		indices.data(), (ui32)indices.size(),
		&slot, 1,
		manager
	);

	obj->setMeshData(mesh);
	obj->setName("Sphere");
}

void PrimitiveFactory::createCapsule(ResourceManager* manager, GameObject* obj) {
	std::vector<VertexMesh> verts;
	std::vector<ui32> indices;

	ui32 sliceCount = 24;
	ui32 capStackCount = 8;     
	f32 radius = 0.15f;
	f32 cylinderHeight = 0.3f; 
	f32 halfHeight = cylinderHeight * 0.5f;

	f32 thetaStep = 2.0f * (f32)M_PI / sliceCount;

	VertexMesh top;
	top.m_position = Vector3D(0, halfHeight + radius, 0);
	top.m_texcoord = Vector2D(0.5f, 0.0f);
	top.m_normal = Vector3D(0, 1, 0);
	setTangentBinormal(top, 0.0f);
	verts.push_back(top);

	for (ui32 i = 1; i <= capStackCount; i++) {
		f32 phi = (f32)i / (f32)capStackCount * ((f32)M_PI * 0.5f);
		f32 y = halfHeight + radius * cosf(phi);
		f32 ringRadius = radius * sinf(phi);

		for (ui32 j = 0; j <= sliceCount; j++) {
			f32 theta = j * thetaStep;
			Vector3D p = Vector3D(
				ringRadius * cosf(theta),
				y,
				ringRadius * sinf(theta)
			);
			VertexMesh v;
			v.m_position = p;
			v.m_texcoord = Vector2D((f32)j / sliceCount, (f32)i / (f32)(capStackCount * 2 + 2));
			v.m_normal = (p - Vector3D(0, halfHeight, 0)).normalize();
			setTangentBinormal(v, theta);
			verts.push_back(v);
		}
	}

	for (ui32 j = 0; j <= sliceCount; j++) {
		f32 theta = j * thetaStep;
		Vector3D p = Vector3D(radius * cosf(theta), -halfHeight, radius * sinf(theta));
		VertexMesh v;
		v.m_position = p;
		v.m_texcoord = Vector2D((f32)j / sliceCount, 0.5f);
		v.m_normal = Vector3D(cosf(theta), 0, sinf(theta));
		setTangentBinormal(v, theta);
		verts.push_back(v);
	}

	for (ui32 i = 1; i < capStackCount; i++) {
		f32 phi = (f32)M_PI * 0.5f + (f32)i / (f32)capStackCount * ((f32)M_PI * 0.5f);
		f32 y = -halfHeight + radius * cosf(phi);
		f32 ringRadius = radius * sinf(phi);

		for (ui32 j = 0; j <= sliceCount; j++) {
			f32 theta = j * thetaStep;
			Vector3D p = Vector3D(
				ringRadius * cosf(theta),
				y,
				ringRadius * sinf(theta)
			);
			VertexMesh v;
			v.m_position = p;
			v.m_texcoord = Vector2D((f32)j / sliceCount, 0.5f + (f32)i / (f32)(capStackCount * 2));
			v.m_normal = (p - Vector3D(0, -halfHeight, 0)).normalize();
			setTangentBinormal(v, theta);
			verts.push_back(v);
		}
	}

	VertexMesh bottom;
	bottom.m_position = Vector3D(0, -halfHeight - radius, 0);
	bottom.m_texcoord = Vector2D(0.5f, 1.0f);
	bottom.m_normal = Vector3D(0, -1, 0);
	setTangentBinormal(bottom, 0.0f);
	verts.push_back(bottom);

	ui32 ringVertexCount = sliceCount + 1;
	ui32 ringCount = capStackCount + 1 + (capStackCount - 1);

	for (ui32 i = 1; i <= sliceCount; i++) {
		indices.push_back(0);
		indices.push_back(i + 1);
		indices.push_back(i);
	}

	ui32 baseIndex = 1;
	for (ui32 i = 0; i < ringCount - 1; i++) {
		for (ui32 j = 0; j < sliceCount; j++) {
			indices.push_back(baseIndex + i * ringVertexCount + j);
			indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

			indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
			indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
		}
	}

	ui32 southPoleIndex = (ui32)verts.size() - 1;
	baseIndex = southPoleIndex - ringVertexCount;
	for (ui32 i = 0; i < sliceCount; i++) {
		indices.push_back(southPoleIndex);
		indices.push_back(baseIndex + i);
		indices.push_back(baseIndex + i + 1);
	}

	reverseWinding(indices.data(), indices.size());

	MaterialSlot slot;
	slot.start_index = 0;
	slot.num_indices = (ui32)indices.size();
	slot.maetrial_id = 0;

	auto mesh = std::make_shared<Mesh>(
		verts.data(), (ui32)verts.size(),
		indices.data(), (ui32)indices.size(),
		&slot, 1,
		manager
	);

	obj->setMeshData(mesh);
	obj->setName("Capsule");
}

