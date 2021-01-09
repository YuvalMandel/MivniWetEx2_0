//
// Created by Yuval.M on 04/12/2020.
//
#include "CoursesManager.h"


int timeTree_search(int numOfClasses, int *courses, int *classes, TimeTree* tt_ptr );
int stc_inorder(int numOfClasses, int *courses, int *classes, AVLNode<SubTreeCourse> *stc_node);
int lectures_inorder(int numOfClasses, int *courses, int *classes, AVLNode<Lecture> *lecture_node);

void CoursesManager::AddCourse (int courseID, int numOfClasses) {

    Course* c_ptr = new Course(courseID, numOfClasses);

    if(this -> course_tree -> FindValue(*c_ptr) != nullptr){
        delete c_ptr;
        throw std::invalid_argument("FAILURE");
    }

    // Go one by one and create lectures by id.
    for (int i = 0; i < numOfClasses; ++i) {

        Lecture* l = new Lecture;

        l -> lecture_id = i;
        l -> watch_num = 0;
        l -> holder_sub_tree_course = nullptr;

        // Give pointer to course lectures in the right place.
        c_ptr -> lectures[i] = l;

    }

    TimeTree* tt_ptr;

    // If smallest time is not 0, create it.
    if(this -> smallest_time_tree == nullptr){

        // create time tree 0
        try {
            tt_ptr = new TimeTree;
        }
        catch(std::bad_alloc&)
        {
            throw std::invalid_argument("ALLOCATION_ERROR");
        }

        tt_ptr -> time_watched = 0;
        tt_ptr -> bigger = nullptr;
        tt_ptr -> smaller = nullptr;
        this -> smallest_time_tree = tt_ptr;
        this -> largest_time_tree = tt_ptr;
    } else if(this -> smallest_time_tree -> time_watched > 0){

        // create time tree 0.
        try {
            tt_ptr = new TimeTree;
        }
        catch(std::bad_alloc&)
        {
            throw std::invalid_argument("ALLOCATION_ERROR");
        }

        tt_ptr -> time_watched = 0;
        tt_ptr -> bigger = this -> smallest_time_tree;
        tt_ptr -> bigger -> smaller = tt_ptr;
        tt_ptr -> smaller = nullptr;
        this -> smallest_time_tree = tt_ptr;
    }

    // Create subtree course from course lectures and insert
    SubTreeCourse* stc_ptr = new SubTreeCourse(courseID,
                                               c_ptr -> lectures,
                                               numOfClasses,
                                               (void*)this -> smallest_time_tree);

    for (int i = 0; i < numOfClasses; ++i) {
        c_ptr -> lectures[i]->holder_sub_tree_course = stc_ptr;
    }

    // Add stc to tt.
    this -> smallest_time_tree -> subtree_tree.Insert(stc_ptr);

    this -> course_tree -> Insert(c_ptr);

}


void CoursesManager::RemoveCourse(int courseID){

	Course temp(courseID, 1);

	AVLNode<Course> *course_node = this-> course_tree -> FindValue(temp);

    if(course_node == nullptr){
        throw std::invalid_argument("FAILURE");
    }

	Course* c_ptr = course_node -> val_ptr;

	for(int i=0; i < c_ptr -> lectures_num; i++){

		SubTreeCourse* stc_ptr =
		        (SubTreeCourse*)(c_ptr-> lectures[i] -> holder_sub_tree_course);

		stc_ptr -> lectures_tree -> Remove(*(c_ptr -> lectures[i])); // Do we
		// actually delete lecture.

		// If stc is empty, remove it from the time tree.
		if(stc_ptr->lectures_tree -> root == nullptr){

			TimeTree* tt_ptr = (TimeTree*)stc_ptr -> holder_time_tree;

			tt_ptr -> subtree_tree.Remove(*stc_ptr); // Actually deletes.

			if(tt_ptr->subtree_tree.root == nullptr){

				if(tt_ptr -> bigger != nullptr){
                    tt_ptr -> bigger -> smaller = tt_ptr -> smaller;
				} else{
					this -> largest_time_tree = tt_ptr -> smaller;
				}

				if(tt_ptr -> smaller != nullptr){
                    tt_ptr -> smaller -> bigger = tt_ptr -> bigger;
				} else{
					this -> smallest_time_tree = tt_ptr -> bigger;
				}

				delete tt_ptr;
			
			}
		}

	}


	this -> course_tree -> Remove(*c_ptr); // Deletes course, but not the values
	// in the course array.

}


