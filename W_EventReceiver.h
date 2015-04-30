
#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

#include <irrlicht.h>

#pragma once

using namespace irr;

class W_EventReceiver : public IEventReceiver
{
public:

virtual bool OnEvent(const SEvent& event);

virtual bool IsKeyDown(EKEY_CODE keyCode) const;    
W_EventReceiver();

private:
    // We use this array to store the current state of each key
    bool KeyIsDown[KEY_KEY_CODES_COUNT];
};
