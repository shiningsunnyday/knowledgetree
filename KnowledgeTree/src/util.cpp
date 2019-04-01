#include "util.h"
#include "strlib.h"

Vector<string> readFromSystem(string argString) {
    Vector<string> result = Vector<string>();
    string cmd = "cd /Users/shiningsunnyday/Documents/GitHub/106XFinal/WikiScraper/scrape_venv; ";
    cmd += "source bin/activate; ";
    cmd += ("python /Users/shiningsunnyday/Documents/GitHub/106XFinal/WikiScraper/writeToCSV.py ");
    cmd += argString;
    char buffer[128];
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (fgets(buffer, sizeof buffer, pipe) != NULL)
        {
            int delim = string(buffer).find_last_of(".");
            result.add(string(buffer).substr(0, delim));
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

relationship getRelationship(string topic1, string topic2) {
    Vector<string> result = readFromSystem(stringReplace(topic1, " ", ",") + " " + stringReplace(topic2, " ", ","));
    relation relation;
    if(result.size() > 0) {
        int relationIndex = stoi(result[0]);
        switch(stoi(result[0])) {
        case -1:
            relation = CHILD;
            break;
        case 1:
            relation = PARENT;
            break;
        case 0:
            relation = SIBLING;
            break;
        default:
            relation = UNKNOWN;
            break;
        }
        return relationship { relation, stringToReal(result[1]) };
    } else { return relationship { SIBLING, 0 };}
};
