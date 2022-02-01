#include <vector>
#include <stdio.h> // Include stdio.h, which contains printf-function
#include <Windows.h> // GetAsyncKeyState, Sleep
#include <assert.h> // "ei vara venettä kaada, eikä assert toimivaa softaa"
#include <algorithm>


class GameObject {
public:
	virtual void update(float deltaTime) = 0;
};


class Player : public GameObject {
public:
	virtual void update(float deltaTime) {
		printf("%s\n", __FUNCTION__);
	}
};


class Enemy : public GameObject {
public:
	Enemy() 
		: m_health(5) {
	}

	virtual void update(float deltaTime) {
		printf("%s\n", __FUNCTION__);
	}

	int takeDamage() {
		--m_health;
		printf("New health: %d\n", m_health);
		return m_health;
	}

private:
	int m_health;
};


class Projectile : public GameObject {
public:
	virtual void update(float deltaTime) {
		printf("%s\n", __FUNCTION__);
	}
};


class PickableItem : public GameObject {
public:
	virtual void update(float deltaTime) {
		printf("%s\n", __FUNCTION__);
	}
};

template<typename T>
T* findFirst(const std::vector<GameObject*>& sceneVector) {
	for (auto gameObject : sceneVector) {
		auto p = dynamic_cast<T*>(gameObject);
		if ( p != 0) {
			return p;
		}
	}
	return 0;
}

void removeFromScene(std::vector<GameObject*>& sceneVector, GameObject* gameObjectToRemove) {
	auto it = std::find(sceneVector.begin(), sceneVector.end(), gameObjectToRemove);
	assert(it != sceneVector.end());
	delete gameObjectToRemove;
	sceneVector.erase(it);
}


// The main function of the application
int main() {
	std::vector<GameObject*> scene;
	bool gameRunning = true;
	while (gameRunning) {
		if (GetAsyncKeyState('1')) {
			scene.push_back(new Player());
		}
		if (GetAsyncKeyState('2')) {
			scene.push_back(new Enemy());
		}
		if (GetAsyncKeyState('3')) {
			scene.push_back(new Projectile());
		}
		if (GetAsyncKeyState('4')) {
			scene.push_back(new PickableItem());
		}
		if (GetAsyncKeyState('5')) {
			auto projectile = findFirst<Projectile>(scene);
			if (projectile != 0) {
				auto enemy = findFirst<Enemy>(scene);
				if (enemy->takeDamage() == 0) {
					removeFromScene(scene, enemy);
				}
			}
		}
		if (GetAsyncKeyState('6')) {
			auto player = findFirst<Player>(scene);
			if (player != 0) {
				removeFromScene(scene, findFirst<PickableItem>(scene));
			}
		}		

		for (auto go : scene) {
			go->update(1.0f);
		}
		Sleep(1000);
		printf("End frame\n");
	}
}
