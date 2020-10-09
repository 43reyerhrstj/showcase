/*
 * 
 * You will need to write your B+Tree almost entirely from scratch. 
 * 
 * B+Trees are dynamically balanced tree structures that provides efficient support for insertion, deletion, equality, and range searches. 
 * The internal nodes of the tree direct the search and the leaf nodes hold the base data..
 * 
 * For a basic rundown on B+Trees, we will refer to parts of Chapter 10 of the textbook Ramikrishnan-Gehrke 
 * (all chapters and page numbers in this assignment prompt refer to the 3rd edition of the textbook).
 *
 * Read Chapter 10 which is on Tree Indexing in general. In particular, focus on Chapter 10.3 on B+Tree.
 */

#ifndef BTREE_H
#define BTREE_H

#define FANOUT 339  // Fan-out of the B+Tree. sizeof(Node) is 4088<4096
#define CAPACITY 0.9  // Maximum capacity of a Node before it splits


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "data_types.h"
#include "query.h"

/*
Designing your C Structs for B+Tree nodes (Chapter 10.3.1)
How will you represent a B+Tree as a C Struct (or series of C structs that work together)? There are many valid ways to do this part of your design, and we leave it open to you to try and tune this as you progress through the project.
How will you account for a B+Tree node being an internal node or a leaf node? Will you have a single node type that can conditionally be either of the two types, or will you have two distinct struct types?
How many children does each internal node have? This is called the fanout of the B+Tree.
What is the maximum size for a leaf node? How about for an internal node?
What is the minimum threshold of content for a node, before it has to be part of a rebalancing?
*/


/* This is a Node in the tree. A root is a Node, and a leaf is also a Node.
 * key : Stores the keys of the values in the case of a leaf node, otherwise, stores pivots.
 * children : Stores pointer to data in the case of a leaf node, otherwise, stores pointers to children Nodes.
 * adjacent : Stores a pointer to the adjacent node. Used by scan.
 * isLeaf : Indicates if the node is a leaf.
 * numStored : Stores the number of key/children pairs stored.
 * */

typedef struct Node{
    bool isLeaf;
    int key[FANOUT];
    void * children[FANOUT];
    struct Node * adjacent;
    int numStored;
} Node;

/* The following are methods that can be invoked on B+Tree node(s).
 * Hint: You may want to review different design patterns for passing structs into C functions.
 */

/* FIND (Chapter 10.4)
This is an equality search for an entry whose key matches the target key exactly.
How many nodes need to be accessed during an equality search for a key, within the B+Tree? 
*/


// A simple binary search function through a list of integers.
int BinarySearch(int const list[], int val, int start, int end){
    int middle;
    while (start <= end) {
        middle = (start + end) / 2;
        if (list[middle] == val) {
            return middle;
        }
        if (list[middle] > val) {
            end = middle - 1;
        } else {
            start = middle + 1;
        }
    }
    return -1;
}

int Get(Node * node, int key){
    // if node is NULL, which means that on the last pass childrenIndex was -1
    if (node == NULL) {
        printf("\n");
        return 0;
    }
    // If it is a leaf, prints the result.
    if (node->isLeaf) {
        int index = BinarySearch(node->key, key, 0, node->numStored);
        if(index == -1) {
            printf("\n");
            return 0;
        }
       printf("%d\n", *((int*) node->children[index]));
       return 0;
    }
    // Find the children I want to go down to
    int childrenIndex = -1;
    for (int i = 0; i < node->numStored; i++) {
            childrenIndex += (key >= node->key[i]);
    }
    // Recurse down the tree.
    return Get(node->children[childrenIndex], key);
}

/* INSERT (Chapter 10.5)
How does inserting an entry into the tree differ from finding an entry in the tree?
When you insert a key-value pair into the tree, what happens if there is no space in the leaf node? What is the overflow handling algorithm?
For Splitting B+Tree Nodes (Chapter 10.8.3)
*/
Node * NewTree(){
    Node * root = malloc(sizeof(Node));
    root->isLeaf = 0;
    root->numStored = 0;
    root->adjacent = NULL;
    return root;
}

