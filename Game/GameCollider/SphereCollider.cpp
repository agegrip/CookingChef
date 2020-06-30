#include "pch.h"

#include "SphereCollider.h"
#include "GameObject.h"
#include "CollisionManager.h"



SphereCollider::SphereCollider(GameObject* owner, float radius)
	: Collider(owner)
	, m_radius(radius)
{
}


void SphereCollider::SetRadius(float radius)
{
	m_radius = radius;
}


float SphereCollider::GetRadius() const
{
	return m_radius;
}


bool SphereCollider::IsCollided(const Collider* collider) const
{
	return collider->IsCollided(this);
}


bool SphereCollider::IsCollided(const SphereCollider* collider) const
{
	return CollisionManager::IsCollided(this, collider);
}


bool SphereCollider::IsCollided(const BoxCollider* collider) const
{
	return CollisionManager::IsCollided(this, collider);
}
