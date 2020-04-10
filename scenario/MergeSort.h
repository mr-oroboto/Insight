#ifndef INSIGHT_SCENARIO_MERGESORT_H
#define INSIGHT_SCENARIO_MERGESORT_H

#include <vector>

#include "Scenario.h"
#include "decorator/VectorDecorator.h"

class DisplayManager;

namespace insight {
namespace scenario {

class MergeSort : public Scenario {
public:
    MergeSort(DisplayManager* display_manager) : Scenario(display_manager) {}

    void run();

private:
    std::vector<int> sort(const std::vector<int>& v, unsigned int depth,
                          decorator::vector::VectorDetail* parent_vector,
                          decorator::vector::VectorDetail::RelationshipToParent child_type,
                          decorator::vector::VectorDetail*& vector_detail);
    std::vector<int> merge(const std::vector<int>& left, const std::vector<int>& right);

    decorator::vector::VectorDecorator* decorator_;
    unsigned int deepest_depth_;
};

}   // namespace scenario
}   // namespace insight

#endif //INSIGHT_SCENARIO_MERGESORT_H
