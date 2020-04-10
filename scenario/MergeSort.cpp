#include "MergeSort.h"

#include "core/DisplayManager.h"

void insight::scenario::MergeSort::run()
{
    std::unique_ptr<FrameQueue> frame_queue = std::make_unique<FrameQueue>(display_manager_, true);
    frame_queue->setFrameRate(1);

    glm::vec3 start_coords = glm::vec3(-8, 10, 0);

    decorator_ = new Decorators::VectorDecorator::VectorDecorator(frame_queue.get());
    decorator_->setStartCoords(start_coords);
    decorator_->setInheritPreviousFrameVectors(true);

    int values[] = {
            3, 2, 1, 7, 90, 32
    };

    std::vector<int> v = std::vector<int>(values, values + sizeof(values) / sizeof(int));

    deepest_depth_ = 0;
    Decorators::VectorDecorator::VectorDetail* parent_vector = nullptr;
    sort(v, 0, nullptr, Decorators::VectorDecorator::VectorDetail::CHILD_IS_ONE_OF_MANY, parent_vector);

    decorator_->decorate();

    frame_queue->setReady();
    if (frame_queue->setActive())
    {
        // transfer ownership to DisplayManager
        display_manager_->setFrameQueue(std::move(frame_queue));
    }

    delete decorator_;
}

/**
 * A more time and space effective implementation can be done on in-place array segments, however, for the purpose
 * of decorating the method, it is clearer to take copies of the vectors along the way.
 *
 * NOTE: Rather than specifying the horizontal offset based on the frame number, it should be based on the horizontal
 *       offset of the parent array that the left and right portion are coming from (see the 2, 1 and 90,32 split for
 *       an example).
 */
std::vector<int> insight::scenario::MergeSort::sort(const std::vector<int>& v, unsigned int depth, Decorators::VectorDecorator::VectorDetail* parent_vector, Decorators::VectorDecorator::VectorDetail::RelationshipToParent child_type, Decorators::VectorDecorator::VectorDetail*& vector_detail)
{
    if (depth > deepest_depth_)
    {
        deepest_depth_ = depth;
    }

    vector_detail = decorator_->addVector(depth, v, parent_vector, child_type);

    if (v.size() == 1)
    {
        return v;       // a vector of length 1 is trivially sorted
    }

    // Divide the vector into two halves
    unsigned long len = v.size();
    std::vector<int> left_unsorted = std::vector<int>(v.begin(), v.begin() + (len / 2));
    std::vector<int> right_unsorted = std::vector<int>(v.begin() + (len / 2), v.end());

    // Sort the two halves
    Decorators::VectorDecorator::VectorDetail* left_vector_detail, *right_vector_detail;
    std::vector<int> left_sorted = sort(left_unsorted, depth + 1, vector_detail, Decorators::VectorDecorator::VectorDetail::CHILD_IS_LEFT, left_vector_detail);
    std::vector<int> right_sorted = sort(right_unsorted, depth + 1, vector_detail, Decorators::VectorDecorator::VectorDetail::CHILD_IS_RIGHT, right_vector_detail);

    // Merge the two sorted halves
    std::vector<int> sorted = merge(left_sorted, right_sorted);

    unsigned int frame_number = deepest_depth_ + (deepest_depth_ - depth);
    decorator_->addVector(frame_number, sorted, left_vector_detail, Decorators::VectorDecorator::VectorDetail::CHILD_IS_RIGHT);

    return sorted;
}

std::vector<int> insight::scenario::MergeSort::merge(const std::vector<int>& left, const std::vector<int>& right)
{
    std::vector<int> merged = std::vector<int>();

    std::vector<int>::const_iterator i = left.begin(), j = right.begin();
    while (i != left.end() && j != right.end())
    {
        if (*i < *j)
        {
            merged.push_back(*i);   // take left
            i++;
        }
        else
        {
            merged.push_back(*j);   // take right
            j++;
        }
    }

    // Drain left if any remain
    while (i != left.end())
    {
        merged.push_back(*i);
        i++;
    }

    // Drain right if any remain
    while (j != right.end())
    {
        merged.push_back(*j);
        j++;
    }

    return merged;
}