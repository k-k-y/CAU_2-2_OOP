// Student Information Management Program
#include <iostream>
#include <algorithm>
#include <String>
#include <vector>
#include <fstream>
#include <sstream>
#include "SIMP_header.h"

using namespace std;

int main(void)
{
    ifstream ifs;
    ofstream ofs;
    string line;
    vector<s_infor> infors;
    int sorting_option = 1; // default is sort by name

    // open text file
    ifs.open("file1.txt", ios::in);
    if (!ifs)
    {
        cerr << "file open Error!\n";
        exit(1);
    }

    while (true) // read text file and store in 'infors' vector
    {
        getline(ifs, line);
        if (line.compare("") == 0) break;

        vector<string> words;
        stringstream sstream(line);
        string word;

        while (getline(sstream, word, '/'))
            words.push_back(word);

        s_infor t;
        t.set_name(words[0]);
        t.set_student_id(words[1]);
        t.set_birth_year(words[2]);
        t.set_department(words[3]);
        t.set_telephone(words[4]);
        infors.push_back(t);
    }

    while (true) // while input != 4(exit), loop 
    {
        int op = main_menu_screen();
        if (op == 1) // insertion information
        {
            Insertion(infors);
        }
        else if (op == 2) // search
        {
            Search(infors, sorting_option);
        }
        else if (op == 3) // set sort op
        {
            sorting_option = set_sorting_option();
        }
        else if (op == 4) // exit
        {
            ofs.open("file1.txt", ios::out | ios::ate);
            if (!ofs)
            {
                cerr << "file open Error!\n";
                exit(1);
            }

            for (int i = 0; i < infors.size(); i++)
            {
                ofs << infors[i].get_name() << "/";
                ofs << infors[i].get_student_id() << "/";
                ofs << infors[i].get_department() << "/";
                ofs << infors[i].get_birth_year() << "/";
                ofs << infors[i].get_telephone() << "\n";
            }
            cout << "Thank you!!\n";
            ofs.close();
            ifs.close();
            return 0;
        }
        else cout << "Incorrect Input\n";
    }
}