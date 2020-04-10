#ifndef INSIGHT_DECORATOR_ARRAYDECORATOR_H
#define INSIGHT_DECORATOR_ARRAYDECORATOR_H

#include <map>
#include <vector>

#include <glm/glm.hpp>

#include "Decorator.h"
#include "core/FrameQueue.h"

namespace insight {
namespace decorator {
namespace vector {

class VectorDetail {
public:
    VectorDetail() { parent_ = nullptr; }

    enum RelationshipToParent {
        CHILD_IS_LEFT = 0,
        CHILD_IS_RIGHT,
        CHILD_IS_ONE_OF_MANY
    };

    VectorDetail* parent_;              // a vector can be related to another vector (maybe in another frame)
    RelationshipToParent child_type_;   // hint for decoration (ie. arrow relationship)
    glm::vec3 coords_;
    GLfloat width_;

    std::vector<int> vector_;
};

// VectorDecorator builds up a map of VectorFrames, each of which consists of a vector of VectorDetail.
class VectorFrame {
public:
    VectorFrame() = default;
    ~VectorFrame();

    std::vector<VectorDetail*> vector_details_;
};

// VectorDecorator draws one or more frames of one or more vectors.

// Within a frame, each vector is drawn as a row of connected cubes across the screen. If multiple vectors exist for
// the frame, they are separated by a small amount of horizontal (x) space and no connecting line is drawn between.
//
// If inherit_previous_frame_vectors_ is true, the vectors of all previous frames are included in each new frame. The
// vectors for each frame are drawn "down" (y) the screen so that they build upon one another. If false, each frame
// is simply a new frame and draws its vector of vectors from the starting co-ordinates.
class VectorDecorator : public Decorator {
public:
    VectorDecorator(FrameQueue *frame_queue) : Decorator(frame_queue) { inherit_previous_frame_vectors_ = false; }
    ~VectorDecorator();

    VectorDetail* addVector(unsigned int frame, const std::vector<int> &v, VectorDetail* parent = nullptr, VectorDetail::RelationshipToParent child_type = VectorDetail::CHILD_IS_ONE_OF_MANY);
    void setStartCoords(const glm::vec3 &start_coords);
    void setInheritPreviousFrameVectors(bool inherit_previous_frame_vectors);

    // Create a Frame for each VectorFrame defined in the decorator and draw the frame's vectors onto it.
    void decorate();

protected:
    void drawVectorFrame(VectorFrame* vector_frame, const glm::vec3& start_coords);
    void drawNode(const glm::vec3 &node_coords, const glm::vec3 &last_node_coords, int node_value, bool start_of_vector);

    std::map<unsigned int, VectorFrame*> vector_frames_;
    glm::vec3 start_coords_;
    bool inherit_previous_frame_vectors_;
};

}   // namespace vector
}   // namespace decorator
}   // namespace insight

#endif //INSIGHT_DECORATOR_ARRAYDECORATOR_H
