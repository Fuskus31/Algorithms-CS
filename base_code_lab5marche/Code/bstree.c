#include "bstree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

void bstree_remove_node(ptrBinarySearchTree *t, ptrBinarySearchTree current);

/*------------------------  BSTreeType  -----------------------------*/

struct _bstree {
    BinarySearchTree *parent;
    BinarySearchTree *left;
    BinarySearchTree *right;
    int root;
};

/*------------------------  BaseBSTree  -----------------------------*/

BinarySearchTree *bstree_create(void) {
    return NULL;
}

/* This constructor is private so that we can maintain the oredring invariant on
 * nodes. The only way to add nodes to the tree is with the bstree_add function
 * that ensures the invariant.
 */
BinarySearchTree *bstree_cons(BinarySearchTree *left, BinarySearchTree *right, int root) {
    BinarySearchTree *t = malloc(sizeof(struct _bstree));
    t->parent = NULL;
    t->left = left;
    t->right = right;
    if (t->left != NULL)
        t->left->parent = t;
    if (t->right != NULL)
        t->right->parent = t;
    t->root = root;
    return t;
}

void bstree_delete(ptrBinarySearchTree *t) {
    while (!bstree_empty(*t))
        bstree_remove_node(t, *t);
}

bool bstree_empty(const BinarySearchTree *t) {
    return t == NULL;
}

int bstree_root(const BinarySearchTree *t) {
    assert(!bstree_empty(t));
    return t->root;
}

BinarySearchTree *bstree_left(const BinarySearchTree *t) {
    assert(!bstree_empty(t));
    return t->left;
}

BinarySearchTree *bstree_right(const BinarySearchTree *t) {
    assert(!bstree_empty(t));
    return t->right;
}

BinarySearchTree *bstree_parent(const BinarySearchTree *t) {
    assert(!bstree_empty(t));
    return t->parent;
}

/*------------------------  BSTreeDictionary  -----------------------------*/

/* Obligation de passer l'arbre par référence pour pouvoir le modifier */
void bstree_add(ptrBinarySearchTree *t, int v) {
    ptrBinarySearchTree *cur = t;
	BinarySearchTree *par = NULL;

	while(!bstree_empty(*cur)){
		par = *cur;
		if((*cur)->root == v){
			return;
		}
		if((*cur)->root > v){
      cur = &((**cur).left);
    }
		else{
      cur = &((*cur)->right);
    }
	}
	*cur = bstree_cons(NULL, NULL,v);
	(*cur)->parent = par;
}

bool bstree_search(const BinarySearchTree *t, int v) {
    if(bstree_empty(t)){
        return false;
    }

    while(!bstree_empty(t)){
        if(v == t->root){
            return true;
        } // On vérifie si le noeud est equal à la valeur int v.

        if( v < t->root){
            t = t->left;
        }
        else if( v > t->root){
            t = t->right;
        }
    }
    return false; 
}

BinarySearchTree *bstree_successor(const BinarySearchTree *x) {
    assert(!bstree_empty(x));//TAD assert
    
    BinarySearchTree *y = x->right;
    if(y){ //different de 0 donc adresse.
        while(y->left){
            y = y->left;
        }
    } else {
        y = x->parent;
        while ( y && (x== y->right)){
            x = y;
            y = y->parent;
        }
    }
    return y;
}

BinarySearchTree *bstree_predecessor(const BinarySearchTree *x) {
    assert(!bstree_empty(x));
    
    BinarySearchTree *y = x->left;
    if(y){ //different de 0 donc adresse.
        while(y->right){
            y = y->right;
        }
    } else {
        y = x->parent;
        while ( y && (x== y->left)){
            x = y;
            y = y->parent;
        }
    }
    return y;
}

void bstree_swap_nodes(ptrBinarySearchTree *tree, ptrBinarySearchTree from, ptrBinarySearchTree to) {
    assert(!bstree_empty(*tree) && !bstree_empty(from) && !bstree_empty(to));
    if (from->parent){
        if (from->parent->left == from){
          from->parent->left = to;
        }
        else{
          from->parent->right = to;
        }
        }
    else{
        *tree = to;
    }
    if (to->parent){
	    if (to->parent->left == to) {
	        to->parent->left = from;
      }
	    else{
	        to->parent->right = from;
      }
    }
    BinarySearchTree *tmp = from->parent;
    from->parent = to->parent;
    to->parent = tmp;

    if (from->left){
        from->left->parent = to;
    }

    if (to->left){
        to->left->parent = from;
    }
    tmp = from->left;
    from->left = to->left;
    to->left = tmp;
    if (from->right){
        from->right->parent = to;
    }
    if (to->right){
        to->right->parent = from;
    }
    tmp = from->right;
    from->right = to->right;
    to->right = tmp;
}

// t -> the tree to remove from, current -> the node to remove
void bstree_remove_node(ptrBinarySearchTree *t, ptrBinarySearchTree current) {
    assert(!bstree_empty(*t) && !bstree_empty(current));
    BinarySearchTree *tmp;

  	if (current->left == current->right) {
          tmp = NULL;
      }
      else if (! current->left) {
          tmp = current->right;

      }
      else if (! current->right) {
          tmp = current->left;
      }
      else {
          BinarySearchTree *leaf;
          leaf = bstree_successor(current);
          bstree_swap_nodes(t, current, leaf);
          tmp = current->right;
      }

      if (tmp != NULL){
          tmp->parent = current->parent;
      }
      if ( !current->parent ){
          *t = tmp;
      }

      else if (current->parent->left == current){
          current->parent->left = tmp;
      }
      else{
          current->parent->right = tmp;
      }
      free(current);
      current=NULL;
}

