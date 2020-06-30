#pragma once


#include <SimpleMath.h>


class GameObject;
class SphereCollider;
class BoxCollider;


class Collider
{
	protected:
		GameObject*                  m_owner;
		DirectX::SimpleMath::Vector3 m_offset;


	public:
		Collider(GameObject* owner);

	public:
		virtual ~Collider() = default;


	public:
		virtual bool IsCollided(const Collider*       collider) const = 0;
		virtual bool IsCollided(const SphereCollider* collider) const;
		virtual bool IsCollided(const BoxCollider*    collider) const;


	public:
		void OnCollision(const Collider* object) const;

		const DirectX::SimpleMath::Vector3 GetPosition() const;
		const DirectX::SimpleMath::Vector3 GetOffset() const;

		void  SetOffset(const DirectX::SimpleMath::Vector3& offset);
};
