//======================================================
// File Name	: ObjectManager.h
// Summary		: ゲーム画面と情報画面のオブジェクトマネージャー
//				　をまとめたクラス
// Date			: 2019/11/12
// Author		: Takafumi Ban
//======================================================
#pragma once

class GameObjectManager;

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();

public:	// アクセッサ
		// ゲーム画面のオブジェクトマネージャー取得
	GameObjectManager* GetGameOM()
	{
		return m_pGameOM.get();
	}
	// 情報画面のオブジェクトマネージャー取得
	GameObjectManager* GetInfoOM()
	{
		return m_pInfoOM.get();
	}

private:
	// ゲームオブジェクトマネージャー（ゲーム画面）
	std::unique_ptr<GameObjectManager>      m_pGameOM;
	// ゲームオブジェクトマネージャー（情報画面）
	std::unique_ptr<GameObjectManager>      m_pInfoOM;
};

