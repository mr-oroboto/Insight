#include "AutoPilot.h"

#include <iostream>
#include <random>

#include "primitive/Tesselation.h"

#define FORWARD_UNITS_PER_SEC 120.0f
#define CAMERA_ROLL_DEGREES_PER_SEC 25.0f

void AutoPilot::run()
{
    std::unique_ptr<FrameQueue> frame_queue = std::make_unique<FrameQueue>(display_manager_, true);
    frame_queue->setFrameRate(1);

    frame_ = frame_queue->newFrame();

    setupStarField(250, glm::vec3(0, 0, 6.0f), 40, 40, 60);

    GLfloat initial_z = 36.0f;                                  // should be a factor of TESSELATION_WIDTH
    floor_drawn_from_z_ = -300.0f;                              // should be a factor of TESSELATION_WIDTH

    initial_floor_length_z_ = initial_z - floor_drawn_from_z_;
    z_ = initial_z;                                             // starting camera position, move from here forwards
    display_manager_->setPerspective(0.1f, -1.0f * floor_drawn_from_z_, 45.0f);
    display_manager_->setCameraCoords(glm::vec3(0, 0, z_));     // @todo: this is actually overwritten because DisplayManager::run() runs after us

    camera_pitch_degrees_ = 0;
    camera_yaw_degrees_ = 0;
    camera_roll_degrees_ = 0;
    camera_roll_increment_ = CAMERA_ROLL_DEGREES_PER_SEC;
    camera_roll_max_degrees_ = 45.0f;

    drawFloor(floor_drawn_from_z_, initial_z);

    frame_queue->enqueueFrame(frame_);

    display_manager_->setUpdateSceneCallback(std::bind(&AutoPilot::updateSceneCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));

    frame_queue->setReady();
    if (frame_queue->setActive())
    {
        // transfer ownership to DisplayManager
        display_manager_->setFrameQueue(std::move(frame_queue));
    }
}

void AutoPilot::setupStarField(GLuint num_stars, const glm::vec3& origin, GLfloat x_spread, GLfloat y_spread, GLfloat z_spread)
{
    stars_.clear();

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<> x_distribution(origin.x - x_spread / 2.0f, origin.x + x_spread / 2.0f);
    std::uniform_real_distribution<> y_distribution(origin.y - y_spread / 2.0f, origin.y + y_spread / 2.0f);
    std::uniform_real_distribution<> z_distribution(origin.z - z_spread, origin.z + z_spread / 2.0f);
    std::uniform_real_distribution<> size_distribution(0.05, 0.15);

    for (size_t i = 0; i < num_stars; i++)
    {
        glm::vec3 world_coords = glm::vec3(x_distribution(gen), y_distribution(gen), z_distribution(gen));
        Star* star = new Star(display_manager_, Primitive::Type::CUBE, world_coords, glm::vec3(1, 1, 1));

        if (i % 3 == 0)
        {
            star->setStarColour(Star::Colour::BLUE);
        }
        else if (i % 7 == 0)
        {
            star->setStarColour(Star::Colour::RED);
        }

        star->setScale(static_cast<GLfloat>(size_distribution(gen)));
        star->setForwardUnitsPerSec(0);

        stars_.push_back(star);
        frame_->addObject(star);
    }
}

void AutoPilot::updateSceneCallback(GLfloat secs_since_rendering_started, GLfloat secs_since_framequeue_started, GLfloat secs_since_last_renderloop, GLfloat secs_since_last_frame)
{
    camera_roll_degrees_ -= (camera_roll_increment_ * secs_since_last_renderloop);

    if ((camera_roll_increment_ > 0 && camera_roll_degrees_ < -camera_roll_max_degrees_) || (camera_roll_increment_ < 0 && camera_roll_degrees_ > camera_roll_max_degrees_))
    {
        camera_roll_increment_ *= -1;

        std::random_device rd;
        std::mt19937 gen(rd());

        std::uniform_real_distribution<> dist(0, 90);
        camera_roll_max_degrees_ = static_cast<GLfloat>(dist(gen));
    }

    glm::vec3 camera_up_vector;
    camera_up_vector.x =  sin(glm::radians(camera_roll_degrees_));
    camera_up_vector.y =  cos(glm::radians(camera_roll_degrees_));
    camera_up_vector.z = 0;
    camera_up_vector = glm::normalize(camera_up_vector);

    z_ -= (FORWARD_UNITS_PER_SEC * secs_since_last_renderloop);
    display_manager_->setCameraCoords(glm::vec3(10 * sin(secs_since_rendering_started), (5 * sin(secs_since_rendering_started)) + 5.0, z_));
    display_manager_->setCameraUpVector(camera_up_vector);

    if (z_ < (floor_drawn_from_z_ + (initial_floor_length_z_ / 2.0)))
    {
        drawFloor(floor_drawn_from_z_ - 6.0f, floor_drawn_from_z_);
        floor_drawn_from_z_ -= 6.0f;

        frame_->deleteObjectsOutsideBoundary(glm::vec3(0, 0, z_), initial_floor_length_z_, Primitive::Type::TESSELATION);
    }

    for (Star* star : stars_)
    {
        star->setNearAndFarPlane(z_, z_ - 90.0f);
    }

    frame_->updateObjects(secs_since_rendering_started, secs_since_framequeue_started, secs_since_last_renderloop, secs_since_last_frame, nullptr);
}

void AutoPilot::drawFloor(GLfloat floor_z_start, GLfloat floor_z_end)
{
    GLfloat y_pos = -3.0f;

    assert(floor_z_end > floor_z_start);

    GLfloat floor_x_start = -96.0f;
    GLfloat floor_width_x = 192.0f;
    GLfloat tile_dimension = TESSELATION_WIDTH;                                     // DRAGON: assumes square tiles
    GLfloat x, z;
    GLuint tiles_wide_x = static_cast<GLuint>(floor_width_x / tile_dimension);                  // how many tiles wide (in x-dimension) is this surface?
    GLuint tiles_wide_z = static_cast<GLuint>((floor_z_end - floor_z_start) / tile_dimension);  // how many tiles wide (in z-dimension) is this surface?

    std::vector<std::vector<GLfloat>> previous_bottom_row_bottom_right_y(tiles_wide_x);     // the top of each tile in each subsequent z row joins up against this (one per column along the x-axis)
    std::vector<GLfloat> previous_right_column_bottom_right_y;                              // the left of each subsequent x column joins up against this
    GLuint current_column = 0, current_row = 0;
    GLfloat last_yfree;

    /**
     * The floor is made of tiles (Tesselation primitives), each of which itself is made of sub-tiles. For the seam
     * joining to work correctly, we must "grow" the floor in the positive z and positive x directions.
     */
    for (z = floor_z_start; z < floor_z_end; z += tile_dimension)
    {
        current_column = 0;

        for (x = floor_x_start; x < (floor_x_start + floor_width_x); x += tile_dimension)
        {
            SceneObject* object = new SceneObject(display_manager_, Primitive::Type::TESSELATION, glm::vec3(x, y_pos, z), glm::vec3(1, 1, 1));
            Tesselation* tile = dynamic_cast<Tesselation*>(object->getPrimitive());

            object->setTexture("water");

            tile->setRandomisePeaks(true);
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

            frame_->addObject(object);

            current_column++;
        }

        current_row++;
    }
}