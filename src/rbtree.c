#include "rbtree.h"

#include <stdio.h>
#include <stdlib.h>

rbtree *new_rbtree(void) {

  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));       // rbtree를 위한 메모리 할당
  
  // rbtree를 위한 메모리 할당 실패 시 예외 처리
  if (p == NULL)
  {
    fprintf(stderr, "Memory allocation failed\n");       // stderr: 표준 에러 출력 스트림
    exit(EXIT_FAILURE);                                  // EXIT_FAILURE: C 프로그래밍 언어에서 프로그램이 비정상적으로 종료될 때 사용되는 매크로 상수 (1)
  }

  node_t *NIL = (node_t *)calloc(1, sizeof(node_t));

  if (NIL == NULL)
  {
      // NIL 노드를 위한 메모리 할당 실패 시 예외 처리
      fprintf(stderr, "Memory allocation failed\n");
      exit(EXIT_FAILURE);
  }
  p->nil = NIL;
  p->nil->color = RBTREE_BLACK;                          // NIL 노드 초기화
  // p->nil->left =  NULL; 
  // p->nil->right =  NULL;
  // p->nil->parent =  NULL;                             // nil 노드의 부모를 자기 자신으로 설정 (또는 NULL을 가리기케 하는 방법도 있음)

  // 루트 노드 초기화
  p->root = NIL;

  return p;
}

void rbtree_postorder_node_delete(rbtree *t, node_t *ptr)
{
  if (ptr == t->nil) return;
  rbtree_postorder_node_delete(t, ptr->left);
  rbtree_postorder_node_delete(t, ptr->right);

  free(ptr);
}

void delete_rbtree(rbtree *t) {
  if (t == NULL) return;

  // TODO: reclaim the tree nodes's memory
  rbtree_postorder_node_delete(t, t->root);

  free(t->nil);
  free(t);
}

void rbtree_left_rotate(rbtree *t, node_t *x)
{
    node_t *y;
    y = x -> right;
    x -> right = y -> left;
    if (y -> left != t -> nil)
    {
        y -> left -> parent = x;
    }
    y -> parent = x -> parent;
    if (x -> parent == t -> nil)
    {
        t -> root = y;
    }
    else if (x == x -> parent -> left)
    {
        x -> parent -> left = y;
    }
    else
    {
        x -> parent -> right = y;
    }
    y -> left = x;
    x -> parent = y;
    return;
}

void rbtree_right_rotate(rbtree *t, node_t *x)
{
    node_t *y;
    y = x -> left;
    x -> left = y -> right;
    if (y -> right != t -> nil)
    {
        y -> right -> parent = x;
    }
    y -> parent = x -> parent;
    if (x -> parent == t -> nil)
    {
        t -> root = y;
    }
    else if (x == x -> parent -> right)
    {
        x -> parent -> right = y;
    }
    else
    {
        x -> parent -> left = y;
    }
    y -> right = x;
    x -> parent = y;
    return;
}

void rbtree_insert_fixup(rbtree *t, node_t *z){
    node_t *uncle;
    // while ((z != t->root) && (z->color != RBTREE_BLACK) && (z->parent->color == RBTREE_RED))
    while (z->parent->color == RBTREE_RED)
    {
        if (z -> parent == z -> parent -> parent -> left)
        {
            uncle = z -> parent -> parent -> right;
            //경우1
            if (uncle -> color == RBTREE_RED)
            {
                z -> parent -> color = RBTREE_BLACK;
                uncle -> color = RBTREE_BLACK;
                z -> parent -> parent -> color = RBTREE_RED;
                z = z -> parent -> parent;
            }
            //경우2
            else {
                if (z == z -> parent -> right)
                {
                    z = z -> parent;
                    rbtree_left_rotate(t, z);
                }
                //경우3
                z -> parent -> color = RBTREE_BLACK;
                z -> parent -> parent -> color = RBTREE_RED;
                rbtree_right_rotate(t, z -> parent -> parent);
            }
        }
        //반대로
        else
        {
            uncle = z -> parent -> parent -> left;
            //경우1
            if (uncle != t -> nil && uncle -> color == RBTREE_RED)
            {
                z -> parent -> color = RBTREE_BLACK;
                uncle -> color = RBTREE_BLACK;
                z -> parent -> parent -> color = RBTREE_RED;
                z = z -> parent -> parent;
            }
            //경우2
            else {
                if (z == z -> parent -> left)
                {
                    z = z -> parent;
                    rbtree_right_rotate(t, z);
                }
                //경우3
                if (z != t -> root && z -> parent != t -> root)
                {
                    z -> parent -> color = RBTREE_BLACK;
                    z -> parent -> parent->color = RBTREE_RED;
                    rbtree_left_rotate(t, z -> parent -> parent);
                }
            }
        }
    }
    t -> root -> color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert

  node_t *parent = t->nil;
  node_t *ptr = t->root;
  node_t *z = (node_t *)calloc(1, sizeof(node_t));      // node_t 위한 메모리 할당
  
  if (z == NULL) {                                      // node_t를 위한 메모리 할당 실패 시 예외 처리
        fprintf(stderr, "Memory allocation failed\n");  // stderr: 표준 에러 출력 스트림
        exit(EXIT_FAILURE);   
  }

  z->key = key;                                         // 새롭게 삽입할 노드의 key 설정

  while (ptr != t->nil)
  {
    //if (ptr == NULL) break;

    parent = ptr;                                       // 반복문 첫 번째 시행 시, z의 부모 노드는 잠정적으로 루트 노드인 x
    if (z->key < ptr->key)  ptr = ptr->left;            // pointer를 x의 left로 변경
    else                    ptr = ptr->right;           // pointer를 x의 right로 변경
  }

  z->parent = parent;
  if (parent == t->nil)           t->root = z;
  else if (z->key < parent->key)  parent->left = z;
  else                            parent->right = z;

  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;

  rbtree_insert_fixup(t, z);
  
  return z;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *ptr = t->root;
  
  while (ptr != t->nil)
  {
    if (ptr->key > key)         ptr = ptr->left;
    else if (ptr-> key < key)   ptr = ptr->right;
    else                        return ptr;
  }

  return NULL;
}

  node_t *rbtree_min(const rbtree *t) {
    // TODO: implement find
    node_t *ptr = t->root;

    if (ptr == t->nil)          return ptr;
    while (ptr->left != t->nil) ptr = ptr->left;

    return ptr;
  }

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *ptr = t->root;
  
  if (ptr == t->nil)           return ptr;
  while (ptr->right != t->nil) ptr = ptr->right;

  return ptr;
}

