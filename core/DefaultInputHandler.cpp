#include "DefaultInputHandler.h"

#include "WindowManager.h"

namespace insight {

void DefaultInputHandler::handleKeystroke(insight::WindowManager *window_manager, SDL_Event keystroke_event, GLfloat secs_since_last_renderloop)
{
    if (keystroke_event.type == SDL_KEYUP)
    {
        if (keystroke_event.key.keysym.sym == SDLK_q)
        {
            window_manager->stop();
            return;
        }
        else if (keystroke_event.key.keysym.sym == SDLK_l)
        {
            window_manager->getDisplayManager()->toggleLighting();
        }
    }
}

void DefaultInputHandler::handleMouse(insight::WindowManager *window_manager, SDL_Event mouse_event, GLfloat secs_since_last_renderloop)
{
}

}   // namespace insight