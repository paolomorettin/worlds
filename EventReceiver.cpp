#include "EventReceiver.hpp"

bool EventReceiver::OnEvent(const SEvent& event)
{
	// Remember whether each key is down or up
	if (event.EventType == irr::EET_KEY_INPUT_EVENT)
		KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

	// This function returns true IFF the event is processed internally.
	return false;
}

// This is used to check whether a key is being held down
bool EventReceiver::IsKeyDown(EKEY_CODE keyCode) const
{
	return KeyIsDown[keyCode];
}

EventReceiver::EventReceiver()
{
	for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i) {
		KeyIsDown[i] = false;
	}
}
