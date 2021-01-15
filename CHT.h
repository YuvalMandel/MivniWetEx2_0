//
// Created by Yuval.M on 09/01/2021.
//

#ifndef MIVNIWETEX2_0_CHT_H
#define MIVNIWETEX2_0_CHT_H

#define INIT_TABLE_SIZE 7
#define LOAD_FACTOR_INCREASE 0.75
#define LOAD_FACTOR_DECREASE 0.25
#define INCREASE_FACTOR 4

template<class Value>
class LSValue{
public:
    Value* val_ptr;
    LSValue* prev;
    LSValue* next;
};

template<class Value>
class CHT{

private:

    bool enable_members_delete;
    LSValue<Value>** ls_table;
    int table_size;
    int num_of_members;
    LSValue<Value>* iterator_ls_val;
    int iterator_array_index;

public:

    explicit CHT(bool enable_members_delete);
    ~CHT();

    void insert(Value* val_ptr);
    Value* returnValuePtr(const Value& val);
    void deleteValuePtr(const Value& val);
    void changeTableSize(bool increase);

    Value* iteratorBegin();
    Value* iteratorNext();
};

template<class Value>
CHT<Value>::CHT(bool enable_members_delete){

    this -> enable_members_delete = enable_members_delete;
    this -> ls_table = new LSValue<Value>*[INIT_TABLE_SIZE];
    this -> table_size = INIT_TABLE_SIZE;
    this -> num_of_members = 0;
    this -> iterator_ls_val = nullptr;
    this -> iterator_array_index = 0;

    for (int i = 0; i < this -> table_size; ++i) {
        ls_table[i] = nullptr;
    }

}

template<class Value>
CHT<Value>::~CHT(){

    for (int i = 0; i < table_size; ++i) {

        LSValue<Value>* current_ls_val = this -> ls_table[i];

        while(current_ls_val != nullptr){
            if(enable_members_delete){
                delete current_ls_val -> val_ptr;
            }
            LSValue<Value>* temp = current_ls_val;
            current_ls_val = current_ls_val -> next;
            delete temp;
        }

    }

    delete[] ls_table;

}

template<class Value>
void CHT<Value>::insert(Value* val_ptr){

    int calc_index = (val_ptr -> calc_key()) % this -> table_size;

    LSValue<Value>* new_ls_val = new LSValue<Value>;
    new_ls_val -> val_ptr = val_ptr;

    if(this -> ls_table[calc_index] == nullptr){
        this -> ls_table[calc_index] = new_ls_val;
        new_ls_val -> prev = nullptr;
        new_ls_val -> next = nullptr;
    } else{
        LSValue<Value>* current_val = this -> ls_table[calc_index];
        while(current_val -> next != nullptr){
            if(*(current_val -> val_ptr) == *(val_ptr)){
                delete new_ls_val;
                throw std::invalid_argument("ALREADY_EXISTS");
            }
            current_val = current_val -> next;
        }
        if(*(current_val -> val_ptr) == *(val_ptr)){
            delete new_ls_val;
            throw std::invalid_argument("ALREADY_EXISTS");
        }
        current_val -> next = new_ls_val;
        new_ls_val -> prev = current_val;
        new_ls_val -> next = nullptr;
    }

    this -> num_of_members += 1;
    if(((float)(this -> num_of_members)/(float)(this -> table_size)) > LOAD_FACTOR_INCREASE){
        this -> changeTableSize(true);
    }

}

template<class Value>
Value* CHT<Value>::returnValuePtr(const Value& val){

    int calc_index = (val.calc_key()) % this -> table_size;

    if(this -> ls_table[calc_index] == nullptr){
        throw std::invalid_argument("NOT_EXISTS");
    } else{
        LSValue<Value>* current_val = this -> ls_table[calc_index];
        while(current_val -> next != nullptr){
            if(*(current_val -> val_ptr) == val){
                return current_val -> val_ptr;
            }
            current_val = current_val -> next;
        }
        if(*(current_val -> val_ptr) == val){
            return current_val -> val_ptr;
        }
        throw std::invalid_argument("NOT_EXISTS");
    }

}

