//
// Created by Yuval.M on 04/12/2020.
//

#ifndef MIVNIWETEX1_0_AVL_H
#define MIVNIWETEX1_0_AVL_H

template<class Value>
class AVLNode{

public:
    Value* val_ptr;
    AVLNode<Value>* right_son;
    AVLNode<Value>* left_son;
    int height;
    int rank;

    AVLNode(Value* val_ptr): val_ptr(val_ptr),
                             right_son(nullptr),
                             left_son(nullptr),
                             height(1),
                             rank(1){};

    ~AVLNode();
    AVLNode(const AVLNode& stc);
    AVLNode& operator=(const AVLNode& stc);

};

template<class Value>
AVLNode<Value>::~AVLNode(){
    if(this -> val_ptr != nullptr){
        delete this -> val_ptr;
    }
}

template<class Value>
AVLNode<Value>::AVLNode(const AVLNode& avl_node){
    this->val_ptr = avl_node.val_ptr;
    this->right_son = avl_node.right_son;
    this->left_son = avl_node.left_son;
    this->height = avl_node.height;
    this->rank = avl_node.rank;
}

template<class Value>
AVLNode<Value>& AVLNode<Value>::operator=(const AVLNode& avl_node) {
    this->val_ptr = avl_node.val_ptr;
    this->right_son = avl_node.right_son;
    this->left_son = avl_node.left_son;
    this->height = avl_node.height;
    this->rank = avl_node.rank;
    return *this;
}
template<class Value>
class AVLTree{

public:

    AVLTree() : root(nullptr){}
    AVLTree(Value** vals, int length) :root(createTreeNode(vals, 0, length-1)){}
    ~AVLTree() {
        DestroyTree(root);
    }
//    AVLTree(const AVLTree<Value>& avl_tree);
//    AVLTree<Value>& operator=(const AVLTree<Value>& avl_tree);

    AVLNode<Value>* root;

    void Insert(Value* val);

    void Remove(const Value& val);
    AVLNode<Value>* FindValue(const Value& val);
    AVLNode<Value>* FindValueByIndex(int index);

private:

    AVLNode<Value>* createTreeNode(Value** vals, int start_index, int
    final_index);
    void DestroyTree(AVLNode<Value>* root);
    AVLNode<Value>* FindValueInNode(const Value& val, AVLNode<Value>* node);
    AVLNode<Value>* FindValueByIndexInNode(int index, AVLNode<Value>* node);
    AVLNode<Value>* InsertValueInNode(Value* val_ptr, AVLNode<Value>* node);
    AVLNode<Value>* RemoveValueInNode(const Value& val, AVLNode<Value>* node,
                                      bool delete_node = true);
    void UpdateHeight(AVLNode<Value>* node);
    AVLNode<Value>* BalanceNode(AVLNode<Value>* node);
    int CalcBalanceFactor(AVLNode<Value>* node);
    void UpdateRank(AVLNode<Value>* node);

    AVLNode<Value>* LLRotate(AVLNode<Value>* node);
    AVLNode<Value>* LRRotate(AVLNode<Value>* node);
    AVLNode<Value>* RLRotate(AVLNode<Value>* node);
    AVLNode<Value>* RRRotate(AVLNode<Value>* node);

};

#include <algorithm>
#include <iostream>

template<class Value>
void AVLTree<Value>::Insert(Value* val_ptr) {

    this -> root = InsertValueInNode(val_ptr, this -> root);

}

template<class Value>
void AVLTree<Value>::Remove(const Value& val) {

    AVLNode<Value>* temp = RemoveValueInNode(val, this -> root);

    this -> root = temp;
}

template<class Value>
AVLNode<Value>* AVLTree<Value>::FindValue(const Value& val) {
    return FindValueInNode(val, this -> root);
}

template<class Value>
AVLNode<Value>* AVLTree<Value>::FindValueByIndex(int index) {
    return FindValueByIndexInNode(index, this -> root);
}

template<class Value>
void AVLTree<Value>::DestroyTree(AVLNode<Value>* root){
    if(root != nullptr){
        DestroyTree(root -> right_son);
        DestroyTree(root -> left_son);
    }
    delete root;
}