// We delete the actual lecture!!!, create a new one and insret to new stc,
// and update the course lectures array.
void CoursesManager::WatchClass(int courseID, int classID, int time){
	
	Course temp(courseID, 1);

	AVLNode<Course> *course_node = this-> course_tree -> FindValue(temp);

    if(course_node == nullptr){
        throw std::invalid_argument("FAILURE");
    }

    if(course_node ->val_ptr -> lectures_num < classID + 1){
        throw std::invalid_argument("INVALID_INPUT");
    }

	Lecture* original_lecture_ptr = (course_node -> val_ptr) -> lectures[classID];

	Lecture* new_lecture_ptr =  new Lecture(*original_lecture_ptr);

	SubTreeCourse* original_stc_ptr = (SubTreeCourse*)original_lecture_ptr -> holder_sub_tree_course;

	original_stc_ptr -> lectures_tree -> Remove(*original_lecture_ptr); //TODO

	TimeTree* original_tt_ptr = (TimeTree*)original_stc_ptr -> holder_time_tree;

	TimeTree* current_tt_ptr = original_tt_ptr;

    new_lecture_ptr -> watch_num = original_tt_ptr -> time_watched + time;

	while(current_tt_ptr -> bigger != nullptr){
	    if(current_tt_ptr -> bigger -> time_watched >
	        original_tt_ptr -> time_watched + time){
	        break;
	    }
        current_tt_ptr = current_tt_ptr -> bigger;
	}

	if(current_tt_ptr -> time_watched == original_tt_ptr -> time_watched + time){

		AVLNode<SubTreeCourse>* current_stc_node = current_tt_ptr -> subtree_tree.FindValue(*original_stc_ptr);

		if (current_stc_node){
            new_lecture_ptr -> holder_sub_tree_course =
			        current_stc_node -> val_ptr;
            (course_node -> val_ptr) -> lectures[classID] = new_lecture_ptr;
            current_stc_node -> val_ptr -> lectures_tree -> Insert(new_lecture_ptr);
		}else{
			SubTreeCourse* new_stc_ptr = new SubTreeCourse;
            new_stc_ptr -> holder_time_tree = current_tt_ptr;
            new_stc_ptr -> course_id = courseID;
            new_lecture_ptr -> holder_sub_tree_course = new_stc_ptr;
            (course_node -> val_ptr) -> lectures[classID] = new_lecture_ptr;
            new_stc_ptr -> lectures_tree -> Insert(new_lecture_ptr);
            current_tt_ptr -> subtree_tree.Insert(new_stc_ptr);
		}
	}else{

        TimeTree* new_tt_ptr;

        try {
            new_tt_ptr = new TimeTree;
        }
        catch(std::bad_alloc&)
        {
            throw std::invalid_argument("ALLOCATION_ERROR");
        }

        new_tt_ptr -> time_watched = new_lecture_ptr -> watch_num;
        new_tt_ptr -> smaller = current_tt_ptr;
        new_tt_ptr -> bigger = current_tt_ptr -> bigger;
        current_tt_ptr -> bigger = new_tt_ptr;
		if(new_tt_ptr -> bigger != nullptr){
            new_tt_ptr -> bigger -> smaller = new_tt_ptr;
		}else{
		    this -> largest_time_tree = new_tt_ptr;
		}

		SubTreeCourse* new_stc_ptr = new SubTreeCourse;
        new_stc_ptr -> holder_time_tree = new_tt_ptr;
        new_stc_ptr -> course_id = courseID;
        new_lecture_ptr -> holder_sub_tree_course = new_stc_ptr;
        (course_node -> val_ptr) -> lectures[classID] = new_lecture_ptr;
        new_stc_ptr -> lectures_tree -> Insert(new_lecture_ptr);
        new_tt_ptr -> subtree_tree.Insert(new_stc_ptr);
	}
	
	// If original stc is empty, remove it from the time tree.
	if(original_stc_ptr->lectures_tree -> root == nullptr){

		TimeTree* tt_ptr = (TimeTree*)original_stc_ptr -> holder_time_tree;
		tt_ptr -> subtree_tree.Remove(*original_stc_ptr);

		if(tt_ptr -> subtree_tree.root == nullptr){

			if(tt_ptr -> bigger != nullptr){
                tt_ptr -> bigger -> smaller = tt_ptr -> smaller;
			} else{
				this -> largest_time_tree = tt_ptr -> smaller;
			}

			if(tt_ptr -> smaller != nullptr){
                tt_ptr -> smaller -> bigger = tt_ptr -> bigger;
			} else{
				this -> smallest_time_tree = tt_ptr -> bigger;
			}

			delete tt_ptr;
		
		}
	}

}


