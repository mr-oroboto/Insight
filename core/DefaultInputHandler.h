#ifndef INSIGHT_CORE_DEFAULTINPUT_HANDLER_H
#define INSIGHT_CORE_DEFAULTINPUT_HANDLER_H

#include <iostream>

#include "InputHandler.h"

namespace insight {

class DefaultInputHandler : public InputHandler {
public:
    ~DefaultInputHandler() { std::cout << "Destroying DefaultInputHandler" << std::endl; }

    void handleKeystroke(insight::WindowManager* window_manager, SDL_Event keystroke_event, GLfloat secs_since_last_renderloop);
    void handleMouse(insight::WindowManager* window_manager, SDL_Event mouse_event, GLfloat secs_since_last_renderloop);
};

}   // namespace insight

#endif //INSIGHT_CORE_DEFAULTINPUT_HANDLER_H
