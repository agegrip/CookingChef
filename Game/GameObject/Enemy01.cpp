#include "pch.h"
#include "Enemy01.h"
#include "GameContext.h"
#include "DeviceResources.h"
#include <CommonStates.h>
#include "GameWindow.h"
#include "CollisionManager.h"
#include "Camera.h"

// エネミーの重さ
const float Enemy01::WEIGHT = 0.8f;

// 床に対する摩擦係数
const float Enemy01::FRICTION = 0.1f;

// オブジェクト同士の判定用の半径s
const float Enemy01::RADIUS = 0.1f;

// 床との判定用の幅と高さ
const float Enemy01::WIDTH = 0.2f;
const float Enemy01::HEIGHT = 0.2f;

// エネミーの最大移動速度
const float Enemy01::MAX_SPEED = 0.08f;

// 思考時間(単位:秒)
const float Enemy01::THINK_INTERVAL = 0.04f;

Enemy01::Enemy01(const std::string& tag,DirectX::Model* shadowModel)
	:Object(tag,shadowModel)
	,m_model{nullptr}
	,m_state(STATE_NORMAL)
	,m_thinkTimer(0.0f)
{
}


Enemy01::~Enemy01()
{
}

void Enemy01::Initialize(int x, int y, DirectX::Model* model)
{
	m_x = x;
	m_y = y;
	m_model = model;

	m_radius = 0.4f;

	m_collider = std::make_unique<SphereCollider>(this, m_radius);
	// 質量を設定
	m_weight = WEIGHT;

	m_coefficientOfFriction = FRICTION;

	// 幅と高さ
	m_width = Enemy01::WIDTH;
	m_height = Enemy01::HEIGHT;

	// 位置と向きをリセット
	Reset();
}

void Enemy01::Update(float elapsedTime)
{
	// 削除リクエストがあればタスクを削除
	if (IsInvalid()) return;

	// アクティブフラグがfalseの場合は何もしない
	if (!m_activeFlag) return;

	// 状態によって処理を分岐させる
	switch (m_state)
	{
	case STATE_NORMAL:	// 通常
		State_Normal(elapsedTime);
		break;
	case STATE_HIT:		// ヒット中
		State_Hit(elapsedTime);
		break;
	case STATE_FALL:	// 落下中
		State_Fall(elapsedTime);
		break;
	case STATE_DEAD:
		return;
	default:
		break;
	}
	

	// 摩擦により速度を落とす
	Friction(elapsedTime);

	// ヒット中は速度制限をつけない
	if (m_state != STATE_HIT)
	{
		// 最大速度にならないよう調整
		if (m_vel.LengthSquared()>MAX_SPEED * MAX_SPEED)
		{
			m_vel.Normalize();
			m_vel *= MAX_SPEED;
		}
	}

	// 位置に速度を足す
	m_position += m_vel;


	// プレイヤーと床のチェック
	if (m_state == Enemy01::STATE_NORMAL && CheckFloor() == false)
	{
		// 落ちたので停止させる
		m_vel = DirectX::SimpleMath::Vector3::Zero;
		// 状態を落下中へ
		m_state = STATE_FALL;
		// 描画順を変える
		SetDrawPrio(GameWindow::DRAW_FALL);
	}

	GameContext::Get<CollisionManager>()->Add("Enemy01", m_collider.get());
}

void Enemy01::Render()
{
	GameWindow* gameWindow = GameContext::Get<GameWindow>();
	DX::DeviceResources* deviceResources = GameContext::Get<DX::DeviceResources>();
	Camera* camera = gameWindow->GetCamera();

	if (!gameWindow || !m_model || !m_displayFlag) return;

	// 向いている角度を角度テーブルから取得
	float angle = GameWindow::DIR_ANGLE[m_dir];

	// ワールド行列を作成
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateRotationY(angle) * DirectX::SimpleMath::Matrix::CreateTranslation(m_position);

	// モデルの描画（ジャンプパーツを付けているかどうかでモデルが違う）
	m_model->Draw(deviceResources->GetD3DDeviceContext(),
		*GameContext::Get<DirectX::CommonStates>(),
		world, camera->getViewMatrix(), camera->getProjectionMatrix());
}

void Enemy01::State_Normal(float elapsedTime)
{
	GameWindow* gameWindow = GameContext::Get<GameWindow>();
	// プレイヤーを取得
	Object* target = gameWindow->GetStage()->GetPlayer();

	m_thinkTimer += elapsedTime;
	if (m_thinkTimer >= THINK_INTERVAL)
	{
		m_thinkTimer = 0.0f;

		// ランダムで移動方法を設定
		m_dir = rand() % 8;

		// ターゲット方向に向ける
		if (target)
		{
			m_dir = GetDir(target);
		}

		// 力を加える
		AddForce(GameWindow::DIR_ANGLE[m_dir], 0.03f);

		// 落ちそうなら向きと速度を反転する
		DirectX::SimpleMath::Vector3 tmp = m_position; // 一時的に保存
		m_position += m_vel;
		if (CheckFloor() == false)
		{
			DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(180.0f));
			m_vel = DirectX::SimpleMath::Vector3::Transform(m_vel, rotY);
			m_dir = (m_dir + 4) % 8;
			//m_dir = (m_dir + 8 - 1) % 8;   //　マイナスの場合の処理
		}
		m_position = tmp;
	}
}

void Enemy01::State_Hit(float elapsedTime)
{
	// 摩擦により停止したら
	if (m_vel == DirectX::SimpleMath::Vector3::Zero)
	{
		m_state = STATE_NORMAL;
	}
}

void Enemy01::State_Fall(float elapsedTime)
{
	// 下に落ちる
	m_position.y -= GameWindow::FALL_SPEED * elapsedTime;

	// ある程度落下したら
	if (m_position.y < -GameWindow::FALL_DISTANCE)
	{
		// 死亡
		m_state = STATE::STATE_DEAD;
		// 表示(OFF)
		SetDisplayFlag(false);
	}
}

void Enemy01::OnCollision(GameObject * object)
{
	// 思考タイマーを初期化
	m_thinkTimer = THINK_INTERVAL;
	// 衝突したのでいったん停止させる
	m_vel = DirectX::SimpleMath::Vector3::Zero;
	// 衝突した相手へのベクトルを求めて移動方向を算出
	DirectX::SimpleMath::Vector3 v = object->GetPosition() - this->GetPosition();
	float angle = atan2(v.x, v.z);
	// 相手から受ける力を加える
	AddForce(angle, static_cast<Object*>(object)->GetHitForce());
	// 衝突状態へ
	m_state = STATE_HIT;
}

Enemy01::STATE Enemy01::GetState()
{
	return m_state;
}

void Enemy01::Move(float elapsedTime)
{
	// 加える力
	float force = 0.0f;

	// 力を加える
	AddForce(GameWindow::DIR_ANGLE[m_dir], force);
}

void Enemy01::Reset()
{
	// プレイヤーを元の状態に戻す
	m_dir = 0;
	m_position = DirectX::SimpleMath::Vector3((float)m_x, 0.0f, (float)m_y);
	if (m_state == STATE_FALL)
	{

		SetDisplayFlag(true);
	}
}
