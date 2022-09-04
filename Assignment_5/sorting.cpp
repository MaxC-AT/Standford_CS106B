#include "listnode.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * TODO: Replace this comment with a descriptive function
 * header comment about your implementation of the function.
 */
void addNode(ListNode*& list, ListNode*& node) {
    node->next = list;
    list = node;
}

void partition(ListNode*& front, ListNode*& left, ListNode*& mid, ListNode*& right) {
    if (front == nullptr) return;

    int pivotVal = front->data;
    ListNode* curr = front;
    while (curr != nullptr) {
        ListNode* next = curr->next;
        if (curr->data == pivotVal) addNode(mid, curr);
        if (curr->data  < pivotVal) addNode(left, curr);
        if (curr->data  > pivotVal) addNode(right, curr);
        curr = next;
    }
}


ListNode* concatenate(ListNode*& list1, ListNode*& list2, ListNode*& list3) {
    ListNode* dummy = new ListNode;
    ListNode* curr = dummy;

    if (list1 != nullptr) {
        curr->next = list1;
        while (curr->next != nullptr) curr = curr->next;
    }

    if (list2 != nullptr) {
        curr->next = list2;
        while (curr->next != nullptr) curr = curr->next;
    }

    if (list3 != nullptr) {
        curr->next = list3;
        while (curr->next != nullptr) curr = curr->next;
    }

    ListNode* trueFront = dummy->next;
    delete dummy;
    return trueFront;
}


void quickSort(ListNode*& front) {
    /* TODO: Implement this function. */
    if (front == nullptr || front->next == nullptr) return;

    ListNode* left = nullptr;
    ListNode* mid = nullptr;
    ListNode* right = nullptr;
    partition(front, left, mid, right);
    quickSort(left);
    quickSort(right);
    front = concatenate(left, mid, right);
}

/* * * * * * Test Code Below This Point * * * * * */

/* TODO: Write your own student tests below to test your sorting helper
 * functions and overall sorting algorithm correctness. We have provided
 * the skeleton of some tests below to help you get started with this
 * process.
 */

/*
 * We have provided this utility function as a convenience. This
 * function traverses the linked list represented by the provided
 * front pointer and prints out the data stored in the list along
 * the way in a convenient format. You may find it to be helpful to
 * use this function as a debugging aid.
 */
void printList(ListNode* front) {
   cout << "{";
   for (ListNode *cur = front; cur != nullptr; cur = cur->next) {
       cout << cur->data;
       if (cur->next != nullptr){
           cout << ", ";
       }
   }
   cout << "}" << endl;
}

/*
 * This utility function deallocates the memory for all the nodes in a
 * given linked list. It can be used to recycle the memory allocated
 * during a test case and avoid memory leaks.
 */
void deallocateList(ListNode* front) {
    /* TODO: Implement this function. */
    while (front != nullptr) {
        ListNode* next = front->next;
        delete front;
        front = next;
    }
}

/*
 * This utility function is given a vector of values and constructs
 * an equivalent linked list containing the same values in the same order.
 * It can be used to construct a linked list to use as an input to
 * a function being tested.
 */
ListNode* createList(Vector<int> values){
    /* TODO: Implement this function. */
    ListNode* dummy = new ListNode;
    ListNode* front = dummy;
    for (int i = 0; i < values.size(); i++) {
        ListNode* newNode = new ListNode(values[i], nullptr);
        front->next = newNode;
        front = front->next;
    }
    ListNode* trueFront = dummy->next;
    delete dummy;
    return trueFront;
}

/*
 * This utility function compares a linked list and a vector for
 * equivalence and returns true if both contain the same values in the
 * same order. It can be used to confirm a linked list's contents match
 * the expected.
 */
bool areEquivalent(ListNode* front, Vector<int> v){
    /* TODO: Implement this function. */
    ListNode* node = front;
    for (int i = 0; i < v.size(); i++) {
        if (node == nullptr || node->data != v[i]) return false;
        node = node->next;
    }
    if (node != nullptr) return false;
    return true;
}

