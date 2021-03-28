/*******************************************************************************
*
* Purpose: Binary search tree demonstration.
*
* Description: This binary search tree contains no duplicates.  This file 
* contains all functions created during the week 8 lectures and week 9 
* tutorials.
*
* Author: Kevin Browne
*
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <limits.h>

// Defines the binary search tree type
typedef struct node {
  int key;
  struct node *left_child;
  struct node *right_child; 
} bstNode;


// Functions created during week 8 lectures 
void simple_print(bstNode *node, int depth);
bool verifyBST(bstNode *node, int minkey, int maxkey);
bstNode* create_node(int new_key);
bstNode *insert(bstNode *node, int insert_key, bool *was_inserted);
int find_minimum(bstNode *node);
int find_maximum(bstNode *node);
bool is_member(bstNode *node, int find_key);
bstNode *delete(bstNode *node, int delete_key, bool *was_deleted);

// Functions created during week 9 labs
void pre_order(bstNode *node);
void in_order(bstNode *node);
void reverse_in_order(bstNode *node);
void post_order(bstNode *node);
int num_nodes(bstNode *node);
void breadth_first(bstNode *node);

int main()
{
  // Creates a BST identical to this one:
  // https://upload.wikimedia.org/wikipedia/commons/d/da/Binary_search_tree.svg
  bstNode *tree1 = NULL;
  bool was_inserted = false;
  int to_insert[] = {8,3,10,1,6,14,4,7,13};
  for (int i = 0; i < 9; i++) tree1 = insert(tree1, to_insert[i], &was_inserted);

  // Depth-first traversal examples
  printf("\n\nsimple_print: \n");
  simple_print(tree1, 0);

  printf("\n\nPre-order traversal: ");
  pre_order(tree1);
  
  printf("\n\nIn-order traversal: ");
  in_order(tree1);

  printf("\n\nReverse in-order traversal: ");
  reverse_in_order(tree1);

  printf("\n\nPost-order traversal: ");
  post_order(tree1);
  printf("\n\n");


  // Breadth-first travesal example
  printf("\n\nsimple_print: \n");
  simple_print(tree1, 0);

  printf("\n\nBreadth-first traversal: ");
  breadth_first(tree1);
  printf("\n\n");
}

// Returns the total number of nodes in a BST
int num_nodes(bstNode *node)
{
  if (node == NULL) return 0;
  else return 1 + num_nodes(node->right_child) + num_nodes(node->left_child);
}

// Breadth-first traveral of a BST
void breadth_first(bstNode *node)
{
  // nothing needs to be done for an empty tree
  if (node == NULL) return ;

  // Create a queue large enough to hold pointers to all nodes in the BST
  // set the front to the first element and rear to the next, and put the 
  // root node as the current node in the queue.
  int total = num_nodes(node);
  bstNode **queue = calloc(total, sizeof(bstNode*));
  int front = 0, rear = 1;
  
  // For each node, place its left and right children on he queue, and then 
  // increment the front of the queue
  queue[front] = node;
  while (front < total)
  {
    printf("%d ", queue[front]->key);
    if (queue[front]->left_child != NULL)
    {
      queue[rear] = queue[front]->left_child;
      rear++;
    }
    if (queue[front]->right_child != NULL)
    {
      queue[rear] = queue[front]->right_child;
      rear++;
    }
    front++;
  }
}

// Prints out a BST using a pre-order depth-first traversal
void pre_order(bstNode *node)
{
  if (node == NULL) return ;
  printf ("%d ", node->key);
  pre_order(node->left_child);
  pre_order(node->right_child);
}

// Prints out a BST using a in-order depth-first traversal
void in_order(bstNode *node)
{
  if (node == NULL) return ;
  in_order(node->left_child);
  printf ("%d ", node->key);
  in_order(node->right_child);
}

// Prints out a BST using a reverse in-order depth-first traversal
void reverse_in_order(bstNode *node)
{
  if (node == NULL) return ;
  reverse_in_order(node->right_child);
  printf ("%d ", node->key);
  reverse_in_order(node->left_child);
}

// Prints out a BST using a post-order depth-first traversal
void post_order(bstNode *node)
{
  if (node == NULL) return ;
  post_order(node->left_child);
  post_order(node->right_child);
  printf ("%d ", node->key);
}


// Prints out a BST using depth to offset the key
void simple_print(bstNode *node, int depth)
{
  if (node == NULL) return ;
  for (int i = 0; i < (depth * 3); i++) printf(" ");
  printf ("%d\n", node->key);
  simple_print(node->left_child, depth + 1);
  simple_print(node->right_child, depth + 1);
}

// Verifies that a tree defined with bstNode actually conforms to BST 
// requirements.  It works by progressively tightening the acceptable range 
// of values for a key as it rescursively traverses the tree, by setting a 
// new max and/or min depending on the value of the current key and the 
// direction of the travesal.  If ever a key is out of range, we return 
// false, and if we encounter leaf nodes (or an empty tree) we return true, 
// and by continually "and-ing" the results we're able to verify the structure.
bool verifyBST(bstNode *node, int minkey, int maxkey) {
    if (node == NULL) return true;
    if (node->key < minkey || node->key > maxkey) return false;    
    return verifyBST(node->left_child, minkey, node->key - 1) && 
           verifyBST(node->right_child, node->key + 1, maxkey);
}

// Creates a new BST node with the given key
bstNode* create_node(int new_key)
{
  bstNode *newNode = calloc(1, sizeof(bstNode));
  newNode->key = new_key;
  newNode->right_child = NULL;
  newNode->left_child = NULL;
  return newNode;
}

// Inserts a node in a BST with insert_key.  Sets was_inserted to true if a new 
// node actually had to be inserted.
bstNode *insert(bstNode *node, int insert_key, bool *was_inserted)
{
  *was_inserted = false;

  // In the case of an empty tree, or that we have reached a NULL pointer, we 
  // create the node and return it
  if (node == NULL)
  {
    *was_inserted = true;
    return create_node(insert_key);
  }
  // If the key to insert is greater than the current key, traverse the right
  // subtree to continue to look for the correct position to insert the key.
  // We keep setting the node pointers as we traverse the tree because we 
  // may modify the pointers... e.g. if right_child is NULL in this case, then 
  // on the next function call we'll be returning a new node (with the above) 
  // code, and we'll want right_child to point to that new node.  Otherwise 
  // we'll want it to keep pointing to the node it was pointing to before, 
  // which is achieved with the return node statement below.  
  else if (insert_key > node->key)
  {
    node->right_child = insert(node->right_child, insert_key, was_inserted);
  }
  // Same idea as above, except we traverse the left subtree if the key to 
  // insert is less than the current node's key.
  else if (insert_key < node->key)
  {
    node->left_child = insert(node->left_child, insert_key, was_inserted);
  }

  // Returns the current node, this run if we are traversing the tree and 
  // are returning the existing left or right child node of another node, 
  // OR if we have found a node where insert_key == node->key in which case 
  // we are not actually needing to create a new node.
  return node;
}

// Finds the minimum by repeatedly traversing the left subtree until we cannot
// anymore. 
int find_minimum(bstNode *node)
{
  // Handles an empty BST with a printf and returning INT_MIN... this isn't the 
  // only way we could handle this, but this is one acceptable way.
  if (node == NULL)
  {
    printf("ERROR: Cannot find minimum key of an empty BST.");
    return INT_MIN;
  }

  if (node->left_child == NULL) return node->key;
  else return find_minimum(node->left_child);
}

// Finds the maximum by repeatedly traversing the right subtree until we cannot
// anymore. 
int find_maximum(bstNode *node)
{
  // Handles an empty BST with a printf and returning INT_MAX... this isn't the 
  // only way we could handle this, but this is one acceptable way.  
  if (node == NULL)
  {
    printf("ERROR: Cannot find maximum key of an empty BST.");
    return INT_MAX;
  }

  if (node->right_child == NULL) return node->key;
  else return find_maximum(node->right_child);
}

// Determines whether or not a key is a member of a BST or not.  It traverses 
// the tree until it finds where the node "should" be either because it finds 
// the node with that key, or it finds a NULL (either an empty list, or an 
// empty left or child child pointer).
bool is_member(bstNode *node, int find_key)
{
  if (node == NULL) return false;
  else if (find_key > node->key) return is_member(node->right_child, find_key);
  else if (find_key < node->key) return is_member(node->left_child, find_key);
  else return true;
}

// Deletes a node with delete_key from a tree if it exists.  Sets was_deleted 
// true if a deletion actually occurs and false otherwise.
bstNode *delete(bstNode *node, int delete_key, bool *was_deleted)
{
  *was_deleted = false;
  // in the case of an empty tree we can simple return false
  if (node == NULL) return NULL;

  // If the key we are attempting to delete is greater than the current node's 
  // key than we traverse the right-subtree.  As with insert we're effectively 
  // "re-setting" the pointers as we traverse the tree given that our delete 
  // operation may alter the tree structure (like deleting a node).
  if (delete_key > node->key)
  {
    node->right_child = 
      delete(node->right_child, delete_key, was_deleted);
  }
  // The same as above, except we traverse the left-subtree in the case the 
  // key we are attempting to delete is lower than the current node's key.
  else if (delete_key < node->key)
  {
    node->left_child = 
      delete(node->left_child, delete_key, was_deleted);    
  }
  // in this case, we need to delete a node because we have found the key
  else
  {
    // if the node has no children, it is simple, we free the node, return NULL
    if (node->left_child == NULL && node->right_child == NULL)
    {
      *was_deleted = true;
      free(node);
      return NULL;
    }
    // if the node has one RIGHT child, we return that child, and free the node
    else if (node->left_child == NULL)
    {
      *was_deleted = true;
      bstNode *temp = node->right_child;
      free(node);
      return temp;
    }
    // if the node has one LEFT child, we return that child, and free the node
    else if (node->right_child == NULL)
    {
      *was_deleted = true;
      bstNode *temp = node->left_child;
      free(node);
      return temp;
    }
    // If the node has 2 children we replace it with the minimum value of the 
    // node's right-subtree.  We do this because this value is known to be 
    // higher than all of the node's left subtree values, AND lower than all 
    // of the node's (other) right subtree values.  After setting the node to 
    // this new key value, we delete the node with that key from the right 
    // subtree (using the same function), and return the node.
    else 
    {
      *was_deleted = true;
      int min_of_right_subtree = find_minimum(node->right_child);
      node->key = min_of_right_subtree;
      node->right_child = 
        delete(node->right_child, min_of_right_subtree, was_deleted);
      return node;
    }
  }

  // Will be returned as we traverse the tree in the case that no deletion 
  // occurs... the left_child or right_child of the node that called this 
  // function will be set to the previous left_child or right_child in this
  // case.
  return node;
}