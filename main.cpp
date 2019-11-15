#include <iostream>

#include "core/WindowManager.h"
#include "scenario/AutoPilot/AutoPilot.h"

#define WINDOW_FULLSCREEN false
#define WINDOW_X_SIZE 2560
#define WINDOW_Y_SIZE 1440

/**********************************************************************************************************************
 * Entry point
 **********************************************************************************************************************/

int main(int argc, char *argv[])
{
    WindowManager* window_manager = new WindowManager(WINDOW_X_SIZE, WINDOW_Y_SIZE, WINDOW_FULLSCREEN, glm::vec3(0, 5, 31));

    if ( ! window_manager->initialise())
    {
        std::cerr << "Failed to initialise WindowManager" << std::endl;

        delete window_manager;

        return -1;
    }

    {
        AutoPilot scenario(window_manager->getDisplayManager());
        scenario.run();
        window_manager->run();
    }

    delete window_manager;

    return 0;
}