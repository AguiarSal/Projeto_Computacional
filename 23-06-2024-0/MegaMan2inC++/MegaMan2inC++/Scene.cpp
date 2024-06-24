#include "Scene.h"
#include "GameObject.h"
#include "Engine.h"

Scene::Scene()
{

}

Scene::~Scene()
{
	for (auto obj : statics)
	{
		delete obj;
	}

	for (auto obj : moving)
	{
		delete obj;
	}
}

void Scene::Add(GameObject* obj, int type)
{
	if (type == STATIC)
	{
		statics.push_back(obj);
	}
	else
	{
		moving.push_back(obj);
	}
}

void Scene::Remove(GameObject* obj, int type)
{
	if (type == STATIC)
	{
		statics.remove(obj);
	}
	else
	{
		moving.remove(obj);
	}
}

void Scene::Remove()
{
	if (processing == STATIC)
	{
		statics.remove(*it);
	}
	else
	{
		moving.remove(*it);
	}
}

void Scene::Delete(GameObject* obj, int type)
{
	toDelete.push_back(GameObjectDel{ obj, type });
}

void Scene::Delete()
{
	toDelete.push_back(GameObjectDel{ *it, processing });
}

unsigned int Scene::GetSceneSize()
{
	return unsigned int(moving.size() + statics.size());
}

void Scene::ProcessDeleted()
{
	toDelete.sort();
	toDelete.unique();

	for (const auto & [obj, type] : toDelete)
	{
		delete obj;

		if (type == STATIC)
		{
			statics.remove(obj);
		}
		else
		{
			moving.remove(obj);
		}
	}

	toDelete.clear();
}

void Scene::SceneUpdate()
{
	processing = STATIC;
	for (it = statics.begin(); it != statics.end(); ++it)
	{
		(*it)->ObjUpdate();
	}

	processing = MOVING;
	for (it = moving.begin(); it != moving.end(); ++it)
	{
		(*it)->ObjUpdate();
	}

	ProcessDeleted();
}

void Scene::SceneDraw()
{
	processing = STATIC;
	for (it = statics.begin(); it != statics.end(); ++it)
		(*it)->ObjDraw();

	processing = MOVING;
	for (it = moving.begin(); it != moving.end(); ++it)
		(*it)->ObjDraw();
}

void Scene::DrawBBox()
{
	// inicia desenho de pixels
	Engine::renderer->BeginPixels();

	// desenha bounding box dos objetos estáticos
	for (const auto& obj : statics)
	{
		if (obj->GetObjBBox())
			Engine::renderer->Draw(obj->GetObjBBox(), 0xffff00ff);
	}

	// desenha bounding box dos objetos em movimento
	for (const auto& obj : moving)
	{
		if (obj->GetObjBBox())
			Engine::renderer->Draw(obj->GetObjBBox(), 0xffff00ff);
	}

	// finaliza desenho de pixels
	Engine::renderer->EndPixels();
}

void Scene::Begin()
{
	its = statics.begin();
	itm = moving.begin();
	processing = STATIC;
}

GameObject* Scene::Next()
{
	if (its != statics.end())
	{
		it = its++;
		return *it;
	}
	else
	{
		if (itm != moving.end())
		{
			processing = MOVING;
			it = itm++;
			return *it;
		}
		else
		{
			return nullptr;
		}
	}
}

bool Scene::Collision(Point* p, Point* q)
{
	if (int(p->GetX()) == int(q->GetX()) && int(p->GetY()) == int(q->GetY()))
	{
		return true;
	}

	return false;
}

bool Scene::Collision(Point* p, Rect* r)
{
	if (p->GetX() >= r->Left() && p->GetX() <= r->Right())
	{
		if (p->GetY() >= r->Top() && p->GetY() <= r->Bottom())
		{
			return true;
		}
	}
	return false;
}

