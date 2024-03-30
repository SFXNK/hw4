#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"
#define axx AVLNode<Key, Value>
struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void avlins(axx* curr, Key k, Value v);
    void insfix(axx* curr, axx* par);
    void remfix(axx* curr, int diff);
    void leftrotate(axx* curr);
    void rightrotate(axx* curr);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */

   //          X             X
 //        A              B
 //     c     B        A     d
 //         E  d     c  E     
template<class Key, class Value>
void AVLTree<Key, Value>::leftrotate(axx* a){
  axx* b=a->getRight();
  axx* e=b->getLeft();
  axx* x=a->getParent();
  if(x != NULL){
    if(x->getLeft() == a)
      x->setLeft(b);
    else 
      x->setRight(b);
  }
  else{
    this->root_=b;
  }
  if(e != NULL){
    e->setParent(a);
  }
  a->setRight(e);
  a->setParent(b);
  b->setLeft(a);
  b->setParent(x);
  
}

  //          X             X
 //        A             B
 //     B     c       d     A
 //   d  E                E   c
template<class Key, class Value>
void AVLTree<Key, Value>::rightrotate(axx* a){
  axx* b=a->getLeft();
  axx* e=b->getRight();
  axx* x=a->getParent();
  if(x != NULL){
    if(x->getLeft() == a)
      x->setLeft(b);
    else 
      x->setRight(b);
  }
  else{
    this->root_=b;
  }
  if(e != NULL){
    e->setParent(a);
  }
  a->setLeft(e);
  a->setParent(b);
  b->setRight(a);
  b->setParent(x);
  
}



