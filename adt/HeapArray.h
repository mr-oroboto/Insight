#ifndef INSIGHT_HEAPARRAY_H
#define INSIGHT_HEAPARRAY_H

#include <iostream>
#include <vector>

class HeapArray
{
public:
    HeapArray() {}

    void insert(int d);
    bool validate();

    int nodeValue(size_t nodeIndex);

    int parentIndex(size_t nodeIndex);
    int leftIndex(size_t nodeIndex);
    int rightIndex(size_t nodeIndex);

protected:
    int extract();          // extract root (min or max)
    virtual bool shouldSwapInHeapifyUp(int nodeValue, int comparisonNodeValue) = 0;
    virtual bool shouldSwapInHeapifyDown(int node, int comparisonNodeValue) = 0;

    std::vector<int> store;

private:
    void heapifyUp(size_t nodeIndex);
    void heapifyDown(size_t nodeIndex);
};

class MaxHeapArray : public HeapArray
{
public:
    int max();
    int extractMax() { return extract(); }

protected:
    bool shouldSwapInHeapifyUp(int nodeValue, int comparisonNodeValue);
    bool shouldSwapInHeapifyDown(int node, int comparisonNodeValue);
};

class MinHeapArray : public HeapArray
{
public:
    int min();
    int extractMin() { return extract(); }

protected:
    bool shouldSwapInHeapifyUp(int nodeValue, int comparisonNodeValue);
    bool shouldSwapInHeapifyDown(int node, int comparisonNodeValue);
};

#endif //INSIGHT_HEAPARRAY_H
