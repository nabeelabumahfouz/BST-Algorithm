/*
    CIS*3490 ASSIGNMENT #4:
    Name: Nabeel Abu-Mahfouz
    Student #: 1104136
    DATE: 03/12/2023

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

int numofMW = 0;
int menMatrix[4][4];
int womenMatrix[4][4];
int matches[4];
int resultArray[4][4];

void readingInput(char *fileName);
void marriageProposals();
bool preferences(int w, int currentMale, int nextMale);

int main(int argc, char const *argv[])
{
    char fileName[100];

    printf("Please enter a filename: ");
    scanf("%s", fileName);

    // Reading in Matrix to each respective array:
    // ASSUME MEN = FIRST MATRIX & WOMEN = 2ND (as illustrated in textbook)
    readingInput(fileName);

    // Calling stable marriage problem algo:
    marriageProposals();

    // Storing matches in resultArray:
    for (int i = 0; i < numofMW; i++)
    {
        for (int j = 0; j < numofMW; j++)
        {

            if (matches[i] != j)
            {
                resultArray[i][j] = 0;
            }
            else
            {
                resultArray[i][j] = 1;
            }
        }
    }

    printf("\n\nResult Matrix:\n");
    // Printing out the results (0 = corresponding man and women are NOT matched , or 1 = corresponding man and women matched):
    for (int i = 0; i < numofMW; i++)
    {
        for (int j = 0; j < numofMW; j++)
            printf("%d ", resultArray[i][j]);

        printf("\n");
    }

    return 0;
}

// Reading input from file:
void readingInput(char *fileName)
{

    FILE *fp;
    fp = fopen(fileName, "r");

    if (fp == NULL)
    {
        printf("Error has occured opening the file!\n");
        exit(-1);
    }

    // Reading in first line to see lenght of file:
    fscanf(fp, "%d\n", &numofMW);

    if (fp)
    {

        // Reading in male matrix:
        for (int i = 0; i < numofMW; i++)
        {
            for (int j = 0; j < numofMW; j++)
                fscanf(fp, "%d", &menMatrix[i][j]);
        }

        // Skipping line in between matricies:
        fscanf(fp, "%*[^\n]\n");

        // Reading in female matrix
        for (int i = 0; i < numofMW; i++)
        {
            for (int j = 0; j < numofMW; j++)
                fscanf(fp, "%d", &womenMatrix[i][j]);
        }
    }

    // Print out matrices for verification
    printf("Matrix 1:\n");
    for (int i = 0; i < numofMW; i++)
    {
        for (int j = 0; j < numofMW; j++)
        {
            printf("%d ", menMatrix[i][j]);
        }
        printf("\n");
    }

    printf("\nMatrix 2:\n");
    for (int i = 0; i < numofMW; i++)
    {
        for (int j = 0; j < numofMW; j++)
        {
            printf("%d ", womenMatrix[i][j]);
        }
        printf("\n");
    }

    fclose(fp);
}

// Boolean function that checks to see if next male is more preferred over current:
bool preferences(int w, int currentMale, int nextMale)
{
    bool dummyVar;

    // Check if M prefers another women over his current match (i.e match = same spot in matrix)
    for (int i = 0; i < numofMW; i++)
    {

        // If the current male is a higher preference then the next male, return true, else return false:
        if (womenMatrix[w][i] - 1 == currentMale)
        {
            dummyVar = false;
            break;
        }
        if (womenMatrix[w][i] - 1 == nextMale)
        {
            dummyVar = true;
            break;
        }
    }

    return dummyVar;
}

// Function that deals with free men and matches:
void marriageProposals()
{

    // Initialize variables:
    int currentMale = 0;
    int nextMale = 0;
    int currentWomen = 0;
    int freeMW = numofMW;
    // int currentMatch = 0;
    int preferred = 0;

    // bool foundMatch = false;
    int freeMen[numofMW];
    int proposed[numofMW];
    // bool partnerArr[numofMW];

    // All men & women are free at start (as stated in textbook):
    for (int i = 0; i < numofMW; i++)
    {
        freeMen[i] = i;
        // matches[i] = -1;
        proposed[i] = 0;
    }

    // While there is still a man able to propose (i.e. free man who has not proposed to every women):
    while (freeMW > 0)
    {

        //Get first free man:
        currentMale = freeMen[0];
        // currentWomen = menMatrix[currentMale][proposed[currentMale]];
        // proposed[currentMale]++;

        //Iterate through current man's preference (starting from women he hasnt proposed to yet):
        for (int i = proposed[currentMale]; i < numofMW; i++)
        {
            //index of women in preference list:
            currentWomen = menMatrix[currentMale][i] - 1;

            //If the current women is NOT matched:
            if (matches[currentWomen] == -1)
            {
                //Match curent women with current man, removes man from free men list and subtract 1 from freeMW:
                matches[currentWomen] = currentMale;
                freeMen[0] = freeMen[freeMW - 1];
                freeMW--;
                break;
            }
            else //If the current women is proposed to, check to see if current male is a better preference:
            {
                //Current women index:
                nextMale = matches[currentWomen];

                // currentMatch = matches[currentWomen];

                //Checking to see if current man is preferred more than current match: 
                preferred = preferences(currentWomen, nextMale, currentMale);

                if (preferred)
                {
                    //Match women with current male, & add current match back to "freeMen":
                    matches[currentWomen] = currentMale;
                    freeMen[0] = nextMale;
                    //Updating proposed array with current male
                    proposed[currentMale] = i + 1;
                    break;
                }
                else //If not preferred we still need to update the proposed array with the current male:
                    proposed[currentMale] = i + 1;
            }
        }

        // Checking to see if current man has proposed to all women:
        if (proposed[currentMale] == numofMW)
        {
            freeMen[0] = freeMen[freeMW - 1];
            freeMW--;
        }
    }
}
