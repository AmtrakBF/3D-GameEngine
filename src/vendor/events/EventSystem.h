#pragma once
#include <list>
#include <map>

#include "events/Event.h"
#include "events/Listener.h"

class EventSystem
{
private:
	//! Database of clients and their events
	std::multimap<EVENTID, Listener*> database;

	//! List of events to be processed
	std::list<Event> currentEvents;

	//! Helper methods
	bool AlreadyRegistered(EVENTID eventId, Listener* client);
	void DispatchEvent(Event* event);

	//! Singleton, deny access to constructor/destructor
	EventSystem() {}
	~EventSystem() { Shutdown(); }
	EventSystem(const EventSystem& rhs) {}
	EventSystem& operator = (const EventSystem& rhs) {}

public:
	//! Returns an instance of the Event System
	static EventSystem* Instance();

	//! Registers a client to start listening to an event
	void RegisterClient(EVENTID event, Listener* client);

	//! Unregisters a client from a specific event
	void UnregisterClient(EVENTID event, Listener* client);

	//! Unregisters a client from all events
	void UnregisterAll(Listener* client);

	//! Send an event
	void SendEvent(EVENTID event, void* data = 0);

	//! Process all events
	void ProcessEvents();

	//! Clear all events
	void ClearEvents();

	//! Shutdown Event System
	void Shutdown();
};