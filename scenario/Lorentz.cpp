#include "Lorentz.h"

#include <cmath>
#include <iostream>

#include "core/DisplayManager.h"

namespace insight {
namespace scenario {

void Lorentz::run()
{
    display_manager_->setUpdateSceneCallback(nullptr);
    display_manager_->setCameraCoords(glm::vec3(0, 0, 30));
    display_manager_->setPerspective(1.0, 150.0, 60.0f);

    std::unique_ptr<FrameQueue> frame_queue = std::make_unique<FrameQueue>(display_manager_, true);
    frame_queue->setFrameRate(1000);

    glm::vec3 current_coordinates(1.0, 1.0, 1.0);
    glm::vec3 current_colour(1, 1, 1);
    int frame_count = 0;

    std::cout << "sigma: " << sigma_ << " beta: " << beta_ << " rho: " << rho_ << std::endl;

    std::shared_ptr<Frame> frame = frame_queue->newFrame();

    for (float t = 0; t < 100.0f; t += 0.01, frame_count++)
    {
//      current_colour = current_coordinates;
//      current_colour.r -= t;
        current_colour.b = t;

        frame->addObject(primitive::Primitive::Type::CUBE, current_coordinates, current_colour, "slate", 0.1);
        frame_queue->enqueueFrame(frame);

        glm::vec3 derivatives = solveDerivatives(current_coordinates);
        current_coordinates.x += (derivatives.x * 0.01);
        current_coordinates.y += (derivatives.y * 0.01);
        current_coordinates.z += (derivatives.z * 0.01);

//      std::cout << "(" << current_coordinates.x << ", " << current_coordinates.y << ", " << current_coordinates.z << ")" << std::endl;

        frame = frame->clone();
    }

    std::cout << "Enqueued " << frame_count << " frames" << std::endl;

    display_manager_->setUpdateSceneCallback(std::bind(&Lorentz::updateSceneCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));

    frame_queue->setReady();
    if (frame_queue->setActive())
    {
        // transfer ownership to DisplayManager
        display_manager_->setFrameQueue(std::move(frame_queue));
    }
}

glm::vec3 Lorentz::solveDerivatives(const glm::vec3& current_coordinates)
{
    glm::vec3 derivatives;

    derivatives.x = sigma_ * (current_coordinates.y - current_coordinates.x);
    derivatives.y = (current_coordinates.x * (rho_ - current_coordinates.z)) - current_coordinates.y;
    derivatives.z = (current_coordinates.x * current_coordinates.y) - (beta_ * current_coordinates.z);

    return derivatives;
}

void Lorentz::updateSceneCallback(GLfloat secs_since_rendering_started, GLfloat secs_since_framequeue_started, GLfloat secs_since_last_renderloop, GLfloat secs_since_last_frame)
{
    float camera_z = 1.0 + (80.0 * cos(secs_since_rendering_started * 0.1 * M_PI));
    float camera_x = 1.0 + (80.0 * sin(secs_since_rendering_started * 0.1 * M_PI));

    display_manager_->setCameraCoords(glm::vec3(camera_x, 0, camera_z));
    display_manager_->setCameraPointingVector(glm::vec3(-camera_x, 0, -camera_z));
}

void Lorentz::handleKeystroke(insight::WindowManager* window_manager, SDL_Event keystroke_event, GLfloat secs_since_last_renderloop)
{
    if (keystroke_event.type == SDL_KEYDOWN)
    {
       if (keystroke_event.key.keysym.sym == SDLK_q)
       {
           sigma_ -= 0.1;
           run();
       }
       else if (keystroke_event.key.keysym.sym == SDLK_w)
       {
            sigma_ += 0.1;
            run();
       }
       else if (keystroke_event.key.keysym.sym == SDLK_a)
       {
            beta_ -= 0.1;
            run();
       }
       else if (keystroke_event.key.keysym.sym == SDLK_s)
       {
            beta_ += 0.1;
            run();
       }
       else if (keystroke_event.key.keysym.sym == SDLK_z)
       {
            rho_ -= 0.1;
            run();
       }
       else if (keystroke_event.key.keysym.sym == SDLK_x)
       {
            rho_ += 0.1;
            run();
       }
    }
}

}   // namespace scenario
}   // namespace insight