#include "pch.h"
#include "Enemy01.h"
#include "GameContext.h"
#include "DeviceResources.h"
#include <CommonStates.h>
#include "GameWindow.h"
#include "CollisionManager.h"
#include "Camera.h"

// �G�l�~�[�̏d��
const float Enemy01::WEIGHT = 0.8f;

// ���ɑ΂��門�C�W��
const float Enemy01::FRICTION = 0.1f;

// �I�u�W�F�N�g���m�̔���p�̔��as
const float Enemy01::RADIUS = 0.1f;

// ���Ƃ̔���p�̕��ƍ���
const float Enemy01::WIDTH = 0.2f;
const float Enemy01::HEIGHT = 0.2f;

// �G�l�~�[�̍ő�ړ����x
const float Enemy01::MAX_SPEED = 0.08f;

// �v�l����(�P��:�b)
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
	// ���ʂ�ݒ�
	m_weight = WEIGHT;

	m_coefficientOfFriction = FRICTION;

	// ���ƍ���
	m_width = Enemy01::WIDTH;
	m_height = Enemy01::HEIGHT;

	// �ʒu�ƌ��������Z�b�g
	Reset();
}

void Enemy01::Update(float elapsedTime)
{
	// �폜���N�G�X�g������΃^�X�N���폜
	if (IsInvalid()) return;

	// �A�N�e�B�u�t���O��false�̏ꍇ�͉������Ȃ�
	if (!m_activeFlag) return;

	// ��Ԃɂ���ď����𕪊򂳂���
	switch (m_state)
	{
	case STATE_NORMAL:	// �ʏ�
		State_Normal(elapsedTime);
		break;
	case STATE_HIT:		// �q�b�g��
		State_Hit(elapsedTime);
		break;
	case STATE_FALL:	// ������
		State_Fall(elapsedTime);
		break;
	case STATE_DEAD:
		return;
	default:
		break;
	}
	

	// ���C�ɂ�葬�x�𗎂Ƃ�
	Friction(elapsedTime);

	// �q�b�g���͑��x���������Ȃ�
	if (m_state != STATE_HIT)
	{
		// �ő呬�x�ɂȂ�Ȃ��悤����
		if (m_vel.LengthSquared()>MAX_SPEED * MAX_SPEED)
		{
			m_vel.Normalize();
			m_vel *= MAX_SPEED;
		}
	}

	// �ʒu�ɑ��x�𑫂�
	m_position += m_vel;


	// �v���C���[�Ə��̃`�F�b�N
	if (m_state == Enemy01::STATE_NORMAL && CheckFloor() == false)
	{
		// �������̂Œ�~������
		m_vel = DirectX::SimpleMath::Vector3::Zero;
		// ��Ԃ𗎉�����
		m_state = STATE_FALL;
		// �`�揇��ς���
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

	// �����Ă���p�x���p�x�e�[�u������擾
	float angle = GameWindow::DIR_ANGLE[m_dir];

	// ���[���h�s����쐬
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateRotationY(angle) * DirectX::SimpleMath::Matrix::CreateTranslation(m_position);

	// ���f���̕`��i�W�����v�p�[�c��t���Ă��邩�ǂ����Ń��f�����Ⴄ�j
	m_model->Draw(deviceResources->GetD3DDeviceContext(),
		*GameContext::Get<DirectX::CommonStates>(),
		world, camera->getViewMatrix(), camera->getProjectionMatrix());
}

void Enemy01::State_Normal(float elapsedTime)
{
	GameWindow* gameWindow = GameContext::Get<GameWindow>();
	// �v���C���[���擾
	Object* target = gameWindow->GetStage()->GetPlayer();

	m_thinkTimer += elapsedTime;
	if (m_thinkTimer >= THINK_INTERVAL)
	{
		m_thinkTimer = 0.0f;

		// �����_���ňړ����@��ݒ�
		m_dir = rand() % 8;

		// �^�[�Q�b�g�����Ɍ�����
		if (target)
		{
			m_dir = GetDir(target);
		}

		// �͂�������
		AddForce(GameWindow::DIR_ANGLE[m_dir], 0.03f);

		// ���������Ȃ�����Ƒ��x�𔽓]����
		DirectX::SimpleMath::Vector3 tmp = m_position; // �ꎞ�I�ɕۑ�
		m_position += m_vel;
		if (CheckFloor() == false)
		{
			DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(180.0f));
			m_vel = DirectX::SimpleMath::Vector3::Transform(m_vel, rotY);
			m_dir = (m_dir + 4) % 8;
			//m_dir = (m_dir + 8 - 1) % 8;   //�@�}�C�i�X�̏ꍇ�̏���
		}
		m_position = tmp;
	}
}

void Enemy01::State_Hit(float elapsedTime)
{
	// ���C�ɂ���~������
	if (m_vel == DirectX::SimpleMath::Vector3::Zero)
	{
		m_state = STATE_NORMAL;
	}
}

void Enemy01::State_Fall(float elapsedTime)
{
	// ���ɗ�����
	m_position.y -= GameWindow::FALL_SPEED * elapsedTime;

	// ������x����������
	if (m_position.y < -GameWindow::FALL_DISTANCE)
	{
		// ���S
		m_state = STATE::STATE_DEAD;
		// �\��(OFF)
		SetDisplayFlag(false);
	}
}

void Enemy01::OnCollision(GameObject * object)
{
	// �v�l�^�C�}�[��������
	m_thinkTimer = THINK_INTERVAL;
	// �Փ˂����̂ł��������~������
	m_vel = DirectX::SimpleMath::Vector3::Zero;
	// �Փ˂�������ւ̃x�N�g�������߂Ĉړ��������Z�o
	DirectX::SimpleMath::Vector3 v = object->GetPosition() - this->GetPosition();
	float angle = atan2(v.x, v.z);
	// ���肩��󂯂�͂�������
	AddForce(angle, static_cast<Object*>(object)->GetHitForce());
	// �Փˏ�Ԃ�
	m_state = STATE_HIT;
}

Enemy01::STATE Enemy01::GetState()
{
	return m_state;
}

void Enemy01::Move(float elapsedTime)
{
	// �������
	float force = 0.0f;

	// �͂�������
	AddForce(GameWindow::DIR_ANGLE[m_dir], force);
}

void Enemy01::Reset()
{
	// �v���C���[�����̏�Ԃɖ߂�
	m_dir = 0;
	m_position = DirectX::SimpleMath::Vector3((float)m_x, 0.0f, (float)m_y);
	if (m_state == STATE_FALL)
	{

		SetDisplayFlag(true);
	}
}
