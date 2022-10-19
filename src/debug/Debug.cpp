#include "debug/Debug.h"

Debug* Debug::Instance()
{
	static Debug instance;
	return &instance;
}