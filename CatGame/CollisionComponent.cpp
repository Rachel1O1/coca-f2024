#include "CollisionComponent.h"
#include "Actor.h"
#include <algorithm>

CollisionComponent::CollisionComponent(class Actor* owner)
: Component(owner)
, mWidth(0.0f)
, mHeight(0.0f)
{
}

CollisionComponent::~CollisionComponent()
{
}

bool CollisionComponent::Intersect(const CollisionComponent* other) const
{
	bool noIntersection = ((GetMax().x < other->GetMin().x) || (other->GetMax().x < GetMin().x) ||
						   (GetMax().y < other->GetMin().y) || (other->GetMax().y < GetMin().y));
	return !noIntersection;
}

Vector2 CollisionComponent::GetMin() const
{
	Vector2 min;
	min.x = (mOwner->GetPosition().x - ((mWidth * mOwner->GetScale()) / 2.0f));
	min.y = (mOwner->GetPosition().y - ((mHeight * mOwner->GetScale()) / 2.0f));
	return min;
}

Vector2 CollisionComponent::GetMax() const
{
	Vector2 max;
	max.x = (mOwner->GetPosition().x + ((mWidth * mOwner->GetScale()) / 2.0f));
	max.y = (mOwner->GetPosition().y + ((mHeight * mOwner->GetScale()) / 2.0f));
	return max;
}

const Vector2& CollisionComponent::GetCenter() const
{
	return mOwner->GetPosition();
}

CollSide CollisionComponent::GetMinOverlap(const CollisionComponent* other, Vector2& offset) const
{
	offset = Vector2::Zero;
	CollSide returnMe = CollSide::None;
	if (Intersect(other))
	{
		float topDist = other->GetMin().y - GetMax().y;
		float bottomDist = other->GetMax().y - GetMin().y;
		float leftDist = other->GetMin().x - GetMax().x;
		float rightDist = other->GetMax().x - GetMin().x;

		float currentLowestDist = std::abs(topDist);
		returnMe = CollSide::Top;
		offset.y = (topDist);
		if (std::abs(bottomDist) < currentLowestDist)
		{
			returnMe = CollSide::Bottom;
			currentLowestDist = std::abs(bottomDist);
			offset.y = (bottomDist);
		}
		if (std::abs(leftDist) < currentLowestDist)
		{
			returnMe = CollSide::Left;
			currentLowestDist = std::abs(leftDist);
			offset = Vector2(leftDist, 0.0f);
		}
		if (std::abs(rightDist) < currentLowestDist)
		{
			returnMe = CollSide::Right;
			offset = Vector2(rightDist, 0.0f);
		}
	}
	return returnMe;
}
