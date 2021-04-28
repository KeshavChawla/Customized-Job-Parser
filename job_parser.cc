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
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "job_parser_constants.h"
#include "job_parser_helper.cc"
using namespace std;

string parseJobListing(const string& passed_text, const string& key) {
    string firstLine = passed_text;
    int str_length = firstLine.length();
    int keyLength = key.length();
    size_t found = firstLine.find(key);
    if (found != string::npos) {
        int posToEraseTo = int(found);
        firstLine.erase(0, posToEraseTo + keyLength);
    } else
        cerr << "ERROR can't find start of key: " << key << endl;
    str_length = firstLine.length();
    found = firstLine.find(key);
    if (found != string::npos) {
        int posToEraseTo = int(found);
        // We move back one position and remove the comma
        firstLine.erase(posToEraseTo - 1, str_length);
    } else
        cerr << "ERROR can't find end of the key: " << key << endl;
    return firstLine;
}

map<string, int>* countOccurencesMap(const string& description_block) {
    string current_word;
    istringstream stream_of_word(description_block);
    map<string, int>* occurences_map = new map<string, int>;
    while (stream_of_word >> current_word) {
        word_normalizer(current_word);
        (*occurences_map)[current_word]++;
    }
    return occurences_map;
}

void parseJob(ofstream& output_file, ifstream& jobFile, skillsPackage* current_skills) {
    string text_block;
    // This is why every row of the csv file needs to end with a '~'
    // Any other char deliminater could be used as well
    // This is a bit hacky but it's better than trying to rewrite
    // getline with a string delimeter (I've tried) not to mention
    // the differences with performance on large files

    // For now just make sure your CSV files don't contain except at
    // the end of each row on a seperate cell'~'
    while (getline(jobFile, text_block, END_CHAR)) {
        vector<string>* current_skills_list = (current_skills->skills_list);
        map<string, string>* current_skills_list_map = (current_skills->skills_to_desc_map);
        string job_id_block = parseJobListing(text_block, job_id_key);
        string job_title_block = parseJobListing(text_block, job_title_key);
        string company_block = parseJobListing(text_block, company_key);
        string city_block = parseJobListing(text_block, city_key);
        string deadline_block = parseJobListing(text_block, deadline_key);
        string submission_date_block = parseJobListing(text_block, submission_date_key);
        string description_block = parseJobListing(text_block, description_key);
        outputToFileCSV<string>(output_file, job_id_block);
        outputToFileCSV<string>(output_file, job_title_block);
        outputToFileCSV<string>(output_file, company_block);
        outputToFileCSV<string>(output_file, city_block);
        outputToFileCSV<string>(output_file, deadline_block);
        outputToFileCSV<string>(output_file, submission_date_block);
        outputToFileCSV<string>(output_file, description_block);
        convertToLowerCase(description_block);
        map<string, int>* all_words_mapped = countOccurencesMap(description_block);
        vector<pair<string, int>> just_skills_mapped;
        int number_of_skills = current_skills_list->size();

        for (int i = 0; i <= number_of_skills; i++) {
            string current_skill = (*current_skills_list)[i];
            string current_skill_lower_case = current_skill;
            convertToLowerCase(current_skill_lower_case);
            just_skills_mapped.push_back(
                make_pair(current_skill, (*all_words_mapped)[current_skill_lower_case]));
        }
        delete all_words_mapped;
        // Sorts all the skills in descending order of number of occurences
        // in the all_words_mapped
        sortPairVector(just_skills_mapped);
        if (NUMBER_OF_SKILLS_PER_JOB > number_of_skills) {
            cerr << RED << "ERROR: The number of skills to output: " << NUMBER_OF_SKILLS_PER_JOB << " is less than the total number of skills: " << number_of_skills << endl;
        }
        for (int i = 0; i <= NUMBER_OF_SKILLS_PER_JOB; i++) {
            outputToFileCSV<string>(output_file, just_skills_mapped[i].first);
            outputToFileCSV<string>(output_file, (*current_skills_list_map)[just_skills_mapped[i].first]);
        }
        outputToFileCSV<string>(output_file, "EOF", true);
    }
}

skillsPackage* parseSkills(ifstream& skillsFile) {
    string text_block;
    vector<string>* skills_list = new vector<string>;
    map<string, string>* skils_to_desc_map = new map<string, string>;
    while (getline(skillsFile, text_block, END_CHAR)) {
        string cur_skill = parseJobListing(text_block, skills_key);
        string cur_skill_desc = parseJobListing(text_block, skills_desc_key);
        (*skils_to_desc_map)[cur_skill] = cur_skill_desc;
        (skills_list)->emplace_back(cur_skill);
    }
    skillsPackage* current_skills = new skillsPackage;
    current_skills->skills_list = skills_list;
    current_skills->skills_to_desc_map = skils_to_desc_map;
    //(skills_list, skils_to_desc_map);
    return current_skills;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        cout << GREEN << "Usage:" << RESET << endl;
        cout << BOLDMAGENTA << "This program takes in two CSV files as arguments. Please use the provided templates." << endl;
        cout << "The first file, should contain all your job application data." << endl;
        cout << "The second file, should contain your skills and skill descriptions." << endl;
        cout << "The program will output a file and match the keywords from your job descriptions." << endl;
        cout << "All CSV files must have markers in order to read the data properly and must end each row with a '~' in its own cell." << RESET << endl;
        cerr << RED << "Two files are not present! Consult the documentation for more info." << RESET << endl;
        return 0;
    }
    if (argc >= 3) {
        ifstream jobFile(argv[1]);
        ifstream skillsFile(argv[2]);
        if (jobFile.is_open() && skillsFile.is_open()) {
            string output_file_name = "job_parse_output.csv";
            if (argc == 4) {
                string base_file_name = argv[3];
                output_file_name = base_file_name + ".csv";
            }
            ofstream output_file(output_file_name);
            skillsPackage* list_of_all_skills = parseSkills(skillsFile);
            parseJob(output_file, jobFile, list_of_all_skills);
            cout << GREEN << "All file data was parsed and saved to: " << output_file_name << RESET << endl;
            delete list_of_all_skills->skills_list;
            delete list_of_all_skills->skills_to_desc_map;
            delete list_of_all_skills;
        } else
            cerr << RED << "There was a problem opening one of the the two files." << RESET << endl;
    }
}
