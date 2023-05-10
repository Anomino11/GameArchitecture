#include "GameObject.h"

#include <iostream>
#include <random>

using namespace std;



void GameObject::Initialize(GameObjectsType kindOfObject, PictureIndex image, Vector2D startPosition)
{
	this->kindOfGameObject = kindOfObject;
	this->position = startPosition;
	this->image = image;
}

void GameObject::UpdatePosition(Vector2D updatedPosition, float scale, float spin)
{
	this->GetDrawEngine()->DrawAt(updatedPosition, this->image, scale, spin);

	//collisionHitbox.PlaceAt(updatedPosition, 35);
	//de->FillCircle(collisionHitbox.GetCentre(), 35, MyDrawEngine::RED);

	this->position = updatedPosition;
}

GameObjectsType GameObject::GetGameObjectType()
{
	return this->kindOfGameObject;
}

void GameObject::Rerender(Vector2D position, float spin, float scale)
{
	this->GetDrawEngine()->DrawAt(position, this->image, scale, spin);
}

Vector2D GameObject::GetPosition()
{
	return this->position;
}

MyDrawEngine* GameObject::GetDrawEngine()
{
	return this->pDE;
}

std::list<Bullet*> CheckBulletState(std::list<Bullet*>& bullets)
{
	std::list<Bullet*> bulletsToDelete;

	for (auto it = bullets.begin(); it != bullets.end();)
	{
		Bullet* bullet = *it;
		bullet->MoveBullet();

		MyDrawEngine* de = MyDrawEngine::GetInstance();
		// Get the screen size
		int screenWidth = de->GetScreenWidth();
		int screenHeight = de->GetScreenHeight();

		// Check if the bullet is out of bounds
		if (bullet->position.XValue < -screenWidth || bullet->position.XValue > screenWidth || bullet->position.YValue < -screenHeight || bullet->position.YValue > screenHeight)
		{
			// If so, add it into the list with bullets to delete
			bulletsToDelete.push_back(bullet);

			// And erase it from the active bullets list
			it = bullets.erase(it);
		}
		else
		{
			// Otherwise, the bullet is in the scene and it still should be alive
			it++;
		}
	}
	return bulletsToDelete;
}

Player::Player(PictureIndex image, Vector2D startPosition)
{
	// Call the GameObject's Initialize method
	Initialize(GameObjectsType::Player_Type, image, startPosition);
	collisionHitbox.PlaceAt(startPosition, 35);
}

Player::Player()
{
	/*this->image = pDE->LoadPicture(L"assets\\ship.bmp");
	Initialize(GameObjectsType::Player_Type, image, Vector2D(0, 0));
	collisionHitbox.PlaceAt(this->GetPosition(), 35);
	*/
}

void Player::CreateForceFieled(Vector2D startingFieledPos)
{
	startingFieledPosition = new Vector2D(startingFieledPos);
	this->forceFieledPoints.push_back(*startingFieledPosition);
}

double Distance(Vector2D v1, Vector2D v2)
{
	return sqrt(pow(v1.XValue - v2.XValue, 2) + pow(v1.YValue - v2.YValue, 2));
}

Vector2D FindCrossedPoint(std::list<Vector2D>* forceFieledPoints, Vector2D currentPoint)
{
	// Copy the elements from the list to the array using a loop
	int x = 0;
	for (const auto& element : *forceFieledPoints)
	{
		double distance = Distance(element, currentPoint);

		if (distance < 2)
		{
			return element;
		}

		x++;
	}

	return currentPoint;
}

bool IsCrossedBetweenSegments(std::list<Segment2D>* segmentList, MyDrawEngine* de)
{

	for (auto it1 = segmentList->begin(); it1 != segmentList->end(); ++it1)
	{
		for (auto it2 = std::next(it1); it2 != segmentList->end(); ++it2)
		{
			if (it1->Intersects(*it2))
			{
				// get element ID for it1
				int id1 = std::distance(segmentList->begin(), it1);
				// get element ID for it2
				int id2 = std::distance(segmentList->begin(), it2);
				// Vetsi
				de->WriteInt(Vector2D(400, 600), id1, MyDrawEngine::WHITE);

				// Mensi
				de->WriteInt(Vector2D(400, 700), id2, MyDrawEngine::WHITE);

				auto sonnerSegment = std::next(segmentList->begin(), id2);
				auto laterSegment = std::next(segmentList->begin(), id1);

				Vector2D start = sonnerSegment->GetStart();
				double radius = Distance(start, laterSegment->GetStart());

				Circle2D circle;
				circle.PlaceAt(start, radius);
				de->FillCircle(start, radius, MyDrawEngine::BLUE);

				return true;
				// The two elements are the same
			}
		}
	}
	return false;

}

