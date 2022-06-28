#pragma once

class Component
{
public:
	// Constructor
	Component(class Actor* owner, int updateOrder = 100);
	// Destructor
	virtual ~Component();
	// Update this component by delta time
	virtual void Update(float deltaTime);

	int GetUpdateOrder() const { return mUpdateOrder; }

    Actor* GetOwner() { return mOwner; }

protected:
	// Owning actor
	class Actor* mOwner;
	// Update order
	int mUpdateOrder;
};
