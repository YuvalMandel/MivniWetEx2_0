""" ===================================================================================================================

Written by Yaniv Wolf.
Last updated 07/01/2021.

Stress Test generator for DS1 Wet2.

Generates an in and out test file in the following format:
- Init
- All AddCourse commands in random order/sorted (can be set using SHUFFLE_INTERNAL_COMMAND_ORDER)
- All AddClass commands in random order/sorted (can be set using SHUFFLE_INTERNAL_COMMAND_ORDER)
- All WatchClass commands of non-zero classes in random order/sorted (can be set using SHUFFLE_INTERNAL_COMMAND_ORDER)
- All TimeViewed commands in random order/sorted (can be set using SHUFFLE_INTERNAL_COMMAND_ORDER)
- All GetIthViewedClass commands in random order/sorted (can be set using SHUFFLE_INTERNAL_COMMAND_ORDER)
- All RemoveCourse commands in random order/sorted (can be set using SHUFFLE_INTERNAL_COMMAND_ORDER)
- Quit

All generated input is legal, meaning all functions return SUCCESS and perform the required action.
Therefore, run these tests once you are sure that your functions work properly.

=================================================================================================================== """

""" ========================================================
Import (DO NOT EDIT):
======================================================== """

import os
import math
import random

""" ========================================================
Set parameters (EDIT HERE):
======================================================== """

# Set 1 to shuffle internal order of each command, or 0 not to shuffle.
SHUFFLE_INTERNAL_COMMAND_ORDER = 1

# Set max parameters for random.
MAX_COURSE_ID = 1000000 #Keep it higher than NUMBER_OF_COURSES
MAX_CLASSES_IN_COURSE = 10 #Keep it low, or bad things will happen!!
MAX_TIME_VIEWED = 50

# Set parameters for size of test.
NUMBER_OF_COURSES = 10000 #Don't go too wild, espcially if you're testing on the csl3 server.

# Set the directory to which you want to save the test files. 
# If you want to use the current directory, leave a blank string ("").
# Example for Windows directory: "C:\\Users\\USERNAME\\Technion\\Courses\\234218_Data_Structures_1\\HW\\Wet_2\\tests"
# Example for Linux directory: "/home/USERNAME/ds1/wet2/tests"
DIRECTORY = ""

# Set names of test in and out files. Recommended format: "testStress1.in" and "testStress1.out"
TEST_IN_NAME = "testStress1.in"
TEST_OUT_NAME = "testStress1.out"

""" ========================================================
Code (DO NOT EDIT FROM HERE):
======================================================== """

# ==================================
# Generate random data for testing:
# ==================================
def generateData():
    # Generate unique course IDs.
    courseIDs = random.sample(range(1,MAX_COURSE_ID), NUMBER_OF_COURSES)
    # Generate the number of classe for each course.
    courses = {courseIDs[i] : random.randint(1,MAX_CLASSES_IN_COURSE) for i in range(NUMBER_OF_COURSES)}
    # Create a list of tuples (CourseID,ClassID,time_viewed).
    classes = []
    for courseID, num_classes in courses.items():
        for i in range(num_classes):
            specific_class = (courseID, i, random.randint(0,MAX_TIME_VIEWED))
            classes.append(specific_class)
    # Sort the list based on the parameters of the assignment.
    classes = sorted(classes, key = lambda x: (x[0],x[1]))
    classes = sorted(classes, key = lambda x: (x[2]), reverse=True)
    return courses, classes

