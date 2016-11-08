#ifndef GAME_OBJECT_CONTROLLER_H_
#define GAME_OBJECT_CONTROLLER_H_

namespace engine
{
// forward declaration
class GameObject;
class Vec2D;

/*
	An interface that offers handles to control a GameObject.
*/
class GameObjectController
{
public:
	virtual ~GameObjectController() {}

	virtual void SetGameObject(GameObject* game_object) = 0;
	virtual GameObject* GetGameObject() = 0;

	virtual void UpdateGameObject() = 0;
};

} // namespace engine

#endif // GAME_OBJECT_CONTROLLER_H_
