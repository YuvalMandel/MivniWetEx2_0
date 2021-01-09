#include"library.h"
#include"CoursesManager.h"

void *Init() {

    CoursesManager *DS = new CoursesManager();

    return (void*)DS;

}
StatusType AddCourse(void *DS, int courseID, int numOfClasses) {

    if(DS == nullptr || courseID <= 0 || numOfClasses <= 0) return INVALID_INPUT;

    try{
        CoursesManager* CM = (CoursesManager*)DS;
        CM -> AddCourse(courseID, numOfClasses);
    }catch(std::exception& e) {
        if(std::string(e.what()) == "ALLOCATION_ERROR") return ALLOCATION_ERROR;
        if(std::string(e.what()) == "INVALID_INPUT") return INVALID_INPUT;
        if(std::string(e.what()) == "FAILURE") return FAILURE;
    }

    return SUCCESS;
}

StatusType RemoveCourse(void *DS, int courseID) {

    if(DS == nullptr || courseID <= 0) return INVALID_INPUT;

    try{
        CoursesManager* CM = (CoursesManager*)DS;
        CM -> RemoveCourse(courseID);
    }catch(std::exception& e) {
        if(std::string(e.what()) == "ALLOCATION_ERROR") return ALLOCATION_ERROR;
        if(std::string(e.what()) == "INVALID_INPUT") return INVALID_INPUT;
        if(std::string(e.what()) == "FAILURE") return FAILURE;
    }
    return SUCCESS;
}

StatusType WatchClass(void *DS, int courseID, int classID, int time){
    if(DS == nullptr || courseID <= 0 || classID < 0 || time <= 0) return
    INVALID_INPUT;
    try{
        CoursesManager* CM = (CoursesManager*)DS;
        CM -> WatchClass(courseID, classID, time);
    }catch(std::exception& e) {
        if(std::string(e.what()) == "ALLOCATION_ERROR") return ALLOCATION_ERROR;
        if(std::string(e.what()) == "INVALID_INPUT") return INVALID_INPUT;
        if(std::string(e.what()) == "FAILURE") return FAILURE;
    }
    return SUCCESS;
}

StatusType TimeViewed(void *DS, int courseID, int classID, int *timeViewed){
    if(DS == nullptr || courseID <= 0 || classID < 0 || timeViewed == nullptr)
        return INVALID_INPUT;
    try{
        CoursesManager* CM = (CoursesManager*)DS;
        CM -> TimeViewed(courseID, classID, timeViewed);
    }catch(std::exception& e) {
        if(std::string(e.what()) == "ALLOCATION_ERROR") return ALLOCATION_ERROR;
        if(std::string(e.what()) == "INVALID_INPUT") return INVALID_INPUT;
        if(std::string(e.what()) == "FAILURE") return FAILURE;
    }
    return SUCCESS;
}

StatusType GetMostViewedClasses(void *DS, int numOfClasses, int *courses, int
*classes){
    if(DS == nullptr || numOfClasses <= 0 || courses == nullptr || classes ==
    nullptr)
        return INVALID_INPUT;
    try{
        CoursesManager* CM = (CoursesManager*)DS;
        CM -> GetMostViewedClasses(numOfClasses, courses, classes);
    }catch(std::exception& e) {
        if(std::string(e.what()) == "ALLOCATION_ERROR") return ALLOCATION_ERROR;
        if(std::string(e.what()) == "INVALID_INPUT") return INVALID_INPUT;
        if(std::string(e.what()) == "FAILURE") return FAILURE;
    }
    return SUCCESS;
}

void Quit(void** DS){
    CoursesManager* CM = (CoursesManager*)*DS;
    delete CM;
    *DS = nullptr;
}