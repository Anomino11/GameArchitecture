#pragma once

#include "../shell2023b/Shapes.h"
#include "../shell2023b/mydrawengine.h"
#include <list>
#include <chrono>

enum GameObjectsType
{
	Player_Type = 1,
	Enemy_Type = 2,
	Bullet_Type = 3,
	PersonToSave_Type = 4
};

class GameObject
{
protected:
	MyDrawEngine* pDE = MyDrawEngine::GetInstance();
	GameObjectsType kindOfGameObject;
	void Rerender(Vector2D position, float spin, float scale);
public:
	PictureIndex image;
	Vector2D position;

	void Initialize(GameObjectsType kindOfObject, PictureIndex image, Vector2D startPosition);
	virtual void UpdatePosition(Vector2D updatedPosition, float scale, float spin);

	Vector2D GetPosition();
	MyDrawEngine* GetDrawEngine();
	GameObjectsType GetGameObjectType();
};

class Bullet : public GameObject
{
private:
	Vector2D directionToGo;
	Circle2D bulletCollisionHitbox;

public:
	bool isOutsideOfScreen = false;
	bool shotSomething = false;

	GameObjectsType ownerType;

	Circle2D GetHitBox();

	void MoveBullet();
	Bullet(Vector2D creatorPosition, Vector2D direction, GameObjectsType ownerType);
};

class Player : public GameObject
{
private:
	Vector2D* startingFieledPosition;
	Vector2D* currentFieledPoistion;

	std::list<Vector2D> forceFieledPoints;

	bool isDrawingForceFieled = false;
	int lastDrawnPointIndex = 0;

	std::chrono::steady_clock::time_point startCooldown = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point currentTime;

	bool ShouldContinueDrawingForceFieled(Vector2D positionToCheck, double maxDistance);

public:
	Circle2D collisionHitbox;
	Circle2D forceCircle;

	std::list<Segment2D> foreFieledSegments;
	std::list<Bullet*> playerShotBullets;

	bool isDirectionChanged = false;
	bool forceFieledFinished = false;


	bool isAlive = true;

	void CreateForceFieled(Vector2D forceFieledStartPosition);
	void ContinueForceFieled(Vector2D currentPosition);
	void EndForceFieled();

	void ChangeIsDrawingForceFieledState(bool isDrawing);
	bool GetIsDrawingForceFieledState();

	void UpdatePosition(Vector2D updatedPosition, float scale, float spin);

	bool Shoot(Vector2D direction);

	Player(PictureIndex image, Vector2D startPosition);
	Player();
	void Dispose();
};

class SavePerson : public GameObject
{
private:
public:
	Rectangle2D collisionHitbox;

	void UpdatePosition(Vector2D updatedPosition, float scale, float spin);
	SavePerson(PictureIndex image, Vector2D startPosition);
	SavePerson();
};

class Enemy : public GameObject
{
private:
	Vector2D enemyCurrentPosition;

public:
	Circle2D collisionHitbox;
	std::chrono::steady_clock::time_point startCooldown = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point currentTime;
	std::list<Bullet*> shotBullets;

	int distance = 10;
	float lastAngle = 0;
	bool isAlive = true;

	Enemy(PictureIndex image, Vector2D startPosition);
	Enemy();
	void Shoot(Vector2D direction);
	void EnemyUpdatePosition(Vector2D playerPos, float speed);
	void Dispose();
};
