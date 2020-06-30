#include "pch.h"

#include "GameObjectManager.h"

#include "GameObject.h"



GameObjectManager::GameObjectManager()
	: m_objects()
	, m_objectQueue()
{
}



GameObjectManager::~GameObjectManager()
{
}



void GameObjectManager::Update(float elapsedTime)
{
	DestroyObjects();


	if (!m_objectQueue.empty())
	{
		AcceptObjects();
	}


	UpdateObjects(elapsedTime);
}



void GameObjectManager::Render()
{
	// 描画順反映パターン
	std::list<GameObject*> pObjects;
	GameObjectList::iterator it  = m_objects.begin();
	GameObjectList::iterator end = m_objects.end();
	while (it != end)
	{
		pObjects.push_back((*it).get());
		++it;
	}
	pObjects.sort([](const GameObject* a, const GameObject* b)
	{
		return a->GetDrawPrio() > b->GetDrawPrio();
	});
	for (GameObject* object : pObjects)
	{
		object->Render();
	}
}



void GameObjectManager::Add(GameObjectPtr&& object)
{
	m_objectQueue.push_back(std::move(object));
}



void GameObjectManager::UpdateObjects(float elapsedTime)
{
	// 実装1 範囲for文
	for (GameObjectPtr& object : m_objects)
	{
		object->Update(elapsedTime);
	}


	// 実装2 イテレータ
	//GameObjectList::iterator it  = m_objects.begin();
	//GameObjectList::iterator end = m_objects.end();
	//while (it != end)
	//{
	//	(*it)->Update(elapsedTime);
	//	++it;
	//}


	// 実装3 ラムダ式
	//std::for_each(
	//	m_objects.begin(),
	//	m_objects.end(),
	//	[&](GameObjectPtr& object) { object->Update(elapsedTime); }
	//);
}



void GameObjectManager::AcceptObjects()
{
	// 実装1 範囲for文
	//for (GameObjectPtr& object : m_objectQueue)
	//{
	//	m_objects.push_back(std::move(object));
	//}
	//m_objectQueue.clear();


	// 実装2 イテレータ
	//GameObjectList::iterator it  = m_objectQueue.begin();
	//GameObjectList::iterator end = m_objectQueue.end();
	//while (it != end)
	//{
	//	m_objects.push_back(std::move(*it));
	//	++it;
	//}
	//m_objectQueue.clear();


	// 実装3 要素の移動
	m_objects.splice(m_objects.end(), m_objectQueue);
}



void GameObjectManager::DestroyObjects()
{
	// 実装1 イテレータ
	//GameObjectList::iterator it  = m_objects.begin();
	//while (it != m_objects.end())
	//{
	//	if ((*it)->IsInvalid())
	//	{
	//		it = m_objects.erase(it);
	//	}
	//	else
	//	{
	//		++it;
	//	}
	//}


	// 実装2 条件一致する全ての要素を削除
	m_objects.remove_if(std::mem_fn(&GameObject::IsInvalid));


	// 実装3 ラムダ式
	//m_objects.remove_if([](const GameObjectPtr& object) { return object->IsInvalid(); });
}
