#include "pqheap.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "datapoint.h"
#include "testing/SimpleTest.h"
using namespace std;

const int INITIAL_CAPACITY = 10;
const int NONE = -1; // used as sentinel index

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
PQHeap::PQHeap() {
    /* TODO: Implement this function. */
    _elements = new DataPoint[INITIAL_CAPACITY]();
    _capacity = INITIAL_CAPACITY;
    _numElements = 0;
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
PQHeap::~PQHeap() {
    /* TODO: Implement this function. */
    delete[] _elements;
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
void PQHeap::enqueue(DataPoint elem) {
    /* TODO: Implement this function. */
    if (size() == _capacity) expand();
    _elements[_numElements++] = elem;
    bubbleUp();
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
DataPoint PQHeap::peek() const {
    /* TODO: Implement this function. */
    if (size() == 0) return { "", 0 };
    return _elements[0];
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
DataPoint PQHeap::dequeue() {
    /* TODO: Implement this function. */
    if (size() == 0) return { "", 0 };
    swap(0, size()-1);
    DataPoint result = _elements[size()-1];
    _numElements--;
    bubbleDown();
    return result;
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
bool PQHeap::isEmpty() const {
    /* TODO: Implement this function. */
    if (size() == 0) return true;
    return false;
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
int PQHeap::size() const {
    /* TODO: Implement this function. */
    return _numElements;
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
void PQHeap::clear() {
    /* TODO: Implement this function. */
    delete[] _elements;
    _elements = new DataPoint[INITIAL_CAPACITY]();
    _capacity = INITIAL_CAPACITY;
    _numElements = 0;
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
void PQHeap::printDebugInfo(string msg) const {
    cout << msg << endl;
    /* TODO: Implement this function. */
}

/*
 * We strongly recommend implementing this helper function, which
 * should traverse the heap array and ensure that the heap property
 * holds for all elements in the array. If elements are found that
 * violate the heap property, an error should be thrown.
 */
void PQHeap::validateInternalState() const {
    /*
     * If there are more elements than spots in the array, we have a problem.
     */
    if (size() > _capacity) error("Too many elements in not enough space!");
    if (size() == 0) return;
    int loopBound = (_numElements - 1) / 2;
    for (int parent = 0; parent < loopBound; parent++) {
        int left = getLeftChildIndex(parent);
        int right = getRightChildIndex(parent);
        if (left != NONE && getPriorityByIndex(parent) > getPriorityByIndex(left)) error("PQHeap has elements out of order at index " + integerToString(left));
        if (right != NONE && getPriorityByIndex(parent) > getPriorityByIndex(right)) error("PQHeap has elements out of order at index " + integerToString(right));
    }
}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the parent of the
 * specified child index. If this child has no parent, return
 * the sentinel value NONE.
 */
int PQHeap::getParentIndex(int child) const {
    /* TODO: Implement this function. */
    if (child <= 0) return NONE;
    return (child-1)/2;
}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the left child of the
 * specified parent index. If this parent has no left child, return
 * the sentinel value NONE.
 */
int PQHeap::getLeftChildIndex(int parent) const {
    /* TODO: Implement this function. */
    if (parent < 0) return NONE;
    int leftChildIndex = parent * 2 + 1;
    if (leftChildIndex >= size()) return NONE;
    return leftChildIndex;
}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the right child of the
 * specified parent index. If this parent has no right child, return
 * the sentinel value NONE.
 */
int PQHeap::getRightChildIndex(int parent) const {
    /* TODO: Implement this function. */
    if (parent < 0) return NONE;
    int rightChildIndex = parent * 2 + 2;
    if (rightChildIndex >= size()) return NONE;
    return rightChildIndex;
}

void PQHeap::swap(int indexA, int indexB) {
    DataPoint tmp = _elements[indexA];
    _elements[indexA] = _elements[indexB];
    _elements[indexB] = tmp;
}

void PQHeap::expand() {
    _capacity *= 2;
    DataPoint* newElements = new DataPoint[_capacity]();
    for (int i = 0; i < _numElements; i++) {
        newElements[i] = _elements[i];
    }
   delete[] _elements;
    _elements = newElements;
}

double PQHeap::getPriorityByIndex(int index) const {
    if (index == NONE) return numeric_limits<float>::infinity();
    if (index >= size()) return numeric_limits<float>::infinity();
    return _elements[index].priority;
}

int PQHeap::whichChildToSwap(int parent, int leftChild, int rightChild) const {
    int smallerChild = (getPriorityByIndex(leftChild) < getPriorityByIndex(rightChild))? leftChild : rightChild;
    return (getPriorityByIndex(smallerChild) < getPriorityByIndex(parent))? smallerChild : NONE;
}

void PQHeap::bubbleUp() {
    int parent;
    int child = size()-1;
    while ((parent = getParentIndex(child)) != -1) {
        if (getPriorityByIndex(child) < getPriorityByIndex(parent)) {
            swap(child, parent);
            child = parent;
        } else break;
    }
}

void PQHeap::bubbleDown() {
    int parent = 0;
    int leftChild, rightChild, childToSwap;
    while (getLeftChildIndex(parent) != NONE) {
        leftChild = getLeftChildIndex(parent);
        rightChild = getRightChildIndex(parent);
        childToSwap = whichChildToSwap(parent, leftChild, rightChild);
        if (childToSwap != NONE) swap(parent, childToSwap);
        parent = childToSwap;
    }
}

/* * * * * * Test Cases Below This Point * * * * * */

/* TODO: Add your own custom tests here! */

/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("PQHeap example from writeup, validate each step") {
    PQHeap pq;
    Vector<DataPoint> input = {
        { "R", 4 }, { "A", 5 }, { "B", 3 }, { "K", 7 }, { "G", 2 },
        { "V", 9 }, { "T", 1 }, { "O", 8 }, { "S", 6 } };

    pq.validateInternalState();
    for (DataPoint dp : input) {
        pq.enqueue(dp);
        pq.validateInternalState();
    }
    while (!pq.isEmpty()) {
        pq.dequeue();
        pq.validateInternalState();
    }
}
