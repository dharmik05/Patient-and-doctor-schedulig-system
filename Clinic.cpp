#include "Clinic.h"
#include <cctype>
#include <string>

const int specialityTypes = 12;

// array of specialitiess
string speciality[specialityTypes] = {"PED", "GEN", "INT", "CAR", "SUR", "OBS", "PSY", "NEU", "ORT", "DET", "OPT", "ENT"};

Clinic::Clinic(){
    transactionFile.open("trasaction.txt");
    if(!transactionFile) {
        cout << "error while opening file." << endl; // error message when file can't be opened
    }
}

Clinic::~Clinic(){
    transactionFile.close(); // close the transaction file
}

void Clinic::printTransaction(const string &message) {
    if(transactionFile.is_open()) {
        transactionFile << message << endl; // write message to the file
    }
    cout << message << endl; // print message to console
}

int Clinic::shortestWaiting(string s) {
    int minWaiting = 100;
    int minWaitingRoom;
    for (int i = 0; i < 100; i++) {
        // check if the room's patient queue is shorter and matches the specialist code
        if(rooms[i].patientQueue.Size() < minWaiting && rooms[i].doctor.specialistCode == s && !rooms[i].doctor.canSeePatients) {
            minWaiting = rooms[i].patientQueue.Size();
            minWaitingRoom = i;
        }
    }
    return minWaitingRoom; // return the room with the shortest waiting list
}

int Clinic::shortestWaiting() {
    int minWaiting = 100;
    int minWaitingRoom = -1;
    for (int i = 0; i < 100; i++) {
        // check if the room's patient queue is shorter
        if(rooms[i].patientQueue.Size() < minWaiting && !rooms[i].doctor.canSeePatients) {
            minWaiting = rooms[i].patientQueue.Size();
            minWaitingRoom = i;
        }
    }
    return minWaitingRoom; // return the room with the shortest waiting list
}

void Clinic::checkInDoctor(string n, int r, string s) {
    for(int i = 0; i < 3; i++) {
        s[i] = toupper(s[i]); // convert speciality code to uppercase
    }

    bool isValid = checkSpeciality(s); // check if the speciality is valid

    if(isValid) {
        if(!rooms[r].doctor.canSeePatients) {
            printTransaction("your requested room is occupied. please try again."); // message if the room is occupied
            return;
        }
        rooms[r].doctor = Doctor(n, r, s); // check in the doctor to the room
        printTransaction("doctor " + n + " checked into room " + to_string(r+1) + "."); // print check-in message
    } else {
        printTransaction("invalid speciality code."); // message for invalid speciality code
        return;
    }
}

void Clinic::checkOutDoctor(string name) {
    bool doctorFound = false;
    for(int i = 0; i < 100; i++) {
        if(rooms[i].doctor.name == name && !rooms[i].doctor.canSeePatients) {
            rooms[i].doctor.canSeePatients = true; // mark the doctor as available
            doctorFound = true;

            while(!rooms[i].patientQueue.IsEmpty()) { // checks if queue is empty if not it reassigns queue to other doctor
                Patient p = rooms[i].patientQueue.Front();
                rooms[i].patientQueue.Dequeue(p);

                int patientReassignedRoom = assignPatientToDoctor(p); // assign patient to another doctor

                if(patientReassignedRoom != -1) {
                    rooms[patientReassignedRoom].patientQueue.Enqueue(p); // reassign the patient to a new room
                    printTransaction("patient " + p.name + " reassigned to dr." + rooms[patientReassignedRoom].doctor.name + " in room " + to_string(patientReassignedRoom + 1) + ".");
                } else {
                    printTransaction("no available room to reassign patient " + p.name + ".");
                }

            }

            printTransaction("doctor " + name + " checked out from room " + to_string(i + 1) + "."); // doctor checked out message
            break;
        }
    }
    if(!doctorFound) {
        printTransaction("doctor " + name + " not found."); // message if doctor is not found
    }
}

void Clinic::checkInPatient(string name, int age, string specialistCode, bool isEmergency) {
    for(int i = 0; i < 3; i++) {
        specialistCode[i] = toupper(specialistCode[i]); // convert speciality code to uppercase
    }

    bool isSpecialityValid = checkSpeciality(specialistCode); // check if the speciality is valid

    if(!isSpecialityValid) {
        printTransaction("invalid speciality code."); // message for invalid speciality code
        return;
    }

    Patient p(name, age, specialistCode, isEmergency); // create patient object

    // first, try to find a pediatrician for patients under 16
    int patientAssignedRoom = assignPatientToDoctor(p);

    if (patientAssignedRoom != -1) {
        if (isEmergency) {
            rooms[patientAssignedRoom].patientQueue.EnqueueToFront(p); // enqueue emergency patient to the front
        } else {
            rooms[patientAssignedRoom].patientQueue.Enqueue(p); // enqueue non-emergency patient
        }
        printTransaction("patient " + name + " assigned to dr. " + rooms[patientAssignedRoom].doctor.name +  " in room " + to_string(patientAssignedRoom + 1) + ".");
    } else {
        printTransaction("sorry, no doctor is available right now."); // message when no doctor is available
    }

}

void Clinic::checkOutPatient(string name) {
    Patient p;
    for(int i  = 0; i < 100; i++) {
        if(!rooms[i].patientQueue.IsEmpty() && rooms[i].patientQueue.Front().name == name) {
            rooms[i].patientQueue.Dequeue(p); // dequeue patient when they check out

            printTransaction("patient " + p.name + " checked out from room " + to_string(i+1) + ".");
            return;
        }
    }
    printTransaction("patient " + name + " cannot checkout or not found."); // message if patient is not found
}

int Clinic::assignPatientToDoctor(Patient p) {
    int patientAssignedRoom = -1;
    if (p.age < 16) { // first checks if age is < 16
        for (int i = 0; i < 100; i++) {
            if (rooms[i].doctor.specialistCode == "PED" && !rooms[i].doctor.canSeePatients) {
                patientAssignedRoom = shortestWaiting("PED"); // assigns to pediatricial
            }
        }
    }

    if (patientAssignedRoom == -1) { // if age > 16 assugn to asked specialist
        for (int i = 0; i < 100; i++) {
            if (rooms[i].doctor.specialistCode == p.specialistCode && !rooms[i].doctor.canSeePatients) {
                patientAssignedRoom = shortestWaiting(p.specialistCode); // assigns to the demanded specialist
            }
        }
    }

    if (patientAssignedRoom == -1) { // no specialist found assign to general
        for (int i = 0; i < 100; i++) {
            if (rooms[i].doctor.specialistCode == "GEN" && !rooms[i].doctor.canSeePatients) {
                patientAssignedRoom = shortestWaiting("GEN"); // assigns to genera; practioneir
            }
        }
    }

    if(patientAssignedRoom == -1) { // no general found assign patient to any doctoe available
        for(int i = 0; i < 100; i++) {
            if (!rooms[i].doctor.canSeePatients) {
                patientAssignedRoom = shortestWaiting(); // assign to room with the least waiting patients
            }
        }
    }

    return patientAssignedRoom;
}

bool Clinic::checkSpeciality(string specialistCode) { // checks speciality if found in array defined
    bool isSpecialityValid = false;

    for(int i = 0; i < specialityTypes; i++) {
        if(speciality[i] == specialistCode) {
            isSpecialityValid = true;
            break;
        }
    }

    return isSpecialityValid; // return whether the speciality code is valid
}
