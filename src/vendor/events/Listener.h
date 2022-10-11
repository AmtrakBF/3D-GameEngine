#pragma once
#include "events/Event.h"

class Listener
{
public:
	//! Destructor
	virtual ~Listener() {}
	//! Handles event passed to it
	virtual void HandleEvent(Event* event) {};
};