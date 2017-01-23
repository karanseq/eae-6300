#ifndef ENGINE_GAME_OBJECT_CONTROLLER_H_
#define ENGINE_GAME_OBJECT_CONTROLLER_H_

namespace engine {
namespace gameobject {

// forward declaration
class GameObject;

/*
	InterfaceGameObjectController
	- An interface that offers handles to control a GameObject.
*/
class InterfaceGameObjectController
{
public:
	virtual ~InterfaceGameObjectController() {}

	virtual InterfaceGameObjectController* Clone() const = 0;

	virtual GameObject* GetGameObject() const = 0;
	virtual void SetGameObject(GameObject* i_game_object) = 0;

	virtual void UpdateGameObject() = 0;
}; // class InterfaceGameObjectController

} // namespace gameobject
} // namespace engine

#endif // ENGINE_GAME_OBJECT_CONTROLLER_H_
