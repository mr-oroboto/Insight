#include <argp.h>

#include <iostream>

#include "Insight.h"
#include "core/GameMovementInputHandler.h"
#include "scenario/HeapBreadthFirstTraversal.h"
#include "scenario/HeapDepthFirstTraversal.h"
#include "scenario/MergeSort.h"
#include "scenario/MinHeap.h"
#include "scenario/AutoPilot/AutoPilot.h"
#include "scenario/StarField/StarField.h"
#include "scenario/Lorentz.h"

#define WINDOW_FULLSCREEN false
#define WINDOW_X_SIZE 950
#define WINDOW_Y_SIZE 600

insight::scenario::ScenarioCollection scenarios;

/**********************************************************************************************************************
 * Command-line argument parsing
 **********************************************************************************************************************/

std::string font_path = "/usr/share/fonts/truetype/ttf-bitstream-vera";
std::string texture_path;

static error_t parse(int key, char *arg)
{
    switch (key)
    {
        case 'f':
            font_path = std::string(arg);
            break;
        case 't':
            texture_path = std::string(arg);
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

static error_t parse_argument(int key, char *arg, struct argp_state* state)
{
    return parse(key, arg);
}

static argp_option options[] = {
        {"font_path", 'f', "STRING", 0, "Full path (excluding trailing slash) to TTF fonts", 0},
        {"texture_path", 't', "STRING", 0, "Full path (excluding trailing slash) to texture files", 0},
        0
};

static argp parser = {
        options,
        parse_argument,
        0,
        0
};

/**********************************************************************************************************************
 * Entry point
 **********************************************************************************************************************/

int main(int argc, char *argv[])
{
    insight::GameMovementInputHandler* game_movement_input_handler = new insight::GameMovementInputHandler();

    argp_parse(&parser, argc, argv, 0, 0, NULL);

    insight::WindowManager window_manager(WINDOW_X_SIZE, WINDOW_Y_SIZE, WINDOW_FULLSCREEN);
    if ( ! window_manager.initialise())
    {
        std::cerr << "Failed to initialise WindowManager" << std::endl;
        return -1;
    }

    insight::DisplayManager* display_manager = window_manager.getDisplayManager();
    if ( ! display_manager->getPrimitiveCollection()->addPrimitive(insight::primitive::Primitive::Type::TESSELATION,
                                                                   new insight::primitive::Tesselation(window_manager.getDisplayManager()->getObjectShader())))
    {
        std::cerr << "Failed to add Tesselation primitive" << std::endl;
        return -1;
    }

    if ( ! display_manager->registerTexture(texture_path.append("/water.jpg"), "water"))
    {
        std::cerr << "Failed to add water texture" << std::endl;
    }

    if ( ! display_manager->registerFont(insight::Font::Type::FONT_DEFAULT, font_path.append("/Vera.ttf")))
    {
        std::cerr << "Failed to register font" << std::endl;
        return -1;
    }

    window_manager.pushInputHandler(game_movement_input_handler);

    scenarios.initialise(&window_manager);
    scenarios.addScenario(new insight::scenario::Lorentz(display_manager));
//    scenarios.addScenario(new insight::scenario::AutoPilot(display_manager));
    scenarios.addScenario(new insight::scenario::MergeSort(display_manager));
    scenarios.addScenario(new insight::scenario::MinHeap(display_manager));
    scenarios.addScenario(new insight::scenario::HeapDepthFirstTraversal(display_manager));
    scenarios.addScenario(new insight::scenario::HeapBreadthFirstTraversal(display_manager));
//    scenarios.addScenario(new insight::scenario::StarField(display_manager));

    scenarios.nextScenario();
    window_manager.run();

    return 0;
}

