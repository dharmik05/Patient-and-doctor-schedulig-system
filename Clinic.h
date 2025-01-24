#ifndef CLINIC_H
#define CLINIC_H

#include "queue.h"

#include <iostream>
#include <fstream>
using namespace std;

// patient struct to hold patient information
struct Patient {
    string name;
    int age;
    string specialistCode;
    bool isEmergency;

    Patient() { // default constructor
        name = ""; age = -1; specialistCode = ""; isEmergency = false; 
    }

    Patient(string n, int a, string s, bool isEmergency) { // parameterized constructor
        name = n;
        age = a;
        specialistCode = s;
        isEmergency = isEmergency; 
    }
};

// doctor struct to hold doctor information
struct Doctor {
    string name;
    int roomNum;
    string specialistCode;
    bool canSeePatients;

    Doctor() { // default constructor
        name = ""; roomNum = -1; specialistCode = ""; canSeePatients = true; 
    }

    Doctor(string n, int r, string s) { // parameterized constructor
        name = n;
        roomNum = r;
        specialistCode = s; 
    }
};

// room struct to hold doctor and patient queue for each room
struct Room {
    Doctor doctor;
    Queue<Patient> patientQueue; // queue to manage patients
};

class Clinic {
    private:
        Room rooms[100]; // array of rooms
        ofstream transactionFile; // fstream for transcation file

        void printTransaction(const string &message); // prints transaction

    public:
        Clinic(); // constructor
        ~Clinic(); // destructor
        int shortestWaiting(string s); // find the shortest waiting in speciality s
        int shortestWaiting(); // find shortest waiting in all rooms available
        void checkInDoctor(string n, int r, string s); // checks in doctor
        void checkOutDoctor(string name); // checks cout doctor
        void checkInPatient(string name, int age, string specialistCode, bool isEmergency); //checks in patient
        void checkOutPatient(string name); //checks out patient
        int assignPatientToDoctor(Patient p); // assign patient to doctor with given condition
        bool checkSpeciality(string); // check if speciality is valid
};

#endif