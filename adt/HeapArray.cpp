#include "HeapArray.h"
#include <assert.h>

int HeapArray::nodeValue(size_t nodeIndex)
{
    assert(nodeIndex < store.size());

    return store[nodeIndex];
}

int HeapArray::parentIndex(size_t nodeIndex)
{
    if (store.size() == 0)
    {
        return -1;      // empty, ensure we don't explode on removal of root when it's the only node left
    }

    if (nodeIndex == 0)
    {
        // the root has no parent
        return -1;
    }

    return (nodeIndex - 1) / 2;
}

int HeapArray::leftIndex(size_t nodeIndex)
{
    if (store.size() == 0)
    {
        return -1;      // empty, ensure we don't explode on removal of root when it's the only node left
    }

    int leftIndex = nodeIndex + (nodeIndex + 1);

    if (leftIndex > (store.size() - 1))
    {
        return -1;      // no left branch
    }

    return leftIndex;
}

int HeapArray::rightIndex(size_t nodeIndex)
{
    if (store.size() == 0)
    {
        return -1;      // empty, ensure we don't explode on removal of root when it's the only node left
    }

    int rightIndex = nodeIndex + (nodeIndex + 2);

    if (rightIndex > (store.size() - 1))
    {
        return -1;      // no right branch
    }

    return rightIndex;
}

/**
 * Insert a new value into the heap.
 *
 * This involves finding the next insertion position, putting the value there and then bubbling it up the heap until
 * the heap property (min or max) is restored.
 *
 * In our array implementation the next insertion position is the element after the last element of the array.
 */
void HeapArray::insert(int d)
{
    size_t insertIndex = store.size();
    store.push_back(d);

    heapifyUp(insertIndex);
}

void HeapArray::heapifyUp(size_t nodeIndex)
{
    int parentIdx = parentIndex(nodeIndex);

    if (parentIdx >= 0)
    {
        if (shouldSwapInHeapifyUp(store[nodeIndex], store[parentIdx]))
        {
            // swap them
            int temp = store[parentIdx];
            store[parentIdx] = store[nodeIndex];
            store[nodeIndex] = temp;
            heapifyUp(parentIdx);
        }
    }
}

bool MaxHeapArray::shouldSwapInHeapifyUp(int nodeValue, int comparisonNodeValue)
{
    return nodeValue > comparisonNodeValue;
}

bool MinHeapArray::shouldSwapInHeapifyUp(int nodeValue, int comparisonNodeValue)
{
    return nodeValue < comparisonNodeValue;
}

void HeapArray::heapifyDown(size_t nodeIndex)
{
    int leftIdx = leftIndex(nodeIndex);
    int rightIdx = rightIndex(nodeIndex);
    int idxOfNodeToSwapWith = leftIdx;

    if (rightIdx > 0)
    {
        // We have a complete tree, so if we have a right branch, we have a left branch too. We should swap the value
        // (if at all) with the larger or smaller of our two nodes depending on the heap property (min or max).
        if ( ! shouldSwapInHeapifyDown(store[rightIdx], store[leftIdx]))
        {
            idxOfNodeToSwapWith = rightIdx;
        }
    }

    if (idxOfNodeToSwapWith > 0)       // we may not have had a left node
    {
        if (shouldSwapInHeapifyDown(store[nodeIndex], store[idxOfNodeToSwapWith]))
        {
            int temp = store[nodeIndex];
            store[nodeIndex] = store[idxOfNodeToSwapWith];
            store[idxOfNodeToSwapWith] = temp;
            heapifyDown(idxOfNodeToSwapWith);
        }
    }
}

bool MaxHeapArray::shouldSwapInHeapifyDown(int nodeValue, int comparisonNodeValue)
{
    return nodeValue < comparisonNodeValue;
}

bool MinHeapArray::shouldSwapInHeapifyDown(int nodeValue, int comparisonNodeValue)
{
    return nodeValue > comparisonNodeValue;
}

/**
 * Find the root and remove it.
 *
 * To remove the root we find the bottom-most, right-most element, put it at the top of the heap (the old min/max value),
 * remove the node we just took the value from and then bubble the "new" root down the tree until the heap property is
 * restored.
 */
int HeapArray::extract()
{
    assert(store.size());

    int root = store[0];

    int nodeToRemove = store[store.size() - 1];       // bottom-most, right-most

    std::cout << "swapping " << root << " with " << nodeToRemove << std::endl;

    store[0] = nodeToRemove;

    store.pop_back();

    heapifyDown(0);

    return root;
}

bool HeapArray::validate()
{
    bool valid = true;

    for (size_t i = 0; i < store.size(); i++)
    {
        int parentIdx = parentIndex(i);
        if (parentIdx >= 0)
        {
            if (shouldSwapInHeapifyUp(store[i], store[parentIdx]))
            {
                std::cerr << "validation failed on " << store[i] << std::endl;
                valid = false;
            }
        }
    }

    return valid;
}

int MaxHeapArray::max()
{
    assert(store.size());

    return store[0];
}

int MinHeapArray::min()
{
    assert(store.size());

    return store[0];
}
