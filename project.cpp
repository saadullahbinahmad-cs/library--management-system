
//------------------------------LIBRARY MANAGEMENT SYSTEM CODE---------------------------------
#include <iostream>
#include <fstream>  // USED FOR FILE HANDLING
#include <cstring>  //USED TO USE OLD C LANGUAGE TYPE STRING USE

using namespace std;

struct LibraryBook {           //DEAL WITH SIX THINGS OF BOOK SO USED STRUCT TO ORGANIZE DATA OF A BOOK
    int book_serial_num;
    char book_isbn[25];  //FOR FILE HANDLING IT IS BETTER TO USE CHARACTER ARRAY IN PLACE OF STRING, BECAUSE STRING DOESNOT HAVE FIXED SIZE.
    char book_title[50];
    char book_edition[20];
    char book_author[50];
    bool book_is_reserved; 
};

void addNewBook() {  //FUNCTION TO ADD BOOK
    ofstream bookFile("book_information.dat", ios::binary | ios::app);  //OFSTREAM FOR(LINK FROM CODE TO FILE) //BINARY TO STORE DATA IN BYTES FROM //APPEND TO WRITE NEW DATA BELOW OLD DATA
    LibraryBook bookData; 

    cout << " -----------Add New Book ------------"<<endl;
    cout << "Enter the Serial Number of book : "<<endl; 
    cin >> bookData.book_serial_num;
    
    cin.ignore();   //TO IGNORE ENTER WHICH IS STUCKED IN BUFFER
    
    cout << "Enter the book ISBN : "<<endl; 
    cin.getline(bookData.book_isbn, 25);
    
    cout << "Enter the book Title: "<<endl; 
    cin.getline(bookData.book_title, 50);
    
    cout << "Enter the book Edition: "<<endl; 
    cin.getline(bookData.book_edition, 20);
    
    cout << "Enter the Author name : "<<endl; 
    cin.getline(bookData.book_author, 50);
    
    bookData.book_is_reserved = false;
    
    bookFile.write((char*)&bookData, sizeof(bookData));   // MEANS TO STORE DATA PRESENT AT ADRESS OF BOOKDATA , CHAR MEANS AS SINGLE BYTE OR ONE PARTICLE , * IS POINTING ADRESS OF BOOKDATA
    bookFile.close();
    
    cout << "Book is added successfully to the library !"<<endl;
}

void deleteBook() {
    ifstream bookFile("book_information.dat", ios::binary);
    ofstream temporaryBookFile("temporary_book.dat", ios::binary); // TEMP FILE TO STORE FILES WHICH DONT DELETE
    
    int book_id; 
    bool bookFound = false; 
    LibraryBook bookData;
    
    cout << "Enter Serial Number to Delete: "<<endl; 
    cin >> book_id;
    
    while (bookFile.read((char*)&bookData, sizeof(bookData))) {
        if (bookData.book_serial_num != book_id) {
            temporaryBookFile.write((char*)&bookData, sizeof(bookData));
        } else {
            bookFound = true;
        }
    }
    
    bookFile.close(); 
    temporaryBookFile.close();
    
    remove("book_information.dat"); 
    rename("temporary_book.dat", "book_information.dat");
    
    if (bookFound) {
	cout << "Book is deleted successfully from library "<<endl;}
    else 
	{
	cout << "Record not found!"<<endl;}
}

void searchBook() {
    ifstream bookFile("book_information.dat", ios::binary);
    int  Choice;
    char subChoice[50];
    LibraryBook bookData;
    bool bookFound = false;

    cout << "Search the book  by"<<endl;
	cout<<"1. ISBN"<<endl;
	cout<<"2. Title"<<endl;
	cout<<"3. Author"<<endl;
    cin >>Choice;
    
    cin.ignore(); 
    
    cout << "Enter Search Term: "<<endl; 
    cin.getline(subChoice, 50);

    while (bookFile.read((char*)&bookData, sizeof(bookData))) {
        if ((Choice == 1 && strcmp(bookData.book_isbn, subChoice) == 0) ||
            (Choice == 2 && strcmp(bookData.book_title, subChoice) == 0) ||      // USED STRCMP BECAUSE IF WE COMPARE TWO ARRAYS , THERE ADRESS IS COMPARED AS A RESULT WE GET WRONG OUTPUT
            (Choice == 3 && strcmp(bookData.book_author, subChoice) == 0)) {
            
            cout << "Found a book : " << bookData.book_title << " by " << bookData.book_author << " and ISBN : " << bookData.book_isbn<< endl;
            bookFound = true;
        }
    }
    
    if (!bookFound) {
	cout << "No matching book found!"<<endl;
	}
    bookFile.close();
}