template<class Value>
AVLNode<Value>* AVLTree<Value>::createTreeNode(Value** vals, int start_index,
                                               int final_index){
    if(final_index < start_index) return nullptr;

    AVLNode<Value>* base;

    int middle_index = start_index + (final_index - start_index)/2;

    try {
        base = new AVLNode<Value>(vals[middle_index]);
    }
    catch(std::bad_alloc&)
    {
        throw std::invalid_argument("ALLOCATION_ERROR");
    }

    if(start_index != final_index) {
        base->left_son = createTreeNode(vals, start_index, middle_index - 1);
        base->right_son = createTreeNode(vals, middle_index + 1, final_index);
    }
    return base;
}

template<class Value>
AVLNode<Value>* AVLTree<Value>::FindValueInNode(const Value& val, AVLNode<Value>* node) {
    if(node == nullptr){
        return nullptr;
    }
    else if(*(node -> val_ptr) == val){
        return node;
    }
    else if (val < *(node -> val_ptr)){
        return FindValueInNode(val, node -> left_son);
    }
    else if (val > *(node -> val_ptr)){
        return FindValueInNode(val, node -> right_son);
    }

    return nullptr;

}

template<class Value>
AVLNode<Value>* AVLTree<Value>::FindValueByIndexInNode(int index,
                                                       AVLNode<Value>* node) {
    if(node == nullptr){
        return nullptr;
    }
    int left_rank = 0;
    if(node -> left_son != nullptr){
        left_rank = node -> left_son -> rank;
    }

    if(left_rank + 1 == index){
        return node;
    } else if(left_rank >= index){
        return FindValueByIndexInNode(index, node -> left_son);
    } else{
        return FindValueByIndexInNode(index - left_rank - 1, node -> right_son);
    }

}

template<class Value>
AVLNode<Value>* AVLTree<Value>::InsertValueInNode(Value* val_ptr,
                                                  AVLNode<Value>* node) {
    if(node == nullptr){

        AVLNode<Value>* temp;

        try {
            temp = new AVLNode<Value>(val_ptr);
        }
        catch(std::bad_alloc&)
        {
            throw std::invalid_argument("ALLOCATION_ERROR");
        }

        return temp;
    }
    else if(*(node -> val_ptr) == *val_ptr){
        return nullptr; // TODO add exception
    }
    else if (*val_ptr < *(node -> val_ptr)){
        if(node -> left_son == nullptr){

            try {
                node -> left_son = new AVLNode<Value>(val_ptr);
            }
            catch(std::bad_alloc&)
            {
                throw std::invalid_argument("ALLOCATION_ERROR");
            }

        }
        else{
            node -> left_son = InsertValueInNode(val_ptr, node -> left_son);
        }
    }
    else if (*val_ptr > *(node -> val_ptr)){
        if(node -> right_son == nullptr){

            try {
                node -> right_son = new AVLNode<Value>(val_ptr);
            }
            catch(std::bad_alloc&)
            {
                throw std::invalid_argument("ALLOCATION_ERROR");
            }

        }
        else{
            node -> right_son = InsertValueInNode(val_ptr, node -> right_son);
        }
    }
    UpdateHeight(node);
    UpdateRank(node);
    return BalanceNode(node);
}

template<class Value>
AVLNode<Value>* AVLTree<Value>::RemoveValueInNode(const Value &val,
                                                  AVLNode<Value> *node, bool
                                                  delete_node) {
    if(node == nullptr){
        return nullptr;
    }
    else if(val < *(node -> val_ptr)){
        node -> left_son = RemoveValueInNode(val, node -> left_son, delete_node);
        UpdateHeight(node);
        UpdateRank(node);
        return BalanceNode(node);
    }
    else if(val > *(node -> val_ptr)){
        node -> right_son = RemoveValueInNode(val, node -> right_son, delete_node);
        UpdateHeight(node);
        UpdateRank(node);
        return BalanceNode(node);
    }else{
        if(node -> right_son == nullptr && node -> left_son == nullptr){
            if(delete_node){
                delete node;
            }
            return nullptr;
        } else if(node -> right_son == nullptr){
            AVLNode<Value> *newNode = node -> left_son;
            if(delete_node){
                delete node;
            }
            return newNode;
        } else if(node -> left_son == nullptr){
            AVLNode<Value> *newNode = node -> right_son;
            if(delete_node){
                delete node;
            }
            return newNode;
        } else{

            // Go to the smallest node under the current node.
            AVLNode<Value>* newNode_ptr = node -> right_son;

            while (newNode_ptr -> left_son != nullptr){
                newNode_ptr = newNode_ptr -> left_son;
            }

            node -> right_son = RemoveValueInNode(
                    *(newNode_ptr -> val_ptr), node -> right_son, false);

            newNode_ptr -> right_son = node -> right_son;
            newNode_ptr -> left_son = node -> left_son;

            UpdateHeight(newNode_ptr);
            UpdateRank(newNode_ptr);

            AVLNode<Value> *temp = BalanceNode(newNode_ptr);

            node -> right_son = nullptr;
            node -> left_son = nullptr;

            if(delete_node){
                delete node;
            }

            return temp;

        }
    }
}

