#include "DisplayManager.h"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp> // makes view and projection matrices easier to generate

#include "shader/StandardShader.h"
#include "TextDrawer.h"

DisplayManager::DisplayManager()
{
    initialised_ = false;

    frame_queue_ = nullptr;
    primitives_ = nullptr;
    textures_ = nullptr;

    object_shader_ = nullptr;
    text_drawer_ = nullptr;
}

DisplayManager::~DisplayManager()
{
    std::cout << "DisplayManager::~DisplayManager()" << std::endl;

    if (initialised_)
    {
        teardown();
    }
}

bool DisplayManager::initialise(GLuint wnd_width, GLuint wnd_height)
{
    if (initialised_)
    {
        return true;
    }

    wnd_width_ = wnd_width;
    wnd_height_ = wnd_height;

    try
    {
        object_shader_ = new StandardShader();
        if ( ! object_shader_->initialise())
        {
            throw "Can't initialise object shader";
        }

        text_drawer_ = new TextDrawer(this);
        if ( ! text_drawer_->initialise(wnd_width, wnd_height))
        {
            throw "Can't initialise text drawer";
        }

        primitives_ = new PrimitiveCollection(object_shader_);
        textures_ = new TextureCollection(object_shader_);
        textures_->registerTexture("/home/sysop/textures/conifer.jpg", "conifer");
        textures_->registerTexture("/home/sysop/textures/foam.jpg", "foam");
        textures_->registerTexture("/home/sysop/textures/ice.jpg", "ice");
        textures_->registerTexture("/home/sysop/textures/leaves.jpg", "leaves");
        textures_->registerTexture("/home/sysop/textures/orange_leather.jpg", "leather");
        textures_->registerTexture("/home/sysop/textures/sky.jpg", "sky");
        textures_->registerTexture("/home/sysop/textures/slate.jpg", "slate");
        textures_->registerTexture("/home/sysop/textures/water.jpg", "water");
        textures_->registerTexture("/home/sysop/textures/wood.jpg", "wood");

        setPerspective(0.1f, 100.0f, 45.0f);
        setCameraCoords(glm::vec3(0, 0, 0));

        setLightingOn(true);
        setLightColour(glm::vec3(1, 1, 1));
        setLightCoords(glm::vec3(5, 3, -5));

        initialised_ = true;
    }
    catch (const char* exception)
    {
        std::cout << exception << std::endl;
        teardown();
    }

    return initialised_;
}

void DisplayManager::teardown()
{
    if (primitives_)
    {
        delete primitives_;
        primitives_ = nullptr;
    }

    if (textures_)
    {
        delete textures_;
        textures_ = nullptr;
    }

    if (frame_queue_)
    {
        delete frame_queue_;
        frame_queue_ = nullptr;
    }

    if (object_shader_)
    {
        delete object_shader_;
        object_shader_ = nullptr;
    }

    if (text_drawer_)
    {
        delete text_drawer_;
        text_drawer_ = nullptr;
    }

    initialised_ = false;
}

void DisplayManager::setCameraCoords(const glm::vec3& world_coords, const glm::vec3& camera_direction_vector)
{
    camera_coords_ = world_coords;
    camera_direction_vector_ = camera_direction_vector;

    object_shader_->setCameraCoords(camera_coords_);
}

void DisplayManager::setLightingOn(bool on)
{
    lighting_on_ = on ? 1 : 0;

    object_shader_->setLightingOn(on);
}

void DisplayManager::setLightCoords(const glm::vec3& world_coords)
{
    light_coords_ = world_coords;

    object_shader_->setLightCoords(light_coords_);
}

void DisplayManager::setLightColour(const glm::vec3 &colour, GLfloat intensity)
{
    light_colour_ = colour;
    light_intensity_ = intensity;

    object_shader_->setLightColour(light_colour_, light_intensity_);
}

void DisplayManager::setPerspective(GLfloat near_plane, GLfloat far_plane, GLfloat fov)
{
    projection_transform_ = glm::perspective(
            glm::radians(fov),        // vertical field-of-view (see open.gl/transformations)
            static_cast<GLfloat>(wnd_width_) / static_cast<GLfloat>(wnd_height_), // aspect ratio of the screen
            near_plane,               // near clipping plane (any vertex closer to camera than this disappears)
            far_plane                 // far clipping plane (any vertex further from camera than this disappears)
    );

    object_shader_->setProjectionTransform(projection_transform_);
}

void DisplayManager::drawScene()
{
    if ( ! initialised_)
    {
        return;
    }

    object_shader_->use();

    object_shader_->setViewTransform(getViewTransform());

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);               // clear screen to black, otherwise we'll paint over the last frame which looks weird
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the colour buffer if we've got GL_DEPTH_TEST enabled (this fixes the glitchy graphics)

    if (frame_queue_)
    {
        frame_queue_->drawCurrentFrame();
    }
}

void DisplayManager::drawText(const std::string& text, const glm::vec3& world_coords, bool ortho, GLfloat scale, const glm::vec3& colour)
{
    if ( ! initialised_)
    {
        return;
    }

    text_drawer_->print(text, world_coords, ortho, Font::Type::FONT_VERA, scale, colour);
}

void DisplayManager::setFrameQueue(FrameQueue* queue)
{
    // If we already have a FrameQueue, destroy it.
    if (frame_queue_)
    {
        delete frame_queue_;
    }

    frame_queue_ = queue;
}

PrimitiveCollection* DisplayManager::getPrimitiveCollection()
{
    return primitives_;
}

TextureCollection* DisplayManager::getTextureCollection()
{
    return textures_;
}

StandardShader* DisplayManager::getObjectShader()
{
    return object_shader_;
}

glm::vec3 DisplayManager::getCameraDirectionVector()
{
    return camera_direction_vector_;
}

glm::vec3 DisplayManager::getCameraUpVector()
{
    // OpenGL right-handed co-ordinate system (+y is up)
    return glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat4 DisplayManager::getViewTransform()
{
    /**
     * View Transform (OpenGL right-handed co-ordinates)
     *
     * -x   left of screen
     * +x   right of screen
     * +y   top of screen
     * -y   bottom of screen
     * -z   into screen
     * +z   out of screen
     */
    glm::mat4 view_transform = glm::lookAt(
            camera_coords_,                                                   // camera position within world co-ordinates
            camera_coords_ + getCameraDirectionVector(),                      // looking at target (facing whatever direction is specified (into screen by default))
            getCameraUpVector()                                               // OpenGL right-handed co-ordinates (+y is up)
    );

    return view_transform;
}

glm::mat4 DisplayManager::getProjectionTransform()
{
    return projection_transform_;
}