void rbtree_transplant(rbtree *t, node_t *u, node_t *v)
{
  if (u->parent == t->nil)       t->root = v;
  else if (u == u->parent->left) u->parent->left = v;
  else                           u->parent->right = v;

  v->parent = u->parent;
}

node_t *subtree_min(const rbtree *t, node_t *sub_root) {
  // TODO: implement find
  node_t *ptr = sub_root;

  if (ptr == t->nil) return ptr;
  while (ptr->left != t->nil) ptr = ptr->left;

  return ptr;
}

node_t *subtree_max(const rbtree *t, node_t *sub_root) {
  // TODO: implement find
  node_t *ptr = t->root;
  
  if (ptr == t->nil) return ptr;
  while (ptr->right != t->nil) ptr = ptr->right;

  return ptr;
}

void rb_delete_fixup(rbtree *t, node_t *x){
    node_t *w;
    while ((x != t -> root) && (x -> color == RBTREE_BLACK))
    {
        if (x == x -> parent -> left)
        {
            w = x -> parent -> right;
            if (w -> color == RBTREE_RED)
            {
                w -> color = RBTREE_BLACK;
                x -> parent -> color = RBTREE_RED;
                rbtree_left_rotate(t, x -> parent);
                w = x -> parent -> right;
            }
            if (w -> left -> color == RBTREE_BLACK && w -> right -> color == RBTREE_BLACK)
            {
                w -> color = RBTREE_RED;
                x = x -> parent;
            }
            else
            {
                if (w -> right -> color == RBTREE_BLACK)
                {
                    w -> left -> color = RBTREE_BLACK;
                    w -> color = RBTREE_RED;
                    rbtree_right_rotate(t, w);
                    w = x -> parent -> right;
                }
                w -> color = x -> parent -> color;
                x -> parent -> color = RBTREE_BLACK;
                w -> right -> color = RBTREE_BLACK;
                rbtree_left_rotate(t, x -> parent);
                x = t->root;
            }
        }
        else
        {
            w = x -> parent -> left;
            if (w -> color == RBTREE_RED)
            {
                w -> color = RBTREE_BLACK;
                x -> parent->color = RBTREE_RED;
                rbtree_right_rotate(t, x -> parent);
                w = x -> parent->left;
            }
            if (w -> right -> color == RBTREE_BLACK && w -> left -> color == RBTREE_BLACK)
            {
                w -> color = RBTREE_RED;
                x = x -> parent;
            }
            else
            {
                if (w -> left -> color == RBTREE_BLACK)
                {
                    w -> right -> color = RBTREE_BLACK;
                    w -> color = RBTREE_RED;
                    rbtree_left_rotate(t, w);
                    w = x -> parent -> left;
                }
                w -> color = x -> parent -> color;
                x -> parent -> color = RBTREE_BLACK;
                w -> left -> color = RBTREE_BLACK;
                rbtree_right_rotate(t, x -> parent);
                x = t -> root;
            }
        }
    }
    x -> color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *z){
    node_t *y = z;
    color_t y_orginal_color = y->color;
    node_t *x;
    if (z -> left == t -> nil)
    {
        x = z -> right;
        rbtree_transplant(t, z, z -> right);
    }
    else if (z -> right == t -> nil)
    {
        x = z -> left;
        rbtree_transplant(t, z, z -> left);
    }
    else
    {
        y = subtree_min(t, z -> right);
        y_orginal_color = y->color;
        x = y -> right;
        if (y -> parent == z)
        {
            x -> parent = y;
        }
        else
        {
            rbtree_transplant(t, y, y -> right);
            y -> right = z -> right;
            y -> right -> parent = y;
        }
        rbtree_transplant(t, z, y);
        y -> left = z -> left;
        y -> left -> parent = y;
        y -> color = z -> color;
    }
    if (y_orginal_color == RBTREE_BLACK)
    {
        rb_delete_fixup(t, x);
    }
    free(z);
    return 0;
}


int rbtree_inorder_tree_walk(const rbtree *t, node_t *ptr, key_t *arr, int depth)
{
  if (ptr == t->nil)
  {
    return depth;
  }
  depth = rbtree_inorder_tree_walk(t, ptr->left, arr, depth);
  arr[depth] = ptr->key;
  depth += 1;
  depth = rbtree_inorder_tree_walk(t, ptr->right, arr, depth);

  return depth;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array

  if (t->root == t->nil) return -1;

  rbtree_inorder_tree_walk(t, t->root, arr, 0);

  return 0;
}
