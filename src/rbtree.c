#include "rbtree.h"

#include <stdio.h>
#include <stdlib.h>

// int main(void)
// {
//   rbtree *t = new_rbtree();
//   node_t *p = rbtree_insert(t, 1024);
//   printf("%p\n", p);
//   printf("%i\n", t->root->key);
// }

rbtree *new_rbtree(void) {

  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));     // rbtree를 위한 메모리 할당
  
  // rbtree를 위한 메모리 할당 실패 시 예외 처리
  if (p == NULL)
  {
        fprintf(stderr, "Memory allocation failed\n"); // stderr: 표준 에러 출력 스트림
        exit(EXIT_FAILURE);                            // EXIT_FAILURE: C 프로그래밍 언어에서 프로그램이 비정상적으로 종료될 때 사용되는 매크로 상수 (1)

        /*-----------------------------
        * 프로그램 종료 방법 2가지
        * -----------------------------
        * return 1: 
        *   - main 함수가 종료되면서 프로그램이 종료됩니다. 
        *   - 사용 상황: main 함수 내에서 프로그램의 종료 상태를 설정하는 데 사용됩니다.
        * exit(1) : 
        *   - 호출 시점에서 즉시 프로그램을 종료합니다. 
        *   - 현재 호출 스택의 모든 함수의 종료를 유도합니다.
        *   - 프로그램 종료 전에 `atexit`로 등록된 종료 처리 함수들이 실행됩니다.
        *   - 사용 상황: 프로그램의 어떤 위치에서도 종료 상태를 설정하고 즉시 종료시킬 때 사용됩니다.
        */

        /*-----------------------------
        * 매크로 상수와 enum 상수
        * -----------------------------
        * 매크로 상수:
        *   - <예시> #define MAX_SIZE 100;
        *   - 장점: 정의된 상수를 한 곳에서 변경하면 코드 전체에 반영된다.
        *   - 단점: 
        *       - 매크로 상수는 단순한 치환이므로, 타입 체크가 없다. 컴파일 타임에 문제가 발생할 수 있다.
        * enum 상수:
        *   - <예시> enum EnumName { CONSTANT1, CONSTANT2, ...};
        *   - 열거형 상수는 자동으로 정수 값이 부여된다. 첫 번째 상수는 `0`, 두 번째는 `1` 이렇게 계속해서 증가한다.
        */
  }

  // TODO: initialize struct if needed

  p->nil = (node_t *)calloc(1, sizeof(node_t));          // nil을 위한 메모리 할당

  // NIL 노드를 위한 메모리 할당 실패 시 예외 처리
  if (p->nil == NULL)
  {
      fprintf(stderr, "Memory allocation failed\n");
      exit(EXIT_FAILURE);
  }

  // NIL 노드 초기화
  p->nil->color = RBTREE_BLACK;
  // p->nil->left =  NULL; 
  // p->nil->right =  NULL;
  // p->nil->parent =  NULL;                            // nil 노드의 부모를 자기 자신으로 설정 (또는 NULL을 가리기케 하는 방법도 있음)
  

  // 루트 노드 초기화
  p->root = p->nil;
  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

int rbtree_left_rotate(rbtree *t, node_t *x)
{
  node_t *y = x->right;     // y는 x의 left

  x->right = y->left;                                  // x 노드의 right: [as-is] y, [to-be] y->left

  if (y->left != t->nil)
  {
    y->left->parent = x;                               // y의 left의 parent: [as-is] y, [to-be] x
    // => y->left를 x의 right로 양방향 연결 성공

  } // [y 노드의 left]가 NIL 노드라면 [y 노드의 left]의 부모 노드의 정보는 그대로 유지 (parent: NIL)

  y->parent = x->parent;                               // y 노드의 parent: [as-is] x, [to-be] x->parent

  if (x->parent == t->nil)
  {
    t->root = y;                                       // 루트 노드: [as-is] x, [to-be] y
  }
  else if (x == x->parent->left)
  {
    x->parent->left = y;                               // x 노드의 parent의 left: [as-is] x, [to-be] y
  }
  else
  {
    y->left = x;                                       // x 노드의 parent의 right: [as-is] x, [to-be] y
    x->parent = y;
  }

  y->left = x;                                         // y 노드의 left: [as-is] y->left, [to-be] x
  x->parent = y;                                       // x 노드의 parent: [as-is] x->parent, [to-be] y
  
  return 0;
}

int rbtree_right_rotate(rbtree *t, node_t *x)
{
  node_t *y = x->left;     // y는 x의 left

  x->left = y->right;                                  // x 노드의 left: [as-is] y, [to-be] y->left

  if (y->right != t->nil)
  {
    y->right->parent = x;                               // y 노드의 right의 parent: [as-is] y, [to-be] x
    // => y->right x의 left로 양방향 연결 성공

  }

  // 여기부터 아래까지는 LEFT_ROTATE와 코드 동일
  y->parent = x->parent;                               // y 노드의 parent: [as-is] x, [to-be] x->parent

  if (x->parent == t->nil)
  {
    t->root = y;                                       // 루트 노드: [as-is] x, [to-be] y
  }
  else if (x == x->parent->left)
  {
    x->parent->left = y;                               // x 노드의 parent의 left: [as-is] x, [to-be] y
  }
  else
  {
    y->left = x;                                       // x 노드의 parent의 right: [as-is] x, [to-be] y
    x->parent = y;
  }

  y->left = x;                                         // y 노드의 left: [as-is] y->left, [to-be] x
  x->parent = y;                                       // x 노드의 parent: [as-is] x->parent, [to-be] y

  return 0;
}

