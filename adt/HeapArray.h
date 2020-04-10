#ifndef INSIGHT_ADT_HEAPARRAY_H
#define INSIGHT_ADT_HEAPARRAY_H

#include <iostream>
#include <vector>

#include "decorator/HeapDecorator.h"

namespace insight {
namespace adt {

class HeapArray {
public:
    HeapArray(decorator::HeapDecorator* decorator = nullptr);

    // Insert a new value into the heap.
    //
    // This involves finding the next insertion position, putting the value there and then bubbling it up the heap
    // until the heap property (min or max) is restored.
    //
    // In our array implementation the next insertion position is the element after the last element of the array.
    void insert(int value);
    bool validate();

    int getNodeValue(size_t node_index);

    int getParentIndex(size_t node_index);
    int getLeftIndex(size_t node_index);
    int getRightIndex(size_t node_index);

    unsigned long getNodeCount();

protected:
    // Find the root and remove it.
    //
    // To remove the root we find the bottom-most, right-most element, put it at the top of the heap (the old min/max
    // value), remove the node we just took the value from and then bubble the "new" root down the tree until the heap
    // property is restored.
    int extractRoot();

    virtual bool shouldSwapInHeapifyUp(int node_value, int comparison_node_value) = 0;
    virtual bool shouldSwapInHeapifyDown(int node_value, int comparison_node_value) = 0;

    std::vector<int> store_;
    decorator::HeapDecorator* decorator_;

private:
    void heapifyUp(size_t node_index);
    void heapifyDown(size_t node_index);
};

class MaxHeapArray : public HeapArray {
public:
    MaxHeapArray(decorator::HeapDecorator* decorator) : HeapArray(decorator) {}

    int max();
    int extractMax() { return extractRoot(); }

protected:
    bool shouldSwapInHeapifyUp(int node_value, int comparison_node_value);
    bool shouldSwapInHeapifyDown(int node_value, int comparison_node_value);
};

class MinHeapArray : public HeapArray {
public:
    MinHeapArray(decorator::HeapDecorator* decorator) : HeapArray(decorator) {}

    int min();
    int extractMin() { return extractRoot(); }

protected:
    bool shouldSwapInHeapifyUp(int node_value, int comparison_node_value);
    bool shouldSwapInHeapifyDown(int node_value, int comparison_node_value);
};

}   // namespace adt
}   // namespace insight

#endif //INSIGHT_ADT_HEAPARRAY_H
