/*	File: lab1.cpp
*		Author: Matt Clinard
*		Date: 9/5/2017
* 	This application allows the user to get information about programming jobs
*		in the Knoxville area.
*/

#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include <string>

using namespace std;

const int MAX_NUMBER_OF_SKILLS = 10;
const int MAX_NUMBER_OF_JOBS = 10;
const int MAX_STR_LENGTH = 256;

struct jobs{
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
void printMenu(jobs[], int);
void userInput(jobs[], int);
void searchJobsBySkills(jobs[], int);
void searchJobsByJobTitle(jobs[], int);
void printFoundJob(jobs);
void printFoundJobSkills(jobs);
void printJobSkillHeader(char[]);
void printJobTitleHeader(char[]);
void toUpperFirstChar(char[]);
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
	printMenu(jobsArray, totalJobs);
}

/* welcome: This function welcomes the user and gives a brief description of the program.
*  Parameters: none
*	 Returns: nothing
*/
void welcome(){
    printf( "Welcome to iSearch!\nThis program allows you to search job postings based on job title or skills required for the job.\n");
	fflush(stdout);
	fflush(stdin);
}

/* openFile: This function asks the user for the inputFile name and opens the ifstream
*  Parameters: input - ifstream variable name
* 						 inputFile - string that the user enters
*	 Returns: nothing
*/
void openFile(ifstream &input, string &inputFile){
    cout << "Enter the file name: ";
	cin >> inputFile;
	input.open(inputFile.c_str( ));
}

/* checkFileFail: This function checks to make sure the file does exist and is not empty.
*  Parameters: input - ifstream variable name
* 						 inputFile - string that the user enters
*	 Returns: Nothing
*/
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