Vector2D GetMiddlePoint(const std::list<Vector2D>& points)
{
	double sumX = 0;
	double sumY = 0;
	int count = points.size();

	for (const auto& point : points)
	{
		sumX += point.XValue;
		sumY += point.YValue;
	}

	double averageX = sumX / count;
	double averageY = sumY / count;

	return Vector2D(averageX, averageY);
}

void Player::ContinueForceFieled(Vector2D currentPosition)
{
	if (startingFieledPosition != nullptr)
	{
		MyDrawEngine* de = GetDrawEngine();
		bool shouldDraw = true;
		auto lastElement = forceFieledPoints.end();
		lastElement--;

		if (!forceFieledFinished)
		{

			Vector2D crossedPoint = FindCrossedPoint(&forceFieledPoints, currentPosition);

			if (crossedPoint.XValue == currentPosition.XValue && crossedPoint.YValue == currentPosition.YValue)
			{
				if (this->isDirectionChanged)
				{
					for (const Vector2D& element : forceFieledPoints)
					{
						if (Distance(element, currentPosition) <= 10)
						{
							shouldDraw = false;
						}
					}

					if (shouldDraw)
					{
						this->forceFieledPoints.push_back(currentPosition);
					}
				}
				else
				{
					if (forceFieledPoints.size() <= 2)
					{
						this->forceFieledPoints.push_back(currentPosition);
					}
					else
					{
						// Overwrite last element
						*lastElement = currentPosition;
					}
				}
			}
			else
			{
				de->WriteText(Vector2D(400, 500), L"CROSSED POINT", MyDrawEngine::WHITE);
			}

		}

		Vector2D* forceFieledArrayPoints = new Vector2D[this->forceFieledPoints.size()];
		int number = this->forceFieledPoints.size();

		Vector2D first = forceFieledPoints.front();
		Vector2D last = forceFieledPoints.back();

		// Copy the elements from the list to the array using a loop
		int x = 0;
		for (const auto& element : this->forceFieledPoints)
		{
			forceFieledArrayPoints[x] = element;
			if (x > 0)
			{
				de->WriteInt(450, 300, forceFieledPoints.size(), MyDrawEngine::WHITE);

				de->DrawLine(forceFieledArrayPoints[x - 1], forceFieledArrayPoints[x], MyDrawEngine::YELLOW);

				//de->DrawPointList(forceFieledArrayPoints, MyDrawEngine::YELLOW, forceFieledPoints.size());
				//de->DrawLine(forceFieledArrayPoints[x - 1], forceFieledArrayPoints[x], MyDrawEngine::YELLOW);

				if (!forceFieledFinished)
				{
					Segment2D segment;
					segment.PlaceAt(forceFieledArrayPoints[x - 1], forceFieledArrayPoints[x]);
					foreFieledSegments.push_back(segment);
				}

				//de->
			}
			x++;
		}

		bool pojistka = IsCrossedBetweenSegments(&foreFieledSegments, de);

		if (pojistka)
		{
			de->WriteText(Vector2D(400, 500), L"CROSSED POINTS BABY", MyDrawEngine::WHITE);
		}

		Vector2D center = GetMiddlePoint(forceFieledPoints);

		auto firstElement = forceFieledPoints.begin();
		double radius = Distance(*firstElement, center);
		//forceCircle.PlaceAt(center, radius / 2);
		//de->FillCircle(center, radius, MyDrawEngine::WHITE);

		//de->DrawPointList(forceFieledArrayPoints, MyDrawEngine::YELLOW, number);


		if (forceFieledPoints.size() > 4 && Distance(first, last) < 50)
		{
			if (!forceFieledFinished)
			{
				this->forceFieledPoints.push_back(first);
			}

			forceFieledFinished = true;

			de->WriteText(Vector2D(400, 300), L"FINISHED", MyDrawEngine::WHITE);
		}
		else
		{
			foreFieledSegments.clear();
		}
		de->WriteDouble(Vector2D(300, 300), foreFieledSegments.size(), MyDrawEngine::WHITE);

		delete[] forceFieledArrayPoints;
	}
}

void Player::EndForceFieled()
{
	// If the player is not drawing, set reset the fieleds
	if (!this->isDrawingForceFieled)
	{
		this->currentFieledPoistion = nullptr;
		this->startingFieledPosition = nullptr;
		forceFieledPoints.clear();
		forceFieledFinished = false;
	}
}

void Player::ChangeIsDrawingForceFieledState(bool isDrawing)
{
	this->isDrawingForceFieled = isDrawing;
}

bool Player::GetIsDrawingForceFieledState()
{
	return this->isDrawingForceFieled;
}

void Player::UpdatePosition(Vector2D updatedPosition, float scale, float spin)
{
	this->position = updatedPosition;

	this->GetDrawEngine()->DrawAt(this->GetPosition(), image, scale, spin);
	collisionHitbox.PlaceAt(this->GetPosition(), 35);
	//de->FillCircle(collisionHitbox.GetCentre(), 35, MyDrawEngine::RED);

	std::list<Bullet*> bulletsToDelete;

	bulletsToDelete = CheckBulletState(this->playerShotBullets);

	for (Bullet* bullet : bulletsToDelete)
	{
		delete bullet;
	}
}

