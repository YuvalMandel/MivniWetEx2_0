//
// Created by Yuval.M on 09/01/2021.
//

#ifndef MIVNIWETEX2_0_CHT_H
#define MIVNIWETEX2_0_CHT_H

#define INIT_TABLE_SIZE 7
#include "AVL.h"

template<class Value>
class LSValue{
private:
    Value* val_ptr;
    LSValue* prev;
    LSValue* next;
};

template<class Value>
class CHT{

private:

    bool enable_members_delete;
    LSValue** ls_table;
    int table_size;
    int num_of_members;

public:

    CHT(bool enable_members_delete);
    ~CHT();

    void insert(*Value val_ptr);
    *Value returnValuePtr(const Value& val);
    void deleteValuePtr(const Value& val);

    void increaseTable();
    void decreaseTable();

};

template<class Value>
CHT::CHT(bool enable_members_delete){

    this -> enable_members_delete = enable_members_delete;
    this -> trees_array = new LSValue*[INIT_TABLE_SIZE];
    this -> table_size = INIT_TABLE_SIZE;
    this -> num_of_members = 0;

    for (int i = 0; i < this -> table_size; ++i) {
        trees_array[i] = nullptr;
    }

}

template<class Value>
CHT::~CHT(){

    for (int i = 0; i < table_size; ++i) {

        LSValue* current_ls_val = this -> ls_table[i];

        while(current_ls_val != nullptr){
            if(enable_members_delete){
                delete current_ls_val -> val_ptr;
            }
            LSValue* temp = current_ls_val;
            current_ls_val = current_ls_val -> next;
            delete temp;
        }

    }

    delete[] ls_table;

}



#endif //MIVNIWETEX2_0_CHT_H