void bstree_remove(ptrBinarySearchTree *t, int v) {
    BinarySearchTree *node = *t;
    while(node && node->root != v){
        node = ( (node->root > v) ? node->left : node->right);
    }
    if(node){
        //printf("trouver node dans bstreeremovee %d",node->root);
        bstree_remove_node(t,node);
    }
}

/*------------------------  BSTreeVisitors  -----------------------------*/

void bstree_depth_prefix(const BinarySearchTree *t, OperateFunctor f, void *userData) {//cours
    f(t, userData);

    if(t->left!=NULL){
      bstree_depth_prefix(t->left, f, userData);
    }
    if(t->right!=NULL){
      bstree_depth_prefix(t->right, f, userData);
    }
}

void bstree_depth_infix(const BinarySearchTree *t, OperateFunctor f, void *userData) {
  if(t->left!=NULL){
    bstree_depth_infix(t->left, f, userData);
  }

  f(t, userData);

  if(t->right!=NULL){
    bstree_depth_infix(t->right, f, userData);
  }
}

void bstree_depth_postfix(const BinarySearchTree *t, OperateFunctor f, void *userData) {
  if(t->left!=NULL){
    bstree_depth_postfix(t->left, f, userData);
  }

  if(t->right!=NULL){
    bstree_depth_postfix(t->right, f, userData);
  }

  f(t, userData);
}

void bstree_iterative_depth_infix(const BinarySearchTree *t, OperateFunctor f, void *userData) {
  if (t==NULL){
        return;
    }
    const BinarySearchTree *current = t;
    const BinarySearchTree *next = bstree_parent(t);
    const BinarySearchTree *previous = bstree_parent(t);

    while (!bstree_empty(current))
    {
        if(previous == bstree_parent(current)){
            previous = current;
            next = bstree_left(current);   
        }

        if(bstree_empty(next) || previous == bstree_left(current)){
            f(current,userData);
            previous = current;
            next = bstree_right(current);
        }

        if(bstree_empty(next) || previous == bstree_right(current)){
            previous = current;
            next = bstree_parent(current);
        }
        current = next;
    }
}

void bstree_iterative_breadth_prefix(const BinarySearchTree *t, OperateFunctor f, void *userData) {
    

     if (bstree_empty(t)) {
        return; //vide donc pas de parcour
    }

    Queue *q = createQueue();
    queuePush(q, t); //racine 

    while (!queueEmpty(q)) {
        const BinarySearchTree *temp = queueTop(q);
        queuePop(q);
        f(temp, userData); 

       
        if (temp->left != NULL) {
            queuePush(q, temp->left);
        }
        if (temp->right != NULL) {
            queuePush(q, temp->right);
        }
    }

    deleteQueue(&q);//check
}

/*------------------------  BSTreeIterator  -----------------------------*/

struct _BSTreeIterator {
    /* the collection the iterator is attached to */
    const BinarySearchTree *collection;
    /* the first element according to the iterator direction */
    const BinarySearchTree *(*begin)(const BinarySearchTree *);
    /* the current element pointed by the iterator */
    const BinarySearchTree *current;
    /* function that goes to the next element according to the iterator direction */
    BinarySearchTree *(*next)(const BinarySearchTree *);
};

/* minimum element of the collection */
const BinarySearchTree *goto_min(const BinarySearchTree *e) {
    const BinarySearchTree *t = e;
    while(!bstree_empty(t->left)){
        t = t->left;
        }
    return t;
	
}

/* maximum element of the collection */
const BinarySearchTree *goto_max(const BinarySearchTree *e) {
    const BinarySearchTree *t = e;
    while(!bstree_empty(t->right)){
        t = t->right;
    }
    return t;
}

/* constructor */
BSTreeIterator *bstree_iterator_create(const BinarySearchTree *collection, IteratorDirection direction) {
    BSTreeIterator *i = malloc(sizeof(struct _BSTreeIterator));

	i->collection = collection;
	if(direction == forward){
		i->begin = goto_min;
		i->next = bstree_successor;
	}
	else{
		i->begin = goto_max;
		i->next = bstree_predecessor;
	}
	i->current = i->begin(collection);
	return i;
}

/* destructor */
void bstree_iterator_delete(ptrBSTreeIterator *i) {
    free(*i);
    *i = NULL;
}

BSTreeIterator *bstree_iterator_begin(BSTreeIterator *i) {
    i->current = i->begin(i->collection);
    return i;
}

bool bstree_iterator_end(const BSTreeIterator *i) {
    return i->current == NULL;
}

BSTreeIterator *bstree_iterator_next(BSTreeIterator *i) {
    i->current = i->next(i->current);
    return i;
}

const BinarySearchTree *bstree_iterator_value(const BSTreeIterator *i) {
    return i->current;
}