template<class Value>
void AVLTree<Value>::UpdateHeight(AVLNode<Value>* node) {

    if(node == nullptr) return;

    int left_height;
    int right_height;

    if(node -> left_son != nullptr) {
        left_height = node->left_son->height;
    }else{
        left_height = 0;
    }

    if(node -> right_son != nullptr) {
        right_height = node->right_son->height;
    }else{
        right_height = 0;
    }

    node -> height = std::max(left_height, right_height) + 1;
}

template<class Value>
void AVLTree<Value>::UpdateRank(AVLNode<Value>* node) {

    if(node == nullptr) return;

    int left_rank;
    int right_rank;

    if(node -> left_son != nullptr) {
        left_rank = node -> left_son -> rank;
    }else{
        left_rank = 0;
    }

    if(node -> right_son != nullptr) {
        right_rank = node -> right_son -> rank;
    }else{
        right_rank = 0;
    }

    node -> rank = left_rank + right_rank + 1;
}

template<class Value>
int AVLTree<Value>::CalcBalanceFactor(AVLNode<Value> *node) {

    if(node == nullptr) return 0;

    int left_height = (node -> left_son != nullptr) ?
                      node -> left_son -> height : 0;
    int right_height = (node -> right_son != nullptr) ?
                       node -> right_son -> height : 0;

    return left_height - right_height;

}

template<class Value>
AVLNode<Value>* AVLTree<Value>::BalanceNode(AVLNode<Value> *node) {

    int balance_factor = CalcBalanceFactor(node);

    if(balance_factor > 1){
        if(CalcBalanceFactor(node -> left_son) > 0){
            return LLRotate(node);
        } else {
            return LRRotate(node);
        }
    }else if(balance_factor < -1){
        if(CalcBalanceFactor(node -> right_son) > 0){
            return RLRotate(node);
        } else {
            return RRRotate(node);
        }
    }

    return node;

}

template<class Value>
AVLNode<Value>* AVLTree<Value>::LLRotate(AVLNode<Value> *node) {
    AVLNode<Value> *temp = node ->left_son;
    node -> left_son = temp -> right_son;
    temp -> right_son = node;
    UpdateHeight(node);
    UpdateRank(node);
    UpdateHeight(temp);
    UpdateRank(temp);
    return temp;
}

template<class Value>
AVLNode<Value>* AVLTree<Value>::RRRotate(AVLNode<Value> *node) {
    AVLNode<Value> *temp = node ->right_son;
    node -> right_son = temp -> left_son;
    temp -> left_son = node;
    UpdateHeight(node);
    UpdateRank(node);
    UpdateHeight(temp);
    UpdateRank(temp);
    return temp;
}

template<class Value>
AVLNode<Value>* AVLTree<Value>::LRRotate(AVLNode<Value> *node) {
    AVLNode<Value> *temp = node ->left_son;
    node -> left_son = RRRotate(temp);
    UpdateHeight(node);
    UpdateRank(node);
    return LLRotate(node);
}

template<class Value>
AVLNode<Value>* AVLTree<Value>::RLRotate(AVLNode<Value> *node) {
    AVLNode<Value> *temp = node ->right_son;
    node -> right_son = LLRotate(temp);
    UpdateHeight(node);
    UpdateRank(node);
    return RRRotate(node);
}



#endif //MIVNIWETEX1_0_AVL_H
