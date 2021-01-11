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

    void changeTableSize(bool increase);

};

template<class Value>
CHT::CHT(bool enable_members_delete){

    this -> enable_members_delete = enable_members_delete;
    this -> ls_table = new LSValue*[INIT_TABLE_SIZE];
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

template<class Value>
void CHT::insert(*Value val_ptr){

    int calc_index = (val_ptr -> calc_key()) % this -> table_size;

    LSValue* new_ls_val = new LSValue;
    new_ls_val -> val_ptr = val_ptr;

    if(this -> trees_array[calc_index] == nullptr){
        this -> trees_array[calc_index] = new_ls_val;
        new_ls_val -> prev = nullptr;
        new_ls_val -> next = nullptr;
    } else{
        LSValue* current_val = this -> trees_array[calc_index];
        while(current_val -> next != nullptr){
            if(*(current -> val_ptr) == *(val_ptr)){
                delete new_ls_val;
                throw std::invalid_argument("ALREADY_EXISTS");
            }
            current_val = current_val -> next;
        }
        if(*(current -> val_ptr) == *(val_ptr)){
            delete new_ls_val;
            throw std::invalid_argument("ALREADY_EXISTS");
        }
        current_val -> next = new_ls_val;
        new_ls_val -> prev = current_val;
        new_ls_val -> next = nullptr;
    }

    this -> num_of_members += 1;
    if(((this -> num_of_members)/(this -> table_size)) > LOAD_FACTOR_INCREASE){
        this -> changeTableSize(true);
    }

}

*Value CHT::returnValuePtr(const Value& val){

    int calc_index = (val_ptr -> calc_key()) % this -> table_size;

    if(this -> trees_array[calc_index] == nullptr){
        throw std::invalid_argument("NOT_EXISTS");
    } else{
        LSValue* current_val = this -> trees_array[calc_index];
        while(current_val -> next != nullptr){
            if(*(current -> val_ptr) == val){
                return current -> val_ptr;
            }
            current_val = current_val -> next;
        }
        if(*(current -> val_ptr) == val){
            return current -> val_ptr;
        }
        throw std::invalid_argument("NOT_EXISTS");
    }

}

void CHT::deleteValuePtr(const Value& val){

    int calc_index = (val_ptr -> calc_key()) % this -> table_size;

    if(this -> trees_array[calc_index] == nullptr){
        throw std::invalid_argument("NOT_EXISTS");
    } else{
        LSValue* current_val = this -> trees_array[calc_index];
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
                break;
            }
            current_val = current_val -> next;
        }
        if(*(current_val -> val_ptr) == val){
            if(current_val -> prev != nullptr){
                current_val -> prev -> next = current_val -> next;
            }else{
                this -> trees_array[calc_index] = current_val -> next;
            }
            if(this -> enable_members_delete){
                delete current_val -> val_ptr;
            }
            delete current_val;
            break;
        }
        throw std::invalid_argument("NOT_EXISTS");
    }

    this -> num_of_members -= 1;
    if(((this -> num_of_members)/(this -> table_size)) < LOAD_FACTOR_DECREASE){
        this -> changeTableSize(false);
    }
}

void CHT::changeTableSize(bool increase){

    if(increase || ((this -> table_size/INCREASE_FACTOR) > INIT_TABLE_SIZE)) {

        int old_size = this->table_size;
        LSValue **old_ls_table = this->ls_table;

        if (increae) {
            this->table_size = old_size * INCREASE_FACTOR;
            this->ls_table = new LSValue *[this->table_size];
        } else {
            this->table_size = old_size / INCREASE_FACTOR;
            this->ls_table = new LSValue *[this->table_size];
        }

        for (int i = 0; i < this->table_size; ++i) {
            this->ls_table[i] = nullptr;
        }

        for (int i = 0; i < old_size; ++i) {
            if (old_ls_table[i] != nullptr) {
                LSValue *current_val = old_ls_table[i];
                while (current_val != nullptr) {
                    this->insert(current_val->val_ptr);
                    LSValue *temp = current_val;
                    current_val = current_val->next;
                    delete temp;
                }
            }
        }

        delete[] old_ls_table;
    }

}



#endif //MIVNIWETEX2_0_CHT_H
