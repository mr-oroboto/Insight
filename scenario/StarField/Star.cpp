#include "Star.h"

#include <iostream>

#include "core/SceneObject.h"

Star::Star(DisplayManager* display_manager, Primitive::Type type, const glm::vec3& world_coords, const glm::vec3& colour) : SceneObject(display_manager, type, world_coords, colour)
{
    setForwardUnitsPerSec(20.0f);
    setNearAndFarPlane(30.0f, -30.0f);
    setStarColour(Star::Colour::WHITE);
}

Star::~Star()
{
}

void Star::setNearAndFarPlane(GLfloat near, GLfloat far)
{
    fov_near_z_ = near;
    fov_far_z_ = far;
}

void Star::setForwardUnitsPerSec(GLfloat forward_units_per_sec)
{
    forward_units_per_sec_ = forward_units_per_sec;
}

void Star::setStarColour(Star::Colour colour)
{
    star_colour_ = colour;

    switch (star_colour_)
    {
        case Colour::RED:
            setColour(glm::vec3(1, 0, 0));
            break;
        case Colour::BLUE:
            setColour(glm::vec3(0, 0, 1));
            break;
        case Colour::WHITE:
        default:
            setColour(glm::vec3(1, 1, 1));
            break;
    }
}

void Star::update(GLfloat secs_since_rendering_started, GLfloat secs_since_framequeue_started, GLfloat secs_since_last_renderloop, GLfloat secs_since_last_frame, void* context)
{
    world_coords_.z += (forward_units_per_sec_ * secs_since_last_renderloop);

    if (world_coords_.z > fov_near_z_)
    {
        world_coords_.z = fov_far_z_;
    }

    GLfloat span = fov_near_z_ - fov_far_z_;
    GLfloat distance_from_camera_ = fov_near_z_ - world_coords_.z;

    switch (star_colour_)
    {
        case Colour::RED:
            colour_.r = 1.0f - (distance_from_camera_ / span);
            break;
        case Colour::BLUE:
            colour_.b = 1.0f - (distance_from_camera_ / span);
            break;
        case Colour::WHITE:
        default:
            colour_.r = 1.0f - (distance_from_camera_ / span);
            colour_.g = colour_.b = colour_.r;
            break;
    }
}