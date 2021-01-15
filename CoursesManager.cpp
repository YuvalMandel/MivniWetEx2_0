//
// Created by Yuval.M on 04/12/2020.
//
#include "CoursesManager.h"


void CoursesManager::AddCourse(int courseID) {

    Course* c_ptr = nullptr;

    try {
        c_ptr = new Course(courseID);
    }
    catch(std::bad_alloc&) {
        throw std::invalid_argument("ALLOCATION_ERROR");
    }

    // When insert already exists, delete c_ptr and throw.
    try{
        this -> course_cht -> insert(c_ptr);
    }catch(std::exception& e) {
        if(std::string(e.what()) == "ALREADY_EXISTS"){
            delete c_ptr;
            throw std::invalid_argument("FAILURE");
        }
    }

}


void CoursesManager::RemoveCourse(int courseID){

    int temp_int;

    Course temp(courseID+temp_int);

	Course* c_ptr = nullptr;

    try{
        c_ptr = this -> course_cht -> returnValuePtr(temp);
    }catch(std::exception& e) {
        if(std::string(e.what()) == "NOT_EXISTS"){
            delete c_ptr;
            throw std::invalid_argument("FAILURE");
        }
    }

    Lecture* l_ptr = c_ptr -> lectures_cht -> iteratorBegin();

    while(l_ptr != nullptr){
        if(l_ptr -> time == 0){
            this -> non_watched_lectures_cht -> deleteValuePtr(*(l_ptr));
        }else{
            this -> watch_lectures_avl -> Remove(*(l_ptr));
        }
        l_ptr = c_ptr -> lectures_cht -> iteratorNext();
    }

	this -> course_cht -> deleteValuePtr(*c_ptr);

}

void CoursesManager::AddClass(int courseID, int* classID){

    Course temp(courseID);

    Course* c_ptr;

    try{
        c_ptr = this -> course_cht -> returnValuePtr(temp);
    }catch(std::exception& e) {
        if(std::string(e.what()) == "NOT_EXISTS"){
            throw std::invalid_argument("FAILURE");
        }
    }

    *classID = c_ptr -> lectures_num;

    Lecture* l_ptr = new Lecture(courseID, *classID);

    c_ptr -> lectures_num++;

    c_ptr -> lectures_cht -> insert(l_ptr);

    this -> non_watched_lectures_cht -> insert(l_ptr);

}

void CoursesManager::WatchClass(int courseID, int classID, int time){

    Course c_temp(courseID);

    Course* c_ptr;

    try{
        c_ptr = this -> course_cht -> returnValuePtr(c_temp);
    }catch(std::exception& e) {
        if(std::string(e.what()) == "NOT_EXISTS"){
            throw std::invalid_argument("FAILURE");
        }
    }

    if(c_ptr -> lectures_num < classID + 1){
        throw std::invalid_argument("INVALID_INPUT");
    }

    Lecture l_temp(courseID, classID);

    Lecture* l_ptr;

    try{
        l_ptr = c_ptr -> lectures_cht -> returnValuePtr(l_temp);
    }catch(std::exception& e) {
        if(std::string(e.what()) == "NOT_EXISTS"){
            throw std::invalid_argument("FAILURE");
        }
    }

    // Prepare new lecture.
    Lecture* new_l_ptr = new Lecture(*l_ptr);
    new_l_ptr -> time += time;

    // Remove old.
    c_ptr -> lectures_cht -> deleteValuePtr(*l_ptr);
    if(l_ptr -> time == 0){
        this -> non_watched_lectures_cht -> deleteValuePtr(*(l_ptr));
    }else{
        this -> watch_lectures_avl -> Remove(*(l_ptr));
    }

    // Insert new.
    c_ptr -> lectures_cht -> insert(new_l_ptr);
    this -> watch_lectures_avl -> Insert(new_l_ptr);

}


void CoursesManager::TimeViewed(int courseID, int classID, int *timeViewed){
    Course c_temp(courseID);

    Course* c_ptr;

    try{
        c_ptr = this -> course_cht -> returnValuePtr(c_temp);
    }catch(std::exception& e) {
        if(std::string(e.what()) == "NOT_EXISTS"){
            throw std::invalid_argument("FAILURE");
        }
    }

    if(c_ptr -> lectures_num < classID + 1){
        throw std::invalid_argument("INVALID_INPUT");
    }

    Lecture l_temp(courseID, classID);

    Lecture* l_ptr;

    try{
        l_ptr = c_ptr -> lectures_cht -> returnValuePtr(l_temp);
    }catch(std::exception& e) {
        if(std::string(e.what()) == "NOT_EXISTS"){
            throw std::invalid_argument("FAILURE");
        }
    }

    *timeViewed = l_ptr -> time;

}


void CoursesManager::GetIthWatchedClass(int i, int* courseID, int* classID){

    if(this -> watch_lectures_avl -> root == nullptr){
        throw std::invalid_argument("FAILURE");
    }

    int num_of_watched_classes = (this -> watch_lectures_avl -> root -> rank);

    if(num_of_watched_classes < i){
        throw std::invalid_argument("FAILURE");
    }

	int index = num_of_watched_classes - i + 1;

	Lecture* l_ptr =
	        (this -> watch_lectures_avl -> FindValueByIndex(index)) -> val_ptr;

	if(l_ptr == nullptr){
        throw std::invalid_argument("FAILURE");
	}

	*courseID = l_ptr -> course_id;
    *classID = l_ptr -> lecture_id;

}

