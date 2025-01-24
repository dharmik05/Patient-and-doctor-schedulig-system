#include "Clinic.h"

int main() {
    Clinic clinic;

    char choice, checkInOut;

    while(true) {
        // prompt user to select doctor, patient, or quit
        cout << "Type D for Doctor or P for Patient or Q to Quit the program: ";
        cin >> choice;

        // exit the program if user chooses to quit
        if(choice == 'Q' || choice == 'q') { break; }

        // prompt user to select check-in or check-out
        cout << "Type I for check-in or O for checkout: ";
        cin >> checkInOut;

        if(choice == 'D' || choice == 'd') { // for doctor
            string name, specialityCode;
            int roomNum;

            if(checkInOut == 'I' || checkInOut == 'i') {
                // prompt for doctor details during check-in
                cout << "Enter doctor's name, speciality code, and room number: ";
                cin >> name >> specialityCode >> roomNum;
                clinic.checkInDoctor(name, roomNum-1, specialityCode);
            } else if(checkInOut == 'O' || checkInOut == 'o') {
                // prompt for doctor name during check-out
                cout << "Enter doctor's name: ";
                cin >> name;
                clinic.checkOutDoctor(name);
            }
        } else if(choice == 'P' || choice == 'p') { // for patient
            string name, specialityCode;
            int age;
            bool emergency;

            if(checkInOut == 'I' || checkInOut == 'i') {
                // prompt for patient details during check-in
                cout << "Enter patient's name, age, speciality code, and emergency (1 for yes, 0 for no): ";
                cin >> name >> age >> specialityCode >> emergency;
                clinic.checkInPatient(name, age, specialityCode, emergency);
            } else if(checkInOut == 'O' || checkInOut == 'o') {
                // prompt for patient name during check-out
                cout << "Enter patient's name: ";
                cin >> name;
                clinic.checkOutPatient(name);
            }
        } else {
            // inform user about invalid input
            cout << "Enter a valid input." << endl;
        }
    }

    return 0;
}
