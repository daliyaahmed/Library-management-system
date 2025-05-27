///////////////////////////////////////LIBRARY MANAGEMENT SYSTEM///////////////////////
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


//creating preprocessor directives constants to remain organized
#define MAX_STUDENTS 10
#define MAX_BOOKS 10
#define MAX_LIBRARIANS 5
#define MAX_BNR_ENTRIES 50
#define RETURN_PERIOD 14
#define MAX_DAYS_IN_MONTH 30




//defining all structures
typedef struct studentData{
    char studentFirstName[20];
    char studentLastName[20];
    int studentNumber;
    char studentEmail[30];

} studentData;

typedef struct bookData{
    int bookNumber;
    int  ISBN;
    char author[30];
    char title[40];
    char category[30];
    int yearPublished;
    int totalCopies;
    int copiesAvailable;
}bookData;

typedef struct librarianData {
    int librarianNumber;
    char librarianFirstName[20];
    char librarianLastName[20];
    char librarianEmail[15];
}librarianData;



typedef struct borrowNreturnData{
    char borrowDate;
    int returnDate;
}borrowNreturnData;

typedef struct penaltyData {
    int studentNumber;
    double penAmount;
}penaltyData;




//function for displaying all books in the book array
void displayAllBooks(struct bookData *book) {


//using bubble sort to list the books alphabetically

    for (int i = 0; i < 10 - 1; i++) {
        for (int j = 0; j < 10 - i - 1; j++) {
            if (strcmp(book[j].title, book[j + 1].title) > 0) {
                // Swap books
                struct bookData temp = book[j];
                book[j] = book[j + 1];
                book[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < MAX_BOOKS; i++) {


        printf("%-25s| ", book[i].title);
        printf("%-20s| ", book[i].author);
        printf("%-13d|", book[i].ISBN);
        printf("%-15d|", book[i].yearPublished);
        printf("%-17d|", book[i].copiesAvailable);
        printf("%-13d|\n", book[i].totalCopies);
    }
}

//function to display ONE book and it's details
void displayBook(struct bookData *book, int index) {
    //Display list in tabular form
    printf("%-25s| ", book[index].title);
    printf("%-20s| ", book[index].author);
    printf("%-13d|", book[index].ISBN);
    printf("%-15d|", book[index].yearPublished);
    printf("%-17d|", book[index].copiesAvailable);
    printf("%-13d|\n", book[index].totalCopies);
    // end of list in tabular form
}

//function to display a book when user inputs Book ISBN
void displaySearchBook(struct bookData *book, int index) {
    printf("\nBook Found:\n");
    printf("Title           : %s\n", book[index].title);
    printf("Author          : %s\n", book[index].author);
    printf("ISBN            : %d\n", book[index].ISBN);
    printf("Year Published  : %d\n", book[index].yearPublished);
    printf("Copies Available: %d\n", book[index].copiesAvailable);
    printf("Total Copies    : %d\n", book[index].totalCopies);
}

//function that calculates penalty according to borrow date and return date given
double calculatePenalty(int borrowDate, int returnDate) {
    int expectedReturnDate = borrowDate + RETURN_PERIOD;
    if (returnDate>expectedReturnDate){
        int daysLate = returnDate - expectedReturnDate;
        /*if(daysLate > 24){
            returnDate = returnDate -69;

        }*/
        double penalty = daysLate * 1.00;
        return penalty;
    }else{
        return 0.00;

    }

}


//function to borrow book
//uses the student id number, ISBN, and borrow date
//takes in the borrow date and slices it into three pieces
//day, month, and year
//then, fact checks if it is valid or else it inputs in into the bnrList file
//checks if the book is available
void borrow_book(struct bookData *book) {
    char student_id[20];
    char book_isbn[20];
    struct tm borrow_date = {0};
    int day, month, year;
    int i;

    printf("Enter Student ID: ");
    scanf("%s", student_id);

    printf("Enter Book ISBN: ");
    scanf("%s", book_isbn);

    // Find the book in the book array
    for (i = 0; i < MAX_BOOKS; i++) {
        if (book[i].ISBN == atoi(book_isbn)) {
            break;
        }
    }

    if (i == MAX_BOOKS) {
        printf("Book not found.\n");
        return;
    }

    if (book[i].copiesAvailable == 0) {
        printf("No copies of this book are available.\n");
        return;
    }

    // Decrease the number of available copies
    book[i].copiesAvailable--;

    printf("Enter Borrow Date (dd-mm-yyyy): ");
    scanf("%d-%d-%d", &day, &month, &year);

    borrow_date.tm_mday = day;
    borrow_date.tm_mon = month - 1; // In struct tm, month is 0-11
    borrow_date.tm_year = year - 1900; // Years since 1900

    if (mktime(&borrow_date) == -1) {
        printf("Invalid date.\n");
        return;
    }

    FILE *file = fopen("bnrList.txt", "a+");
    if (file == NULL) {
        printf("Could not open file.\n");
        return;
    }

    fprintf(file, "Student ID: %s, Book ISBN: %s, Borrow Date: %02d-%02d-%04d\n",
            student_id, book_isbn, day, month, year);
    fclose(file);

    printf("Book borrowed successfully.\n");
    displayBook(book, i); // Display the updated book information
}

//function to return Book
//takes  the student id number, ISBN, and return date
//checks if book isbn is correct
//checks if the book was borrowed first
//then, if borrowed, return date will be checked
//once return date is validated, all the information will be printed into returns.txt file
//it will be erased from the bnrList.txt file
//penalty is calculated using the calculatePenalty function
//then the penaltyList.txt is updated


void return_book(struct bookData *book, struct penaltyData *penaltyData1) {
    char student_id[20];
    char book_isbn[20];
    struct tm return_date = {0};
    int day, month, year;
    char line[100];
    int i;

    printf("Enter Student ID: ");
    scanf("%s", student_id);

    printf("Enter Book ISBN: ");
    scanf("%s", book_isbn);

    // Find the book in the book array
    for (i = 0; i < MAX_BOOKS; i++) {
        if (book[i].ISBN == atoi(book_isbn)) {
            break;
        }
    }

    if (i == MAX_BOOKS) {
        printf("Book not found.\n");
        return;
    }

    printf("Enter Return Date (dd-mm-yyyy): ");
    scanf("%d-%d-%d", &day, &month, &year);

    return_date.tm_mday = day;
    return_date.tm_mon = month - 1; // In struct tm, month is 0-11
    return_date.tm_year = year - 1900; // Years since 1900

    if (mktime(&return_date) == -1) {
        printf("Invalid date.\n");
        return;
    }

    FILE *file = fopen("bnrList.txt", "r");
    if (file == NULL) {
        printf("Could not open file.\n");
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, student_id) && strstr(line, book_isbn)) {
            FILE *returns = fopen("returns.txt", "a+");
            if (returns == NULL) {
                printf("Could not open returns file.\n");
                return;
            }

            char borrow_date[11]; // to hold the borrow date in the format dd-mm-yyyy
            sscanf(strstr(line, "Borrow Date: ") + 13, "%10s", borrow_date); // extract only the borrow date

            // Calculate the penalty
            int borrowDate = atoi(borrow_date);
            int returnDate = day + month * 100 + year * 10000; // Convert the return date to the same format as the borrow date
            double penalty = calculatePenalty(borrowDate, returnDate);

            // Update the penalty in the penaltyData1 array
            for (int j = 0; j < MAX_STUDENTS; j++) {
                if (penaltyData1[j].studentNumber == atoi(student_id)) {
                    penaltyData1[j].penAmount += penalty;
                    break;
                }
            }

            fprintf(returns, "Student ID: %s, Book ISBN: %s, Borrow Date: %s, Return Date: %02d-%02d-%04d\n",
                    student_id, book_isbn, borrow_date, day, month, year);
            fclose(returns);

            // Open the penaltyList.txt file for writing
            FILE *penaltyFile = fopen("penaltyList.txt", "w");
            if (penaltyFile == NULL) {
                printf("Could not open penaltyList.txt file.\n");
                return;
            }

            // Write the updated penalties to the file
            for (int j = 0; j < MAX_STUDENTS; j++) {
                fprintf(penaltyFile, "%d: %.2lf\n", penaltyData1[j].studentNumber, penaltyData1[j].penAmount);
            }

            fclose(penaltyFile);

            // Increase the number of available copies
            book[i].copiesAvailable++;

            printf("Book returned successfully.\n");
            displayBook(book, i); // Display the updated book information
            return;
        }
    }

    printf("No matching borrow record found.\n");
}



//function to search book with isbn
//is also linked with displaySearchBook
//it takes in ISBN to be validated
void searchBook(struct bookData *book, int bookISBN) {
    int isISBNvalid = 0;
    for (int i = 0; i < MAX_BOOKS; i++) {
        if (book[i].ISBN == bookISBN) {

            displaySearchBook(book, i);
            isISBNvalid = 1;
            break;

        }
    }

    if(isISBNvalid == 0) {
        printf("\nThe ISBN number is invalid.");

    }
}


//function to display penalties from the penaltyData1 array
//takes student number to be validated
void displayPenalties(struct penaltyData *penaltyData1, int studentNumber) {
    int isStudentNumberPen = 0;
    for (int i = 0; i < MAX_STUDENTS; i++) {
        if (penaltyData1[i].studentNumber == studentNumber) {

            printf("\nPenalty Amount for %d: $%.2lf\n", studentNumber, penaltyData1[i].penAmount);
            isStudentNumberPen = 1;
            break;
        }


    }
    if(isStudentNumberPen == 0){
        printf("\nThe student number is invalid.");
    }

}


//function to display Monthly report
void displayMonthlyReport(struct bookData *book, struct borrowNreturnData *bnr, struct penaltyData *penaltyData1) {

    int booksBorrowed = 0;
    int booksReturned = 0;
    int booksToBeReturned = 0;
    double totalFines = 0.0;


    for(int i =0; i< MAX_BNR_ENTRIES; i++){
        if(bnr[i].returnDate > 0){
            booksBorrowed++;

            if(bnr[i].returnDate > bnr[i].borrowDate + MAX_DAYS_IN_MONTH){
                booksToBeReturned++;
            }
            booksReturned++;

        }
    }

    for(int j =0; j < MAX_STUDENTS; j++){
         totalFines += penaltyData1[j].penAmount;

    }


    printf("\nMonthly Report (November 2023):\n");
    printf("Number of Books Borrowed: %d\n", booksBorrowed);
    printf("Number of Books Returned: %d\n", booksReturned);
    printf("Number of Books to be Returned: %d\n", booksToBeReturned);
    printf("Total Fines Collected: RM%.2lf\n", totalFines);




    FILE *monthlyReportPtr = fopen("monthlyReport.txt","w");
    if(monthlyReportPtr == NULL){
        puts("Error opening monthlyReport.txt for writing ");

    }

    fprintf(monthlyReportPtr, "Monthly Report (November 2023):\n");
    //Assuming that all months have only 30 days



    fprintf(monthlyReportPtr, "Number of Books Borrowed: %d\n", booksBorrowed);
    fprintf(monthlyReportPtr, "Number of Books Returned: %d\n", booksReturned);
    fprintf(monthlyReportPtr,"Number of Books to be Returned: %d\n", booksToBeReturned);
    fprintf(monthlyReportPtr,"Total Fines Collected: RM%.2lf\n", totalFines);

    fclose(monthlyReportPtr);
    printf("Monthly Report generated and saved to monthlyReport.txt\n");


}





int main() {

    char username[30];
    char password[20];
    char cmpPassword[20] = "unmc_lms";
    char libPassword[20] = "unmc_libr";
    int libChoice;
    char studChoice2;
    int bookISBN;
    int studnum;




    // Student Data array
    struct studentData students[MAX_STUDENTS];

    students[0] = (struct studentData) {"Hoorya", "Masood", 15780, "hcyhm"};
    students[1] = (struct studentData) {"Daliya", "Ahmed", 24691, "hfyda"};
    students[2] = (struct studentData) {"Norrth", "Jenner", 34257, "hmumf"};
    students[3] = (struct studentData) {"Melisa", "Keren", 54215, "hyfmk"};
    students[4] = (struct studentData) {"Sannya", "Gupta", 91456, "hcysj"};
    students[5] = (struct studentData) {"Mareha ", "Ahmed", 25436, "hsymc"};
    students[6] = (struct studentData) {"Zaimah", "Pandit", 54389, "hyazp"};
    students[7] = (struct studentData) {"Sarah", "Wafai", 90876, "hsusf"};
    students[8] = (struct studentData) {"Aireen", "Sofea", 76387, "hwyad"};
    students[9] = (struct studentData) {"Sharif", "Keren", 56473, "hdysk"};

    // Book Data array
    struct bookData book[MAX_BOOKS];
    book[0] = (struct bookData) {1, 125674898, "Harper Lee", "To Kill a Mockingbird", "Coming of Age", 1960, 25,
                                 12};
    book[1] = (struct bookData) {2, 123678436, "F. Scott Fitzgerald", "The Great Gatsby", "Tragedy", 1925, 20, 10};
    book[2] = (struct bookData) {3, 154673987, "Charlotte Bronte", "Jane Eyre", "Gothic fiction", 1847, 30, 10};
    book[3] = (struct bookData) {4, 157441857, "Jane Bronte", "Cheryl Eyre", "Romantic Classic", 1847, 30, 10};
    book[4] = (struct bookData) {5, 123460789, "Jane Austen", "Pride and Prejudice", "Classic Regency", 1813, 10,
                                 2};
    book[5] = (struct bookData) {6, 123426789, "Charles Dickens", "Great Expectations", "Bildungsroman", 1861, 20,
                                 10};
    book[6] = (struct bookData) {7, 190876789, "Louisa May Alcott", "Little Women", "Coming of Age", 1868, 10, 5};
    book[7] = (struct bookData) {8, 126748389, "Charles Dickens", "Great Expectations", "Bildungsroman", 1861, 20,
                                 10};
    book[8] = (struct bookData) {9, 123478499, "John Ronald", "The lord of Rings", "Fantasy Fiction", 1954, 30, 23};
    book[9] = (struct bookData) {10, 125426789, "Chinua Achebe", "Things Fall Apart", "Historical Fiction", 1958, 20,
                                 19};

    // Librarian Data array
    struct librarianData librarian[MAX_LIBRARIANS];
    librarian[0] = (struct librarianData) {1, "Jemima", "Khan", "lyiemy"};
    librarian[1] = (struct librarianData) {2, "Luna", "Jahan", "lmynrn"};
    librarian[2] = (struct librarianData) {3, "Meimona", "Ahmed", "lbumew"};
    librarian[3] = (struct librarianData) {4, "Noor", "E Jahan", "lymwer"};
    librarian[4] = (struct librarianData) {5, "Emma", "Johnson", "lrywme"};


    // Penalty Data array
    struct penaltyData penaltyData1[MAX_STUDENTS] = {
            penaltyData1[0] = (struct penaltyData) {15780, 0.00},
            penaltyData1[1] = (struct penaltyData) {24691, 0.00},
            penaltyData1[2] = (struct penaltyData) {34257, 5.00},
            penaltyData1[3] = (struct penaltyData) {54215, 0.00},
            penaltyData1[4] = (struct penaltyData) {91456, 0.00},
            penaltyData1[5] = (struct penaltyData) {25436, 2.00},
            penaltyData1[6] = (struct penaltyData) {54389, 0.00},
            penaltyData1[7] = (struct penaltyData) {90876, 0.00},
            penaltyData1[8] = (struct penaltyData) {76387, 7.00},
            penaltyData1[9] = (struct penaltyData) {56473, 0.00}
    };

    // bnr data array
    struct borrowNreturnData bnr[MAX_BNR_ENTRIES];


    //Opening student list and displaying the student list on the studentList.txt
    FILE *studPtr = fopen("studentList.txt", "w");

    if (studPtr == NULL) {
        perror("Error opening studentList.txt");
        return EXIT_FAILURE;
    } else {


        fprintf(studPtr, "%-15s%-20s%s\n", "|||||||||||Student Name|||||||", "||Student Number||||",
                "||||||||Student E-mail||||||||");
        for (int i = 0; i < 10; i++) {
            fprintf(studPtr, "%-15s%-21s%-27d%-25s\n", students[i].studentFirstName, students[i].studentLastName,
                    students[i].studentNumber, students[i].studentEmail);
        }

        fclose(studPtr);


    }

    //opening book list and displaying the book list on the bookList.txt
    FILE *bookPtr = NULL;


    if ((bookPtr = fopen("bookList.txt", "w")) == NULL) {
        puts("File could not be opened.");

    } else {

        fprintf(bookPtr, "%-15s%-15s%-20s%-25s%-25s%-20s%-20s%-25s\n", "||Book #|||||||", "||ISBN|||||||||",
                "||Author||||||||||||", "||Title||||||||||||||||||", "||Category|||||||||||||||",
                "||Year Published||||", "||Total Copies||||||", "||Available Copies||");

        for (int i = 0; i < MAX_BOOKS; i++) {
            fprintf(bookPtr, "%-15d%-15d%-20s%-25s%-25s%-20d%-20d%-25d\n", book[i].bookNumber, book[i].ISBN,
                    book[i].author, book[i].title, book[i].category, book[i].yearPublished, book[i].totalCopies,
                    book[i].copiesAvailable);
        }
        fclose(bookPtr);

    }

    //opening librarian list and displaying the librarian list on the librarianList.txt
    FILE *libPtr = NULL;


    if ((libPtr = fopen("librarianList.txt", "w")) == NULL) {
        puts("File could not be opened.");

    } else {
        fprintf(libPtr, "%-15s%-20s%-20s%-30s\n", "||Librarian Number||", "||Librarian First Name||  ",
                "||Librarian Last Name||   ", "||Librarian E-mail||");
        for (int i = 0; i < MAX_LIBRARIANS; i++) {
            fprintf(libPtr, "%-25d%-30s%-25s%-21s\n", librarian[i].librarianNumber, librarian[i].librarianFirstName,
                    librarian[i].librarianLastName, librarian[i].librarianEmail);
        }
        fclose(libPtr);
    }


    //opening penalty list and displaying the penalty list on the penaltyList.txt
    FILE *penPtr = NULL;


        if ((penPtr = fopen("penaltyList.txt", "w")) == NULL) {
            puts("Error opening penaltyList.txt");

        } else {
            fprintf(penPtr, "%-15s%s\n", "||Student Number|||||||", "||Penalty Amount|||||||||");
            for (int i = 0; i < MAX_STUDENTS; i++) {
                fprintf(penPtr, "%-30d%f\n", penaltyData1[i].studentNumber, penaltyData1[i].penAmount);
            }
            fclose(penPtr);
        }

    //opening and initializing borrow list
    FILE *bnrPtr = NULL;

    if ((bnrPtr = fopen("bnrList.txt", "w")) == NULL) {
        puts("Error opening bnrList.txt");
    } else {

        fclose(bnrPtr);
    }
        //displaying main screen

        printf("\n*****************************************************************************************************");
        printf("\n*****************************************************************************************************");
        printf("\n************************************Library management system ***************************************");
        printf("\n****************************************project in C*************************************************");
        printf("\n*****************************************************************************************************");
        printf("\n*****************************************************************************************************");
        printf("\n\n");
        //displaying login screen
        printf("#########################################Login Details:##############################################");
        printf("\n################################Username:");

        scanf("%s", &username);

        printf("\n################################Password:");
        scanf("%s", &password);

        //validates student username
        int isStuUsernameValid = 0;
        for (int i = 0; i < MAX_STUDENTS; i++) {
            if (strcmp(username, students[i].studentEmail) == 0) {
                isStuUsernameValid = 1;  // Set flag to indicate a valid username
                break;  // Exit the loop since a match is found
            }
        }
        //validates library username
        int isLibUsernameValid = 0;
        for (int i = 0; i < MAX_LIBRARIANS; i++) {
            if (strcmp(username, librarian[i].librarianEmail) == 0) {
                isLibUsernameValid = 1;
                break;

            }
        }

        int isStuPasswordValid = 0;
        int isLibPasswordValid = 0;
        //validates student password
        //All students have a universal password: "unmc_lms"
        for (int i = 0; i < MAX_STUDENTS; i++) {
            if (strcmp(cmpPassword, password) == 0) {
                isStuPasswordValid = 1;  // Set flag to indicate a valid username
                break;  // Exit the loop since a match is found
            }
        }
        //validates librarian password
        //All librarians have a universal password: "unmc_libr"
        for (int i = 0; i < MAX_LIBRARIANS; i++) {
            if (strcmp(libPassword, password) == 0) {
                isLibPasswordValid = 1;
                break;

            }
        }

        do {
            if (isStuUsernameValid) {
                if (isStuPasswordValid) {
                    //when student username and password is validated, it will display student Menu
                    printf("\n____________________________________________________________________________________________");
                    printf("\n***************************************Student's Menu***************************************");
                    printf("\n____________________________________________________________________________________________");
                    printf("\n\n1.Search Books\n2.View Book list\n3.View Penalties\n4.Exit\n");
                    printf("\n\n\nEnter Choice:");
                    //asks student user to input which option they want
                    int stuChoice;
                    scanf("%d", &stuChoice);


                    switch (stuChoice) {
                        case 1:
                            //case 1 is for searching books using ISBN input by student user
                            printf("\nPlease enter ISBN number of the book:");
                            scanf("%d", &bookISBN);
                            searchBook(book, bookISBN);

                            break;


                        case 2:



                            //case 2 is displaying book list in a tabular form
                            printf("\nBook List:\n");
                            printf("______________________________________________________________________________________________________________\n");
                            printf("Title                    | ");
                            printf("Author              | ");
                            printf("ISBN         | ");
                            printf("Year Published| ");
                            printf("Copies Available| ");
                            printf("Total Copies|\n");
                            printf("______________________________________________________________________________________________________________\n");


                            for (int i = 0; i < MAX_BOOKS; i++) {
                                //calls the display book function
                                displayBook(book, i);

                            }
                            printf("______________________________________________________________________________________________________________\n");
                            break;
                        case 3:
                            //case 3 is displaying penalties and student is asked for student number
                            printf("Enter the student number: ");
                            scanf("%d", &studnum);
                            displayPenalties(penaltyData1, studnum);


                            break;
                        case 4:
                            //display student's logging out
                            printf("\n*************************************************************************************");
                            printf("\n*************************************Logging out*************************************");
                            printf("\n*************************************************************************************");
                            return 0;
                        default:
                            printf("\nError: The choice you entered is invalid.");
                            return 0;
                    }


                } else {
                    printf("\nIncorrect Student Password");
                    return 0;

                }
            } else if (isLibUsernameValid) {
                if (isLibPasswordValid) {
                    //displaying Librarian's menu
                    printf("\n____________________________________________________________________________________________");
                    printf("\n*************************************Librarian's Menu***************************************");
                    printf("\n____________________________________________________________________________________________");
                    printf("\n\n1.Search Books\n2.Borrow Books\n3.Return Books\n4.Display list of books\n5.View penalties\n6.View monthly reports\n7.Exit\n");
                    printf("\n\n\nEnter Choice:");
                    scanf("%d", &libChoice);

                    switch (libChoice) {
                        case 1:
                            //
                            printf("\nPlease enter ISBN number of the book: ");
                            scanf("%d", &bookISBN);
                            searchBook(book, bookISBN);

                            break;
                        case 2:

                            borrow_book(book);

                            break;

                        case 3:

                            return_book(book,penaltyData1);

                            break;

                        case 4:
                            //displaying books in a tabular form
                            printf("\nList of All Books (Alphabetical Order):\n");
                            printf("______________________________________________________________________________________________________________\n");
                            printf("Title                    | ");
                            printf("Author              | ");
                            printf("ISBN         | ");
                            printf("Year Published| ");
                            printf("Copies Available| ");
                            printf("Total Copies|\n");
                            printf("______________________________________________________________________________________________________________\n");

                            displayAllBooks(book);

                            break;
                        case 5:
                                //displaying penalties of all students from the penaltyData1 array
                                for (int i = 0; i < MAX_STUDENTS; i++) {
                                    displayPenalties(penaltyData1, penaltyData1[i].studentNumber);
                                }

                            break;
                        case 6:
                            //displaying the Monthly report
                            displayMonthlyReport(book, bnr, penaltyData1);
                            break;
                        case 7:
                            //display Librarian logging out of program
                            printf("\n*************************************************************************************");
                            printf("\n*************************************Logging out*************************************");
                            printf("\n*************************************************************************************");
                            return 0;
                        default:
                            printf("\nLibrarian Menu: Error: The choice you entered is invalid.");
                            return 0;
                    }


                } else {
                    printf("\nIncorrect Librarian Password");
                    return 0;
                }
            } else {
                printf("\nIncorrect Username or Password");
                return 0;

            }

            printf("\nDo you want to continue?(y/n)");
            scanf("%s", &studChoice2);

        } while (studChoice2 == 'y' || studChoice2 == 'Y');
        if (studChoice2 == 'N' || studChoice2 == 'n') {
            //displaying students/librarians logging out
            printf("\n______________________________________________________________________________________________");
            printf("\n_______________________________You have successfully logged out_______________________________");
            printf("\n______________________________________________________________________________________________");

        }

        return 0;
    }

