#include "DisplayManager.h"

#include <iostream>
#include <utility>

#include <glm/gtc/matrix_transform.hpp> // makes view and projection matrices easier to generate

#include "shader/StandardShader.h"
#include "TextDrawer.h"

DisplayManager::DisplayManager()
    : initialised_(false),
      primitives_(nullptr),
      update_scene_callback_(nullptr),
      object_shader_(nullptr) {}

DisplayManager::~DisplayManager()
{
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

        text_drawer_ = std::make_unique<TextDrawer>(this);
        if ( ! text_drawer_->initialise(wnd_width, wnd_height))
        {
            throw "Can't initialise text drawer";
        }

        primitives_ = new PrimitiveCollection(object_shader_);
        textures_ = std::make_unique<TextureCollection>(object_shader_);

        setPerspective(0.1f, 100.0f, 45.0f);

        setCameraUpVector(glm::vec3(0.0f, 1.0f, 0.0f));     // OpenGL right-handed co-ordinate system (+y is up)
        setCameraPointingVector(glm::vec3(0, 0, -1));       // facing into screen
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

    textures_.reset();
    frame_queue_.reset();

    if (object_shader_)
    {
        delete object_shader_;
        object_shader_ = nullptr;
    }

    text_drawer_.reset();

    initialised_ = false;
}

void DisplayManager::setCameraCoords(const glm::vec3& world_coords)
{
    camera_coords_ = world_coords;
    object_shader_->setCameraCoords(camera_coords_);
}

void DisplayManager::setCameraPointingVector(const glm::vec3 &vector)
{
    camera_pointing_vector_ = glm::normalize(vector);
}

void DisplayManager::setCameraUpVector(const glm::vec3 &vector)
{
    camera_up_vector_ = vector;
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

bool DisplayManager::registerFont(Font::Type font_type, const std::string& path)
{
    if ( ! text_drawer_)
    {
        return false;
    }

    return text_drawer_->registerFont(font_type, path);
}

bool DisplayManager::registerTexture(const std::string& path, const std::string& name)
{
    if ( ! textures_)
    {
        return false;
    }

    return textures_->registerTexture(path, name);
}

Texture* DisplayManager::getTexture(const std::string& texture_name)
{
    if ( ! textures_)
    {
        return nullptr;
    }

    return textures_->getTexture(texture_name);
}

void DisplayManager::drawScene(GLfloat secs_since_rendering_started, GLfloat secs_since_last_renderloop)
{
    if ( ! initialised_)
    {
        return;
    }

    object_shader_->use();

    object_shader_->setViewTransform(getViewTransform());

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);               // clear screen to black, otherwise we'll paint over the last frame which looks weird
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the colour buffer if we've got GL_DEPTH_TEST enabled (this fixes the glitchy graphics)

    GLfloat secs_since_framequeue_started = 0, secs_since_last_frame = 0;

    if (frame_queue_)
    {
        frame_queue_->drawCurrentFrame(secs_since_rendering_started, secs_since_framequeue_started, secs_since_last_renderloop, secs_since_last_frame);
    }

    if (update_scene_callback_)
    {
        update_scene_callback_(secs_since_rendering_started, secs_since_framequeue_started, secs_since_last_renderloop, secs_since_last_frame);
    }
}

void DisplayManager::drawText(const std::string& text, const glm::vec3& world_coords, bool ortho, GLfloat scale, const glm::vec3& colour)
{
    if ( ! initialised_)
    {
        return;
    }

    text_drawer_->print(text, world_coords, ortho, Font::Type::FONT_DEFAULT, scale, colour);
}

void DisplayManager::setFrameQueue(std::unique_ptr<FrameQueue> queue)
{
    assert(queue->isActive());

    // If we already have a FrameQueue, destroy it.
    if (frame_queue_)
    {
        frame_queue_.reset();
    }

    frame_queue_ = std::move(queue);
}

void DisplayManager::setUpdateSceneCallback(std::function<void(GLfloat, GLfloat, GLfloat, GLfloat)> callback)
{
    update_scene_callback_ = callback;
}

PrimitiveCollection* DisplayManager::getPrimitiveCollection()
{
    return primitives_;
}

StandardShader* DisplayManager::getObjectShader()
{
    return object_shader_;
}

glm::vec3 DisplayManager::getCameraCoords()
{
    return camera_coords_;
}

glm::vec3 DisplayManager::getCameraPointingVector()
{
    return camera_pointing_vector_;
}

glm::vec3 DisplayManager::getCameraUpVector()
{
    return camera_up_vector_;
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
            camera_coords_ + getCameraPointingVector(),                       // looking at target (facing whatever direction is specified (into screen by default))
            getCameraUpVector()                                               // OpenGL right-handed co-ordinates (+y is up)
    );

    return view_transform;
}

glm::mat4 DisplayManager::getProjectionTransform()
{
    return projection_transform_;
}

GLuint DisplayManager::getWindowWidth()
{
    return wnd_width_;
}

GLuint DisplayManager::getWindowHeight()
{
    return wnd_height_;
}

/**
 * Generate a ray from the camera (origin) to a picked mouse position (ray casting).
 *
 * http://antongerdelan.net/opengl/raycasting.html
 * https://stackoverflow.com/questions/29997209/opengl-c-mouse-ray-picking-glmunproject
 * http://www.iquilezles.org/www/articles/intersectors/intersectors.htm
 */
glm::vec3 DisplayManager::getRayFromCamera(GLuint to_mouse_x, GLuint to_mouse_y)
{
    GLfloat ndc_mouse_x = to_mouse_x / (getWindowWidth()  * 0.5f) - 1.0f;     // NDC (-1.0 to 1.0)
    GLfloat ndc_mouse_y = to_mouse_y / (getWindowHeight() * 0.5f) - 1.0f;

    glm::mat4 view = getViewTransform();
    view = glm::lookAt(glm::vec3(0.0f), getCameraPointingVector(), getCameraUpVector());

    glm::mat4 inverse_view_projection = glm::inverse(getProjectionTransform() * view);
    glm::vec4 mouse_screen_coords = glm::vec4(ndc_mouse_x, -1.0 * ndc_mouse_y, 1.0f, 1.0f);
    glm::vec4 mouse_world_coords = inverse_view_projection * mouse_screen_coords;

    glm::vec3 ray_direction = glm::normalize(glm::vec3(mouse_world_coords));

    return ray_direction;
}
