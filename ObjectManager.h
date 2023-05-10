#pragma once

#include "GameObject.h"

// Define the generic method in the header file
template<typename T>
T CreateObject() 
{

	// Do something with obj
}

class ObjectManager
{
protected:
	MyDrawEngine* pDE = MyDrawEngine::GetInstance();
	GameObjectsType kindOfGameObject;

public:
	std::list<GameObject*> pObjectList;

	void UpdateAll();
	void RenderAll();
	void AddObject(GameObject* pNewObject);
	void RemoveObject(GameObject* objectToRemove);
};