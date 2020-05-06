#pragma once
#include <list>
#include "SceneNode.h"

// A scene node that contains all nodes in the scene, applying any updates to all of its children
class SceneGraph : public SceneNode
{
public:
	SceneGraph() : SceneNode(L"Root") {};
	SceneGraph(wstring name) : SceneNode(name) {};
	~SceneGraph(void) {};

	virtual bool Initialise(void);
	virtual void Update(FXMMATRIX& currentWorldTransformation);
	virtual void Render(void);
	virtual void Shutdown(void);

	void Add(SceneNodePointer node);
	void Remove(SceneNodePointer node);
	SceneNodePointer Find(wstring name);

	bool GetRenderBoundingVolumes() { return _renderBoundingVolumes; }
	void ToggleRenderBoundingVolumes() { _renderBoundingVolumes = !_renderBoundingVolumes; }

private:
	list<SceneNodePointer> _children;
	bool				   _renderBoundingVolumes = false;
};

typedef shared_ptr<SceneGraph>			 SceneGraphPointer;
