#include "events/EventSystem.h"

bool EventSystem::AlreadyRegistered(EVENTID eventId, Listener* client)
{
	bool alreadyRegistered = false;

	//! Range iterator
	std::pair<std::multimap<EVENTID, Listener*>::iterator, 
		std::multimap<EVENTID, Listener*>::iterator> range;

	//! Find all the clients able to receive this event
	range = database.equal_range(eventId);

	//! Go through list of clients
	for (std::multimap<EVENTID, Listener*>::iterator iter = range.first; iter != range.second; iter++)
	{
		if (iter->second == client)
		{
			alreadyRegistered = true;
			break;
		}
	}

	return alreadyRegistered;
}

void EventSystem::DispatchEvent(Event* event)
{
	//! Range iterator
	std::pair<std::multimap<EVENTID, Listener*>::iterator,
		std::multimap<EVENTID, Listener*>::iterator> range;

	//! Get all clients who respond to this event
	range = database.equal_range(event->EventId());

	//! For every client
	for (std::multimap<EVENTID, Listener*>::iterator iter = range.first; iter != range.second; iter++)
	{
		//! Handle event
		iter->second->HandleEvent(event);
	}
}

EventSystem* EventSystem::Instance()
{
	static EventSystem instance;
	return &instance;
}

void EventSystem::RegisterClient(EVENTID event, Listener* client)
{
	//! If client is null or already registered
	if (!client || AlreadyRegistered(event, client))
		return;

	//! Insert client into the database
	database.insert(std::make_pair(event, client));
}

void EventSystem::UnregisterClient(EVENTID event, Listener* client)
{
	//! Range iterator
	std::pair<std::multimap<EVENTID, Listener*>::iterator,
		std::multimap<EVENTID, Listener*>::iterator> range;

	//! Find all clients able to receive this event
	range = database.equal_range(event);

	//! For every client that can respond to this event
	for (std::multimap<EVENTID, Listener*>::iterator iter = range.first; iter != range.second; iter++)
	{
		if (iter->second == client)
		{
			//! Erase and quit
			iter = database.erase(iter);
			break;
		}
	}
}

void EventSystem::UnregisterAll(Listener* client)
{
	//! Iterator
	std::multimap<EVENTID, Listener*>::iterator iter = database.begin();

	//! For every entry in database
	while (iter != database.end())
	{
		if (iter->second == client)
			iter = database.erase(iter);
		else
			iter++;
	}
}

void EventSystem::SendEvent(EVENTID event, void* data /*= 0*/)
{
	//! Queue up new event
	currentEvents.push_back({ event, data });
}

void EventSystem::ProcessEvents()
{
	//! For every event
	while (currentEvents.size())
	{
		DispatchEvent(&currentEvents.front());
		currentEvents.pop_front();
	}
}

void EventSystem::ClearEvents()
{
	currentEvents.clear();
}

void EventSystem::Shutdown()
{
	database.clear();
	currentEvents.clear();
}

