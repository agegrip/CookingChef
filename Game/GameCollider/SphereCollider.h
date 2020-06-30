#pragma once


#include "Collider.h"



class SphereCollider : public Collider
{
	private:
		float m_radius;


	public:
		SphereCollider(GameObject* owner, float radius = 1.0f);

	public:
		~SphereCollider() = default;


	public:
		bool IsCollided(const Collider*       collider) const override;
		bool IsCollided(const SphereCollider* collider) const override;
		bool IsCollided(const BoxCollider*    collider) const override;


	public:
		float GetRadius() const;
		void  SetRadius(float radius);
};
