#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>

//For ease of reading
typedef std::map<std::string, int> courseMap;
typedef std::vector<std::string> commentVector;
typedef std::map<std::string, std::string> outputMap;
typedef std::vector<std::vector<std::string> > reqsVector;
                                           
//Create a namespace
namespace Algo {

    //Forward declarations
    bool parse_reqs(const std::string &fName, reqsVector& majorReqs, commentVector&
            majorComments, reqsVector& hassReqs, commentVector& hassComments);
    void parse_reqs(reqsVector& reqs, const std::string &fName);
    bool specialCompare(const std::string &req, courseMap& classes,
                const std::set<std::string> &unacceptable, bool noRepeat);
    int free_electives(courseMap& classes, const std::string &credits);
    int concentration_compare(const std::string &initConcentration, courseMap& classes_credits);
    void compare_courses(reqsVector& reqs, courseMap& classes,
                std::vector<int> &needed, bool noRepeat, int numCourses);
    void compare_courses(reqsVector& reqs, courseMap& classes, std::vector<int> &needed);
    std::pair<outputMap*, outputMap* > runAlgo(const std::string &req_file, courseMap courses);

}

#endif