void CoursesManager::TimeViewed(int courseID, int classID, int *timeViewed){
	Course temp(courseID, 1);

	AVLNode<Course> *course_node = this-> course_tree -> FindValue(temp);

    if(course_node == nullptr){
        throw std::invalid_argument("FAILURE");
    }

    if(course_node -> val_ptr -> lectures_num < classID + 1){
        throw std::invalid_argument("INVALID_INPUT");
    }

	Lecture *lecture_ptr = (course_node -> val_ptr) -> lectures[classID];
	*timeViewed = lecture_ptr -> watch_num;
}


void CoursesManager::GetMostViewedClasses(
        int numOfClasses, int *courses, int *classes){

	int num_Of_Classes_left = timeTree_search(numOfClasses, courses, classes,
                                         this -> largest_time_tree);



	if(num_Of_Classes_left > 0){
	    throw std::invalid_argument("FAILURE");
	}
}

int timeTree_search(
        int numOfClasses, int *courses, int *classes, TimeTree* tt_ptr ){

	if(tt_ptr == nullptr){
		return numOfClasses;
	}

	int num_Of_Classes_left = stc_inorder(numOfClasses, courses, classes,
                                          tt_ptr -> subtree_tree.root);

	if(num_Of_Classes_left > 0){
		return timeTree_search(num_Of_Classes_left,
                         &(courses[numOfClasses - num_Of_Classes_left]),
                         &(classes[numOfClasses - num_Of_Classes_left]),
                         tt_ptr -> smaller);
	}
	else{
		return 0;
	}
}

int stc_inorder(int numOfClasses, int *courses, int *classes,
                AVLNode<SubTreeCourse> *stc_node){

	if(stc_node == nullptr){
		return numOfClasses;
	}

	int num_Of_Classes_left =
	        stc_inorder(numOfClasses,courses, classes, stc_node->left_son);

	if(num_Of_Classes_left == 0){
		return 0;
	}
	else{
		num_Of_Classes_left =
		        lectures_inorder(num_Of_Classes_left,
                &courses[numOfClasses - num_Of_Classes_left],
                &classes[numOfClasses - num_Of_Classes_left],
                stc_node -> val_ptr -> lectures_tree -> root);
	}
	if(num_Of_Classes_left == 0) {
        return 0;
    }else{
		num_Of_Classes_left =
		        stc_inorder(num_Of_Classes_left,
                &courses[numOfClasses - num_Of_Classes_left],
                &classes[numOfClasses - num_Of_Classes_left],
                stc_node->right_son);

        return num_Of_Classes_left;
    }
}

int lectures_inorder(int numOfClasses, int *courses, int *classes,
                     AVLNode<Lecture> *lecture_node){

	if(lecture_node == nullptr){
		return numOfClasses;
	}

	int num_Of_Classes_left = lectures_inorder(numOfClasses, courses,
                                            classes, lecture_node->left_son);
	
	if(num_Of_Classes_left == 0) {
        return 0;
    }else{
		classes[numOfClasses - num_Of_Classes_left] =
		        lecture_node-> val_ptr -> lecture_id;
		SubTreeCourse* temp_stc_ptr =
		        (SubTreeCourse*)lecture_node-> val_ptr ->
		        holder_sub_tree_course;
		courses[numOfClasses - num_Of_Classes_left] =
		        temp_stc_ptr -> course_id;
		num_Of_Classes_left -= 1;
	}
	if(num_Of_Classes_left == 0) {
        return 0;
    }else{
		num_Of_Classes_left = lectures_inorder(num_Of_Classes_left,
               &courses[numOfClasses - num_Of_Classes_left],
               &classes[numOfClasses - num_Of_Classes_left],
               lecture_node -> right_son);

        return num_Of_Classes_left;
	}
}

CoursesManager::CoursesManager(){
    this -> smallest_time_tree = nullptr;
    this -> largest_time_tree = nullptr;
    this -> course_tree = new AVLTree<Course>;
}

CoursesManager::~CoursesManager(){

    // Go to each tree from smallest to largest and call delete each tree,so
    // each tree destructor will be called.
    TimeTree* current_tt_ptr = this -> smallest_time_tree;
    TimeTree* next_tt_ptr;

    while(current_tt_ptr != nullptr){
        next_tt_ptr = current_tt_ptr -> bigger;
        delete current_tt_ptr;
        current_tt_ptr = next_tt_ptr;
    }

    // The course tree destructor will be called in the end of the time trees.
    delete this -> course_tree;

}


