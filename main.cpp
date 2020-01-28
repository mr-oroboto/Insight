#include <iostream>

#include "core/WindowManager.h"
#include "scenario/ScenarioCollection.h"
#include "scenario/HeapBreadthFirstTraversal.h"
#include "scenario/HeapDepthFirstTraversal.h"
#include "scenario/MergeSort.h"
#include "scenario/MinHeap.h"
#include "scenario/AutoPilot/AutoPilot.h"
#include "scenario/StarField/StarField.h"
#include "primitive/Tesselation.h"

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

    DisplayManager* display_manager = window_manager.getDisplayManager();

    if ( ! display_manager->getPrimitiveCollection()->addPrimitive(Primitive::Type::TESSELATION, new Tesselation(window_manager.getDisplayManager()->getObjectShader())))
    {
        std::cerr << "Failed to add Tesselation primitive" << std::endl;
        return -1;
    }

    if ( ! display_manager->getTextureCollection()->registerTexture("/home/sysop/textures/water.jpg", "water"))
    {
        std::cerr << "Failed to add water texture" << std::endl;
        return -1;
    }

    if ( ! display_manager->getTextDrawer()->registerFont(Font::Type::FONT_DEFAULT, "/home/sysop/ClionProjects/Insight/font/Vera.ttf"))
    {
        std::cerr << "Failed to register font" << std::endl;
        return -1;
    }

    scenarios.initialise(&window_manager);

    scenarios.addScenario(new AutoPilot(display_manager));
    scenarios.addScenario(new MergeSort(display_manager));
    scenarios.addScenario(new MinHeap(display_manager));
    scenarios.addScenario(new HeapDepthFirstTraversal(display_manager));
    scenarios.addScenario(new HeapBreadthFirstTraversal(display_manager));
    scenarios.addScenario(new StarField(display_manager));

    window_manager.setHandleKeystrokeCallback(std::bind(&handleKeystroke, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    scenarios.nextScenario();
    window_manager.run();

    return 0;
}

