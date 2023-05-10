#include "ObjectManager.h"

void ObjectManager::AddObject(GameObject* createdObject)
{
	pObjectList.push_back(createdObject);
}

void ObjectManager::UpdateAll()
{
   /* for (auto element = pObjectList.begin(); element != pObjectList.end(); element++)
    {
        GameObject* obj = *element;

        switch (obj->GetGameObjectType())
        {
        case GameObjectsType::Enemy_Type:
            // The game object is Enemy
            Enemy* enemy = dynamic_cast<Enemy*>(*element);
            break;
        case GameObjectsType::Player_Type:
            Player* player = dynamic_cast<Player*>(*element);
            //player->UpdatePosition();
            break;

        case GameObjectsType::PersonToSave_Type:
            SavePerson* savePerson = dynamic_cast<SavePerson*>(*element);
            break;
        }
    }
    */
}


void ObjectManager::RenderAll()
{
    for (auto element = pObjectList.begin(); element != pObjectList.end(); element++) 
    {
        GameObject* obj = *element;
        Bullet* bullet;
        Enemy* enemy;
        Player* player;
        SavePerson* savePerson;

        switch (obj->GetGameObjectType()) 
        {
        case GameObjectsType::Bullet_Type:
            // The game object is Bullet
            bullet = dynamic_cast<Bullet*>(*element);
            break;
        case GameObjectsType::Enemy_Type:
            // The game object is Enemy
            enemy = dynamic_cast<Enemy*>(*element);
            break;
        case GameObjectsType::Player_Type:
            player = dynamic_cast<Player*>(*element);
            break;

        case GameObjectsType::PersonToSave_Type:
            savePerson = dynamic_cast<SavePerson*>(*element);
        break;
        }
    }
}