// Splits a node into two nodes, dividing its data.
void Split(Node * parent, int i){
    // Defining old node and new node
    Node * node = parent->children[i];
    Node * newNode = malloc(sizeof(Node));
    // Altering meta data
    newNode->isLeaf = node->isLeaf;
    newNode->adjacent = node->adjacent;
    node->adjacent = newNode;
    int oldNumStored = node->numStored;
    node->numStored = oldNumStored / 2;
    newNode->numStored = oldNumStored - oldNumStored / 2;
    // Moving data
    int k = 0;
    for (int j = oldNumStored / 2; j < oldNumStored; j++) {
        newNode->key[k] = node->key[j];
        newNode->children[k] = node->children[j];
        node->children[j] = NULL;
        k++;
    }
    // Redefining Parent Anchors
    for (int j = parent->numStored - 1; j > i; j--) {
        parent->key[j+1] = parent->key[j];
        parent->children[j+1] = parent->children[j];
    }
    parent->key[i+1] = newNode->key[0];
    parent->children[i+1] = newNode;
    parent->numStored++;
}


// Goes Down the tree, splitting nodes if at capacity, and finding the insertion leaf of the key.
// This is the recursive function called by Put
Node * GetLeafAndSplit(Node * node, int key){
    // If node is a leaf, returns the leaf
    if (node->isLeaf) {
        return node;
    }
    // Determine which children to go down
    int childrenIndex = -1;
    for (int i = 0; i < node->numStored; i++) {
        childrenIndex += (key >= node->key[i]);
    }
        childrenIndex += (childrenIndex == -1);
    // If children is almost full, do a split
    if (((Node*) (node->children[childrenIndex]))->numStored > FANOUT * CAPACITY) {
        Split(node, childrenIndex);
        childrenIndex += (key >= (node->key[childrenIndex + 1])); // Determine which of the daughter nodes to go down.
    }
    // Alter the first anchor if the new key is less then this key
    if (key < node->key[0]) {
        node->key[0] = key;
    }
    // Recurse down the tree.
    return GetLeafAndSplit(node->children[childrenIndex], key);
}


// Inserts a key-value pair.
// Returns pointer to the root.
Node * Put(Node * root, int key, int * val){
    // If the tree is empty, create the first root and insert the first value
    if (!(root->numStored)) {
        Node *leaf = malloc(sizeof(Node));
        leaf->isLeaf = 1;
        leaf->numStored = 1;
        leaf->adjacent = NULL;
        leaf->key[0] = key;
        leaf->children[0] = val;
        root->numStored = 1;
        root->key[0] = key;
        root->children[0] = leaf;
        return root;
    }
    // Create a new root if root node is beyond capacity
    if (root->numStored > (int) FANOUT * CAPACITY) {
        Node * newRoot = malloc(sizeof(Node));
        newRoot->children[0] = root;
        newRoot->key[0] = root->key[0];
        newRoot->numStored = 1;
        newRoot->isLeaf = 0;
        root = newRoot;
    }
    // Calls GetLeafAndSplit
    Node * leaf = GetLeafAndSplit(root, key);
    // Finds position of insertion
    int insertPos = 0;
    for (int i = 0; i < leaf->numStored; i++) {
            insertPos += leaf->key[insertPos] < key;
    }
    // If key is found
    if (leaf->key[insertPos] == key) {
        leaf->children[insertPos] = val;
        return root;
    }
    // Shifting old values
    for (int i = leaf->numStored - 1; i >= insertPos; i--) {
        leaf->key[i + 1] = leaf->key[i];
        leaf->children[i + 1] = leaf->children[i];
    }
    // Inserting new value
    leaf->key[insertPos] = key;
    leaf->children[insertPos] = val;
    leaf->numStored++;
    // Returns back the root.
    return root;
}

// To free up memory of a B+Tree
void FreeMem(Node * node){
    if(node->isLeaf){
        for(int i = 0; i < (node->numStored); i++){
            free(node->children[i]);
        }
        free(node);
    } else {
        for (int i = 0; i < (node->numStored); i++) {
            FreeMem(node->children[i]);
        }
        free(node);
    }
}

#endif