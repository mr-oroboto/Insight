#include "StarField.h"

#include <iostream>
#include <random>

#include "core/FrameQueue.h"
#include "scenario/StarField/Star.h"

#define NUM_STARS 100
#define MIN_Y -20
#define MAX_Y 20
#define MIN_X -20
#define MAX_X 20
#define MIN_Z -30
#define MAX_Z 30

StarField::~StarField()
{
    std::cout << "StarField::~StarField()" << std::endl;
}

void StarField::run()
{
    /**
     * 1. Create a new repeating FrameQueue
     * 2. Create a single Frame that will be reused over and over
     * 3. Place initial SceneObjects in the frame
     * 4. Register a callback to be used to update SceneObjects
     * 5. Run the FrameQueue
     */
    FrameQueue* frame_queue = new FrameQueue(display_manager_, true);
    frame_queue->setFrameRate(1);

    frame_ = frame_queue->newFrame();

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<> x_distribution(MIN_X, MAX_X);
    std::uniform_real_distribution<> y_distribution(MIN_Y, MAX_Y);
    std::uniform_real_distribution<> z_distribution(MIN_Z, MAX_Z);

    display_manager_->setCameraCoords(glm::vec3(0, 0, 30));

    for (size_t i = 0; i < NUM_STARS; i++)
    {
        glm::vec3 world_coords = glm::vec3(x_distribution(gen), y_distribution(gen), z_distribution(gen));
        Star* star = new Star(display_manager_, Primitive::Type::CUBE, world_coords, glm::vec3(1, 1, 1));
        star->setScale(0.1);
        frame_->addObject(star);
    }

    frame_queue->enqueueFrame(frame_);  // @todo we should use a shared pointer so we also retain ownership

    display_manager_->setUpdateSceneCallback(std::bind(&StarField::updateSceneCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));

    frame_queue->setReady();
    frame_queue->setActive();    // transfer ownership to DisplayManager
}

void StarField::updateSceneCallback(GLfloat secs_since_rendering_started, GLfloat secs_since_framequeue_started, GLfloat secs_since_last_renderloop, GLfloat secs_since_last_frame)
{
    frame_->updateObjects(secs_since_rendering_started, secs_since_framequeue_started, secs_since_last_renderloop, secs_since_last_frame, nullptr);
}