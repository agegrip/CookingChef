#pragma once

#include "Object.h"
#include "SphereCollider.h"

class Enemy01 : public Object
{
public:
	enum STATE
	{
		STATE_NORMAL,   // 通常
		STATE_HIT,      // 吹き飛ばされ状態
		STATE_FALL,     // 落下中
		STATE_DEAD,     // 死亡
	};
	// 重さ
	static const float WEIGHT;
	// 床との判定用の幅と高さ
	static const float WIDTH;
	static const float HEIGHT;
	// オブジェクト同士の判定用の半径
	static const float RADIUS;
	// 最大移動速度
	static const float MAX_SPEED;
	// 床に対する摩擦係数
	static const float FRICTION;
	// 思考時間（単位：秒）
	static const float THINK_INTERVAL;
private:
	//
	DirectX::Model* m_model;

	//
	STATE m_state;

	// 思考タイマー
	float m_thinkTimer;

	// スフィアコライダー
	std::unique_ptr<SphereCollider> m_collider;
public:
	// コンストラクタ
	Enemy01(const std::string& tag,DirectX::Model* shadowModel);
	// デストラクタ
	~Enemy01();

	// 初期化関数
	void Initialize(int x, int y, DirectX::Model* model);

	// 更新関数
	void Update(float elapsedTime) override;
	
	// 
	Object::OBJECT_ID GetID() { return Enemy01::OBJECT_ID::ENEMY_1; }

	// 描画関数
	void Render() override;

	// 更新処理（通常）
	void State_Normal(float elapsedTime);

	// 更新処理（衝突して制御不能中）
	void State_Hit(float elapsedTime);

	// 更新処理（落下中）
	void State_Fall(float elapsedTime);

	// 衝突したら呼ばれる関数
	void OnCollision(GameObject* object);

	// エネミーの状態を取得する関数
	Enemy01::STATE GetState();

	// エネミーの移動関数
	void Move(float elapsedTime);

	// リセット関数
	void Reset();
};

