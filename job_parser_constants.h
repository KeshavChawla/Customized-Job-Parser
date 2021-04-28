/*******************************************************
 * Copyright (C) 2021 Keshav Chawla 
 * hello [at] keshavchawla.com
 * 
 * This file is part of Job Parser Project.
 *
 *  Unauthorized copying of this file, via any medium is strictly prohibited
 *  
 * Job Parser Project can not be copied and/or distributed without the express
 * permission of Keshav Chawla.
 *******************************************************/

#include <string>
using namespace std;

#define RESET "\033[0m"
#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define BOLDBLACK "\033[1m\033[30m"
#define BOLDRED "\033[1m\033[31m"
#define BOLDGREEN "\033[1m\033[32m"
#define BOLDYELLOW "\033[1m\033[33m"
#define BOLDBLUE "\033[1m\033[34m"
#define BOLDMAGENTA "\033[1m\033[35m"
#define BOLDCYAN "\033[1m\033[36m"
#define BOLDWHITE "\033[1m\033[37m"

string makeKey(string keywordToKey) {
    string key = "_" + keywordToKey + "_,";
    int keyLength = key.length();
    for (int i = 0; i <= keyLength; i++) {
        key[i] = toupper(key[i]);
    }
    return key;
}


const char END_CHAR = '~';

const string job_id_key = makeKey("Job ID");
const string job_title_key = makeKey("Job Title");
const string company_key = makeKey("Company");
const string city_key = makeKey("City");
const string deadline_key = makeKey("deadline");
const string submission_date_key = makeKey("submission date");
const string description_key = makeKey("Description");
const string skills_key = makeKey("Skills");
const string skills_desc_key = makeKey("skills desc");

const int NUMBER_OF_SKILLS_PER_JOB = 3;
