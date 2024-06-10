#ifndef SCENE_H
#define SCENE_H

#include "Geom.h"
#include <list>
using std::list;
using std::pair;
class GameObject;

enum GameObjectType{STATIC, MOVING};

using GameObjectPair = pair<GameObject*, GameObject*>;
using GameObjectDel = pair<GameObject*, int>;

class Scene
{
private:
	list<GameObject*> statics;
	list<GameObject*> moving;

	list<GameObjectPair> collisions;
	list<GameObjectDel> toDelete;

	list<GameObject*>::iterator its;
	list<GameObject*>::iterator itm;
	list<GameObject*>::iterator it;

	int processing = STATIC;
	// colisões relacionadas a pontos
	bool Collision(Point* p, Point* q);       
	bool Collision(Point* p, Rect* r);           
	bool Collision(Point* p, Circle* c);
	// colisões relacionadas a retângulos
	bool Collision(Rect* ra, Rect* rb);       
	bool Collision(Rect* r, Point* p);        
	bool Collision(Rect* r, Circle* c);
	// colisões relacionadas a círculos
	bool Collision(Circle* ca, Circle* cb);   
	bool Collision(Circle* c, Point* p);      
	bool Collision(Circle* c, Rect* r);


	void ProcessDeleted();

public:
	Scene();
	~Scene();

	void Add(GameObject* obj, int type);
	void Remove(GameObject* obj, int type);
	void Remove();
	void Delete(GameObject* obj, int type);
	void Delete();
	unsigned int GetSceneSize(); // serve para retornar a quantidade de objectos na cena

	void Begin();
	GameObject* Next();
	
	void SceneUpdate();
	void SceneDraw();
	void DrawBBox();

	bool Collision(GameObject* oa, GameObject* ob);
	void CollisionDetection();
};

inline bool Scene::Collision(Rect* r, Point* p)
{
	return Collision(p, r);
}

inline bool Scene::Collision(Circle* c, Point* p)
{
	return Collision(p, c);
}

inline bool Scene::Collision(Circle* c, Rect* r)
{
	return Collision(r, c);
}

#endif
