#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

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
    void rotate(AVLNode<Key,Value>* n);
    void insertfix(AVLNode<Key,Value>* n, AVLNode<Key,Value>* c);
    void removefix(AVLNode<Key,Value>* n, int8_t diff);

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    AVLNode<Key, Value>* cnode = static_cast<AVLNode<Key, Value>*>(this->root_);
    if(cnode == NULL){
      AVLNode<Key, Value>* aNode = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
      this->root_ = aNode;
    }else{
      while(cnode -> getRight() != NULL || cnode -> getLeft() != NULL){
        if(new_item.first > cnode -> getKey()){
          if(cnode -> getRight() == NULL){
            AVLNode<Key, Value>* aNode = new AVLNode<Key, Value>(new_item.first, new_item.second, cnode);
            cnode -> setRight(aNode);
            cnode -> updateBalance(1);
            if(cnode -> getBalance() == 1){
              insertfix(cnode, aNode);
            }
            return;
          }else{
            cnode = cnode -> getRight();
          }
        }else if(new_item.first < cnode -> getKey()){
          if(cnode -> getLeft() == NULL){
            AVLNode<Key, Value>* aNode = new AVLNode<Key, Value>(new_item.first, new_item.second, cnode);
            cnode -> setLeft(aNode);
            cnode -> updateBalance(-1);
            if(cnode -> getBalance() == -1){
              insertfix(cnode, aNode);
            }
            return;
          }else{
            cnode = cnode -> getLeft();
          }
        }else if(new_item.first == cnode -> getKey()){
          cnode -> setValue(new_item.second);
          return;
        }
      }
      if(new_item.first > cnode -> getKey()){
        AVLNode<Key, Value>* aNode = new AVLNode<Key, Value>(new_item.first, new_item.second, cnode);
        cnode -> setRight(aNode);
        cnode -> updateBalance(1);
          if(cnode -> getBalance() == 1){
            insertfix(cnode, aNode);
          }
      }else if (new_item.first < cnode -> getKey()){
        AVLNode<Key, Value>* aNode = new AVLNode<Key, Value>(new_item.first, new_item.second, cnode);
        cnode -> setLeft(aNode);
        cnode -> updateBalance(-1);
          if(cnode -> getBalance() == -1){
            insertfix(cnode, aNode);
          }
      }else{
        cnode -> setValue(new_item.second);
      }
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    //TODO
    //this -> print();
    //std::cout << key << std::endl;
    AVLNode<Key, Value>* cnode = static_cast<AVLNode<Key, Value>*>(this -> internalFind(key));
    if(cnode == NULL){
      return;
    }else{
      if(cnode -> getRight() != NULL && cnode -> getLeft() != NULL){
        nodeSwap(cnode, static_cast<AVLNode<Key, Value>*> (this -> predecessor(cnode)));
        //this -> print();
      }
      if(cnode -> getRight() == NULL && cnode -> getLeft() == NULL){
        if(cnode -> getParent() != NULL){
          AVLNode<Key, Value>* pnode = cnode -> getParent();
          if(pnode -> getRight() == cnode){
            pnode -> setRight(NULL);
            removefix(pnode, -1);
          }else{
            pnode -> setLeft(NULL);
            removefix(pnode, 1);
          }
        }else{
          this -> root_ = NULL;
        }
        delete cnode;
      }else if(cnode -> getRight() != NULL){
        if(cnode -> getParent() != NULL){
          AVLNode<Key, Value>* pnode = cnode -> getParent();
          if(pnode -> getRight() == cnode){
            pnode -> setRight(cnode -> getRight());
            removefix(pnode, -1);
          }else{
            pnode -> setLeft(cnode -> getRight());
            removefix(pnode, 1);
          }
          cnode -> getRight() -> setParent(cnode -> getParent());
        }else{
          this -> root_ = cnode -> getRight();
          this -> root_ -> setParent(NULL);
        }
        delete cnode;
      }else if(cnode -> getLeft() != NULL){
        if(cnode -> getParent() != NULL){
          AVLNode<Key, Value>* pnode = cnode -> getParent();
          if(pnode -> getRight() == cnode){
            pnode -> setRight(cnode -> getLeft());
            removefix(pnode, -1);
          }else{
            pnode -> setLeft(cnode -> getLeft());
            removefix(pnode, 1);
          }
          cnode -> getLeft() -> setParent(cnode -> getParent());
        }else{
          this -> root_ = cnode -> getLeft();
          this -> root_ -> setParent(NULL);
        }
        delete cnode;
      }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotate(AVLNode<Key,Value>* n){
  AVLNode<Key,Value>* p = n -> getParent();
  AVLNode<Key,Value>* c;
  AVLNode<Key,Value>* gp = p -> getParent();
  if(n == p -> getLeft()){
    c = n -> getRight();
    p -> setLeft(c);
    if(c != NULL){
      c -> setParent(p);
    }
    n -> setRight(p);
    p -> setParent(n);
  }else{
    c = n -> getLeft();
    p -> setRight(c);
    if(c != NULL){
      c -> setParent(p);
    }
    n -> setLeft(p);
    p -> setParent(n);
  }
  if(gp != NULL){
    if(p == gp -> getLeft()){
      gp -> setLeft(n);
    }else{
      gp -> setRight(n);
    }
  }else{
    this -> root_ = n;
  }
  n -> setParent(gp);
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertfix(AVLNode<Key,Value>* n, AVLNode<Key,Value>* c){
  if(n == NULL || n -> getParent() == NULL){
    return;
  }
  AVLNode<Key,Value>* p = n -> getParent();
  if(n == p -> getLeft()){
    p -> updateBalance(-1);
  }else{
    p -> updateBalance(1);
  }
  if(p -> getBalance() == 0){
    return;
  }else if(p -> getBalance() == 1 || p -> getBalance() == -1){
    insertfix(p,n);
  }else{
    if((n == p->getLeft() && c == n->getLeft()) || (n == p->getRight() && c == n->getRight())){
      rotate(n);
      n -> setBalance(0);
      p -> setBalance(0);
    }else{
      if(c == n -> getRight()){
        if(c -> getBalance() == 0){
          n -> setBalance(0);
          p -> setBalance(0);
        }else if(c -> getBalance() == 1){
          c -> setBalance(0);
          n -> setBalance(-1);
          p -> setBalance(0);
        }else{
          c -> setBalance(0);
          n -> setBalance(0);
          p -> setBalance(1);
        }
      }else{
        if(c -> getBalance() == 0){
          n -> setBalance(0);
          p -> setBalance(0);
        }else if(c -> getBalance() == 1){
          c -> setBalance(0);
          n -> setBalance(0);
          p -> setBalance(-1);
        }else{
          c -> setBalance(0);
          n -> setBalance(1);
          p -> setBalance(0);
        }
      }
      rotate(c);
      rotate(c);
    }
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::removefix(AVLNode<Key,Value>* n, int8_t diff){
  if(n == NULL){
    return;
  }
  AVLNode<Key,Value>* p = n -> getParent();
  int8_t nextdif;
  if(p != NULL){
    if(n == p -> getLeft()){
      nextdif = 1;
    }else{
      nextdif = -1;
    }
  }
  if(diff == -1){
    if(n -> getBalance() + diff == -2){
      AVLNode<Key,Value>* c = n -> getLeft();
      if(c -> getBalance() == -1){
        rotate(c);
        n -> setBalance(0);
        c -> setBalance(0);
        removefix(p,nextdif);
      }else if(c -> getBalance() == 0){
        rotate(c);
        n -> setBalance(-1);
        c -> setBalance(1);
      }else{
        AVLNode<Key,Value>* gc = c -> getRight();
        rotate(gc);
        rotate(gc);
        if(gc -> getBalance() == 0){
          n -> setBalance(0);
          c -> setBalance(0);
        }else if(gc -> getBalance() == 1){
          c -> setBalance(-1);
          n -> setBalance(0);
          gc -> setBalance(0);
        }else{
          c -> setBalance(0);
          n -> setBalance(1);
          gc -> setBalance(0);
        }
        removefix(p,nextdif);
      }
    }else if(n -> getBalance() + diff == -1){
      n -> setBalance(-1);
    }else{
      n -> setBalance(0);
      removefix(p,nextdif);
    }
  }else{
    if(n -> getBalance() + diff == 2){
      AVLNode<Key,Value>* c = n -> getRight();
      if(c -> getBalance() == 1){
        rotate(c);
        n -> setBalance(0);
        c -> setBalance(0);
        removefix(p,nextdif);
      }else if(c -> getBalance() == 0){
        rotate(c);
        n -> setBalance(1);
        c -> setBalance(-1);
      }else{
        AVLNode<Key,Value>* gc = c -> getLeft();
        rotate(gc);
        rotate(gc);
        if(gc -> getBalance() == 0){
          n -> setBalance(0);
          c -> setBalance(0);
        }else if(gc -> getBalance() == -1){
          c -> setBalance(1);
          n -> setBalance(0);
          gc -> setBalance(0);
        }else{
          c -> setBalance(0);
          n -> setBalance(-1);
          gc -> setBalance(0);
        }
        removefix(p,nextdif);
      }
    }else if(n -> getBalance() + diff == 1){
      n -> setBalance(1);
    }else{
      n -> setBalance(0);
      removefix(p,nextdif);
    }
  }
}
#endif
