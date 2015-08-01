#include "EventReceiver.hpp"
#include "IGameObject.hpp"

bool EventReceiver::OnEvent(const SEvent& event) {
    for (IGameObject* obs : observers[event.EventType]) {
        obs->notify(event);
    }
    return false;
}

void EventReceiver::attach (IGameObject* obs, irr::EEVENT_TYPE evt_t) {
    observers[evt_t].push_back(obs);
}

void EventReceiver::detach (IGameObject* obs, irr::EEVENT_TYPE evt_t) {
    std::vector<IGameObject*> *evt_vec = &observers[evt_t];
    auto iter = evt_vec->begin();
    for (; iter != evt_vec->end(); iter++) {
        if (*iter == obs) break;
    }
    if (iter != evt_vec->end()) {
        evt_vec->erase(iter);
    }
}
