#ifndef MOVEMENT_CONTROLLER_H_
#define MOVEMENT_CONTROLLER_H_

namespace engine
{
/*
	An interface that offers handles to move a GameObject.
*/
class MovementController
{
public:
	enum class MoveDirection2D
	{
		MoveDirectionLeft = 0,
		MoveDirectionRight,
		MoveDirectionUp,
		MoveDirectionDown
	};

	virtual ~MovementController() {}

	virtual void Move(MoveDirection2D direction) = 0;
};

} // namespace engine

#endif // MOVEMENT_CONTROLLER_H_