bool Scene::Collision(Point* p, Circle* c)
{
	if (p->Distance(Point(c->GetCenterX(), c->GetCenterY())) <= c->Radius)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Scene::Collision(Rect* ra, Rect* rb)
{
	bool overlapX = (rb->Left() <= ra->Right() && ra->Left() <= rb->Right());

	bool overlapY = (rb->Top() <= ra->Bottom() && ra->Top() <= rb->Bottom());

	return overlapX && overlapY;
}

bool Scene::Collision(Rect* r, Circle* c)
{
	float px, py;

	if (c->GetCenterX() < r->Left())
		px = r->Left();
	else
		if (c->GetCenterX() > r->Right())
			px = r->Right();
		else
			px = c->GetCenterX();

	if (c->GetCenterY() < r->Top())
		py = r->Top();
	else
		if (c->GetCenterY() > r->Bottom())
			py = r->Bottom();
		else
			py = c->GetCenterY();

	Point point(px, py);
	return Collision(&point, c);
}

bool Scene::Collision(Circle* ca, Circle* cb)
{
	float deltaX = abs(ca->GetCenterX() - cb->GetCenterX());
	float deltaY = abs(ca->GetCenterY() - cb->GetCenterY());

	float distance = float(sqrt(double(deltaX) * double(deltaX) + double(deltaY) * double(deltaY)));

	if (distance <= (ca->Radius + cb->Radius))
		return true;

	return false;
}

bool Scene::Collision(GameObject* oa, GameObject* ob)
{
	if (!(oa->GetObjBBox() && ob->GetObjBBox()))
		return false;

	switch (oa->GetObjBBox()->GetType())
	{
	case POINT_TYPE:

		switch (ob->GetObjBBox()->GetType())
		{
		case POINT_TYPE:
			return Collision((Point*)oa->GetObjBBox(), (Point*)ob->GetObjBBox());

		case CIRCLE_TYPE:
			return Collision((Point*)oa->GetObjBBox(), (Circle*)ob->GetObjBBox());

		case RECT_TYPE:
			return Collision((Point*)oa->GetObjBBox(), (Rect*)ob->GetObjBBox());

		default:
			return false;
		}
    
	case CIRCLE_TYPE:

		switch (ob->GetObjBBox()->GetType())
		{
		case POINT_TYPE:
			// Circle && Point
			return Collision((Circle*)oa->GetObjBBox(), (Point*)ob->GetObjBBox());

		case CIRCLE_TYPE:
			// Circle && Circle
			return Collision((Circle*)oa->GetObjBBox(), (Circle*)ob->GetObjBBox());

		case RECT_TYPE:
			// Circle && Rectangle
			return Collision((Circle*)oa->GetObjBBox(), (Rect*)ob->GetObjBBox());

		default:
			return false;
		}

	case RECT_TYPE:

		switch (ob->GetObjBBox()->GetType())
		{
		case POINT_TYPE:
			return Collision((Rect*)oa->GetObjBBox(), (Point*)ob->GetObjBBox());

		case CIRCLE_TYPE:
			return Collision((Rect*)oa->GetObjBBox(), (Circle*)ob->GetObjBBox());

		case RECT_TYPE:
			return Collision((Rect*)oa->GetObjBBox(), (Rect*)ob->GetObjBBox());

		default:
			return false;
		}

	default:
		return false;
	}
}

void Scene::CollisionDetection()
{
	list<GameObject*>::iterator start, end;

	collisions.clear();

	if (moving.size() >= 2)
	{
		end = moving.end();
		end--;
		for (auto i = moving.begin(); i != end; ++i)
		{
			start = i;

			for (auto j = ++start; j != moving.end(); ++j)
			{
				if (Collision(*i, *j))
					collisions.push_back(GameObjectPair(*i, *j));
			}
		}
	}

	for (auto i : moving)
	{
		for (auto j : statics)
		{
			if (Collision(i, j))
				collisions.push_back(GameObjectPair(i, j));
		}
	}

	if (!collisions.empty())
	{
		for (auto [a,b] : collisions)
		{
			a->OnCollision(b);
			b->OnCollision(a);
		}
	}

	ProcessDeleted();
}