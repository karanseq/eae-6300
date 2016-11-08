#ifndef NAME_CONTROLLER_H_
#define NAME_CONTROLLER_H_

namespace engine
{
/*
	An interface that offers handles to name a GameObject.
*/
class NameController
{
public:
	virtual ~NameController() {}

	virtual void SetName(const char* name) = 0;
	virtual const char* GetName() const = 0;
};

} // namespace engine

#endif // NAME_CONTROLLER_H_
