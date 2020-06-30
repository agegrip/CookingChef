//======================================================
// File Name	: ObjectManager.cpp
// Summary		: ゲーム画面と情報画面のオブジェクトマネージャー
//				　をまとめたクラス
// Date			: 2019/11/12
// Author		: Takafumi Ban
//======================================================
#include "pch.h"
#include "ObjectManager.h"

#include "GameObjectManager.h"

ObjectManager::ObjectManager()
{
	// ゲーム画面用オブジェクトマネージャー生成
	m_pGameOM = std::make_unique<GameObjectManager>();
	// 情報画面用オブジェクトマネージャー生成
	m_pInfoOM = std::make_unique<GameObjectManager>();
}


ObjectManager::~ObjectManager()
{
}
