//======================================================
// File Name	: Parts.h
// Summary		: プレイヤークラス
// Date			: 2019/12/10
// Author		: Takafumi Ban
//======================================================
#pragma once

#include "Object.h"
#include "CollisionManager.h"

class GameWindow;

class Parts : public Object
{
public:
	// パーツの種類
	enum Kind
	{
		NONE,		// 未指定
		POWERUP,	// パワーアップ
		JUMP,		// ジャンプ

		KIND_NUM
	};

	// パーツの状態
	enum STATE
	{
		STATE_NORMAL,	// 通常
		STATE_FALL,		// 落下中
		STATE_DEAD,		// 死亡
	};

	enum Model
	{
		POWERUP_MODEL,
		JUMP_MODEL,

		MODEL_NUM
	};

	// 床との判定用の幅と高さ
	static const float WIDTH;
	static const float HEIGHT;

private:
	// パーツの種類
	Kind m_kind;

	// パーツの状態
	STATE m_state;
	std::unique_ptr<SphereCollider> m_collider;
public:
	// コンストラクタ
	Parts(const std::string& tag,DirectX::Model* shadowModel);

	// 初期化関数
	void Initialize(Kind kind, int x, int y, DirectX::Model* model);

	// 更新関数
	void Update(float elapsedTime) override;

	// オブジェクトＩＤを取得する関数
	Object::OBJECT_ID GetID() override;

	// パーツの種類を取得する関数
	Kind GetPartsKind() { return m_kind; }

	// 衝突したら呼ばれる関数
	void OnCollision(GameObject* object);

	// リセット関数
	void Reset();

	// 更新処理（落下中）
	void State_Fall(float elapsedTime);
};
