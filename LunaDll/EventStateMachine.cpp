#include <string>
#include "Globals.h"
#include "EventStateMachine.h"
#include "Misc\RuntimeHook.h"

// Global instance
EventStateMachine g_EventHandler;

// Helper function
template <typename... Ts>
static inline void sendSimpleLuaEvent(const std::string& eventName, Ts&&... args) {
    if (gLunaLua.isValid()) {
        std::shared_ptr<Event> inputEvent = std::make_shared<Event>(eventName, false);
        inputEvent->setDirectEventName(eventName);
        inputEvent->setLoopable(false);
        gLunaLua.callEvent(inputEvent, std::forward<Ts>(args)...);
    }
}

static inline void ProcessWindowsMessageQueue(void) {
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

// Public methods (Notifications of State)
void EventStateMachine::reset(void) {
    m_onTickReady = false;
    m_onTickEndReady = false;
    m_onDrawEndReady = false;
    m_RequestPause = false;
    m_RequestUnpause = false;
    m_IsPaused = false;
}

void EventStateMachine::hookLevelLoop(void) {
    // Check if we should pause
    checkPause();

    sendOnLoop();
}

void EventStateMachine::hookWorldLoop(void) {
    // Check if we should pause
    checkPause();

    sendOnLoop();
}

void EventStateMachine::hookInputUpdate(void) {
    sendOnInputUpdate();

    if (m_onTickReady) {
        sendOnTick();
    }
}

void EventStateMachine::hookLevelRenderStart(void) {
    if (m_onTickEndReady) {
        sendOnTickEnd();
    }

    sendOnDraw();
}
void EventStateMachine::hookLevelRenderEnd(void) {
    if (m_onDrawEndReady) {
        sendOnDrawEnd();
    }
}

void EventStateMachine::hookWorldRenderStart(void) {
    if (m_onTickEndReady) {
        sendOnTickEnd();
    }

    sendOnDraw();
}
void EventStateMachine::hookWorldRenderEnd(void) {
    if (m_onDrawEndReady) {
        sendOnDrawEnd();
    }
}


// Private methods (Outgoing events)
void EventStateMachine::sendOnLoop(void) {
    // We're ready for onTick after any onLoop
    m_onTickReady = true;

    // TODO: Consider moving onStart handling into this class
    gLunaLua.doEvents();
}

void EventStateMachine::sendOnInputUpdate(void) {
    sendSimpleLuaEvent("onInputUpdate");
}

void EventStateMachine::sendOnTick(void) {
    m_onTickReady = false;

    sendSimpleLuaEvent("onTick");

    m_onTickEndReady = true;
}

void EventStateMachine::sendOnTickEnd(void) {
    m_onTickEndReady = false;

    sendSimpleLuaEvent("onTickEnd");
}

void EventStateMachine::sendOnDraw(void) {
    sendSimpleLuaEvent("onDraw");

    m_onDrawEndReady = true;
}

void EventStateMachine::sendOnDrawEnd(void) {
    m_onDrawEndReady = false;

    sendSimpleLuaEvent("onDrawEnd");
}

// Public methods (pause requests)
void EventStateMachine::requestPause(void) {
    if (!m_IsPaused) {
        m_RequestPause = true;
    }
    m_RequestUnpause = false;
}

void EventStateMachine::requestUnpause(void) {
    if (m_IsPaused) {
        m_RequestUnpause = true;
    }
    m_RequestPause = false;
}

bool EventStateMachine::isPaused(void) {
    return m_IsPaused;
}

// Paused game logic
void EventStateMachine::checkPause(void) {
    if (!m_IsPaused && m_RequestPause) {
        m_RequestPause = false;
        runPause();
    }
}

void EventStateMachine::runPause(void) {
    m_IsPaused = true;
    while (!m_RequestUnpause) {
        native_updateInput();

        // Render the world
        if (gIsOverworld) {
            // TODO: Is there any animation we want to update in the overworld?
            native_renderWorld();
        } else {
            native_updateBlockAnim();
            native_renderLevel();
        }

        // Audio management...
        native_audioManagement();

        ProcessWindowsMessageQueue();

        if (gIsWindowsVistaOrNewer) {
            FrameTimingMaxFPSHookQPC();
        } else {
            FrameTimingMaxFPSHook();
        }

        ProcessWindowsMessageQueue();
    }
    m_RequestUnpause = false;
    m_IsPaused = false;
}