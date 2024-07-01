# Library-management-system
The Library Management System is designed to streamline and manage the day-to-day operations of a university library. It facilitates the management of student and librarian accounts, book inventories, borrowing and returning processes, and penalty tracking. 

LOGIN DETAILS FOR ALL STUDENTS 
Username : (studentEmail)
Password: unmc_lms

LOGIN DETAILS FOR ALL LIBRARIANS 
Username : (librarianEmail)
Password: unmc_libr

TEXT FILES:
1)	studentList.txt
-	includes the details of the students in the university in the following format:
-	“studentFirstName,studentLastName, studentNumber, studentEmail”
-	
2)	bookList.txt
-	includes the details of the books in the library in the following format:
“bookNumber,ISBN,author,title,category,yearPublished,totalCopies,copiesAvailable”.
3)	librarianList.txt
-includes the details of the librarians working in the university in the following format:
“librarianNumber,librarianFirstName,librarianLastName,librarianEmail”.
4)	penaltyList.txt
-includes the details of the penalties displayed of the students in the following format:
“studentNumber,penAmount”
5)	bnrList.txt
–includes the details of the borrowed books of the students in the following format:
“student_id, book_isbn, borrowDate”
6)	returns.txt
-includes the details of the returned books of the students in the following format:
“student_id, book_isbn, borrowDate,returnDate”.
7)	monthlyReport.txt
-includes the details of the monthly report of May 2023 in the following format:
“number of the books borrowed, number of the books returned, number of books to be returned, total fines collected”

