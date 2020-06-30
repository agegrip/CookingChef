//======================================================
// File Name	: Object.cpp
// Summary		: オブジェクトクラス
// Date			: 2019/12/02
// Author		: Takafumi Ban
//======================================================
#include "pch.h"
#include "Object.h"
#include "ObjectManager.h"
#include "GameObjectManager.h"
#include "DeviceResources.h"
#include <CommonStates.h>
#include "GameWindow.h"
#include "GameContext.h"
#include "Camera.h"

Object::Object(const std::string& tag,DirectX::Model* shadowModel)
	: GameObject(tag)
	, m_model(nullptr)
	, m_dir(0)
	, m_weight(0.0f)
	, m_coefficientOfFriction(0.0f)
	, m_radius(0.0f)
	, m_width(0.0f)
	, m_height(0.0f)
	, m_hitFlag(false)
	, m_displayFlag(true)
	, m_activeFlag(true)
	, m_x(0)
	, m_y(0)
{
	SetDrawPrio(GameWindow::DRAW_OBJECT);
	if (shadowModel != nullptr)
	{
		std::unique_ptr<Shadow> pShadow = std::make_unique<Shadow>();
		m_shadow = pShadow.get();
		m_shadow->Initialize(this, shadowModel);
		GameContext::Get<ObjectManager>()->GetGameOM()->Add(std::move(pShadow));
	}
}

Object::~Object()
{
	if (m_shadow != nullptr)
	{
		m_shadow->Destroy(this);
	}
}

void Object::Update(float elapsedTime)
{
	if (!m_activeFlag) return;

	// 位置に速度を足す
	m_position += m_vel;
}

void Object::Render()
{
	GameWindow* gameWindow = GameContext::Get<GameWindow>();
	DX::DeviceResources* deviceResources = GameContext::Get<DX::DeviceResources>();
	Camera* camera = gameWindow->GetCamera();

	if (!gameWindow || !m_model || !m_displayFlag) return;
	
	// ワールド行列を作成
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateTranslation(m_position);

	// モデルの描画
	m_model->Draw(deviceResources->GetD3DDeviceContext(), *GameContext::Get<DirectX::CommonStates>()
		, world, camera->getViewMatrix(), camera->getProjectionMatrix());
}

float Object::GetHitForce()
{
	float force = 0.1f;

	// 連続ヒット時の速度の影響が大きすぎたので速度を抑える
	float v = m_vel.Length();
	if (v >= 0.1f) v = 0.1f;
	force += m_weight * v;

	return force;
}

void Object::AddForce(float angle, float force)
{
	// 進行方向ベクトル
	DirectX::SimpleMath::Vector3 dir(0.0f, 0.0f, -1.0f);

	// 自機の向きベクトルを求める
	DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(angle);
	dir = DirectX::SimpleMath::Vector3::Transform(dir, rotY);

	// 加速度
	m_acc = dir * (force / m_weight);

	// 速度に加速度を足す
	m_vel += m_acc;
}

void Object::Friction(float elapsedTime)
{
	// 摩擦力 = 摩擦係数 × 重力加速度 × 質量
	float frictionForce = m_coefficientOfFriction * m_weight * GameWindow::GRAVITY;

	// 摩擦力によって速度を落とす
	float v = (frictionForce / m_weight) * elapsedTime;

	if (m_vel.LengthSquared() > v * v)
	{
		v = m_vel.Length() - v;
		m_vel.Normalize();
		m_vel *= v;
	}
	else
	{
		m_vel = DirectX::SimpleMath::Vector3::Zero;
	}
}

int Object::GetDir(Object * object)
{
	// ターゲットへのベクトルを求める
	DirectX::SimpleMath::Vector3 v = object->GetPosition() - m_position;

	// 内積を使い一番近い角度を求める
	DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(45.0f));
	DirectX::SimpleMath::Vector3 dirVec(0.0f, 0.0f, -1.0f);
	int dir = 0;
	float maxVal = dirVec.Dot(v);
	// 内積が一番大きい方向が近い
	for (int i = 0; i < 7; i++)
	{
		dirVec = DirectX::SimpleMath::Vector3::Transform(dirVec, rotY);
		float val = dirVec.Dot(v);
		if (val > maxVal)
		{
			dir = i + 1;
			maxVal = val;
		}
	}
	return dir;
}

bool Object::CheckFloor()
{
	if (m_floorCheckFunction)
	{
		return m_floorCheckFunction(this);
	}
	return true;
}

void Object::AddScore(int score)
{
	if (m_addScoreFunction)
	{
		m_addScoreFunction(score);
	}
}

void Object::SetCheckFloorFunction(std::function<bool(Object*)> func)
{
	m_floorCheckFunction = func;
}

void Object::SetAddScoreFunction(std::function<void(int score)> func)
{
	m_addScoreFunction = func;
}

void Object::ShadowActive(bool flag)
{
	if (m_shadow != nullptr)
	{
		m_shadow->Active(flag);
	}
}