template<class Key, class Value>
void AVLTree<Key, Value>::avlins(axx* curr, Key k, Value v){
  if(curr == NULL)  return;
  if(k == curr->getKey()){
    curr->item_.second=v;
    return;
  }
  else if(k < curr->getKey()){
    if(curr->getLeft() == NULL){
      curr->setLeft(new axx(k, v, curr));
      if(curr->getBalance() == 0){
        curr->updateBalance(-1);
        insfix(curr->getLeft(), curr);
      }
      else{
        curr->setBalance(0);
        return;
      }
    }
    else avlins(curr->getLeft(), k, v);
  }
  else{
    if(curr->getRight() == NULL){
      curr->setRight(new axx(k, v, curr));
      if(curr->getBalance() == 0){
        curr->updateBalance(1);
        insfix(curr->getRight(), curr);
      }
      else{
        curr->setBalance(0);
        return;
      }
    }
    else {
      avlins(curr->getRight(), k, v);
    }
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insfix(axx* curr, axx* par){
  axx* gpar=par->getParent();
  if(gpar == NULL) return;

  if(par == gpar->getRight()){
    gpar->updateBalance(1);
    if(gpar->getBalance() == 0)
      return;
    if(gpar->getBalance() == 1)
      insfix(par, gpar);
    if(gpar->getBalance() == 2){
      if(par->getBalance() == 1){
        par->setBalance(0);
        gpar->setBalance(0);
        leftrotate(gpar);
      }
      else{
        rightrotate(par);
        leftrotate(gpar);
        if(curr->getBalance() == 1){
          curr->setBalance(0);
          par->setBalance(0);
          gpar->setBalance(-1);
        }
        else if(curr->getBalance() == 0){
          curr->setBalance(0);
          par->setBalance(0);
          gpar->setBalance(0);
        }
        else{
          curr->setBalance(0);
          par->setBalance(1);
          gpar->setBalance(0);
        }
      }
    }
  }

  else{
    gpar->updateBalance(-1);
    if(gpar->getBalance() == 0)
      return;
    if(gpar->getBalance() == -1)
      insfix(par, gpar);
    if(gpar->getBalance() == -2){
      if(par->getBalance() == -1){
        par->setBalance(0);
        gpar->setBalance(0);
        rightrotate(gpar);
      }
      else{
        leftrotate(par);
        rightrotate(gpar);
        if(curr->getBalance() == -1){
          curr->setBalance(0);
          par->setBalance(0);
          gpar->setBalance(1);
        }
        else if(curr->getBalance() == 0){
          curr->setBalance(0);
          par->setBalance(0);
          gpar->setBalance(0);
        }
        else{
          curr->setBalance(0);
          par->setBalance(-1);
          gpar->setBalance(0);
        }
      }
    }
  }
}



template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    if(this->root_ == NULL){
      this->root_=new axx(new_item.first, new_item.second, NULL);
      return;
    }
    avlins(static_cast<axx*>(this->root_), new_item.first, new_item.second);
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remfix(axx* curr, int diff){
  if(curr == NULL)
    return;
  int ndiff=0;
  axx* par=curr->getParent();
  axx* child=NULL;
  axx* gchild=NULL;
  if(par){
    if(par->getLeft() == curr)
      ndiff=1;
    else if(par->getRight() == curr)
      ndiff=-1;
  }
  if(diff == 1){
    if(curr->getBalance() + diff == 2){
      child=curr->getRight();
      if(child->getBalance() == 1){
        curr->setBalance(0);
        child->setBalance(0);
        leftrotate(curr);
        remfix(par, ndiff);
      }
      else if(child->getBalance() == 0){
        curr->setBalance(1);
        child->setBalance(-1);
        leftrotate(curr);
      }
      else if(child->getBalance() == -1){
        gchild=child->getLeft();
        rightrotate(child);
        leftrotate(curr);
        if(gchild->getBalance() == -1){
          curr->setBalance(0);
          child->setBalance(1);
          gchild->setBalance(0);
        }
        else if(gchild->getBalance() == 0){
          curr->setBalance(0);
          child->setBalance(0);
          gchild->setBalance(0);
        }
        else if(gchild->getBalance() == 1){
          curr->setBalance(-1);
          child->setBalance(0);
          gchild->setBalance(0);
        }
        remfix(par, ndiff);
      }
    }
    else if(curr->getBalance() + diff == 0){
      curr->setBalance(0);
      remfix(par, ndiff);
    }
    else if(curr->getBalance() + diff == 1){
      curr->setBalance(1);
    }
    
  }
  else if(diff == -1){
    if(curr->getBalance() + diff == -2){
      child=curr->getLeft();
      if(child->getBalance() == -1){
        curr->setBalance(0);
        child->setBalance(0);
        rightrotate(curr);
        remfix(par, ndiff);
      }
      else if(child->getBalance() == 0){
        curr->setBalance(-1);
        child->setBalance(1);
        rightrotate(curr);
      }
      else if(child->getBalance() == 1){
        gchild=child->getRight();
        leftrotate(child);
        rightrotate(curr);
        if(gchild->getBalance() == 1){
          curr->setBalance(0);
          child->setBalance(-1);
          gchild->setBalance(0);
        }
        else if(gchild->getBalance() == 0){
          curr->setBalance(0);
          child->setBalance(0);
          gchild->setBalance(0);
        }
        else if(gchild->getBalance() == -1){
          curr->setBalance(1);
          child->setBalance(0);
          gchild->setBalance(0);
        }
        remfix(par, ndiff);
      }
    }
    else if(curr->getBalance() + diff == 0){
      curr->setBalance(0);
      remfix(par, ndiff);
    }
    else if(curr->getBalance() + diff == -1){
      curr->setBalance(-1);
    }
  }
}



template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
  if(this->root_ == NULL)
    return;
  axx* curr=static_cast<axx*>(this->internalFind(key));
  if(curr == NULL)
    return;

  axx* par=curr->getParent();

  if(curr->getLeft()==NULL && curr->getRight()==NULL){
    if(par){
      if(par->getLeft() == curr){
        par->setLeft(NULL);
        remfix(curr->getParent(), 1);
      }
      else if(par->getRight() == curr){
        par->setRight(NULL);
        remfix(curr->getParent(), -1);
      }
    }
    else
      this->root_=NULL;
    delete curr;
    return;
  }

  if(curr->getLeft()!=NULL && curr->getRight()!=NULL){
    axx* pred=static_cast<axx*>(this->predecessor(curr));
    nodeSwap(curr, pred);
    par=curr->getParent();
    if(par){
      int diff=0;
      axx* child=NULL;
      if(curr->getLeft())
        child=curr->getLeft();
      else if(curr->getRight())
        child=curr->getRight();
      if(par->getLeft() == curr){
        diff=1;
        if(child==NULL)
          par->setLeft(NULL);
        else
          par->setLeft(child);
      }
      else{
        diff=-1;
        if(child==NULL)
          par->setRight(NULL);
        else
          par->setRight(child);
      }
      if(child!=NULL)
        child->setParent(par);
      delete curr;
      remfix(par, diff);
      return;
    }
  }

  if(curr->getLeft()!=NULL || curr->getRight()!=NULL){
    axx* child=NULL;
    if(curr->getLeft())
      child=curr->getLeft();
    else if(curr->getRight())
      child=curr->getRight();
    if(child){
      child->setParent(par);
      if(par){
        if(par->getLeft() == curr){
          par->setLeft(child);
          remfix(par, 1);
        }
        else if(par->getRight() == curr){
          par->setRight(child);
          remfix(par, -1);
        }
      }
      else{
        this->root_=child;
      }
      delete curr;
      return;
    }
  }
    return;
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif