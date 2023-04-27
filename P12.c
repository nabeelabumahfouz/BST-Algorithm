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

// NOTE: DO NOT NEED FREQ TABLES WITH GREEDY TECHNIQUE!!!

// double mainT[1000][1000];
// int rootT[1000][1000];
double probability[1000];

typedef struct Node
{
    char word[51];
    int counter;
    struct Node *left;
    struct Node *right;

} Node;

int FileRead(char *wordArr[2045]);
int compareNode(const void *a, const void *b);
void freeBST(Node *rt);
// void freqTables(int uniqueWordCount, Node *temp);
Node *createTree(Node *temp, int uniqueWordCount);
void searchGreedyBST(Node *rt, char *keyWord, Node *temp);
Node *insertNode(Node *rt, Node *temp, int left, int right);

int main(int argc, char const *argv[])
{
    // struct Node *root[2045];
    char *arr[2045];
    char keyword[100];
    int uniqueWordCount = 0;
    int wordComp;
    int x = 0;
    bool match = false;

    // Read into files & obtain unique word count:
    uniqueWordCount = FileRead(arr);
    Node temp[uniqueWordCount]; // Storing only unique words in struct

    // Initialize all values for num of unique words:
    for (int i = 0; i < uniqueWordCount; i++)
    {
        strcpy(temp[i].word, "");
        temp[i].counter = 0;
    }

    // Loop through word array to populate our struct:
    // Check if word already exists (we only want to store UNIQUE ones):
    for (int i = 0; i < 2045; i++)
    {

        // Setting match found to false initially:
        match = false;

        // Loop up until the current index of temp array (i.e. current # of unique words):
        for (int j = 0; j < x; j++)
        {
            wordComp = strcmp(arr[i], temp[j].word);

            if (wordComp == 0)
            {
                temp[j].counter++;
                match = true;
                break;
            }
        }

        // If no match was found that means word is unique! (therefore add it to struct):

        if (match == false)
        {
            strcpy(temp[x].word, arr[i]);
            temp[x].counter = 1;
            temp[x].word[strlen(arr[i])] = '\0';
            x++;
        }
    }

    // Sorting list of words by something other than O n^2 (as discussed in lecture):
    qsort(temp, uniqueWordCount, sizeof(Node), *compareNode);

    // Assigning frequencies & 2 Matricies (C & R) for main root table and main table:
    for (int i = 0; i < uniqueWordCount; i++)
    {

        double totwords = (double)2045;
        double currWord = (double)temp[i].counter;

        probability[i] = currWord / totwords;
    }

    // NOTE: DO NOT NEED FREQ TABLES WITH GREEDY TECHNIQUE!!!

    // Construct the freq tables based on pseudocode in class:
    // freqTables(uniqueWordCount, temp);

    // Create optimal BST:
    Node *rt = createTree(temp, uniqueWordCount);

    // Prompt user for keyword:
    printf("Please enter in a keyword: ");
    scanf("%s", keyword);

    // SEARCH TREE:
    searchGreedyBST(rt, keyword, temp);

    // Freeing Tree:
    freeBST(rt);

    // //Freeing Word Array:
    for (int i = 0; i < 2045; i++)
        free(arr[i]);

    return 0;
}

///////FUNCTION DEFINITIONS BELOW:///////

// This function is used to take in a text file with 2045 words (accounts for extra spaces and tabs between words):
int FileRead(char *wordArr[2045])
{

    char fileName[100];
    char buff[51] = "";
    int uniqueWordCount = 0;
    int x = 0;
    int wordComp;
    bool match = false;
    FILE *fp;

    // Prompting user for filename:
    printf("Please enter the name of the file (for P12): ");
    scanf("%s", fileName);

    // Opening file and error checking:
    fp = fopen(fileName, "r");

    if (fp == NULL)
    {
        printf("AN ERROR HAS OCCURED WHILE OPENING FILE!\n");
        exit(-1);
    }

    while (x < 2045 && !feof(fp))
    {

        fscanf(fp, " %s ", buff);

        match = false;

        // Check if the word already exists:
        for (int i = 0; i < x; i++)
        {

            wordComp = strcmp(buff, wordArr[i]);

            if (wordComp == 0)
            {
                match = true;
                break;
            }
        }

        // If word does not exist, add 1 to the unique number of words var:
        if (match == false)
            uniqueWordCount++;

        // Storing current word located in buffer into array:
        wordArr[x] = malloc((strlen(buff) + 1) * sizeof(char));
        strcpy(wordArr[x], buff);

        // Incrementing x for next word:
        x++;
    }

    // Closing file:
    fclose(fp);

    return uniqueWordCount;
}

// Compare helper function for q sort:
int compareNode(const void *a, const void *b)
{

    // Initializing:
    Node *nodeA;
    Node *nodeB;

    // Assigning:
    nodeA = (Node *)a;
    nodeB = (Node *)b;

    // Comapring & returning
    return strcmp(nodeA->word, nodeB->word);
}

