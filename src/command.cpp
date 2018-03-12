#include <iostream>
#include <algorithm>
#include <string>
#include <stack>
#include <queue>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include "command.h"
#include "fstream" 
#include<fcntl.h>

using namespace std;

Command::Command(vector<string> v) {
	for (unsigned i = 0; i < v.size(); ++i) {
		cmds.push_back(v.at(i));
	}
}

bool Command::evaluate() {
	// cout << "Running COMMAND evaluate" << endl;
	if (cmds.at(0) == "exit") {
		exit(0);
	}
	//check if its a test command
	if (cmds.at(0) == "test" || cmds.at(0) == "[") {
		if (cmds.at(0) == "[") {
			//check if brackets are valid
			if (cmds.at(cmds.size()-1) != "]") {
				cout << "Invalid brackets" << endl;
				return false;
			}
		}
		//the different test commands
		if (cmds.at(1) == "-e") {
			struct stat info;
			//check if file/directory is accessible
			if (stat(const_cast<char*>(cmds.at(2).c_str()), &info) != 0) {
				cout << "Cannot access " << cmds.at(2) << endl;
				cout << "(False)" << endl;
				return false;
			}
			else {
				cout << "(True)" << endl;
				return true;
			}
		}
		else if (cmds.at(1) == "-f"){
			struct stat info;
			if (stat(const_cast<char*>(cmds.at(2).c_str()), &info) != 0){
				cout << "Cannot access " << cmds.at(2) << endl;
				cout << "(False)" << endl;
				return false;
			}
			else if (info.st_mode & S_IFREG) {
				cout << "(True)" << endl;
				return true;
			}
			else {
				cout << "(False)" << endl;
				return false;
			}
		}
		else if (cmds.at(1) == "-d"){
			struct stat info;
			if (stat(const_cast<char*>(cmds.at(2).c_str()), &info) != 0){
				cout << "Cannot access " << cmds.at(2) << endl;
				cout << "(False)" << endl;
				return false;
			}
			else if (info.st_mode & S_IFDIR){
				cout << "(True)" << endl;
				return true;
			}
			else {
				cout << "(False)" << endl;
				return false;
			}
		}
		else {
			struct stat info;
			if (stat(const_cast<char*>(cmds.at(1).c_str()), &info) != 0){
				cout << "cannot access " << cmds.at(1) << endl;
				cout << "(False)" << endl;
				return false;
			}
			else {
				cout << "(True)" << endl;
				return true;
			}
		}
		
	}
	//handles < operator
	// else if (find(cmds.begin(), cmds.end(), "<") != cmds.end()){
	// 	cout << "Contains <" << endl;

	// 	vector<string> left;
	// 	//vector<string> right;
	// 	string right;

	// 	int count = 0;
	// 	for (unsigned i = 0; i < cmds.size(); ++i) {
	// 		if (cmds.at(i) != "<") {
	// 			left.push_back(cmds.at(i));
	// 			++count;
	// 		}
	// 		else {
	// 			break;
	// 		}
	// 	}

	// 	++count;
	// 	right = cmds.at(count);
	// 	// for (unsigned i = count; i < cmds.size(); ++i) {
	// 	// 	right.push_back(cmds.at(i));
	// 	// }
	// 	int f_descriptor = open(right.c_str(), O_WRONLY | O_APPEND);
	// 	if (f_descriptor < 0){
	// 		cout << "Error opening the file" <<endl;
	// 		return false;
	// 	}

	// 	dup2(f_descriptor, 1);
	// 	printf("hi");

	// 	return true;
	// }
	// //handles > and >> operator
	// else if (find(cmds.begin(), cmds.end(), ">") != cmds.end() || find(cmds.begin(), cmds.end(), ">>") != cmds.end()){
	// 	cout << "Contains > or >>" << endl;

	// 	if (find(cmds.begin(), cmds.end(), ">") != cmds.end()) {
	// 		vector<string> left;
	// 		//vector<string> right;
	// 		string right;

	// 		int count = 0;
	// 		for (unsigned i = 0; i < cmds.size(); ++i) {
	// 			if (cmds.at(i) != ">") {
	// 				left.push_back(cmds.at(i));
	// 				++count;
	// 			}
	// 			else {
	// 				break;
	// 			}
	// 		}

	// 		++count;
	// 		right = cmds.at(count);
	// 		// for (unsigned i = count; i < cmds.size(); ++i) {
	// 		// 	right.push_back(cmds.at(i));
	// 		// }
	// 		int f_descriptor = open(right.c_str(), O_WRONLY | O_APPEND);
	// 		if (f_descriptor < 0){
	// 			cout << "Error opening the file" <<endl;
	// 			return false;
	// 		}
	// 		cout <<" asdf" << endl;
	// 		dup2(f_descriptor, STDOUT_FILENO);
	// 		cout << ";lkj" << endl;
	// 		close (f_descriptor);
			
	// 		printf("hi");
	// 		return true;
	// 	}

	// 	else {
	// 		vector<string> left;
	// 		//vector<string> right;
	// 		string right;

	// 		int count = 0;
	// 		for (unsigned i = 0; i < cmds.size(); ++i) {
	// 			if (cmds.at(i) != ">>") {
	// 				left.push_back(cmds.at(i));
	// 				++count;
	// 			}
	// 			else {
	// 				break;
	// 			}
	// 		}

	// 		++count;
	// 		right = cmds.at(count);
	// 		// for (unsigned i = count; i < cmds.size(); ++i) {
	// 		// 	right.push_back(cmds.at(i));
	// 		// }
	// 	}

	// }
	else {
		int status = 0;
		pid_t pid = fork();
		pid_t w;

		if (pid < 0) { // if pid returns a negative value, then error
			cout << "*** ERROR: forking child process failed\n" << endl;
			exit(1);
		}
		else if (pid == 0) {
			if (find(cmds.begin(), cmds.end(), "<") != cmds.end()){
				cout << "Contains <" << endl;

				vector<string> left;
				//vector<string> right;
				string right;

				int count = 0;
				for (unsigned i = 0; i < cmds.size(); ++i) {
					if (cmds.at(i) != "<") {
						left.push_back(cmds.at(i));
						++count;
					}
					else {
						break;
					}
				}

				++count;
				right = cmds.at(count);

				cout << "Contents of left vector are: " << endl;
					for (unsigned i = 0; i < left.size(); ++i) {
						cout << left.at(i) << endl;
					}
				cout << endl;


				cout << "Contents of right string is: " << endl;
				cout << right << endl;

				ifstream inputfile;
				inputfile.open(right.c_str(), ios::in);
				string linecontents = "";
				string filecontents = "";
				if (inputfile){
					while(!inputfile.eof()){
						inputfile >> linecontents;
						cout << "contents of the line are: " <<linecontents << endl;
						filecontents += linecontents + "\n";
					}
					inputfile.close();
				}
				else{
					cout << "unable to open file" << endl;
					exit(1);
				}
				cout << "Contents of the file are: " << endl;
				cout << filecontents << endl;

				unsigned arrSizeIO = left.size() + 2;
				char * argsIO[arrSizeIO]; // make a char pointer array of the same size as left vector
					// populate argsIO array with commands in left vector
				for (unsigned i = 0; i < arrSizeIO - 2; ++i) {
					argsIO[i] = const_cast<char*>(left.at(i).c_str());
				}
				argsIO[arrSizeIO-2] = const_cast<char*>(filecontents.c_str());
				argsIO[arrSizeIO - 1] = NULL; // make last index NULL

				if (execvp(*argsIO, argsIO) < 0) { // if execvp returns, then error
					cout << "*** ERROR: exec failed\n" << endl;
			        exit(1);
				}
			}
			//handles > and >> operator
			else if (find(cmds.begin(), cmds.end(), ">") != cmds.end() || find(cmds.begin(), cmds.end(), ">>") != cmds.end()) {
				if (find(cmds.begin(), cmds.end(), ">") != cmds.end()) {
					cout << "Contains >" << endl;
					vector<string> left;
					//vector<string> right;
					string right;

					int count = 0;
					for (unsigned i = 0; i < cmds.size(); ++i) {
						if (cmds.at(i) != ">") {
							left.push_back(cmds.at(i));
							++count;
						}
						else {
							break;
						}
					}

					++count;
					right = cmds.at(count);

					cout << "Contents of left vector are: " << endl;
					for (unsigned i = 0; i < left.size(); ++i) {
						cout << left.at(i) << endl;
					}
					cout << endl;

					unsigned arrSizeIO = left.size() + 1;
					char * argsIO[arrSizeIO]; // make a char pointer array of the same size as left vector
					// populate argsIO array with commands in left vector
					for (unsigned i = 0; i < arrSizeIO - 1; ++i) {
						argsIO[i] = const_cast<char*>(left.at(i).c_str());
					}
					argsIO[arrSizeIO - 1] = NULL; // make last index NULL

					cout << "Contents of right string is: " << endl;
					cout << right << endl;

					// open the file
					// O_RDWR | O_CREAT, S_IRUSR | S_IWUSR
					int f_descriptor = open(right.c_str(), O_RDWR | O_TRUNC | O_CREAT, S_IRWXU | S_IRWXG);

					if (f_descriptor < 0) {
						cout << "Error opening the file" << endl;
						return false;
					}
					
					dup2(f_descriptor, STDOUT_FILENO);
					close(f_descriptor);

					if (execvp(*argsIO, argsIO) < 0) { // if execvp returns, then error
						cout << "*** ERROR: exec failed\n" << endl;
			            exit(1);
					}
				}
				else {
					vector<string> left;
					//vector<string> right;
					string right;

					int count = 0;
					for (unsigned i = 0; i < cmds.size(); ++i) {
						if (cmds.at(i) != ">>") {
							left.push_back(cmds.at(i));
							++count;
						}
						else {
							break;
						}
					}

					++count;
					right = cmds.at(count);
					// for (unsigned i = count; i < cmds.size(); ++i) {
					// 	right.push_back(cmds.at(i));
					// }
					cout << "Contents of left vector are: " << endl;
					for (unsigned i = 0; i < left.size(); ++i) {
						cout << left.at(i) << endl;
					}
					cout << endl;

					unsigned arrSizeIO = left.size() + 1;
					char * argsIO[arrSizeIO]; // make a char pointer array of the same size as left vector
					// populate argsIO array with commands in left vector
					for (unsigned i = 0; i < arrSizeIO - 1; ++i) {
						argsIO[i] = const_cast<char*>(left.at(i).c_str());
					}
					argsIO[arrSizeIO - 1] = NULL; // make last index NULL

					cout << "Contents of right string is: " << endl;
					cout << right << endl;

					// open the file
					// O_RDWR | O_CREAT, S_IRUSR | S_IWUSR
					int f_descriptor = open(right.c_str(), O_RDWR | O_APPEND | O_CREAT, S_IRWXU | S_IRWXG);

					if (f_descriptor < 0) {
						cout << "Error opening the file" << endl;
						return false;
					}
					
					dup2(f_descriptor, STDOUT_FILENO);
					close(f_descriptor);

					if (execvp(*argsIO, argsIO) < 0) { // if execvp returns, then error
						cout << "*** ERROR: exec failed\n" << endl;
			            exit(1);
					}
				}
			}
			else { // user enters a regular command like "ls -a"
				unsigned arrSize = cmds.size() + 1;
				char * args[arrSize]; // make a char pointer array of the same size as cmds vector
				// populate args array with commands in cmds vector
				for (unsigned i = 0; i < arrSize - 1; ++i) {
					args[i] = const_cast<char*>(cmds.at(i).c_str());
				}
				args[arrSize - 1] = NULL; // make last index NULL

				if (execvp(*args, args) < 0) { // if execvp returns, then error
					cout << "*** ERROR: exec failed\n" << endl;
		            exit(1);
				}
			}
		}
		else {
			w = waitpid(pid, &status, 0);
			if (w == -1){
				cout << "Issue with waitpid" << endl;
				exit(EXIT_FAILURE);
			}
			if (WEXITSTATUS(status) == 0){
				return true;
			}
		}
	}

	return false;
}


string Command::element() {
	string total;
	for (unsigned i = 0; i < cmds.size() - 1; ++i) {
		total = total + cmds.at(i) + " ";
	}
	total = total + cmds.at(cmds.size() - 1);
	return total;
}

void Command::setLeft(Base* node) {}
void Command::setRight(Base* node) {}