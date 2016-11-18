#ifndef ENGINE_GAME_OBJECT_CONTROLLER_H_
#define ENGINE_GAME_OBJECT_CONTROLLER_H_

namespace engine
{
// forward declaration
class GameObject;
class Vec2D;

/*
	An interface that offers handles to control a GameObject.
*/
class InterfaceGameObjectController
{
public:
	virtual ~InterfaceGameObjectController() {}

	virtual GameObject* GetGameObject() = 0;
	virtual void SetGameObject(GameObject* game_object) = 0;

	virtual void UpdateGameObject() = 0;
};

} // namespace engine

#endif // ENGINE_GAME_OBJECT_CONTROLLER_H_