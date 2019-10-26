#ifndef INSIGHT_ADT_HEAPARRAY_H
#define INSIGHT_ADT_HEAPARRAY_H

#include <iostream>
#include <vector>

#include "decorator/HeapDecorator.h"

class HeapArray
{
public:
    HeapArray(HeapDecorator* decorator);

    void insert(int value);
    bool validate();

    int getNodeValue(size_t node_index);

    int getParentIndex(size_t node_index);
    int getLeftIndex(size_t node_index);
    int getRightIndex(size_t node_index);

    unsigned long getNodeCount();

protected:
    int extractRoot();          // extract root (min or max)

    virtual bool shouldSwapInHeapifyUp(int node_value, int comparison_node_value) = 0;
    virtual bool shouldSwapInHeapifyDown(int node_value, int comparison_node_value) = 0;

    std::vector<int> store_;
    HeapDecorator* decorator_;

private:
    void heapifyUp(size_t node_index);
    void heapifyDown(size_t node_index);
};

class MaxHeapArray : public HeapArray
{
public:
    MaxHeapArray(HeapDecorator* decorator) : HeapArray(decorator) {}

    int max();
    int extractMax() { return extractRoot(); }

protected:
    bool shouldSwapInHeapifyUp(int node_value, int comparison_node_value);
    bool shouldSwapInHeapifyDown(int node_value, int comparison_node_value);
};

class MinHeapArray : public HeapArray
{
public:
    MinHeapArray(HeapDecorator* decorator) : HeapArray(decorator) {}

    int min();
    int extractMin() { return extractRoot(); }

protected:
    bool shouldSwapInHeapifyUp(int node_value, int comparison_node_value);
    bool shouldSwapInHeapifyDown(int node_value, int comparison_node_value);
};

#endif //INSIGHT_ADT_HEAPARRAY_H