/* readFile: Reads the file and puts each job object into an array of structures
*  Parameters: jobsArray - an array of job structures
							 input - ifstream variable name
	 Precondition: File exists, is valid, and has opened successfully
	 Postcondition: array of job structures filled to i
*	 Returns: number of job structures created
*/
int readFile(jobs jobsArray[], ifstream &input){
    int i = 0;
    char c;
    string tempString;

	input.getline(jobsArray[i].jobTitle, MAX_STR_LENGTH);

	//While not eof and jobs < 10 add data to array
	while(!input.eof() && i < MAX_NUMBER_OF_JOBS){
        input >> jobsArray[i].numberOfSkills;
		input.get(c);
        for(int j = 0; j < jobsArray[i].numberOfSkills;){
            for(int k = 0; k < MAX_STR_LENGTH;){
                input.get(c);
                if(c == '\n'){
					jobsArray[i].skills[j][k] = '\0';
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
	input.close();
	return i;
}

/* sortJobsArray: Sorts the jobsArray once the array of structures has been populated
*  Parameters: jobsArray - an array of job structures
* 						 totalJobs - total number of jobs within the jobArray
*	 Precondition: jobsArray has job objects
*  Postcondition: Array of job structures is sorted by jobTitle
*	 Returns: Nothing
*/
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

/* printMenu: Prints the menu for the user
*  Parameters: jobArray - an array of job structures
* 						 totalJobs - total number of jobs within the jobArray
*	 Returns: Nothing
*/
void printMenu(jobs jobArray[], int totalJobs){
    cout << "1 Search for a job by skill" << endl;
    cout << "2 Search for a job by title" << endl;
    cout << "3 Quit" << endl;
    cout << "option> ";
	cin.clear();
	userInput(jobArray, totalJobs);
}

/* userInput: This function contains the logic to determine where to go following
							input from the user
*  Parameters: jobArray - an array of job structures
							 totalJobs - total number of jobs within the jobArray
*	 Returns: Nothing
*/
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
            cout << "Invalid Response." << endl;
            printMenu(jobArray, totalJobs);
        }
		printMenu(jobArray, totalJobs);
    }
    printGoodBye();
}

/* searchJobsBySkills: This method searches for jobs in the jobArray by skills.
*  Parameters: jobArray - an array of job structures
							 totalJobs - total number of jobs within the jobArray
*	 Returns: Nothing
*/
void searchJobsBySkills(jobs jobArray[], int totalJobs){
    char searchString[MAX_STR_LENGTH];
    char tempStr[MAX_STR_LENGTH];
	int numFoundJobs = 0;
	jobs tempJobArray[MAX_NUMBER_OF_JOBS];

    cout << "Please enter what skill you want to search for: ";
    cin >> searchString;
    toLower(searchString);

    for(int i = 0; i < totalJobs; i++){
        for(int j = 0; j < jobArray[i].numberOfSkills; j++){
            strcpy(tempStr, jobArray[i].skills[j]);
            toLower(tempStr);
            if(strcmp(searchString, tempStr) == 0){
				tempJobArray[numFoundJobs] = jobArray[i];
				numFoundJobs++;
            }
        }
    }

	if(numFoundJobs == 0){
		cout << "No jobs found with that search query." << endl;
		return;
	}

	printJobSkillHeader(searchString);
	for(int i = 0; i < numFoundJobs; i++){
		printFoundJobSkills(tempJobArray[i]);
	}
}

/* searchJobsByJobTitle : This method searches for jobs in the jobArray by title.
*  Parameters: jobArray - an array of job structures
							 totalJobs - total number of jobs within the jobArray
*	 Returns: Nothing
*/
void searchJobsByJobTitle(jobs jobArray[], int totalJobs){
    char searchString[MAX_STR_LENGTH];
    char tempStr[MAX_STR_LENGTH];
	int numberFoundJobs = 0;
	jobs tempJobsArray[MAX_NUMBER_OF_JOBS] = {'\0'}; //initialize all elements of tempJobsArray to null characters for troubleshooting reference

	//Ask user for input
    printf("Please enter what job title you want to search for: ");
    cin >> searchString;
    toLower(searchString);

    for(int i = 0; i < totalJobs; i++){
		memset(tempStr, '\0', sizeof(tempStr)); //sets all elements of tempStr to null characters
        strcpy(tempStr, jobArray[i].jobTitle);
        toLower(tempStr);

		//if at any point a match is found within the tempstr add job to tempJobsArray
        if(strstr(tempStr, searchString) != NULL){
            tempJobsArray[numberFoundJobs] = jobArray[i];
			numberFoundJobs++;
        }
    }

	//if no jobs are found return
	if(numberFoundJobs == 0){
		printf("No jobs found with that search query.\n");
		return;
	}

	printJobTitleHeader(searchString);
	for(int k = 0; k < numberFoundJobs; k++){
		printFoundJob(tempJobsArray[k]);
	}
}

/* printJobTitleHeader: Prints the header for jobs found searching by job title
*  Parameters: searchString - a character array that contains the users search term
*	 Returns: Nothing
*/
void printJobTitleHeader(char searchString[]){
	printf("Job Title: %s\n", searchString);
	printf("%-50s%-21s%s\n", "Job Title", "Salary", "Company");
    printf("%s\n","----------------------------------------------------------------------------------------------");
	fflush(stdout);
}

/* printJobSkillHeader: Prints the header for jobs found by searching by skill
*  Parameters: searchString - a character array that contains the users search term
*	 Returns: Nothing
*/
void printJobSkillHeader(char searchString[]){
	printf("Job Skill: %s\n", searchString);
	printf("%-50s%-21s%s\n", "Job Title", "Salary", "Company");
    printf("%s\n","----------------------------------------------------------------------------------------------");
	fflush(stdout);
}

/* printFoundJob: Prints a job object that the search functions found relevent to searched term.
*  Parameters: job - a job object that was found by the searchJobsByTitle function
*	 Returns: Nothing
*/
void printFoundJob(jobs job){
	printf("%-50s%s%-20.2f%s\n",job.jobTitle, "$", job.salary, job.companyName);
	fflush(stdout);
	for(int i = 0; i < job.numberOfSkills; i++){
		printf("     %-s\n", job.skills[i]);
		fflush(stdout);
	}
    printf("\n");
}

/* printFoundJobSkills: Prints a job object that the search functions found relevent to searched term.
*  Parameters: job - a job object that was found by the searchJobsBySkills function
*	 Returns: Nothing
*/
void printFoundJobSkills(jobs job){
	printf("%-50s%s%-20.2f%s\n",job.jobTitle, "$", job.salary, job.companyName);
	fflush(stdout);
	for(int i = 0; i < job.numberOfSkills; i++){
		printf("     %-s\n", job.skills[i]);
		fflush(stdout);
	}
    printf("\n");
}

/* printGoodBye: Gives the user a farewell message and exits the program
*  Parameters: None
*	 Returns: Nothing
*/
void printGoodBye(){
    cout << "Thanks for using this program!\nGoodBye.";
	exit(0);
}

/* toLower: This function lowercases the users search input
*  Parameters: charArray - an array of characters
*	 Returns: Nothing
*/
void toLower(char charArray[]){
    for(int i = 0; i < MAX_STR_LENGTH; i++){
        charArray[i] = tolower(charArray[i]);
    }
}