template<class Value>
void CHT<Value>::deleteValuePtr(const Value& val){

    int calc_index = (val.calc_key()) % this -> table_size;
    bool deleted_val = false;

    if(this -> ls_table[calc_index] == nullptr){
        throw std::invalid_argument("NOT_EXISTS");
    } else{
        LSValue<Value>* current_val = this -> ls_table[calc_index];
        while(current_val -> next != nullptr){
            if(*(current_val -> val_ptr) == val){
                current_val -> next -> prev = current_val  -> prev;
                if(current_val -> prev != nullptr){
                    current_val -> prev -> next = current_val -> next;
                }
                if(this -> enable_members_delete){
                    delete current_val -> val_ptr;
                }
                delete current_val;
                deleted_val = true;
                break;
            }
            current_val = current_val -> next;
        }
        if(*(current_val -> val_ptr) == val){
            if(current_val -> prev != nullptr){
                current_val -> prev -> next = current_val -> next;
            }else{
                this -> ls_table[calc_index] = current_val -> next;
            }
            if(this -> enable_members_delete){
                delete current_val -> val_ptr;
            }
            delete current_val;
            deleted_val = true;
        }
        if(!deleted_val) {
            throw std::invalid_argument("NOT_EXISTS");
        }
    }

    this -> num_of_members -= 1;
    if(((float)(this -> num_of_members)/(float)(this -> table_size)) <
    LOAD_FACTOR_DECREASE){
        this -> changeTableSize(false);
    }
}

template<class Value>
void CHT<Value>::changeTableSize(bool increase){

    if(increase || ((this -> table_size/INCREASE_FACTOR) > INIT_TABLE_SIZE)) {

        int old_size = this->table_size;
        LSValue<Value>**old_ls_table = this->ls_table;

        if (increase) {
            this->table_size = old_size * INCREASE_FACTOR;
            this->ls_table = new LSValue<Value>*[this->table_size];
        } else {
            this->table_size = old_size / INCREASE_FACTOR;
            this->ls_table = new LSValue<Value>*[this->table_size];
        }

        for (int i = 0; i < this->table_size; ++i) {
            this->ls_table[i] = nullptr;
        }

        for (int i = 0; i < old_size; ++i) {
            if (old_ls_table[i] != nullptr) {
                LSValue<Value>*current_val = old_ls_table[i];
                while (current_val != nullptr) {
                    this->insert(current_val->val_ptr);
                    LSValue<Value>*temp = current_val;
                    current_val = current_val->next;
                    delete temp;
                }
            }
        }

        delete[] old_ls_table;
    }

}

template<class Value>
Value* CHT<Value>::iteratorBegin(){

    this -> iterator_array_index = 0;
    this -> iterator_ls_val = nullptr;
    while(this -> iterator_ls_val == nullptr &&
    iterator_array_index < this -> table_size){
        if(this->ls_table[this -> iterator_array_index] != nullptr){
            this -> iterator_ls_val =
                    this->ls_table[this -> iterator_array_index];
        }else{
            this -> iterator_array_index++;
        }
    }

    if(this -> iterator_ls_val != nullptr){
        return this -> iterator_ls_val -> val_ptr;
    }else{
        return nullptr;
    }


}

template<class Value>
Value* CHT<Value>::iteratorNext(){

    LSValue<Value>* old_iterator = iterator_ls_val;
    while((iterator_ls_val == old_iterator ||
    iterator_ls_val == nullptr) &&
    iterator_array_index < this -> table_size){
        if(iterator_ls_val == old_iterator){
            iterator_ls_val = old_iterator -> next;
        }else{
            this -> iterator_array_index++;
            if(iterator_array_index < this -> table_size) {
                iterator_ls_val = this->ls_table[this->iterator_array_index];
            }
        }
    }

    if((this -> iterator_ls_val != nullptr) &&
    (iterator_array_index < this -> table_size)){
        return this -> iterator_ls_val -> val_ptr;
    }else{
        return nullptr;
    }

}

#endif //MIVNIWETEX2_0_CHT_H
