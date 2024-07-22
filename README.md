# Concurrent Directory-Based Stack

Overview
I'm working on a directory-based stack that supports concurrent push and pop operations. This stack uses a hash table to store values, ensuring efficient insertion and deletion. The implementation leverages atomic operations to maintain thread safety without the need for explicit locks, which improves performance in a multithreaded environment.

Files
main.c: Contains the main function and thread operations.
stack.c: Implements the stack's push and pop logic, including hash table insertions and deletions.
stack.h: Header file with function declarations and necessary includes.

Build and Run
To compile and run the program, follow these steps:

Clone the Repository:
git clone https://github.com/yourusername/your-repo-name.git
cd your-repo-name

Compile the Program:
make

Run the Executable:
./program

Requirements
GCC (GNU Compiler Collection)
Make

Contributing
Feel free to fork the repository and submit pull requests. For major changes, please open an issue first to discuss what you would like to change.