void modifyBook() { 
    fstream bookFile("book_information.dat", ios::binary | ios::in | ios::out);   // USED FSTREAM BECAUSE WE HAVE TO WRITE AND READ AT SAME TIME
    int book_id; 
    bool bookFound = false; 
    LibraryBook bookData;
    
    cout << "Enter Serial Number to Modify: "<<endl; 
    cin >> book_id;
    
    while (bookFile.read((char*)&bookData, sizeof(bookData))) {
        if (bookData.book_serial_num == book_id) {
            cout << "Enter  Title of book : "<<endl; 
            
            cin.ignore(); 
            
            cin.getline(bookData.book_title, 50);
            cout << "Enter  Author of book : "<<endl; 
            cin.getline(bookData.book_author, 50);
             cout << "Enter Edition of book : "<<endl; 
            cin.getline(bookData.book_edition, 20);
             cout << "Enter ISBN of book : "<<endl; 
            cin.getline(bookData.book_isbn, 25);
            
            int book_position = -1 * sizeof(bookData);   //  FOR EXAMPLE A BOOK SIZE IS 100 BYTES , IT IS MAKING 100 BYTES TO -100BYTES
            bookFile.seekp(book_position, ios::cur);  // IT IS FINDING POINT WHERE TO WRITE
            bookFile.write((char*)&bookData, sizeof(bookData));
            
            bookFound = true; 
            break;
        }
    }
    
    if (bookFound) {
	cout << "Records are  updated! "<<endl;}
    else
	 {cout << "Not found book in library "<<endl;}
    bookFile.close();
}

void displayAll() {
    ifstream bookFile("book_information.dat", ios::binary);
    LibraryBook bookData;
    
    if (!bookFile) {
        cout << "No Data found "<<endl;
        return;
    }

    cout << "\n======= Dr Tariq Najmi LIBRARY COLLECTION ======="<<endl<<endl;
    int book_count = 1; 
    
    while (bookFile.read((char*)&bookData, sizeof(bookData))) {
        cout << "------------------------------------"<<endl;
        cout << " Book Number  " << book_count<<" Record"<<endl;
        cout << "------------------------------------"<<endl;
        cout << " Book Serial number    : " << bookData.book_serial_num<<endl;
        cout << " Book ISBN             : " << bookData.book_isbn<<endl;
        cout << " Book Title            : " << bookData.book_title<<endl;
        cout << " Book Author           : " << bookData.book_author<<endl;
        cout << " Book Edition          : " << bookData.book_edition<<endl;
        cout << " Book Status           : " << (bookData.book_is_reserved ? "Issued" : "Available")<<endl;
        cout << "------------------------------------\n";
        book_count++; 
    }
    bookFile.close();
}

void issueBook() {

    fstream bookFile("book_information.dat", ios::binary | ios::in | ios::out);

    int book_id;

    bool bookFound = false;

    LibraryBook bookData;

    cout << "Enter Serial Number to Issue: " << endl;

    cin >> book_id;

    while (bookFile.read((char*)&bookData, sizeof(bookData))) {

        if (bookData.book_serial_num == book_id) {

            if (bookData.book_is_reserved) {

                cout << "Book Already Issued!" << endl;
            }

            else {

                bookData.book_is_reserved = true;

                int book_position = -1 * sizeof(bookData);

                bookFile.seekp(book_position, ios::cur);

                bookFile.write((char*)&bookData, sizeof(bookData));

                cout << "Book Issued Successfully!" << endl;
            }

            bookFound = true;

            break;
        }
    }
if (bookFound) {
	cout << "Book issued successfully!"<<endl;}
    else {
	cout << "Not data found "<<endl;}
    bookFile.close();
}
int main() {
    int menu_choice;
    do {
        cout <<"\n------------------ Library Menu -------------------"<<endl;
        cout <<"1.Add New Book to Library "<<endl;
		cout<<"2.Delete Book from library "<<endl;
		cout<<"3.Search Book from library"<<endl;
	    cout<<"4.Modify Record in library "<<endl;
		cout<<"5.Display All data "<<endl;
		cout<<"6.Issue/Reserve any book "<<endl;
		cout<<"7.Exit the program  "<<endl;
		cout<<"Choose on from the menu: "<<endl;
        cin >> menu_choice;
        
        switch (menu_choice) {
            case 1:
		    addNewBook();
			 break;
            case 2: 
			deleteBook();
			 break;
            case 3:
			 searchBook();
			  break;
            case 4: 
			modifyBook();
			 break;
            case 5: 
			displayAll();
			 break;
            case 6: 
			issueBook(); 
			break;
            case 7:
			 cout << "Exited program" << endl;
			 break;
            default:
                cout << "Invalid menu choice" << endl;
                break;
        }
    } while (menu_choice != 7);
    
    return 0;
}