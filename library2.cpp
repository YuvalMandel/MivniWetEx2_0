#include"library2.h"
#include"CoursesManager.h"

void *Init() {

    CoursesManager *DS = new CoursesManager();

    return (void*)DS;

}
StatusType AddCourse(void* DS, int courseID){

    if(DS == nullptr || courseID <= 0) return INVALID_INPUT;

    try{
        CoursesManager* CM = (CoursesManager*)DS;
        CM -> AddCourse(courseID);
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

StatusType AddClass(void* DS, int courseID, int* classID){

    if(DS == nullptr || courseID <= 0 || classID == nullptr) return
    INVALID_INPUT;

    try{
        CoursesManager* CM = (CoursesManager*)DS;
        CM -> AddClass(courseID, classID);
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

StatusType GetIthWatchedClass(void* DS, int i, int* courseID, int* classID){
    if(DS == nullptr || i <= 0 || courseID == nullptr || classID == nullptr)
        return INVALID_INPUT;
    try{
        CoursesManager* CM = (CoursesManager*)DS;
        CM -> GetIthWatchedClass(i, courseID, classID);
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