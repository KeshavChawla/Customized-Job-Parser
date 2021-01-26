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

#include <algorithm>
#include <string>
#include <vector>
#include <map>
using namespace std;

struct skillsPackage {
    vector<string>* skills_list;
    map<string, string>* skills_to_desc_map;
};

// Modifies the word but converts to lowercase
void convertToLowerCase(string& text_to_convert) {
    int strLen = text_to_convert.length();
    for (int i = 0; i <= strLen; i++) {
        text_to_convert[i] = tolower(text_to_convert[i]);
    }
}

// Removes the special characters from the word_to_check
// Modifies the word_to_check
void word_normalizer(string& word_to_check) {
    vector<char> special_chars_to_remove = {
        ',', '\"', '\\', '\'', ':', '.', '?',
        '!', '(', '-', ')', '[', ']', '*',
        '/', '=', '+'};
    int num_of_special_chars = special_chars_to_remove.size();
    for (int i = 0; i <= num_of_special_chars; i++) {
        word_to_check.erase(remove(word_to_check.begin(), word_to_check.end(), special_chars_to_remove[i]), word_to_check.end());
    }
}

void sortPairVector(vector<pair<string, int>>& vec) {
    sort(vec.begin(), vec.end(),
         [](const pair<string, int>& a, const pair<string, int>& b) {
             return (a.second > b.second);
         });
}

// Used to output file as a new CSV
// The last parm newLine determines
// if we end the current job row
template <typename T>
void outputToFileCSV(ofstream& output_file, T output_value, bool newLine = false) {
    output_file << output_value;
    if (newLine)
        output_file << endl;
    else
        output_file << ",";
}
