#ifndef SCENE_H
#define SCENE_H

#include <list>
using std::list;
class GameObject;

class Scene
{
private:
	list <GameObject*> objects;
	list <GameObject*>::iterator next;
	list <GameObject*>::iterator it;

public:
	Scene();
	~Scene();

	void Add(GameObject* obj);
	void SceneUpdate();
	void SceneDraw();

	void Begin();
	GameObject* Next();
	void Remove();

};

#endif
