#ifndef DEBUG_PRINT_CONTROLLER_H_
#define DEBUG_PRINT_CONTROLLER_H_

namespace engine
{
/*
	An interface that offers handles to print debug information.
*/
class DebugPrintController
{
public:
	virtual ~DebugPrintController() {}

	virtual void DebugPrint() const = 0;
};

} // namespace engine

#endif // DEBUG_PRINT_CONTROLLER_H_