int rbtree_insert_fixup(rbtree *t, node_t *z)
{
  while (z->parent->color == RBTREE_RED)
  {
    // 부모가 할아버지의 왼쪽에 있을 때
    if (z->parent == z->parent->parent->left)
    {
      node_t *y = z->parent->parent->right;
      if (y->color == RBTREE_RED)
      {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else
      {
        if (z->parent->right)
        {
          z = z->parent;
          rbtree_left_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        rbtree_right_rotate(t, z->parent->parent);
      }
    }
    else 
    {
      node_t *y = z->parent->parent->left;
      if (y->color == RBTREE_RED)
      {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else
      {
        if (z->parent->left)
        {
          z = z->parent;
          rbtree_right_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        rbtree_left_rotate(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
  return 0;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert

  node_t *z = (node_t *)calloc(1, sizeof(node_t));     // node_t 위한 메모리 할당
  printf("%p",z);
  // node_t를 위한 메모리 할당 실패 시 예외 처리
  if (z == NULL)
  {
        fprintf(stderr, "Memory allocation failed\n"); // stderr: 표준 에러 출력 스트림
        exit(EXIT_FAILURE);   
  }

  // 새롭게 삽입할 노드의 key 설정
  z->key = key;

  node_t *y = t->nil;
  node_t *x = t->root;

  while (x != t->nil)
  {
    if (x == NULL) {
      break;
    }

    y = x;                      // 반복문 첫 번째 시행 시, z의 부모 노드는 잠정적으로 루트 노드인 x
    if (z->key < x->key)
    {
      x = x->left;              // pointer를 x의 left로 변경
    }
    else                        // z의 키가 x의 키보다 크거나 또는 두 개가 같을 때
    {
      x = x->right;             // pointer를 x의 right로 변경
    }
  }

  z->parent = y;
  if (y == t->nil)
  {
    t->root = z;
  }
  else if (z->key < y->key)
  {
    y->left = z;
  }
  else 
  {
    y->right = z;
  }

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
    if (ptr->key > key)
    {
      ptr = ptr->left;
    }
    else if (ptr-> key < key)
    {
      ptr = ptr->right;
    }
    else
    {
      return ptr;
    }
  }

  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *ptr = t->root;
  node_t *min = t->nil;

  while (ptr != t->nil)
  {
    min = ptr;
    ptr = ptr->left;
  }

  return min;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *ptr = t->root;
  node_t *max = t->nil;

  while (ptr != t->nil)
  {
    max = ptr;
    ptr = ptr->right;
  }

  return max;
}

int rbtree_transplant(rbtree *t, node_t *u, node_t *v)
{
  if (u->parent == t->nil)
  {
    t->root = v;
  }
  else if (u == u->parent->left)
  {
    u->parent->left = v;
  }
  else
  {
    u->parent->right = v;
  }
  v->parent = u->parent;

  return 0;
}

node_t *subtree_min(const rbtree *t, const node_t *z_right) {
  // TODO: implement find
  node_t *ptr = z_right;
  node_t *min = t->nil;

  while (ptr != t->nil)
  {
    min = ptr;
    ptr = ptr->left;
  }

  return min;
}

node_t *subtree_max(const rbtree *t, const node_t *z_left) {
  // TODO: implement find
  node_t *ptr = z_left;
  node_t *max = t->nil;

  while (ptr != t->nil)
  {
    max = ptr;
    ptr = ptr->right;
  }

  return max;
}

int rbtree_delete_fixup(const rbtree *t, const node_t *x)
{
  while (x != t->root && x->color == RBTREE_BLACK)
  {
    if (x == x->parent->left)
    {
      node_t *w = x->parent->right;
      if (w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        rbtree_left_rotate(t, x->parent);
      }

      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK)
      {
        w->color = RBTREE_BLACK;
        x = x->parent;
      }
      else 
      {
        if (w->right->color = RBTREE_BLACK)
        {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          rbtree_right_rotate(t, w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        rbtree_left_rotate(t, x->parent);
        x = t->root;
      }  
    }
    else
    {
      node_t *w = x->parent->left;
      if (w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        rbtree_right_rotate(t, x->parent);
      }

      if (w->left->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK)
      {
        w->color = RBTREE_BLACK;
        x = x->parent;
      }
      else 
      {
        if (w->left->color = RBTREE_BLACK)
        {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          rbtree_left_rotate(t, w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        rbtree_right_rotate(t, x->parent);
        x = t->root;
      } 
    }
  }

  x->color = RBTREE_BLACK;

  return 0;
}

int rbtree_erase(rbtree *t, node_t *z) {
  // TODO: implement erase
  node_t *y = z;
  color_t y_original_color = y->color;

  if (z->left == t->nil)
  {
    node_t *x = z->right;
    rbtree_transplant(t, z, z->right);
  }
  else if (z->right = t->nil)
  {
    node_t *x = z->left;
    rbtree_transplant(t, z, z->left);
  }
  else
  {
    y = subtree_min(t, z->right);
    y_original_color = y->color;
    node_t *x = y->right;
    if (y->parent == z)
    {
      x->parent = y;
    }
    else
    {
      rbtree_transplant(t, y, y->right);
      y->right = z->right;
      y->right->parent = y;
      rbtree_transplant(t, z, y);
      y->left = z->left;
      y->left->parent = y;
      y->color = z->color;
    }
  }
  if (y_original_color == RBTREE_BLACK)
  {
    rbtree_delete_fixup(t, x);
  }

  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
