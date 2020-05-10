#include "SceneGraph.h"

// Calls initialise method on all child nodes
bool SceneGraph::Initialise(void)
{
	for (SceneNodePointer child : _children) {
		if (!child->Initialise()) {
			return false;
		}
	}
	return true;
}

// Updates world transformation then updates all child nodes
void SceneGraph::Update(FXMMATRIX& currentWorldTransformation)
{
	SceneNode::Update(currentWorldTransformation);
	for(SceneNodePointer child : _children) {
		child->Update(XMLoadFloat4x4(&_combinedWorldTransformation));
	}
}

// Calls render method on all child nodes
void SceneGraph::Render(void)
{
	for(SceneNodePointer child : _children) {
		child->Render();
	}
}

// Calls shutdown method on all children
void SceneGraph::Shutdown(void)
{
	for(SceneNodePointer child : _children) {
		child->Shutdown();
	}
}

// Adds a new node to the list of child nodes
void SceneGraph::Add(SceneNodePointer node)
{
	_children.push_back(node);
}

// Removes a node from the list of child nodes and removes it from children too
void SceneGraph::Remove(SceneNodePointer node)
{
	for(SceneNodePointer child : _children) {
		child->Remove(node);
		if(child == node) {
			_children.remove(node);
		}
	}
}

// Returns child node with a specified name
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
