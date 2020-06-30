#pragma once

#include "Object.h"
#include "SphereCollider.h"

class Enemy01 : public Object
{
public:
	enum STATE
	{
		STATE_NORMAL,   // �ʏ�
		STATE_HIT,      // ������΂�����
		STATE_FALL,     // ������
		STATE_DEAD,     // ���S
	};
	// �d��
	static const float WEIGHT;
	// ���Ƃ̔���p�̕��ƍ���
	static const float WIDTH;
	static const float HEIGHT;
	// �I�u�W�F�N�g���m�̔���p�̔��a
	static const float RADIUS;
	// �ő�ړ����x
	static const float MAX_SPEED;
	// ���ɑ΂��門�C�W��
	static const float FRICTION;
	// �v�l���ԁi�P�ʁF�b�j
	static const float THINK_INTERVAL;
private:
	//
	DirectX::Model* m_model;

	//
	STATE m_state;

	// �v�l�^�C�}�[
	float m_thinkTimer;

	// �X�t�B�A�R���C�_�[
	std::unique_ptr<SphereCollider> m_collider;
public:
	// �R���X�g���N�^
	Enemy01(const std::string& tag,DirectX::Model* shadowModel);
	// �f�X�g���N�^
	~Enemy01();

	// �������֐�
	void Initialize(int x, int y, DirectX::Model* model);

	// �X�V�֐�
	void Update(float elapsedTime) override;
	
	// 
	Object::OBJECT_ID GetID() { return Enemy01::OBJECT_ID::ENEMY_1; }

	// �`��֐�
	void Render() override;

	// �X�V�����i�ʏ�j
	void State_Normal(float elapsedTime);

	// �X�V�����i�Փ˂��Đ���s�\���j
	void State_Hit(float elapsedTime);

	// �X�V�����i�������j
	void State_Fall(float elapsedTime);

	// �Փ˂�����Ă΂��֐�
	void OnCollision(GameObject* object);

	// �G�l�~�[�̏�Ԃ��擾����֐�
	Enemy01::STATE GetState();

	// �G�l�~�[�̈ړ��֐�
	void Move(float elapsedTime);

	// ���Z�b�g�֐�
	void Reset();
};