Course::~Course(){

    if(this -> lectures != nullptr) {
        delete[] this -> lectures;
    }

}

Course::Course(int course_id, int lectures_num){

    this->course_id = course_id;
    this->lectures_num=lectures_num;
    try {
        this->lectures = new Lecture*[lectures_num];
    }
    catch(std::bad_alloc&) {
        throw std::invalid_argument("ALLOCATION_ERROR");
    }

}

Course::Course(const Course& c){

    this->course_id = c.course_id;
    this->lectures_num = c.lectures_num;
    try {
        this->lectures=new Lecture*[lectures_num];
    }
    catch(std::bad_alloc&) {
        throw std::invalid_argument("ALLOCATION_ERROR");
    }
    for (int i = 0; i < c.lectures_num; ++i) {
        this->lectures[i] = c.lectures[i];
    }

}

Course& Course::operator=(const Course& c){
    if (this == &c) {
        return *this;
    }
    delete this->lectures;
    this->course_id = c.course_id;
    this->lectures_num = c.lectures_num;
    try {
        this->lectures = new Lecture*[c.lectures_num];
    }
    catch(std::bad_alloc&) {
        throw std::invalid_argument("ALLOCATION_ERROR");
    }
    for (int i = 0; i < c.lectures_num; ++i) {
        this->lectures[i] = c.lectures[i];
    }
    return *this;
}

SubTreeCourse::SubTreeCourse(){
    this -> lectures_tree = new AVLTree<Lecture>;
    this -> holder_time_tree = nullptr;
    this -> course_id = -1;
}

SubTreeCourse::SubTreeCourse(int course_id, Lecture** lectures, int lectures_num, void*
holder_time_tree){
    this -> lectures_tree = new AVLTree<Lecture>(lectures, lectures_num);
    this -> holder_time_tree = holder_time_tree;
    this -> course_id = course_id;
}

SubTreeCourse::~SubTreeCourse(){
    delete this -> lectures_tree;
    this -> holder_time_tree = nullptr;
}

//SubTreeCourse::SubTreeCourse(const SubTreeCourse& stc){
//    this->course_id=stc.course_id;
//    this->holder_time_tree=stc.holder_time_tree;
//    this->lectures_tree=stc.lectures_tree;
//    this -> lectures_tree = new
//}
//
//SubTreeCourse& SubTreeCourse::operator=(const SubTreeCourse& stc){
//    this->course_id=stc.course_id;
//    this->holder_time_tree=stc.holder_time_tree;
//    this->lectures_tree=stc.lectures_tree;
//    return *this;
//}



Lecture::Lecture(){
    this->lecture_id=0;
    this->watch_num=0;
    this->holder_sub_tree_course = nullptr;
}
Lecture::Lecture(const Lecture& l){
    this->lecture_id=l.lecture_id;
    this->watch_num=l.watch_num;
    this->holder_sub_tree_course=l.holder_sub_tree_course;
}
Lecture& Lecture::operator=(const Lecture& l){
    this->lecture_id=l.lecture_id;
    this->watch_num=l.watch_num;
    this->holder_sub_tree_course=l.holder_sub_tree_course;
    return *this;
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

bool operator<(const SubTreeCourse& c1, const SubTreeCourse& c2){
    return c1.course_id < c2.course_id;
}

bool operator>(const SubTreeCourse& c1, const SubTreeCourse& c2){
    return c2 < c1;
}

bool operator==(const SubTreeCourse& c1, const SubTreeCourse& c2){
    return !(c2 < c1) && !(c1 < c2);
}

bool operator<=(const SubTreeCourse& c1, const SubTreeCourse& c2){
    return !(c2 < c1);
}

bool operator>=(const SubTreeCourse& c1, const SubTreeCourse& c2){
    return !(c1 < c2);
}

bool operator<(const Lecture& c1, const Lecture& c2){
    return c1.lecture_id < c2.lecture_id;
}

bool operator>(const Lecture& c1, const Lecture& c2){
    return c2 < c1;
}

bool operator==(const Lecture& c1, const Lecture& c2){
    return !(c2 < c1) && !(c1 < c2);
}

bool operator<=(const Lecture& c1, const Lecture& c2){
    return !(c2 < c1);
}

bool operator>=(const Lecture& c1, const Lecture& c2){
    return !(c1 < c2);
}