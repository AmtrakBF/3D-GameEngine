#pragma once
#include <string>
#define EVENTID std::string


class Event
{
private:
	//! Members
	EVENTID m_EventId;
	void* m_Paramater;
public:
	//! Constructor
	Event(EVENTID eventId, void* paramater = 0) : m_EventId(eventId), m_Paramater(paramater) {}

	// Accessors
	inline EVENTID EventId() const { return m_EventId; }
	inline void* Paramater() { return m_Paramater; }
};