#include "SceneGraph.h"

bool SceneGraph::Initialise(void)
{
	for (SceneNodePointer child : _children) {
		if (!child->Initialise()) {
			return false;
		}
	}
	return true;
}

void SceneGraph::Update(FXMMATRIX& currentWorldTransformation)
{
	XMStoreFloat4x4(&_combinedWorldTransformation, XMLoadFloat4x4(&_worldTransformation) * currentWorldTransformation);
	for(SceneNodePointer child : _children) {
		child->Update(XMLoadFloat4x4(&_combinedWorldTransformation));
	}
}

void SceneGraph::Render(void)
{
	for(SceneNodePointer child : _children) {
		child->Render();
	}
}

void SceneGraph::Shutdown(void)
{
	for(SceneNodePointer child : _children) {
		child->Shutdown();
	}
}

void SceneGraph::Add(SceneNodePointer node)
{
	_children.push_back(node);
}

void SceneGraph::Remove(SceneNodePointer node)
{
	for(SceneNodePointer child : _children) {
		child->Remove(node);
		if(child == node) {
			_children.remove(node);
		}
	}
}

SceneNodePointer SceneGraph::Find(wstring name)
{
	if (name == _name) {
		return shared_from_this();
	}
	for (SceneNodePointer child : _children) {
		if (child->Find(name) != nullptr) {
			return child->Find(name);
		}
	}
	return nullptr;
}