# =========================
# Write to the test files:
# =========================
def writeTestFiles(courses, classes):

    with open (TEST_IN_NAME, "w") as test_in:
        with open (TEST_OUT_NAME, "w") as test_out:

            # =====
            # Init
            # =====
            test_in.write("Init\n")
            test_out.write("init done.\n")

            # ==========
            # AddCourse
            # ==========
            # Add all of the AddCourse commands and outputs for all courses to buffers.
            buffer_in = []
            buffer_out = []
            for courseID,num_classes in courses.items():
                buffer_in.append("AddCourse " + str(courseID) + "\n")
                buffer_out.append(("AddCourse: SUCCESS\n"))
            # Shuffle if needed and write to the test files.
            writeToTests(buffer_in, test_in, buffer_out, test_out)

            # =========
            # AddClass
            # =========
            # Add all of the AddClass commands and outputs for all classes to buffers.
            buffer_in = []
            buffer_out = []
            courseID_order = []
            classes_seen = {courseID:[num_classes,0] for courseID,num_classes in courses.items()}
            for courseID,num_classes in courses.items():
                for i in range(num_classes):
                    courseID_order.append(courseID)
                    buffer_in.append("AddClass " + str(courseID) + "\n")
                    buffer_out.append(("AddClass: " + str(i) + "\n"))
            # Shuffle if needed and write to the test files. Note that shuffling in this function is different
            # since the output changes when shuffling.
            if SHUFFLE_INTERNAL_COMMAND_ORDER == 1:
                buffer_out = []
                buffer_combined = list(zip(buffer_in,courseID_order))
                random.shuffle(buffer_combined)
                buffer_in, courseID_order = zip(*buffer_combined)     
                for courseID in courseID_order:
                    num_classes = classes_seen[courseID][0]
                    num_classes_seen = classes_seen[courseID][1]
                    if num_classes_seen <= num_classes:
                        buffer_out.append(("AddClass: " + str(num_classes_seen) + "\n"))
                    classes_seen[courseID][1] += 1
            for command in buffer_in:
                test_in.write(command)
            for command in buffer_out:
                test_out.write(command)

            # ===========
            # WatchClass
            # ===========
            # Add all of the WatchClass commands and outputs for all classes to buffers.
            buffer_in = []
            buffer_out = []
            for courseID,classID,time_viewed in classes:
                if time_viewed > 0:
                    buffer_in.append("WatchClass " + str(courseID) + " " + str(classID) + " " + str(time_viewed) + "\n")
                    buffer_out.append("WatchClass: SUCCESS\n") 
            # Shuffle if needed and write to the test files.
            writeToTests(buffer_in, test_in, buffer_out, test_out)

            # ===========
            # TimeViewed
            # ===========
            # Add all of the TimeViewed commands and outputs for all classes to buffers.
            buffer_in = [] 
            buffer_out = []
            for courseID,classID,time_viewed in classes:
                buffer_in.append("TimeViewed " + str(courseID) + " " + str(classID) + "\n")
                buffer_out.append("TimeViewed: " + str(time_viewed) + "\n")
            # Shuffle if needed and write to the test files.
            writeToTests(buffer_in, test_in, buffer_out, test_out)

            # ===================
            # GetIthWatchedClass
            # ===================
            # Add all of the GetIthWatchedClass commands and outputs for all positions to buffers.
            buffer_in = [] 
            buffer_out = []
            i = 1
            for courseID,classID,time_viewed in classes:
                if time_viewed != 0:
                    buffer_in.append("GetIthWatchedClass " + str(i) + "\n")
                    buffer_out.append("GetIthWatchedClass: " + str(courseID) + " " + str(classID) + "\n")
                i += 1
            # Shuffle if needed and write to the test files.
            writeToTests(buffer_in, test_in, buffer_out, test_out)

            # =============
            # RemoveCourse
            # =============
            # Add all of the RemoveCourse commands and outputs for all courses to buffers.
            buffer_in = []
            buffer_out = []
            for courseID in courses:
                buffer_in.append("RemoveCourse " + str(courseID) + "\n")
                buffer_out.append("RemoveCourse: SUCCESS\n")
            # Shuffle if needed and write to the test files.
            writeToTests(buffer_in, test_in, buffer_out, test_out)

            # =====
            # Quit
            # =====
            test_in.write("Quit\n")
            test_out.write("quit done.\n")

# ==============================================================
# Helper function to write to test files and shuffle if needed:
# ==============================================================
def writeToTests(buffer_in, test_in, buffer_out, test_out):
    if SHUFFLE_INTERNAL_COMMAND_ORDER == 1:
        buffer_combined = list(zip(buffer_in,buffer_out))
        random.shuffle(buffer_combined)
        buffer_in, buffer_out = zip(*buffer_combined)        
    for command in buffer_in:
        test_in.write(command)
    for command in buffer_out:
        test_out.write(command)

# ===============
# Main function:
# ===============
def main():
    if DIRECTORY != "":
        print("Changing directory to " + DIRECTORY + "\n")
        os.chdir(DIRECTORY)
        print("Changed directory.\n")
    else:
        print("Using current directory: " + os.getcwd() + "\n")
    print("Generating Data...\n")
    courses, classes = generateData()
    print("Done generating Data.\n")
    print("Generating Test Files...\n")
    writeTestFiles(courses,classes)
    print("Done generating Test Files.\n")

# =================
# Run the program:
# =================
if __name__ == "__main__":
    main()