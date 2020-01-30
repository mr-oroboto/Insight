#include "Frame.h"

#include <math.h>

#include <iostream>

#include "DisplayManager.h"
#include "primitive/Tesselation.h"

Frame::Frame(DisplayManager* display_manager, bool draw_object_position, bool draw_reference_axes, bool draw_floor)
{
    display_manager_ = display_manager;

    draw_object_positions_ = draw_object_position;
    draw_reference_axes_ = draw_reference_axes;
    draw_floor_ = draw_floor;
}

Frame::~Frame()
{
    // Delete all the scene objects that belong to this frame.
    for (SceneObject* object : objects_)
    {
        delete object;
    }
}

void Frame::addObject(Primitive::Type type, const glm::vec3& world_coords, const glm::vec3& colour, Texture* texture, GLfloat scale)
{
    SceneObject* object = new SceneObject(display_manager_, type, world_coords, colour);

    object->setScale(scale);
    object->setTexture(texture);

    addObject(object);
}

void Frame::addObject(SceneObject* object)
{
    objects_.push_back(object);
}

void Frame::addLine(const glm::vec3& from_world_coords, const glm::vec3& to_world_coords, const glm::vec3& colour)
{
    SceneObject* object = new SceneObject(display_manager_, Primitive::Type::LINE, from_world_coords, colour);
    object->setAdditionalCoords(to_world_coords);
    objects_.push_back(object);
}

void Frame::addText(const std::string& text, GLfloat x, GLfloat y, GLfloat z, bool ortho, GLfloat scale, const glm::vec3& colour)
{
    glm::vec3 position = glm::vec3(x, y, z);

    Text textObj {
            text,
            position,
            ortho,
            scale,
            colour
    };

    texts_.push_back(textObj);
}

void Frame::draw(GLfloat secs_since_rendering_started, GLfloat secs_since_framequeue_started, GLfloat secs_since_last_renderloop, GLfloat secs_since_last_frame)
{
    if (draw_floor_)
    {
        drawTesselatedFloor();
    }

    if (draw_reference_axes_)
    {
        glm::vec3 colour = glm::vec3(1);
        glm::vec3 from = glm::vec3(0, 0, 0);

        glm::vec3 to = glm::vec3(10, 0, 0);
        addLine(from, to, colour);

        to = glm::vec3(0, 10, 0);
        addLine(from, to, colour);

        to = glm::vec3(0, 0, 10);
        addLine(from, to, colour);

        addObject(Primitive::Type::CUBE, glm::vec3(0, 0, 0),   glm::vec3(1, 1, 1), nullptr, 0.5);
        addObject(Primitive::Type::CUBE, glm::vec3(-10, 0, 0), glm::vec3(1, 0, 0), nullptr, 0.2);
        addObject(Primitive::Type::CUBE, glm::vec3(10, 0, 0),  glm::vec3(0, 0, 1), nullptr, 0.2);
        addObject(Primitive::Type::CUBE, glm::vec3(0, -10, 0), glm::vec3(1, 0, 0), nullptr, 0.2);
        addObject(Primitive::Type::CUBE, glm::vec3(0, 10, 0),  glm::vec3(1, 1, 1), nullptr, 0.2);
        addObject(Primitive::Type::CUBE, glm::vec3(0, 0, -10), glm::vec3(1, 0, 0), nullptr, 0.2);
        addObject(Primitive::Type::CUBE, glm::vec3(0, 0, 10),  glm::vec3(1, 1, 0), nullptr, 0.2);
    }

    // We must first render all objects before we render any text
    for (SceneObject* object : objects_)
    {
        object->draw(secs_since_rendering_started, secs_since_framequeue_started, secs_since_last_renderloop, secs_since_last_frame);
    }

    // Now text can be rendered
    if (draw_object_positions_)
    {
        for (SceneObject* object : objects_)
        {
            char msg[64];
            glm::vec3 text_colour = glm::vec3(1.0, 1.0, 0.0);
            glm::vec3 object_coords = object->getPosition();
            snprintf(msg, sizeof(msg), "(%.1f,%.1f,%.1f)", object_coords.x, object_coords.y, object_coords.z);
            display_manager_->drawText(msg, object_coords, false, 0.015, text_colour);
        }
    }

    for (Text text : texts_)
    {
        display_manager_->drawText(text.text, text.position, text.ortho, text.scale, text.colour);
    }
}