STUDENT_TEST("This is a skeleton to demonstrate an end-to-end student test."){
    /* Initialize vector to sequence of values */
    Vector<int> values = {1, 2, 3, 4};
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

PROVIDED_TEST("This tests some of the testing helper functions you are recommended to implement."){
    /* Creates a small test list containing the values 1->2->3. */
    ListNode* testList = nullptr;
    testList = new ListNode(3, testList);
    testList = new ListNode(2, testList);
    testList = new ListNode(1, testList);

    /* Test equality comparison function. */
    EXPECT(areEquivalent(testList, {1, 2, 3}));
    EXPECT(!areEquivalent(testList, {1, 2}));
    EXPECT(!areEquivalent(testList, {1, 2, 3, 4}));
    EXPECT(!areEquivalent(testList, {1, 3, 2}));

    /* Test create list function. */
    ListNode* studentList = createList({1, 2, 3, 4});
    printList(studentList);
    ListNode* cur = studentList;
    for (int i = 1; i <= 4; i++){
        EXPECT(cur != nullptr);
        EXPECT_EQUAL(cur->data, i);
        cur = cur->next;
    }

    /* Test deallocate list functions. There should be no memory leaks from this test. */
    deallocateList(studentList);
    deallocateList(testList);
}

/*
 * The provided test case demonstrates a comparsion between the same
 * algorithm (quicksort) operating on a linked list versus a vector.
 */
PROVIDED_TEST("Time linked list quicksort vs vector quicksort") {
    int startSize = 50000;

    for(int n = startSize; n < 10*startSize; n *= 2) {
        Vector<int> v(n);
        ListNode* list = nullptr;

        /* Create linked list and vector with the same random sequence. */
        for (int i = n-1; i >= 0; i--) {
            v[i] = randomInteger(-10000, 10000);
            list = new ListNode(v[i], list);
        }

        /* NOTE: This test does not check correctness of the linked list sort
         * function. It only times the two operations to compare relative speed. */
        TIME_OPERATION(n, quickSort(list));
        TIME_OPERATION(n, v.sort());        /* Standard vector sort operation is backed
                                               with quicksort algorithm. */
        EXPECT(areEquivalent(list, v));

        deallocateList(list);
    }
}


STUDENT_TEST("createList && areEquivalent") {
    Vector<int> v1;
    ListNode* list1 = createList(v1);
    EXPECT(areEquivalent(list1, v1));

    Vector<int> v2 {1};
    ListNode* list2 = createList(v2);
    EXPECT(areEquivalent(list2, v2));
    EXPECT(!areEquivalent(list1, v2));
    EXPECT(!areEquivalent(list2, v1));

    Vector<int> v3 {1,2,3};
    ListNode* list3 = createList(v3);
    EXPECT(areEquivalent(list3, v3));

    Vector<int> v4 {1,2,3,4};
    EXPECT(!areEquivalent(list3, v4));

    Vector<int> v5 {1,2};
    EXPECT(!areEquivalent(list3, v5));

    deallocateList(list1);
    deallocateList(list2);
    deallocateList(list3);
}

STUDENT_TEST("simple quick sort test") {
    Vector<int> v1 {4,3,2,1};
    ListNode* list = createList(v1);
    quickSort(list);
    EXPECT(areEquivalent(list, {1,2,3,4}));
    deallocateList(list);

    Vector<int> v2 {1,2,3,4};
    list = createList(v2);
    quickSort(list);
    EXPECT(areEquivalent(list, {1,2,3,4}));
    deallocateList(list);


    Vector<int> v3 {1,1,1,1,1};
    list = createList(v3);
    quickSort(list);
    EXPECT(areEquivalent(list, {1,1,1,1,1}));
    deallocateList(list);

    Vector<int> v4 {1,1,1,3,2,1};
    list = createList(v4);
    quickSort(list);
    EXPECT(areEquivalent(list, {1,1,1,1,2,3}));
    deallocateList(list);

    Vector<int> v5 {-1,1,-1,3,2,1};
    list = createList(v5);
    quickSort(list);
    EXPECT(areEquivalent(list, {-1,-1,1,1,2,3}));
    deallocateList(list);
}