bool Player::Shoot(Vector2D direction)
{
	// Get the current time
	this->currentTime = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsedTime = this->currentTime - this->startCooldown;

	if (elapsedTime.count() >= 1.0)
	{
		Bullet* bullet = new Bullet(this->position, direction, this->kindOfGameObject);
		this->playerShotBullets.push_back(bullet);

		// Reset the timer
		this->startCooldown = std::chrono::steady_clock::now();
		return true;
	}

	return false;
}

void Player::Dispose()
{
	delete startingFieledPosition;
	delete currentFieledPoistion;

	for (Bullet* bullet : this->playerShotBullets)
	{
		delete bullet;
		bullet = nullptr;
	}
}

SavePerson::SavePerson(PictureIndex image, Vector2D startPosition)
{
	Initialize(GameObjectsType::PersonToSave_Type, image, startPosition);
	collisionHitbox.PlaceAt(100, 75, 30, 110);
}

SavePerson::SavePerson()
{

}

void SavePerson::UpdatePosition(Vector2D updatedPosition, float scale, float spin)
{
	this->GetDrawEngine()->DrawAt(updatedPosition, image, scale, spin);

	collisionHitbox.PlaceAt(updatedPosition, Vector2D(updatedPosition.YValue - 100, updatedPosition.YValue - 500));

	this->GetDrawEngine()->FillRect(collisionHitbox, MyDrawEngine::CYAN);
	this->position = updatedPosition;
}

int GetRandomNumber(int max, int min)
{
	// Create a random number generator
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min, max);

	// Generate a random number
	int randomNumber = dis(gen);
	return randomNumber;
}

Enemy::Enemy(PictureIndex image, Vector2D startPosition)
{
	// Call the GameObject's Initialize method
	Initialize(GameObjectsType::Enemy_Type, image, startPosition);
	collisionHitbox.PlaceAt(startPosition, 35);

	this->enemyCurrentPosition = startPosition;

	// Get a random distance between 
	this->distance = GetRandomNumber(100, 10);
}
Enemy::Enemy()
{}

Circle2D Bullet::GetHitBox()
{
	return this->bulletCollisionHitbox;
}

Bullet::Bullet(Vector2D creatorPosition, Vector2D direction, GameObjectsType ownerType)
{
	this->image = this->GetDrawEngine()->LoadPicture(L"assets\\plasma.bmp");
	this->directionToGo = direction;
	this->ownerType = ownerType;

	this->Initialize(GameObjectsType::Bullet_Type, image, creatorPosition);
	this->bulletCollisionHitbox.PlaceAt(creatorPosition, 18);
}

void Bullet::MoveBullet()
{
	float speed = 0.05f; // adjust as needed
	Vector2D displacement = speed * this->directionToGo;
	this->position += displacement;
	this->bulletCollisionHitbox.PlaceAt(this->position, 18);

	this->GetDrawEngine()->DrawAt(this->position, this->image);
}

void Enemy::Shoot(Vector2D direction)
{
	// Get the current time
	this->currentTime = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsedTime = this->currentTime - this->startCooldown;

	if (elapsedTime.count() >= 2.0)
	{
		Bullet* bullet = new Bullet(this->position, direction, this->kindOfGameObject);
		this->shotBullets.push_back(bullet);

		// Reset the timer
		this->startCooldown = std::chrono::steady_clock::now();
	}
}

void Enemy::EnemyUpdatePosition(Vector2D playerPos, float speed)
{
	Vector2D direction = this->position;

	this->enemyCurrentPosition = playerPos;

	this->GetDrawEngine()->WriteDouble(Vector2D(500, 500), this->distance, MyDrawEngine::WHITE);

	Vector2D targetPos = (1 - speed) * this->position + speed * playerPos;
	direction = targetPos - this->position;
	lastAngle = direction.angle();

	// Checks the distance
	if (Distance(playerPos, this->position) >= this->distance * 10)
	{
		// If it is not too close, move forward
		this->position = targetPos;
	}


	this->GetDrawEngine()->DrawAt(position, this->image, 1.0f, lastAngle);
	this->collisionHitbox.PlaceAt(this->position, 35);
	this->GetDrawEngine()->FillCircle(this->position, 35, MyDrawEngine::WHITE);

	this->Shoot(playerPos);

	std::list<Bullet*> bulletsToDelete;

	bulletsToDelete = CheckBulletState(this->shotBullets);

	for (Bullet* bullet : bulletsToDelete)
	{
		delete bullet;
	}
	
}

void Enemy::Dispose()
{
	for (Bullet* bullet : this->shotBullets)
	{
		delete bullet;
		bullet = nullptr;
	}
}

