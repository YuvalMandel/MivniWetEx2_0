//
// Created by Yuval.M on 04/12/2020.
//

#ifndef MIVNIWETEX1_0_COURSESMANAGER_H
#define MIVNIWETEX1_0_COURSESMANAGER_H

#include <iostream>
#include "library.h"
#include "AVL.h"
#include "CHT.h"

class Lecture{

public:

    int lecture_id;
    int course_id;
    int watch_num;
//    void* holder_sub_tree_course;

    Lecture();
    ~Lecture() = default;
    Lecture(const Lecture& l);
    Lecture& operator=(const Lecture& l);

    int calc_key();

    friend bool operator<(const Lecture& c1, const Lecture& c2);
    friend bool operator>(const Lecture& c1, const Lecture& c2);
    friend bool operator==(const Lecture& c1, const Lecture& c2);
    friend bool operator<=(const Lecture& c1, const Lecture& c2);
    friend bool operator>=(const Lecture& c1, const Lecture& c2);

};

class Course{

public:

    Course(int course_id, int lectures_num);
    ~Course();
    Course(const Course& c);
    Course& operator=(const Course& c);
    int lectures_num;
    int course_id;
    int calc_key();

    CHT<Lecture>* lectures_cht;

    friend bool operator<(const Course& c1, const Course& c2);
    friend bool operator>(const Course& c1, const Course& c2);
    friend bool operator==(const Course& c1, const Course& c2);
    friend bool operator<=(const Course& c1, const Course& c2);
    friend bool operator>=(const Course& c1, const Course& c2);

};

class CoursesManager {
private:

    // This is the root of the course tree.
    CHT<Course>* course_cht;

    // This is the watched AVL lecture tree.
    AVLTree<Lecture>* watch_lectures_avl;

    // This si the lectures unwatched cht.
    CHT<Lecture>* non_watched_lectures_cht;


public:

    CoursesManager();
    ~CoursesManager();

    void AddCourse (int courseID, int numOfClasses);

    void RemoveCourse(int courseID);

    void AddClass(int courseID, int* classID);

    void WatchClass(int courseID, int classID, int time);

    void TimeViewed(int courseID, int classID, int *timeViewed);

    void GetIthWatchedClass(int i, int* courseID, int* classID);


};

#endif //MIVNIWETEX1_0_COURSESMANAGER_H