// Helper function used to free BST tree at the end of program:
void freeBST(Node *rt)
{

    // If the node is not the root return (i.e. exit fcn):
    if (!rt)
        return;
    else
    {
        freeBST(rt->left);
        freeBST(rt->right);
        free(rt);
    }
}

/*NOTE: DO NOT NEED FREQ TABLES WITH GREEDY TECHNIQUE (since it searches the BST based on highest frequency):

FREQ TABLE BASED ON PSEUDOCODE PROVIDED IN LECTURES AND IN TEXTBOOK:
void freqTables(int uniqueWordCount, Node *temp)
{

    float sum = 0;
    float min = 1000000.000;
    int min_K = 1000000.000;
    int j = 0;

    // Initialize tables:
    for (int i = 1; i <= uniqueWordCount; i++)
    {
        mainT[i][i - 1] = 0.0;
        mainT[i][i] = probability[i - 1];
        rootT[i][i] = i;
    }

    // mainT[uniqueWordCount + 1][uniqueWordCount] = 0;

    // Loop through table on downwards (going in the bottom right direction):
    for (int d = 1; d <= uniqueWordCount - 1; d++)
    {
        for (int i = 1; i <= (uniqueWordCount - d); i++)
        {
            j = d + i;

            // Summation of probabilities (textbook):
            sum = 0;
            for (int s = i - 1; s <= j - 1; s++)
                sum += probability[s];

            ////Finding mine value of all i->j:////

            // Reset placeholders to max vals:
            min = 1000000.000;
            min_K = 1000000.000;

            for (int k = i; k <= j; k++)
            {

                // Calculating & seeing if current # is less than min:
                if ((mainT[i][k - 1] + mainT[k + 1][j]) < min)
                {

                    // Reset min placeholder:
                    min = (mainT[i][k - 1] + mainT[k + 1][j]);
                    min_K = k;
                }
            }
            // Loading min values into each table:
            rootT[i][j] = min_K;
            mainT[i][j] = sum + min;
        }
    }
}
*/

// Function used for BST tree creation:
Node *createTree(Node *temp, int uniqueWordCount)
{

    // Initialize root node of the tree:
    Node *rt = NULL;

    // Call insert to root table fcn:
    rt = insertNode(rt, temp, 1, uniqueWordCount);

    return rt;
}

// Function used to insert a node to BST tree (recursively):
Node *insertNode(Node *rt, Node *temp, int left, int right)
{

    // base case (reaches the end of diagonal; i.e. end of the subarray):
    if (left > right)
        return NULL;

    ////Continue if its not the base case:////

    // Initializing variables:
    int ind = left;
    float max = 0.00;

    // Find the highest frequency word in the node subarray:
    for (int i = left; i <= right; i++)
    {

        // Updating max if current frequency is higher than previous max:
        if (probability[i] > max)
        {
            max = probability[i];
            ind = i;
        }
    }

    // Allocate memory for new node:
    rt = (Node *)malloc(sizeof(Node));

    // Copy word and counter from corresponding node in the temp array (using current index):
    rt->counter = temp[ind].counter;
    strcpy(rt->word, temp[ind].word);

    // Setting left and right pointers of new node to NULL:
    rt->left = NULL;
    rt->right = NULL;

    // Recursively build left & right subtree (left = left -> ind-1 , right = ind+1 -> right):
    rt->left = insertNode(rt->left, temp, left, ind - 1);
    rt->right = insertNode(rt->right, temp, ind + 1, right);

    return rt;
}

// Function used to search the BST tree for KEY recursively using the GREEDY TECHNIQUE:
void searchGreedyBST(Node *rt, char *keyWord, Node *temp)
{

    // Initializing variables:
    int x;
    double freq;
    int ind;

    // While in the tree:
    if (rt == NULL)
    {
        printf("Word not found!\n");
        return;
    }

    // Getting current frequency of word by looping through probability array (populated in main):
    ind = -1;

    for (int i = 0; i < 1000; i++){

        //Checks t0 see if theres a match between current word and the word stored in our word array:
        if (strcmp(rt->word, temp[i].word) == 0){
            //Setting index to correseponding word array index:
            ind = i;
            break;
        }
    }

    //Checking to see if frequency was found and setting it to our index variable
    if (ind != -1)
        freq = probability[ind];
    


    // Printing out which way tree is going and what it has compared with (as specified in guide handout):
    x = strcmp(keyWord, rt->word);

    if (x < 0)
    {
        printf("Compared with %s (%0.3lf), go to LEFT subtree.\n", rt->word, freq);
        searchGreedyBST(rt->left, keyWord, temp);
    }
    else if (x > 0)
    {
        printf("Compared with %s (%0.3lf), go to RIGHT subtree.\n", rt->word, freq);
        searchGreedyBST(rt->right, keyWord, temp);
    }
    else
    {
        printf("Compared with %s (%0.3lf), ", rt->word, freq);
        printf("FOUND KEY!!!\n\n");
        return;
    }
}
