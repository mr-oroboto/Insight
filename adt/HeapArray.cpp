#include "HeapArray.h"

namespace insight {
namespace adt {

HeapArray::HeapArray(decorator::HeapDecorator* decorator)
{
    decorator_ = decorator;

    if (decorator_)
    {
        decorator_->setHeap(this);
    }
}

unsigned long HeapArray::getNodeCount()
{
    return store_.size();
}

int HeapArray::getNodeValue(size_t node_index)
{
    assert(node_index < store_.size());

    return store_[node_index];
}

int HeapArray::getParentIndex(size_t node_index)
{
    if (store_.size() == 0)
    {
        return -1;      // empty, ensure we don't explode on removal of root when it's the only node left
    }

    if (node_index == 0)
    {
        // the root has no parent
        return -1;
    }

    return (node_index - 1) / 2;
}

int HeapArray::getLeftIndex(size_t node_index)
{
    if (store_.size() == 0)
    {
        return -1;      // empty, ensure we don't explode on removal of root when it's the only node left
    }

    int left_index = node_index + (node_index + 1);

    if (left_index > (store_.size() - 1))
    {
        return -1;      // no left branch
    }

    return left_index;
}

int HeapArray::getRightIndex(size_t node_index)
{
    if (store_.size() == 0)
    {
        return -1;      // empty, ensure we don't explode on removal of root when it's the only node left
    }

    int right_index = node_index + (node_index + 2);

    if (right_index > (store_.size() - 1))
    {
        return -1;      // no right branch
    }

    return right_index;
}

/**
 * Insert a new value into the heap.
 *
 * This involves finding the next insertion position, putting the value there and then bubbling it up the heap until
 * the heap property (min or max) is restored.
 *
 * In our array implementation the next insertion position is the element after the last element of the array.
 */
void HeapArray::insert(int value)
{
    size_t insert_index = store_.size();
    store_.push_back(value);

    if (decorator_)
    {
        decorator_->setHightlightedNodeValue(value);
        decorator_->decorate();
    }

    heapifyUp(insert_index);
}

void HeapArray::heapifyUp(size_t node_index)
{
    int parent_index = getParentIndex(node_index);

    if (parent_index >= 0)
    {
        if (shouldSwapInHeapifyUp(store_[node_index], store_[parent_index]))
        {
            // swap them
            int temp = store_[parent_index];
            store_[parent_index] = store_[node_index];
            store_[node_index] = temp;

            if (decorator_)
            {
                decorator_->decorate();
            }

            heapifyUp(parent_index);
        }
    }
}

bool MaxHeapArray::shouldSwapInHeapifyUp(int node_value, int comparison_node_value)
{
    return node_value > comparison_node_value;
}

bool MinHeapArray::shouldSwapInHeapifyUp(int node_value, int comparison_node_value)
{
    return node_value < comparison_node_value;
}

void HeapArray::heapifyDown(size_t node_index)
{
    int left_index = getLeftIndex(node_index);
    int right_index = getRightIndex(node_index);
    int index_of_node_to_swap_with = left_index;

    if (right_index > 0)
    {
        // We have a complete tree, so if we have a right branch, we have a left branch too. We should swap the value
        // (if at all) with the larger or smaller of our two nodes depending on the heap property (min or max).
        if ( ! shouldSwapInHeapifyDown(store_[right_index], store_[left_index]))
        {
            index_of_node_to_swap_with = right_index;
        }
    }

    if (index_of_node_to_swap_with > 0)       // we may not have had a left node
    {
        if (shouldSwapInHeapifyDown(store_[node_index], store_[index_of_node_to_swap_with]))
        {
            int temp = store_[node_index];
            store_[node_index] = store_[index_of_node_to_swap_with];
            store_[index_of_node_to_swap_with] = temp;

            if (decorator_)
            {
                decorator_->decorate();
            }

            heapifyDown(index_of_node_to_swap_with);
        }
    }
}

bool MaxHeapArray::shouldSwapInHeapifyDown(int node_value, int comparison_node_value)
{
    return node_value < comparison_node_value;
}

bool MinHeapArray::shouldSwapInHeapifyDown(int node_value, int comparison_node_value)
{
    return node_value > comparison_node_value;
}

/**
 * Find the root and remove it.
 *
 * To remove the root we find the bottom-most, right-most element, put it at the top of the heap (the old min/max value),
 * remove the node we just took the value from and then bubble the "new" root down the tree until the heap property is
 * restored.
 */
int HeapArray::extractRoot()
{
    assert(store_.size());

    int root = store_[0];

    int node_value_to_remove = store_[store_.size() - 1];       // bottom-most, right-most

    std::cout << "swapping " << root << " with " << node_value_to_remove << std::endl;

    store_[0] = node_value_to_remove;
    store_.pop_back();

    if (decorator_)
    {
        decorator_->decorate();
    }

    heapifyDown(0);

    return root;
}

bool HeapArray::validate()
{
    bool valid = true;

    for (size_t i = 0; i < store_.size(); i++)
    {
        int parent_index = getParentIndex(i);
        if (parent_index >= 0)
        {
            if (shouldSwapInHeapifyUp(store_[i], store_[parent_index]))
            {
                std::cerr << "validation failed on " << store_[i] << std::endl;
                valid = false;
            }
        }
    }

    return valid;
}

int MaxHeapArray::max()
{
    assert(store_.size());

    return store_[0];
}

int MinHeapArray::min()
{
    assert(store_.size());

    return store_[0];
}

}    // namespace adt
}    // namespace insight