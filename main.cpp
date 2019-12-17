#include <iostream>

#include "core/WindowManager.h"
#include "scenario/ScenarioCollection.h"

#define WINDOW_FULLSCREEN false
#define WINDOW_X_SIZE 2560
#define WINDOW_Y_SIZE 1440

/**********************************************************************************************************************
 * Entry point
 **********************************************************************************************************************/

ScenarioCollection scenarios;

bool handleKeystroke(WindowManager* window_manager, SDL_Event keystroke_event, GLfloat secs_since_last_renderloop)
{
    bool continue_processing_keystrokes = true;

    if (keystroke_event.type == SDL_KEYDOWN)
    {
        if (keystroke_event.key.keysym.sym == SDLK_n)
        {
            scenarios.nextScenario();
        }
    }

    return continue_processing_keystrokes;
}

int main(int argc, char *argv[])
{
    WindowManager window_manager(WINDOW_X_SIZE, WINDOW_Y_SIZE, WINDOW_FULLSCREEN, glm::vec3(0, 5, 31));

    if ( ! window_manager.initialise())
    {
        std::cerr << "Failed to initialise WindowManager" << std::endl;
        return -1;
    }

    scenarios.initialise(&window_manager);
    window_manager.setHandleKeystrokeCallback(std::bind(&handleKeystroke, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    scenarios.nextScenario();
    window_manager.run();

    return 0;
}

