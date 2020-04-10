#include "VectorDecorator.h"

#include "core/DisplayManager.h"

#define NODE_SPACING 0.5f
#define VECTOR_SPACING 3.0f
#define CUBE_SCALE 1.5f

Decorators::VectorDecorator::VectorFrame::~VectorFrame()
{
    for (VectorDetail* v : vector_details_)
    {
        delete v;
    }
}

Decorators::VectorDecorator::VectorDecorator::~VectorDecorator()
{
    for (std::map<unsigned int, VectorFrame*>::iterator i = vector_frames_.begin(); i != vector_frames_.end(); i++)
    {
        delete i->second;
    }
}

Decorators::VectorDecorator::VectorDetail* Decorators::VectorDecorator::VectorDecorator::addVector(unsigned int frame, const std::vector<int>& v, Decorators::VectorDecorator::VectorDetail* parent, Decorators::VectorDecorator::VectorDetail::RelationshipToParent child_type)
{
    if (vector_frames_.find(frame) == vector_frames_.end())
    {
        VectorFrame* vector_frame = new VectorFrame();
        vector_frames_[frame] = vector_frame;
    }

    VectorDetail* vector_detail = new VectorDetail();

    vector_detail->parent_ = parent;
    vector_detail->child_type_ = child_type;
    vector_detail->vector_ = v;

    vector_frames_[frame]->vector_details_.push_back(vector_detail);

    return vector_detail;
}

void Decorators::VectorDecorator::VectorDecorator::setStartCoords(const glm::vec3 &start_coords)
{
    start_coords_ = start_coords;
}

void Decorators::VectorDecorator::VectorDecorator::setInheritPreviousFrameVectors(bool inherit_previous_frame_vectors)
{
    inherit_previous_frame_vectors_ = inherit_previous_frame_vectors;
}

/**
 * Create a Frame for each VectorFrame defined in the decorator and draw the frame's vectors onto it.
 */
void Decorators::VectorDecorator::VectorDecorator::decorate()
{
    glm::vec3 start_coords = start_coords_;

    for (std::map<unsigned int, VectorFrame*>::iterator i = vector_frames_.begin(); i != vector_frames_.end(); i++)
    {
        VectorFrame* vector_frame = i->second;

        if (inherit_previous_frame_vectors_ && current_frame_)
        {
            // Every subsequent Frame contains all of the previous Frame's vectors (used when drawing a sequence of events that build on each other)
            current_frame_ = current_frame_->clone();

            // Vectors in this Frame are drawn "below" the vectors from the previous Frame
            start_coords.y -= 2.0;
        }
        else
        {
            current_frame_ = frame_queue_->newFrame();
        }

        drawVectorFrame(vector_frame, start_coords);

        current_frame_->addText("Merge Sort", 10, 10);

        frame_queue_->enqueueFrame(current_frame_);
    }
}

void Decorators::VectorDecorator::VectorDecorator::drawVectorFrame(VectorFrame* vector_frame, const glm::vec3& start_coords)
{
    glm::vec3 node_coords = start_coords;
    bool is_first_vector = true;

    // Each VectorFrame consists of one or more VectorDetails, which are individual vectors of ints
    for (VectorDetail* vector_detail : vector_frame->vector_details_)
    {
        GLfloat vector_width = (vector_detail->vector_.size() * CUBE_SCALE) + ((vector_detail->vector_.size() - 1) * NODE_SPACING);

        // If the VectorDetail has a semantic parent, start drawing this vector to the left or right of that parent
        if (vector_detail->parent_)
        {
            switch (vector_detail->child_type_)
            {
                case VectorDetail::CHILD_IS_LEFT:
                    node_coords.x = vector_detail->parent_->coords_.x - 0.5f - vector_width;
                    break;

                case VectorDetail::CHILD_IS_RIGHT:
                    node_coords.x = vector_detail->parent_->coords_.x + vector_detail->parent_->width_ + 0.5f;
                    break;

                case VectorDetail::CHILD_IS_ONE_OF_MANY:
                    if (is_first_vector)
                    {
                        node_coords.x = vector_detail->parent_->coords_.x;  // align first vector to parent, subsequent vectors use default inter-vector spacing below
                    }
                default:
                    break;
            }
        }

        // Update the VectorDetail so child of this vector (if any) have correct drawing references
        vector_detail->coords_ = node_coords;
        vector_detail->width_ = vector_width;

        bool start_of_vector = true;
        glm::vec3 last_node_coords;

        for (int node : vector_detail->vector_)
        {
            // Draw a node for this element
            drawNode(node_coords, last_node_coords, node, start_of_vector);

            last_node_coords = node_coords;
            node_coords.x += (CUBE_SCALE + NODE_SPACING);     // inter-node spacing when nodes are in same vector

            start_of_vector = false;
        }

        // If the vector has a parent, draw a line between them
        if (vector_detail->parent_ && false)
        {
            glm::vec3 from_coords = vector_detail->coords_, to_coords = vector_detail->parent_->coords_;

            from_coords.x += vector_detail->width_ / 2.0f;
            to_coords.x += vector_detail->parent_->width_ / 2.0f;

            current_frame_->addLine(from_coords, to_coords, glm::vec3(1, 1, 1));
        }

        node_coords.x += VECTOR_SPACING;       // default inter-vector spacing (when no parent exists or child is one of many)
        is_first_vector = false;
    }
}

void Decorators::VectorDecorator::VectorDecorator::drawNode(const glm::vec3& node_coords, const glm::vec3& last_node_coords, int node_value, bool start_of_vector)
{
    glm::vec3 node_colour = glm::vec3(0.8, 0.8, 0.8);

    current_frame_->addObject(insight::primitive::Primitive::Type::CUBE, node_coords, node_colour, "slate", CUBE_SCALE);

    if ( ! start_of_vector)
    {
        current_frame_->addLine(last_node_coords, node_coords, glm::vec3(1, 1, 1));
    }

    char msg[64];
    sprintf(msg, "%d", node_value);
    current_frame_->addText(msg, node_coords.x, node_coords.y, node_coords.z, false, 0.015, glm::vec3(1.0, 1.0, 0.0));
}