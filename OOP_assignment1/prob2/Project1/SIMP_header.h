#include <iostream>
#include <algorithm>
#include <String>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

class s_infor
{
private:
    string name;
    string student_id;
    string birth_year;
    string department;
    string telephone;

public:
    //setter
    void set_name(string _name)
    {
        name = _name;
    }
    void set_student_id(string _student_id)
    {
        student_id = _student_id;
    }
    void set_birth_year(string _birth_year)
    {
        birth_year = _birth_year;
    }
    void set_department(string _department)
    {
        department = _department;
    }
    void set_telephone(string _telephone)
    {
        telephone = _telephone;
    }
    //getter
    string get_name()
    {
        return name;
    }
    string get_student_id()
    {
        return student_id;
    }
    string get_birth_year()
    {
        return birth_year;
    }
    string get_department()
    {
        return department;
    }
    string get_telephone()
    {
        return telephone;
    }
};

bool compare1(s_infor& a, s_infor& b)
{
    return a.get_name() < b.get_name();
}
bool compare2(s_infor& a, s_infor& b)
{
    return a.get_student_id() < b.get_student_id();
}
bool compare3(s_infor& a, s_infor& b)
{
    string a_s;
    string b_s;
    for (int i = 0; i < 4; i++)
    {
        a_s.push_back(a.get_student_id()[i]);
        b_s.push_back(b.get_student_id()[i]);
    }
    return a_s < b_s;
}
bool compare4(s_infor& a, s_infor& b)
{
    return a.get_department() < b.get_department();
}

void sorting_by_option(int op, vector<s_infor>& infors)
{
    if (op == 1) sort(infors.begin(), infors.end(), compare1); // name
    else if (op == 2) sort(infors.begin(), infors.end(), compare2); // student_id
    else if (op == 3) sort(infors.begin(), infors.end(), compare3); // admission year (first 4 of student_id)
    else if (op == 4) sort(infors.begin(), infors.end(), compare4); // department

}
int main_menu_screen()
{
    int op;
    cout << "### MAIN MENU ###\n";
    cout << "1. Insertion\n";
    cout << "2. Search\n";
    cout << "3. Sorting Option\n";
    cout << "4. Exit\n";
    cout << ">>";
    cin >> op;

    return op;
}

bool is_true(int op, string s)
{
    if (op == 1) // student id
    {
        if (s.size() != 10) return false;
        for (int i = 0; i < 10; i++)
            if (s[i] > '9' || s[i] < '0') return false;
    }
    if (op == 2) // birth year
    {
        if (s.size() != 4) return false;
        for (int i = 0; i < 4; i++)
            if (s[i] > '9' || s[i] < '0') return false;
    }
    if (op == 3) // tel
    {
        if (s.size() > 12) return false;
    }
    return true;
}

void Insertion(vector<s_infor>& infors)
{
    string name;
    string student_id;
    string birth_year;
    string department;
    string telephone;

    cin.ignore();

    cout << "Name ?";
    getline(cin, name);

    cout << "Student ID (10 digits) ?";
    getline(cin, student_id);

    cout << "Birth Year (4 digits) ?";
    getline(cin, birth_year);

    cout << "Department?";
    getline(cin, department);

    cout << "Tel ?";
    getline(cin, telephone);

    if (is_true(1, student_id) == true && is_true(2, birth_year) == true && is_true(3, telephone) == true)
    {
        if (name.size() != 0 && student_id.size() != 0)
        {
            s_infor t;
            t.set_name(name);
            t.set_student_id(student_id);
            t.set_birth_year(birth_year);
            t.set_department(department);
            t.set_telephone(telephone);
            infors.push_back(t);
        }
        else
        {
            cout << "Wrong input!\n";
            cout << "Name and Student ID should not be blank.\n";
        }

    }
    else
    {
        cout << "Wrong input!\n";
        cout << "Student ID should be 10 digits and Birth should be 4 digits.\n";
        cout << "And Tel has up to 12 digits.\n";
    }
}

void select_infor(int op, string answer, vector<s_infor> infors, vector<s_infor>& selected_infor)
{
    if (op == 1) // name
    {
        for (int i = 0; i < infors.size(); i++)
            if (infors[i].get_name().compare(answer) == 0)
                selected_infor.push_back(infors[i]);
    }
    else if (op == 2) // student id
    {
        for (int i = 0; i < infors.size(); i++)
            if (infors[i].get_student_id().compare(answer) == 0)
                selected_infor.push_back(infors[i]);
    }
    else if (op == 3) // admission year(first 4 digits of student id)
    {
        for (int i = 0; i < infors.size(); i++)
        {
            string s;
            for (int j = 0; j < 4; j++)
                s.push_back(infors[i].get_student_id()[j]);

            if (s.compare(answer) == 0)
                selected_infor.push_back(infors[i]);
        }
    }
    else if (op == 4) // department name
    {
        for (int i = 0; i < infors.size(); i++)
            if (infors[i].get_department().compare(answer) == 0)
                selected_infor.push_back(infors[i]);
    }
}

void Search(vector<s_infor> infors, int sorting_option)
{
    int op;
    cout << "- Search -\n";
    cout << "1. Search by name\n";
    cout << "2. Search by student ID (10 numbers)\n";
    cout << "3. Search by admission year (4 numbers)\n";
    cout << "4. Search by department name\n";
    cout << "5. List All\n";
    cout << ">>";

    cin >> op;

    string s;
    vector<s_infor> searched_infor;

    cin.ignore();

    if (op == 1)
    {
        cout << "name keyword? ";
        getline(cin, s);
        select_infor(op, s, infors, searched_infor);
    }
    else if (op == 2)
    {
        cout << "student ID keyword? ";
        getline(cin, s);
        select_infor(op, s, infors, searched_infor);
    }
    else if (op == 3)
    {
        cout << "admission year keyword? ";
        getline(cin, s);
        select_infor(op, s, infors, searched_infor);
    }
    else if (op == 4)
    {
        cout << "Department name keyword? ";
        getline(cin, s);
        select_infor(op, s, infors, searched_infor);
    }
    else if (op == 5)
    {
        searched_infor = infors;
    }

    if (searched_infor.size() == 0)
    {
        cout << "There is no student in that option!\n";
        return;
    }

    sorting_by_option(sorting_option, searched_infor); // sort before print

    cout << "1.Name 2.Student ID 3.Dept 4.Birth Year 5.Tel\n";
    for (int i = 0; i < searched_infor.size(); i++)
    {
        cout << searched_infor[i].get_name() << " | ";
        cout << searched_infor[i].get_student_id() << " | ";
        cout << searched_infor[i].get_department() << " | ";
        cout << searched_infor[i].get_birth_year() << " | ";
        cout << searched_infor[i].get_telephone() << "\n";
    }
}

int set_sorting_option()
{
    int op;
    cout << "- Sorting Option\n";
    cout << "1. Sort by Name\n";
    cout << "2. Sort by Student ID\n";
    cout << "3. Sort by Admission Year\n";
    cout << "4. Sort by Department name\n";
    cout << ">>";
    cin >> op;

    return op;
}