/**
 * Each tesselation tile must be drawn as it is created because its vertices change with each previous tile.
 */
void Frame::drawTesselatedFloor()
{
    bool useVertexColours = false;

    GLfloat y_pos = -3.0f;

    GLfloat floor_x_start = -12.0f;
    GLfloat floor_z_start = -12.0f;
    GLfloat floor_width_z = 24.0;
    GLfloat floor_width_x = 24.0;
    GLfloat tile_dimension = TESSELATION_WIDTH;                                     // DRAGON: assumes square tiles
    GLfloat x, z;
    GLuint tiles_wide_x = static_cast<GLuint>(floor_width_x / tile_dimension);      // how many tiles wide (in x-dimension) is this surface?
    GLuint tiles_wide_z = static_cast<GLuint>(floor_width_z / tile_dimension);      // how many tiles wide (in z-dimension) is this surface?

    std::vector<std::vector<GLfloat>> previous_bottom_row_bottom_right_y(tiles_wide_x);     // the top of each tile in each subsequent z row joins up against this (one per column along the x-axis)
    std::vector<GLfloat> previous_right_column_bottom_right_y;                              // the left of each subsequent x column joins up against this
    GLuint current_column = 0, current_row = 0;
    GLfloat last_yfree;

    /**
     * The floor is made of tiles (Tesselation primitives), each of which itself is made of sub-tiles. For the seam
     * joining to work correctly, we must "grow" the floor in the positive z and positive x directions.
     */
    for (z = floor_z_start; z < (floor_z_start + floor_width_z); z += tile_dimension)
    {
        current_column = 0;     // we make columns along the x-axis and rows along the z-axis

        for (x = floor_x_start; x < (floor_x_start + floor_width_x); x += tile_dimension)
        {
            SceneObject* object = new SceneObject(display_manager_, Primitive::Type::TESSELATION, glm::vec3(x, y_pos, z), glm::vec3(1, 1, 1));
            Tesselation* tile = dynamic_cast<Tesselation*>(object->getPrimitive());

            Texture* texture = display_manager_->getTextureCollection()->getTexture("water");
            object->setTexture(texture);

            tile->setRandomisePeaks(false);
            tile->setType(Tesselation::Type::RANDOM);
            tile->setYFreeSeed(0);
            tile->resetSeamVertices();
            tile->initVertices();

            tile->setBorderRight(false);       // we're growing the tesselation from all tiles
            tile->setBorderBottom(false);      // we're growing the tesselation from all tiles
            tile->setBorderTop(true);          // assume we're a border on the other edges unless reset by setPrevious...
            tile->setBorderLeft(true);

            if (current_row != 0)
            {
                tile->setPreviousBottomRowBottomRightY(previous_bottom_row_bottom_right_y[current_column]);
            }

            if (current_column != 0)
            {
//              t->setZFreeSeed(prevRightColumnBottomRightZ[0]);
                tile->setYFreeSeed(last_yfree);
                tile->setPreviousRightColumnBottomRightY(previous_right_column_bottom_right_y);
            }

            if (current_column == tiles_wide_x - 1)
            {
                tile->setBorderRight(true);
            }

            if (current_row == tiles_wide_z - 1)
            {
                tile->setBorderBottom(true);
            }

            tile->initVertices();

            previous_bottom_row_bottom_right_y[current_column] = tile->getBottomRowBottomRightY();
            previous_right_column_bottom_right_y = tile->getRightColumnBottomRightY();
            last_yfree = tile->getYFree();

            object->draw(0, 0, 0, 0, ! useVertexColours);

            delete object;

            current_column++;
        }

        current_row++;
    }

    return;
}

