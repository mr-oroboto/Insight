#ifndef INSIGHT_SCENARIO_MERGESORT_H
#define INSIGHT_SCENARIO_MERGESORT_H

#include <vector>

#include "Scenario.h"
#include "core/DisplayManager.h"
#include "decorator/VectorDecorator.h"

class MergeSort : public Scenario
{
public:
    MergeSort(DisplayManager* display_manager) : Scenario(display_manager) {}

    void run();

private:
    std::vector<int> sort(const std::vector<int>& v, unsigned int depth, Decorators::VectorDecorator::VectorDetail* parent_vector, Decorators::VectorDecorator::VectorDetail::RelationshipToParent child_type, Decorators::VectorDecorator::VectorDetail*& vector_detail);
    std::vector<int> merge(const std::vector<int>& left, const std::vector<int>& right);

    Decorators::VectorDecorator::VectorDecorator* decorator_;
    unsigned int deepest_depth_;
};


#endif //INSIGHT_SCENARIO_MERGESORT_H