//int lectures_inorder(int numOfClasses, int *courses, int *classes,
//                     AVLNode<Lecture> *lecture_node){
//
//	if(lecture_node == nullptr){
//		return numOfClasses;
//	}
//
//	int num_Of_Classes_left = lectures_inorder(numOfClasses, courses,
//                                            classes, lecture_node->left_son);
//
//	if(num_Of_Classes_left == 0) {
//        return 0;
//    }else{
//		classes[numOfClasses - num_Of_Classes_left] =
//		        lecture_node-> val_ptr -> lecture_id;
//		SubTreeCourse* temp_stc_ptr =
//		        (SubTreeCourse*)lecture_node-> val_ptr ->
//		        holder_sub_tree_course;
//		courses[numOfClasses - num_Of_Classes_left] =
//		        temp_stc_ptr -> course_id;
//		num_Of_Classes_left -= 1;
//	}
//	if(num_Of_Classes_left == 0) {
//        return 0;
//    }else{
//		num_Of_Classes_left = lectures_inorder(num_Of_Classes_left,
//               &courses[numOfClasses - num_Of_Classes_left],
//               &classes[numOfClasses - num_Of_Classes_left],
//               lecture_node -> right_son);
//
//        return num_Of_Classes_left;
//	}
//}

CoursesManager::CoursesManager(){

    try {
        this -> course_cht = new CHT<Course>(true);
    }
    catch(std::bad_alloc&) {
        throw std::invalid_argument("ALLOCATION_ERROR");
    }

    try {
        this -> watch_lectures_avl = new AVLTree<Lecture>;
    }
    catch(std::bad_alloc&) {
        delete this -> course_cht;
        throw std::invalid_argument("ALLOCATION_ERROR");
    }

    try {
        this -> non_watched_lectures_cht = new CHT<Lecture>(true);
    }
    catch(std::bad_alloc&) {
        delete this -> course_cht;
        delete this -> watch_lectures_avl;
        throw std::invalid_argument("ALLOCATION_ERROR");
    }

}

CoursesManager::~CoursesManager(){

    delete this -> course_cht;
    delete this -> watch_lectures_avl;
    delete this -> non_watched_lectures_cht;

}

Course::~Course(){

    if(this -> lectures_cht != nullptr) {
        delete this -> lectures_cht;
    }

}

Course::Course(int course_id){

    this -> course_id = course_id;
    this -> lectures_num = 0;
    try {
        this -> lectures_cht = new CHT<Lecture>(false);
    }
    catch(std::bad_alloc&) {
        throw std::invalid_argument("ALLOCATION_ERROR");
    }

}

unsigned long long Course::calc_key() const{
    return this -> course_id;
}

//Course::Course(const Course& c){
//
//    this -> course_id = c.course_id;
//    this -> lectures_num = c.lectures_num;
//    try {
//        this -> lectures_cht = new CHT<Course>(false);
//    }
//    catch(std::bad_alloc&) {
//        throw std::invalid_argument("ALLOCATION_ERROR");
//    }
//    for (int i = 0; i < c.lectures_num; ++i) {
//        this->lectures[i] = c.lectures[i];
//    }
//
//}

//Course& Course::operator=(const Course& c){
//    if (this == &c) {
//        return *this;
//    }
//    delete this->lectures;
//    this->course_id = c.course_id;
//    this->lectures_num = c.lectures_num;
//    try {
//        this->lectures = new Lecture*[c.lectures_num];
//    }
//    catch(std::bad_alloc&) {
//        throw std::invalid_argument("ALLOCATION_ERROR");
//    }
//    for (int i = 0; i < c.lectures_num; ++i) {
//        this->lectures[i] = c.lectures[i];
//    }
//    return *this;
//}

Lecture::Lecture(int course_id, int lecture_id){
    this -> lecture_id = lecture_id;
    this -> course_id = course_id;
    this -> time = 0;
//    this->holder_sub_tree_course = nullptr;
}
Lecture::Lecture(const Lecture& l){
    this->lecture_id=l.lecture_id;
    this -> course_id = l.course_id;
    this->time=l.time;
//    this->holder_sub_tree_course=l.holder_sub_tree_course;
}
Lecture& Lecture::operator=(const Lecture& l){
    this->lecture_id=l.lecture_id;
    this -> course_id = l.course_id;
    this->time=l.time;
//    this->holder_sub_tree_course=l.holder_sub_tree_course;
    return *this;
}

unsigned long long Lecture::calc_key() const{

    unsigned long long result = (this -> course_id);

    result  = result << 32;

    result += this -> lecture_id;

    return result;
}

bool operator<(const Course& c1, const Course& c2){
    return c1.course_id < c2.course_id;
}

bool operator>(const Course& c1, const Course& c2){
    return c2 < c1;
}

bool operator==(const Course& c1, const Course& c2){
    return !(c2 < c1) && !(c1 < c2);
}

bool operator<=(const Course& c1, const Course& c2){
    return !(c2 < c1);
}

bool operator>=(const Course& c1, const Course& c2){
    return !(c1 < c2);
}

bool operator<(const Lecture& c1, const Lecture& c2) {
    if(c1.time == c2.time){
        if (c1.course_id == c2.course_id) {
            return c1.lecture_id > c2.lecture_id;
        } else {
            return c1.course_id > c2.course_id;
        }
    }else{
        return c1.time < c2.time;
    }
}

bool operator>(const Lecture& c1, const Lecture& c2){
    return c2 < c1;
}

bool operator==(const Lecture& c1, const Lecture& c2){
//    return !(c2 < c1) && !(c1 < c2);
    return (c1.course_id == c2.course_id) && (c1.lecture_id == c2.lecture_id);
}

bool operator<=(const Lecture& c1, const Lecture& c2){
    return !(c2 < c1);
}

bool operator>=(const Lecture& c1, const Lecture& c2){
    return !(c1 < c2);
}