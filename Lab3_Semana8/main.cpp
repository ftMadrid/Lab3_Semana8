#include <QCoreApplication>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <limits>

using namespace std;

#define FILE_NAME "../../employees.csv"

struct Employee {
    int id;
    string name;
    float salary;
    bool fired;
};

vector<Employee> readEmployees(string path) {

    vector<Employee> list;
    ifstream file(path);

    if (!file.is_open()) {
        return list;
    }

    string line;
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string segment;
        Employee emp;

        if (getline(ss, segment, ',')) emp.id = stoi(segment);

        if (getline(ss, segment, ',')) emp.name = segment;

        if (getline(ss, segment, ',')) emp.salary = stof(segment);

        if (getline(ss, segment, ',')) emp.fired = (stoi(segment) == 1);

        list.push_back(emp);
    }

    file.close();
    return list;
}

void saveEmployees(string path, vector<Employee>& employees) {
    ofstream file(path);

    if (file.is_open()) {
        file << "id,name,salary,fired" << endl;

        for (auto& emp : employees) {
            file << emp.id << ","
                    << emp.name << ","
                    << emp.salary << ","
                    << (emp.fired ? "1" : "0") << endl;
        }
        file.close();
        cout << "\n| Changes saved in the CSV!" << endl;
    } else {
        cerr << "\n| The file cant be written!" << endl;
    }
}

int getNextId(vector<Employee>& employees) {
    int maxId = 0;
    for (auto& emp : employees) {
        if (emp.id > maxId) {
            maxId = emp.id;
        }
    }
    return maxId + 1;
}

int searchEmployeeById(vector<Employee>& employees, int id) {
    for (int i = 0; i < employees.size(); ++i) {
        if (employees[i].id == id) {
            return i;
        }
    }
    return -1;
}

void addEmployee(vector<Employee>& employees) {
    Employee newemp;
    newemp.id = getNextId(employees);
    newemp.fired = false;

    cin.ignore();
    cout << "| Set the name: ";
    getline(cin, newemp.name);

    do {
        cout << "\n| Set the salary: ";
        cin >> newemp.salary;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n| Please enter a valid number!" << endl;
            newemp.salary = -1;
        }else if (newemp.salary < 0) {
            cout << "\n| The salary must be positive!" << endl;
        }

    } while (newemp.salary < 0);

    employees.push_back(newemp);
    cout << "\n| Employee added with ID: " << newemp.id << endl;
}

void editEmployee(vector<Employee>& employees) {
    int id;
    cout << "| Write the employee ID: ";
    cin >> id;

    int index = searchEmployeeById(employees, id);

    if (index != -1) {
        if (employees[index].fired) {
            cout << "\n| You cannot edit an employeed who's been fired!" << endl;
            return;
        }

        cout << "\n| Editing: " << employees[index].name << endl;

        cin.ignore();
        cout << "\n| Set the new name: ";
        getline(cin, employees[index].name);

        float newSalary;
        do {
            cout << "\n| Set the new salary: ";
            cin >> newSalary;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "\n| Please enter a valid number!" << endl;
                newSalary = -1;
            }
            else if (newSalary < 0) {
                cout << "\n| The salary must be positive!" << endl;
            }
        } while (newSalary < 0);

        employees[index].salary = newSalary;
        cout << "\n| The employee with ID ["<< id <<"] was successfully edited!" << endl;

    } else {
        cout << "\n| There is no an employee with that ID!" << endl;
    }
}

void fireEmployee(vector<Employee>& employees) {
    int id;
    cout << "| Write the employee ID: ";
    cin >> id;

    int index = searchEmployeeById(employees, id);

    if (index != -1) {
        if (employees[index].fired) {
            cout << "\n| The employee is already fired!" << endl;
        } else {
            employees[index].fired = true;
            cout << "\n| Employee with ID [" << id << "] has been fired!" << endl;
        }
    } else {
        cout << "\n| There is no an employee with that ID!" << endl;
    }
}

void printEmployees(vector<Employee>& employees) {
    cout << "=================================================" << endl;
    cout << left << setw(5) << "ID"
         << left << setw(25) << "NAME"
         << left << setw(10) << "SALARY" << endl;
    cout << "=================================================" << endl;

    bool actives = false;
    for (auto& emp : employees) {
        if (!emp.fired) {
            cout << left << setw(5) << emp.id
                 << left << setw(25) << emp.name
                 << left << "$" << setw(10) << fixed << setprecision(2) << emp.salary << endl;
            actives = true;
        }
    }

    if (!actives) {
        cout << "\n| There is no active employees to show!" << endl;
    }
    cout << "=================================================" << endl;
}

int main(int argc, char *argv[]) {

    QCoreApplication a(argc, argv);

    vector<Employee> employees = readEmployees(FILE_NAME);
    int option;

    do {
        cout << "\n==== MANAGE EMPLOYEES ==== \n" << endl;
        cout << "1. Add employee" << endl;
        cout << "2. Edit employee" << endl;
        cout << "3. Fire employee" << endl;
        cout << "4. Show active employees" << endl;
        cout << "5. Exit" << endl;
        cout << "\n| Select an option: ";

        cin >> option;

        switch (option) {
        case 1:
            cout << "\n==== ADD AN EMPLOYEE ==== \n" << endl;
            addEmployee(employees);
            saveEmployees(FILE_NAME, employees);
            break;
        case 2:
            cout << "\n==== EDIT AN EMPLOYEE ==== \n" << endl;
            editEmployee(employees);
            saveEmployees(FILE_NAME, employees);
            break;
        case 3:
            cout << "\n==== FIRE AN EMPLOYEE ==== \n" << endl;
            fireEmployee(employees);
            saveEmployees(FILE_NAME, employees);
            break;
        case 4:
            cout << "\n==== EMPLOYEES LIST ==== \n" << endl;
            printEmployees(employees);
            break;
        case 5:
            cout << "\n| Have a great day!" << endl;
            break;
        default:
            cout << "\n| This option doesnt exists!" << endl;
            break;
        }
    } while (option != 5);

    return a.exec();
}
