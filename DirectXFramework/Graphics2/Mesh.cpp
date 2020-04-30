#include "Mesh.h"

// Material methods

Material::Material(wstring materialName, XMFLOAT4 diffuseColour, XMFLOAT4 specularColour, float shininess, float opacity, ComPtr<ID3D11ShaderResourceView> texture )
{
	_materialName = materialName;
	_diffuseColour = diffuseColour;
	_specularColour = specularColour;
	_shininess = shininess;
	_opacity = opacity;
    _texture = texture;
}

Material::~Material(void)
{
}

// SubMesh methods

SubMesh::SubMesh(ComPtr<ID3D11Buffer> vertexBuffer,
				 vector<XMFLOAT3> positions,
			 	 ComPtr<ID3D11Buffer> indexBuffer,
				 size_t vertexCount,
				 size_t indexCount,
				 shared_ptr<Material> material,
				 bool isPropelor)
{
	_vertexBuffer = vertexBuffer;
	_positions = positions;
	_indexBuffer = indexBuffer;
	_vertexCount = vertexCount;
	_indexCount = indexCount;
	_material = material;
	_isPropeller = isPropelor;
}

SubMesh::~SubMesh(void)
{
}

// Mesh methods

size_t Mesh::GetSubMeshCount()
{
	return _subMeshList.size();
}

shared_ptr<SubMesh> Mesh::GetSubMesh(unsigned int i)
{
	if (i >= 0 && i < _subMeshList.size())
	{
		return _subMeshList[i];
	}
	else
	{
		return nullptr;
	}
}

void Mesh::AddSubMesh(shared_ptr<SubMesh> subMesh)
{
	_subMeshList.push_back(subMesh);
}

shared_ptr<Node> Mesh::GetRootNode()
{
	return _rootNode;
}

void Mesh::SetRootNode(shared_ptr<Node> node)
{
	_rootNode = node;
}

vector<shared_ptr<SubMesh>> Mesh::GetSubMeshList()
{
	return _subMeshList;
}
