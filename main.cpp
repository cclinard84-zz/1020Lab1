#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include <string>


using namespace std;

const int MAX_NUMBER_OF_SKILLS = 7;
const int MAX_NUMBER_OF_JOBS = 10;
const int MAX_STR_LENGTH = 256;

struct jobs{
    public:
        char jobTitle[MAX_STR_LENGTH];
        int numberOfSkills;
        char skills[MAX_NUMBER_OF_SKILLS][20];
        double salary;
        char companyName[MAX_STR_LENGTH];
};

void welcome();
void openFile(ifstream&, string&);
void checkFileFail(ifstream&, string);
int readFile(jobs[], ifstream&);
void sortJobsArray(jobs[], int);
void printMenu();
void printJobs(jobs[], int);
void userInput(jobs[], int);
void searchJobsBySkills(jobs[], int);
void searchJobsByJobTitle(jobs[], int);
void printFoundJobTitle(jobs, char[]);
void printFoundJobSkills(jobs, char[]);
void toLower(char[]);
void printGoodBye();

int main()
{
    ifstream input;
    string inputFile;
    jobs jobsArray[MAX_NUMBER_OF_JOBS];
    int totalJobs;

    welcome();
    openFile(input, inputFile);
    checkFileFail(input, inputFile);
    totalJobs = readFile(jobsArray, input);
    sortJobsArray(jobsArray, totalJobs);
    printMenu();
    userInput(jobsArray, totalJobs);
}

void welcome(){
    cout << "Welcome to this program\n";
}

void openFile(ifstream &input, string &inputFile){
    cout << "Enter the input file name: ";
	cin >> inputFile;
	input.open(inputFile.c_str( ));
}

void checkFileFail(ifstream &input, string inputFile){
    char c;

	if (input.fail( )){
		cout << "The file " << inputFile << " does not exist.\n";
		exit(1);
	}
        input.get(c);
        if (input.eof( )){
                cout << "The file " << inputFile << " is empty.\n";
                exit(1);
        }
       	input.putback(c);
	return;
}

int readFile(jobs jobsArray[], ifstream &input){
    int i = 0;
    char c;
    string tempString;
    
	input.getline(jobsArray[i].jobTitle, MAX_STR_LENGTH);
	while(!input.eof() && i < MAX_NUMBER_OF_JOBS){
	    getline(input, tempString, '\n'); //don't have to eat a new line character
        jobsArray[i].numberOfSkills = stoi(tempString); 
        for(int j = 0; j < jobsArray[i].numberOfSkills;){
            for(int k = 0; k < MAX_STR_LENGTH;){
                input.get(c);
                if(c == '\n'){
                    k = 0;
                    j++;
                }
                else{
                    jobsArray[i].skills[j][k] = c;
                    k++;
                }
                if(j == jobsArray[i].numberOfSkills){
                    break;
                }
            }
        }
        
        input >> jobsArray[i].salary;
        input.get(c); //eat new line char
        input.getline(jobsArray[i].companyName, MAX_STR_LENGTH);
        i++;
        input.getline(jobsArray[i].jobTitle, MAX_STR_LENGTH);
	}
	return i;
}

void sortJobsArray(jobs jobsArray[], int totalJobs){
	int minIndex, i, top;
	jobs tempJob;
	for(top = 0; top < totalJobs-1; top++){
	// find min from top to bottom
		minIndex = top;
		for(i = top+1; i < totalJobs; i++){
			if (strcmp(jobsArray[minIndex].jobTitle, jobsArray[i].jobTitle) > 0)
				minIndex = i;
		}
		tempJob = jobsArray[minIndex];
		jobsArray[minIndex] = jobsArray[top];
		jobsArray[top] = tempJob;
	}
}

void printMenu(){
    cout << "1 Search for a job by skill" << endl;
    cout << "2 Search for a job by title" << endl;
    cout << "3 Quit" << endl;
    cout << "option> ";
}

void userInput(jobs jobArray[], int totalJobs){
    char userInput[5];
    cin >> userInput;
    while(strcmp(userInput, "3") != 0){
        if(strcmp(userInput, "1") == 0){
            searchJobsBySkills(jobArray, totalJobs);
        }
        else if(strcmp(userInput, "2") == 0){
            searchJobsByJobTitle(jobArray, totalJobs);
        }
        else{
            cout << "Invalid Response.\noption> ";
            cin >> userInput;
        }
    }
    printGoodBye();
}

void searchJobsBySkills(jobs jobArray[], int totalJobs){
    char searchString[MAX_STR_LENGTH];
    char tempStr[MAX_STR_LENGTH];
    cout << "Please enter what skill you wish to search for: ";
    cin >> searchString;
    toLower(searchString);
    for(int i = 0; i < totalJobs; i++){
        for(int j = 0; j < jobArray[i].numberOfSkills; j++){
            strcpy(tempStr, jobArray[i].skills[j]);
            toLower(tempStr);
            if(strcmp(searchString, tempStr) == 0){
                printFoundJobSkills(jobArray[i], searchString);
            }
        }
    }
}

void searchJobsByJobTitle(jobs jobArray[], int totalJobs){
    char searchString[MAX_STR_LENGTH];
    char tempStr[MAX_STR_LENGTH];
    cout << "Please enter what job title you want to search for: ";
    cin >> searchString;
    toLower(searchString);
    for(int i = 0; i < totalJobs; i++){
        strcpy(tempStr, jobArray[i].jobTitle);
        toLower(tempStr);
        if(strstr(tempStr, searchString) != NULL){
            printFoundJobTitle(jobArray[i], searchString);
        }
    }
}

void printFoundJobTitle(jobs job, char searchString[]){
    cout << job.jobTitle << endl;
}

void printFoundJobSkills(jobs job, char searchString[]){
    cout << "Job Skill: " << searchString << endl;
    cout << "Job Title"  << setw(50) << "Salary" << setw(25) << "Company"  << endl;
    cout << "-------------------------------------------------------------------------------------------" << endl;
    cout << job.jobTitle << right;
    cout << setw(25) << job.salary << left << setw(25);
    cout << setw(25) << job.companyName << endl;
    cout << endl;
}
void printGoodBye(){
    cout << "Thanks for using this program!\nGoodBye.";    
}

void toLower(char charArray[]){
    for(int i = 0; i < MAX_STR_LENGTH; i++){
        charArray[i] = tolower(charArray[i]);
    }
}
