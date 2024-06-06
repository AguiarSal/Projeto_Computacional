#include "Scene.h"
#include "GameObject.h"

Scene::Scene()
{

}

Scene::~Scene()
{
	for (auto i : objects)
	{
		delete i;
	}
}

void Scene::Add(GameObject* obj)
{
	objects.push_back(obj);
}

void Scene::SceneUpdate()
{
	next = objects.begin();
	while (next != objects.end())
	{
		it = next++;
		(*it)->ObjUpdate();
	}
}

void Scene::SceneDraw()
{
	next = objects.begin();
	while (next != objects.end())
	{
		it = next++;
		(*it)->ObjDraw();
	}
}

GameObject* Scene::Next()
{
	if (next != objects.end())
	{
		it = next++;
		return *it;
	}
	else
	{
		return nullptr;
	}
}

void Scene::Remove()
{
	delete(*it);
	objects.erase(it);
}