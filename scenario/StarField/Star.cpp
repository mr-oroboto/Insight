#include "Star.h"

#include <iostream>

#include "core/SceneObject.h"

Star::Star(DisplayManager* display_manager, Primitive::Type type, const glm::vec3& world_coords, const glm::vec3& colour) : SceneObject(display_manager, type, world_coords, colour)
{

}

Star::~Star()
{
    std::cout << "Star::~Star()" << std::endl;
}

void Star::update(GLfloat secs_since_last_frame)
{
    world_coords_.z += (20.0f * secs_since_last_frame);

    if (world_coords_.z > 30.0f)
    {
        world_coords_.z = -30.0f;
    }

    colour_.r = ((world_coords_.z + 30.0f) / 60.0f);
    colour_.g = colour_.b = colour_.r;
}