void Frame::drawTesselatedFloorWithIsolatedTiles()
{
    bool useVertexColours = true;

    GLfloat y_pos = -3.0;

    GLfloat floor_x_start = -12.0f;
    GLfloat floor_z_start = -12.0f;
    GLfloat floor_width = 12.0;      // y-dimension
    GLfloat floor_length = 12.0;     // x-dimension
    GLfloat tile_dimension = 6.0;    // assumes square tiles, this is actually defined in Tesselation (DRAGON)
    GLfloat x, z;

    for (z = floor_z_start; z < (floor_z_start + floor_width); z += tile_dimension)
    {
        for (x = floor_x_start; x < (floor_x_start + floor_length); x += tile_dimension)
        {
            SceneObject* object = new SceneObject(display_manager_, Primitive::Type::TESSELATION, glm::vec3(x, y_pos, z),  glm::vec3(0.5, 0.5, 0.5));
            Tesselation* tile = dynamic_cast<Tesselation*>(object->getPrimitive());

            tile->setRandomisePeaks(false);
            tile->setYFreeSeed(0);
            tile->setType(Tesselation::Type::RAMPED);
            tile->setIsolated();
            tile->initVertices();

            object->draw(0, 0, 0, 0, ! useVertexColours);

            delete object;
        }
    }
}

Frame* Frame::clone()
{
    Frame* clone = new Frame(display_manager_, draw_object_positions_, draw_reference_axes_, draw_floor_);

    for (SceneObject* object : objects_)
    {
        clone->objects_.push_back(object->clone());
    }

    for (Text text : texts_)
    {
        clone->texts_.push_back(text);
    }

    return clone;
}

GLuint Frame::deleteObjectsAtPosition(const glm::vec3 &world_coords, Primitive::Type primitive_type)
{
    bool object_deleted = false;
    bool objects_deleted = 0;

    do
    {
        object_deleted = false;

        for (std::vector<SceneObject*>::iterator it = objects_.begin(); it != objects_.end(); it++)
        {
            glm::vec3 object_position = (*it)->getPosition();
            if (object_position.x == world_coords.x && object_position.y == world_coords.y && object_position.z == world_coords.z && (*it)->getPrimitive()->getType() == primitive_type)
            {
                delete *it;
                objects_.erase(it);

                object_deleted = true;
                objects_deleted++;
                break;
            }
        }
    } while (object_deleted);

    return objects_deleted;
}

GLuint Frame::deleteObjectsOutsideBoundary(const glm::vec3 &world_coords, GLfloat bounding_width, Primitive::Type primitive_type)
{
    bool object_deleted = false;
    bool objects_deleted = 0;

    do
    {
        object_deleted = false;

        for (std::vector<SceneObject*>::iterator it = objects_.begin(); it != objects_.end(); it++)
        {
            glm::vec3 object_position = (*it)->getPosition();

            if (coordinatesAreOutsideBoundary(object_position, world_coords, bounding_width) && (*it)->getPrimitive()->getType() == primitive_type)
            {
                delete *it;
                objects_.erase(it);

                object_deleted = true;
                objects_deleted++;
                break;
            }
        }
    } while (object_deleted);

    return objects_deleted;
}

bool Frame::coordinatesAreOutsideBoundary(const glm::vec3& world_coords, const glm::vec3& reference_coords, GLfloat bounding_width)
{
    if ((world_coords.x < (reference_coords.x - bounding_width)) || (world_coords.x > (reference_coords.x + bounding_width)) ||
        (world_coords.y < (reference_coords.y - bounding_width)) || (world_coords.y > (reference_coords.y + bounding_width)) ||
        (world_coords.z < (reference_coords.z - bounding_width)) || (world_coords.z > (reference_coords.z + bounding_width)))
    {
        return true;
    }

    return false;
}

void Frame::updateObjects(GLfloat secs_since_rendering_started, GLfloat secs_since_framequeue_started, GLfloat secs_since_last_renderloop, GLfloat secs_since_last_frame, void* context)
{
    for (SceneObject* object : objects_)
    {
        object->update(secs_since_rendering_started, secs_since_framequeue_started, secs_since_last_renderloop, secs_since_last_frame, context);
    }
}