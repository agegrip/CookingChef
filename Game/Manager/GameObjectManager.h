#pragma once

#include <list>
#include <memory>
#include <SimpleMath.h>



class GameObject;



class GameObjectManager final
{
	using GameObjectPtr  = std::unique_ptr<GameObject>;
	using GameObjectList = std::list<GameObjectPtr>;

	private:
		GameObjectList m_objects;
		GameObjectList m_objectQueue;


	public:
		GameObjectManager();

	public:
		~GameObjectManager();

	public:
		void Update(float elapsedTime);
		void Render();
		void Add(GameObjectPtr&& object);

	private:
		void UpdateObjects(float elapsedTime);
		void AcceptObjects();
		void DestroyObjects();
};
