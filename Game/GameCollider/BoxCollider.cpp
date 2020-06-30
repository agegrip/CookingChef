#include "pch.h"

#include "BoxCollider.h"
#include "GameObject.h"
#include "CollisionManager.h"


BoxCollider::BoxCollider(GameObject* owner, const DirectX::SimpleMath::Vector3& size)
	: Collider(owner)
	, m_size(size)
{
}


bool BoxCollider::IsCollided(const Collider* collider) const
{
	return collider->IsCollided(this);
}


bool BoxCollider::IsCollided(const SphereCollider* collider) const
{
	return CollisionManager::IsCollided(this, collider);
}


bool BoxCollider::IsCollided(const BoxCollider* collider) const
{
	return CollisionManager::IsCollided(this, collider);
}


void BoxCollider::SetSize(const DirectX::SimpleMath::Vector3& size)
{
	m_size = size;
}


const DirectX::SimpleMath::Vector3& BoxCollider::GetSize() const
{
	return m_size;
}
