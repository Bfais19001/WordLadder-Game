
/*
Project 4: Link Lint List - Word Ladder Building Game & Solver
Course: CS 211, spring 2026
Author: Bassel Faisal

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

typedef struct WordNode_struct {
    char* myWord;
    struct WordNode_struct* next; 
} WordNode;

typedef struct LadderNode_struct {
    WordNode* topWord;
    struct LadderNode_struct* next; 
} LadderNode;

// void freeLadder(WordNode* ladder);
// helpers
static int cmp_strptr(const void* a, const void* b) {
    const char* const* sa = (const char* const*)a;
    const char* const* sb = (const char* const*)b;
    return strcmp(*sa, *sb);
}


//------------------- \/\/\/ TOP OF TASK 1 \/\/\/ --------------------

// this function scans the file and counts how many tokens have exactly 
// wordSize chars
int countWordsOfLength(char* filename, int wordSize) { 
    //---------------------------------------------------------
    // TODO - write countWordsOfLength()    
    FILE* fp = fopen(filename, "r");
    if (!fp) return -1;

    int count = 0;
    char buf[256];
    while (fscanf(fp, "%255s", buf)== 1) {
        if ((int)strlen(buf) == wordSize) ++count;
    }

    fclose(fp);
    return count; //modify this line
}


/* This function fills pre allocated words only using the words only using 
the words which length is wordSize from the given file and then it sorts it
alphabetically */
bool buildWordArray(char* filename, char** words, int numWords, int wordSize) { 
    //---------------------------------------------------------
    // TODO - write buildWordArray()
    FILE* fp = fopen(filename, "r");
    if (!fp) return false;

    int filled = 0;
    char buf[256];
    while(fscanf(fp, "%255s", buf) == 1) {
        if ((int)strlen(buf) == wordSize){
            if (filled >= numWords) { fclose(fp); return false;}

            // copy into the pre-allocated spot
            strcpy(words[filled], buf);
            ++filled;
        }
    }
    fclose(fp);
    if (filled != numWords) return false;

    // sort for binary search
    qsort(words, numWords, sizeof(char*), cmp_strptr);

    return true; //modify this line
}

// uses binary search words[] for aWord with the hi and lo indicies
int findWord(char** words, char* aWord, int loInd, int hiInd) {
    //---------------------------------------------------------
    // TODO - write findWord()
    int lo = loInd;
    int hi = hiInd;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        int cmp = strcmp(aWord, words[mid]);
        if (cmp == 0) return mid;
        if (cmp < 0) hi = mid - 1;
        else lo = mid + 1;
    }
    return -1; // modify this line
}

// This function frees a heap allocated array of c strings
void freeWords(char** words, int numWords) {
    //---------------------------------------------------------
    // TODO - write freeWords()
    if (!words) return;
    for (int i = 0; i < numWords; ++i) {
        free(words[i]);
    }
    free(words);
}

//---------------------- ^^^ END OF TASK 1 ^^^ ----------------------


//------------------- \/\/\/ TOP OF TASK 2 \/\/\/ -------------------
// This function counts how many positions between two strings
int strCmpCnt(char* str1, char* str2) {
    //---------------------------------------------------------
    // TODO - write strCmpCnt()
    int diff = 0;
    const char *p = str1, *q = str2;

    while (*p != '\0' || *q != '\0') {
        char a = (*p != '\0') ? *p : '\0';
        char b = (*q != '\0') ? *q : '\0';
        if ( a != b) ++diff;
        if (*p != '\0') ++p;
        if (*q != '\0') ++q;
    }
    return diff; //modify this line
}

int strCmpInd(char* str1, char* str2) {
    //---------------------------------------------------------
    // TODO - write strCmpInd()
    int i = 0;

    while (str1[i] != '\0' || str2[i] != '\0') {
        if (str1[i] != str2[i]) {
            return i;
        }
        i++;
    }

    return -1; //modify this line
}

//---------------------- ^^^ END OF TASK 2 ^^^ ----------------------


//------------------- \/\/\/ TOP OF TASK 3 \/\/\/ -------------------

//This function prepends a word pointer to the linked list
void insertWordAtFront(WordNode** ladder, char* newWord) {
    //---------------------------------------------------------
    // TODO - write insertWordAtFront()
    WordNode* node = (WordNode*)malloc(sizeof(WordNode));
    node->myWord = newWord;
    node->next = *ladder;
    *ladder = node;
}

// This function counts nodes in the laddder list
int getLadderHeight(WordNode* ladder) {
    //---------------------------------------------------------
    // TODO - write getLadderHeight()
    int h = 0;
    while (ladder) {
        ++h;
        ladder = ladder->next;
    }
    return h; // modify this line
}

/* This function validates a user entered word which is aganist the rules and checks if it is the correct
length and if it exists in the dictionary and if it differs from the ladder top */
bool checkForValidWord(char** words, int numWords, int wordSize, WordNode* ladder, char* aWord) {
    //---------------------------------------------------------
    // TODO - write checkForValidWord()
    //---------------------------------------------------------
    
    // print statement for various scenarios:
        if (strcmp(aWord, "DONE") == 0) {
        // user entered "DONE" - valid, top (#1) priority
            printf("Stopping with an incomplete word ladder...\n");
            return true;
        }
        if ((int)strlen(aWord) != wordSize) {
            // user entered a word that is too long/short - invalid, priority #2
            printf("Entered word does NOT have the correct length. Try again...\n");
            return false;
        }
        if (findWord(words, aWord, 0, numWords - 1) < 0) {
            // user entered a word that is not found in the dictionary - invalid, priority #3
            printf("Entered word NOT in dictionary. Try again...\n");
            return false;
        }
        if (ladder != NULL) {
            char* prevWord = ladder->myWord;


            // count how many characters differ
            int diff = strCmpCnt(prevWord, aWord);
            if (diff != 1) {
        // user entered a word that requires changing more than one-character - invalid, priority #4a
        // user entered a word that requires no character difference (the same word) - invalid, priority #4b
            printf("Entered word is NOT a one-character change from the previous word. Try again...\n");
            return false;
            }
        }
        // user entered a word a valid word - valid, priority #5 (default)
        printf("Entered word is valid and will be added to the word ladder.\n");
        return true; //modify this line
}

// thhis function checks if the top of the ladder is equal to finalWord
bool isLadderComplete(WordNode* ladder, char* finalWord) {
    //---------------------------------------------------------
    // TODO - write isLadderComplete()
    return (ladder != NULL) && (strcmp(ladder->myWord, finalWord) == 0); //modify this line
}

// this makes a copy of the ladder nodes but not the strings and the new nodes point to the same c strings
WordNode* copyLadder(WordNode* ladder) {
    //---------------------------------------------------------
    // TODO - write copyLadder()
    if (!ladder) return NULL;
    WordNode *head = NULL;
    WordNode *tail = NULL;
    for (WordNode* cur = ladder; cur != NULL; cur = cur->next) {
        WordNode* node = (WordNode*)malloc(sizeof(WordNode));
        node->myWord = cur->myWord;
        node->next = NULL;
        if (!head) head = tail = node;
        else {
            tail->next = node;
            tail = node;
        }
    }
    return head; //modify this line
}

// This function frees all nodes in the wordNode list
void freeLadder(WordNode* ladder) {
    //---------------------------------------------------------
    // TODO - write freeLadder()
    while (ladder) {
        WordNode* nxt = ladder->next;
        free(ladder);
        ladder = nxt;
    }

}

//---------------------- ^^^ END OF TASK 3 ^^^ ----------------------


//------------------- \/\/\/ TOP OF TASK 4 \/\/\/ -------------------

/* This function displays an incomplete word ladder and it prints three dots
at the top which indicate that its incomplete, it then prints each word in 
the ladder from top to bottom with two spaces of identation */
void displayIncompleteLadder(WordNode* ladder) {
    // TODO - write displayIncompleteLadder()
    //---------------------------------------------------------
    printf("  ...\n");
    printf("  ...\n");
    printf("  ...\n");

    WordNode* curr = ladder;
    while (curr != NULL) {
        printf("  %s\n", curr->myWord);
        curr = curr->next;
    }
}


/* This function displays a completed word ladder and it prints each word 
from top to bottom with two spaces
 */
void displayCompleteLadder(WordNode* ladder) {
    //---------------------------------------------------------
    // TODO - write displayCompleteLadder()
    //---------------------------------------------------------
    WordNode* curr = ladder;

    while (curr != NULL) {
        printf("  %s\n", curr->myWord);

        if (curr->next != NULL) {
            int index = strCmpInd(curr->myWord, curr->next->myWord);

            printf("  ");
            for (int i = 0; i < index; ++i) {
                printf(" ");
            }
            printf("^\n");
        }

        curr = curr->next;
    }

}

//---------------------- ^^^ END OF TASK 4 ^^^ ----------------------


//------------------- \/\/\/ TOP OF TASK 5 \/\/\/ -------------------

// this function appends a ladder to the ends of the ladders queue list
void insertLadderAtBack(LadderNode** list, WordNode* newLadder) {
    //---------------------------------------------------------
    // TODO - write insertLadderAtBack()
    //---------------------------------------------------------
    LadderNode* node = (LadderNode*)malloc(sizeof(LadderNode));
    node->topWord = newLadder;
    node->next = NULL;
    if (*list == NULL) { *list = node; return; }
    LadderNode* cur = *list;
    while (cur->next) cur = cur->next;
    cur->next = node;
}


// pops and returns the first ladder form queue list
WordNode* popLadderFromFront(LadderNode** list) {
    //---------------------------------------------------------
    // TODO - write popLadderFromFront()
    //---------------------------------------------------------
    if (*list == NULL) return NULL;
    LadderNode* front = *list;
    WordNode* ladder = front->topWord;
    *list = front->next; //modify this line
    free(front);
    return ladder; //modify this line
}

// frees the entire queue of ladders and every ladder whoch is stored in it
void freeLadderList(LadderNode* myList) {
    //---------------------------------------------------------
    // TODO - write freeLadderList()
    while (myList) {
        LadderNode* nxt = myList->next;
        freeLadder(myList->topWord);
        free(myList);
        myList = nxt;
    }
}

//---------------------- ^^^ END OF TASK 5 ^^^ ----------------------


//------------------- \/\/\/ TOP OF TASK 6 \/\/\/ -------------------

// use BFS to search over ladders in order to find a minimum height path from the startWord to teh finalWord
WordNode* findShortestWordLadder(   char** words, 
                                    bool* usedWord, 
                                    int numWords, 
                                    int wordSize, 
                                    char* startWord, 
                                    char* finalWord ) {
    //---------------------------------------------------------
    // TODO - write findShortestWordLadder()
    //---------------------------------------------------------
    int startIdx = findWord(words, startWord, 0, numWords - 1);
    int finalIdx = findWord(words, finalWord, 0, numWords - 1);

    if (startIdx < 0 || finalIdx < 0) return NULL;

    // reset used word
    for (int i = 0; i < numWords; ++i) {
        usedWord[i] = false;
    }

    LadderNode* myList = NULL;
    // seed ladder with the start word
    WordNode* seed = NULL;
    insertWordAtFront(&seed, words[startIdx]);
    insertLadderAtBack(&myList, seed);
    usedWord[startIdx] = true;

    //BFS loop over the ladders
    while (myList != NULL) {
        bool tempUsed[numWords];
        for (int i = 0; i < numWords; ++i) tempUsed[i] = false;
        WordNode* myLadder = popLadderFromFront(&myList);
        if (myLadder == NULL) break;
        char* headWord = myLadder->myWord;

        char candidate[64];
        strcpy(candidate, headWord);

        for (int pos = 0; pos < wordSize; ++pos) {
            char original = candidate[pos];
            for (char c = 'a'; c <= 'z'; ++c) {
                if (c == original) continue;
                candidate[pos] = c;

                int idx = findWord(words, candidate, 0, numWords - 1);
                if (idx >= 0 && !usedWord[idx]) {
                    if(strcmp(words[idx], finalWord) == 0) {
                        
                        insertWordAtFront(&myLadder, words[idx]);

                        freeLadderList(myList);
                        return myLadder;
                    }
                    WordNode* another = copyLadder(myLadder);
                    insertWordAtFront(&another, words[idx]);
                    insertLadderAtBack(&myList, another);

                    tempUsed[idx] = true;
                }
            }
            candidate[pos] = original;
        }
        for (int i = 0; i < numWords; ++i) if(tempUsed[i]) usedWord[i] = true;
        freeLadder(myLadder);
    }
     
    return NULL; //modify this line
}

//---------------------- ^^^ END OF TASK 5 ^^^ ----------------------


//------------------- \/\/\/ TOP OF OTHERS \/\/\/ -------------------


// randomly set a word from the dictionary word array
void setWordRand(char** words, int numWords, int wordSize, char* aWord) {
    printf("  Picking a random word for you...\n");
    strcpy(aWord,words[rand()%numWords]);
    printf("  Your word is: %s\n",aWord);
}

// interactive user-input to set a word;
//  ensures the word is in the dictionary word array
void setWord(char** words, int numWords, int wordSize, char* aWord) {
    bool valid = false;
    if (strcmp(aWord,"RAND") != 0) printf("  Enter a %d-letter word (enter RAND for a random word): ", wordSize);
    int count = 0;
    while (!valid) {
        if (strcmp(aWord,"RAND") != 0) scanf("%s",aWord);
        count++;
        valid = (strlen(aWord) == wordSize);
        if (valid) {
            int wordInd = findWord(words, aWord, 0, numWords-1);
            if (wordInd < 0) {
                valid = false;
                printf("    Entered word %s is not in the dictionary.\n",aWord);
                printf("  Enter a %d-letter word (enter RAND for a random word): ", wordSize);
            }
        } else if (strcmp(aWord,"RAND") != 0) {
            printf("    Entered word %s is not a valid %d-letter word.\n",aWord,wordSize);
            printf("  Enter a %d-letter word (enter RAND for a random word): ", wordSize);
        }
        if (!valid && (count >= 5 || strcmp(aWord,"RAND") == 0)) { //too many tries, picking random word
            setWordRand(words, numWords, wordSize, aWord);
            valid = true;
        }
    }
}

// helpful debugging function to print a single Ladder
void printLadder(WordNode* ladder) {
    WordNode* currNode = ladder;
    while (currNode != NULL) {
        printf("\t\t\t%s\n",currNode->myWord);
        currNode = currNode->next;
    }
}

// helpful debugging function to print the entire list of Ladders
void printList(LadderNode* list) {
    printf("\n");
    printf("Printing the full list of ladders:\n");
    LadderNode* currList = list;
    while (currList != NULL) {
        printf("  Printing a ladder:\n");
        printLadder(currList->topWord);
        currList = currList->next;
    }
    printf("\n");
}

//---------------------- ^^^ END OF OTHERS ^^^ ----------------------

/////// TEST CASES ////////
bool test_countWordsOfLength(){
    int result = countWordsOfLength("dictionary.txt", 5);
    if (result > 0) {
        printf("countWordsOfLength PASSED\n");
        return true;
    } else {
        printf("countWordsOfLength FAILED\n");
        return false;
    }
}

bool test_strCmpCnt() {
    int diff = strCmpCnt("cat", "bat");
    if (diff == 1) {
        printf("strCmpCnt PASSED\n");
        return true;
    } else {
        printf("strCmpCnt FAILED\n");
        return false;
    }
}

bool test_checkForValidWord(char** words, int numWords) {
    WordNode* ladder = NULL;
    insertWordAtFront(&ladder, words[0]);

    bool valid = checkForValidWord(words, numWords, strlen(words[0]), ladder, words[1]);

    if (valid) {
        printf("checkForValidWord PASSED\n");
    } else {
        printf("checkForValidWord FAILED\n");
    }

    freeLadder(ladder);
    return valid;
}




//-----------------------------------------------------
// The primary application is mostly fully-develop as
//  provided in main(); changes in main() should be
//  limited to updates made for the game play task(s)
//  and testing-related purposes (such as command-line
//  arguments for "TESTING MODE" to call a test case 
//  master function, or something similar)
//-----------------------------------------------------
int main(int argc, char* argv[]) {
    
    bool testingMode = false;

    printf("\n");
    printf("--------------------------------------------\n");
    printf("Welcome to the CS 211 Word Ladder Generator!\n");
    printf("--------------------------------------------\n\n");
    

    //-------------- \/\/\/ TOP OF PROGRAM SETTINGS \/\/\/ --------------
    //--- COMMAND-LINE ARGUMENTS AND/OR INTERACTIVE USER-INPUT \/\/\/ ---

    
    // default values for program parameters that may be set with
    //  command-line arguments
    int wordSize = -2114430;
    char dict[100] = "notAfile";
    char startWord[30] = "notAword";
    char finalWord[30] = "notValid";
    bool playMode = false;
    
    printf("\nProcessing command-line arguments...\n");

    //-------------------------------------------------------------------
    // command-line arguments:
    //  [-n wordLen] = sets word length for word ladder;
    //                 if wordLen is not a valid input
    //                 (cannot be less than 2 or greater than 20),
    //                 or missing from command-line arguments,
    //                 then let user set it using interactive user input
    // [-d dictFile] = sets dictionary file;
    //                 if dictFile is invalid (file not found) or
    //                 missing from command-line arguments, then let
    //                 user set it using interactive user input
    // [-s startWord] = sets the starting word;
    //                  if startWord is invalid
    //                  (not in dictionary or incorrect length) or
    //                  missing from command-line arguments, then let
    //                  user set it using interactive user input
    // [-f finalWord] = sets the final word;
    //                  if finalWord is invalid
    //                  (not in dictionary or incorrect length) or
    //                  missing from command-line arguments, then let
    //                  user set it using interactive user input
    // [-p playModeSwitch] = turns playMode ON if playModeSwitch is "ON"
    //                       or leaves playMode OFF if playModeSwitch is
    //                       anything else, including "OFF"
    //-------------------------------------------------------------------

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i],"-n") == 0 && argc > i+1) {
            wordSize = atoi(argv[i+1]);
            ++i;
        } else if (strcmp(argv[i],"-d") == 0 && argc > i+1) {
            strcpy(dict, argv[i+1]);
            ++i;
        } else if (strcmp(argv[i],"-s") == 0 && argc > i+1) {
            strcpy(startWord, argv[i+1]);
            ++i;
        } else if (strcmp(argv[i],"-f") == 0 && argc > i+1) {
            strcpy(finalWord, argv[i+1]);
            ++i;
        } else if (strcmp(argv[i],"-p") == 0 && argc > i+1) {
            playMode = (strcmp(argv[i+1],"ON") == 0);
            ++i;
        }
    }
    
    srand((int)time(0));
    
    // set word length using interactive user-input
    //  if wordSize == -2114430, it was NOT set with command-line args
    while (wordSize < 2 || wordSize > 20) {
        if (wordSize != -2114430) printf("Invalid word size for the ladder: %d\n", wordSize);
        printf("Enter the word size for the ladder: ");
        scanf("%d",&wordSize);
        printf("\n");
    }

    printf("This program is a word ladder building game and a solver that\n");
    printf("finds the shortest possible ");
    printf("word ladder between two %d-letter words.\n\n",wordSize);
    
    // interactive user-input to set the dictionary file;
    //  check that file exists; if not, user enters another file
    //  if file exists, count #words of desired length [wordSize];
    //  if dict == "notAfile", it was NOT set with command-line args
    int numWords = countWordsOfLength(dict,wordSize);
    while (numWords < 0) {
        if (strcmp(dict, "notAfile") != 0) {
            printf("  Dictionary %s not found...\n",dict);
        }
        printf("Enter filename for dictionary: ");
        scanf("%s", dict);
        printf("\n");
        numWords = countWordsOfLength(dict,wordSize);
    }
    
    // end program if file does not have at least two words of desired length
    if (numWords < 2) {
        printf("  Dictionary %s contains insufficient %d-letter words...\n",dict,wordSize);
        printf("Terminating program...\n");
        return -1;
    }
    
    // allocate heap memory for the word array; only words with desired length
    char** words = (char**)malloc(numWords*sizeof(char*));
    for (int i = 0; i < numWords; ++i) {
        words[i] = (char*)malloc((wordSize+1)*sizeof(char));
    }
    
    // [usedWord] bool array has same size as word array [words];
    //  all elements initialized to [false];
    //  later, usedWord[i] will be set to [true] whenever
    //      words[i] is added to ANY partial word ladder;
    //      before adding words[i] to another word ladder,
    //      check for previous usage with usedWord[i]
    bool* usedWord = (bool*)malloc(numWords*sizeof(bool));
    for (int i = 0; i < numWords; ++i) {
        usedWord[i] = false;
    }
    
    // build word array (only words with desired length) from dictionary file
    printf("Building array of %d-letter words... ", wordSize);
    bool status = buildWordArray(dict,words,numWords,wordSize);
    if (!status) {
        printf("  ERROR in building word array.\n");
        printf("  File not found or incorrect number of %d-letter words.\n",wordSize);
        printf("Terminating program...\n");
        return -1;
    }
    printf("Done!\n\n");

    // set the two ends of the word ladder using interactive user-input
    //  make sure start and final words are in the word array,
    //  have the correct length (implicit by checking word array), AND
    //  that the two words are not the same
    // start/final words may have already been set using command-line arguments
    // the start/final word can also be set to "RAND" resulting in a random
    //  assignment from any element of the words array
    if (strcmp(startWord,"RAND")==0) {
        printf("Setting the start word randomly...\n");
        setWordRand(words, numWords, wordSize, startWord);
    } else if (findWord(words, startWord,0, numWords-1) < 0 || strlen(startWord) != wordSize) {
        if (strcmp(startWord,"notAword")==0) {
            printf("Setting the start %d-letter word... \n", wordSize);
        } else {
            printf("Invalid start word %s. Resetting the start %d-letter word... \n", startWord, wordSize);
        }
        setWord(words, numWords, wordSize, startWord);
    }
    printf("\n");
    
    if (strcmp(finalWord,"RAND")==0) {
        printf("Setting the final word randomly...\n");
        setWordRand(words, numWords, wordSize, finalWord);
    } else if (findWord(words, finalWord,0, numWords-1) < 0 || strlen(finalWord) != wordSize) {
        if (strcmp(finalWord,"notValid")==0) {
            printf("Setting the final %d-letter word... \n", wordSize);
        } else {
            printf("Invalid final word %s. Resetting the final %d-letter word... \n", finalWord, wordSize);
        }
        setWord(words, numWords, wordSize, finalWord);
    }
    while (strcmp(finalWord,startWord) == 0) {
        printf("  The final word cannot be the same as the start word (%s).\n",startWord);
        printf("Setting the final %d-letter word... \n", wordSize);
        setWord(words, numWords, wordSize, finalWord);
    }
    printf("\n");
    
    //----------------- ^^^ END OF PROGRAM SETTINGS ^^^ -----------------
    
    
    //-------------- \/\/\/ TOP OF GAME PLAY SECTION \/\/\/ --------------
    
    if (!playMode) {
        printf("\n");
        printf("---------------------------------------------\n");
        printf("No Word Ladder Builder Game; Play Mode is OFF\n");
        printf("---------------------------------------------\n");
        printf("\n");
    } else {
        printf("\n");
        printf("-----------------------------------------------\n");
        printf("Welcome to the CS 211 Word Ladder Builder Game!\n");
        printf("-----------------------------------------------\n");
        printf("\n");

        printf("Your goal is to make a word ladder between two ");
        printf("%d-letter words: \n  %s -> %s\n\n",wordSize, startWord,finalWord);
        
        WordNode* userLadder = NULL;
        int ladderHeight = 0; // initially, the ladder is empty
        int startInd = findWord(words, startWord, 0, numWords-1);
        insertWordAtFront(&userLadder, words[startInd]);
        ladderHeight++; // Now, the ladder has a start word
            
        char aWord[30] = "XYZ";
        printf("\n");
        
        // Let the user build a word ladder interactively & iteratively.
        // First, check that ladder is not too long AND not complete.
        //-------------------------------------------------------------------
        // TODO - PART OF Task 4 (GAME PLAY): modify the while loop condition
        //          such that the word ladder building process continues only
        //          if BOTH of the following conditions are met:
        //              1. the user is NOT attempting to stop the word ladder
        //                 building process, which occurs if the entered word
        //                 [aWord] from the last iteration is "DONE"
        //              2. the ladder is still incomplete; i.e. the last word
        //                 added to ladder is not the final word;
        //                 note: this should use a call to isLadderComplete()
        //-------------------------------------------------------------------
        while (strcmp(aWord, "DONE") != 0 && !isLadderComplete(userLadder, finalWord)) {   // modify this line
            printf("The goal is to reach the final word: %s\n",finalWord);
            printf("The ladder is currently: \n");
            displayIncompleteLadder(userLadder);
            printf("Current ladder height: %d\n",ladderHeight);
            printf("Enter the next word (or DONE to stop): ");
            scanf("%s",aWord);
            printf("\n");
            
            // Make sure the entered word is valid for the next ladder rung;
            // if not, repeatedly allow user to enter another word until one is valid
            while (!checkForValidWord(words, numWords, wordSize, userLadder, aWord)) {
                printf("Enter another word (or DONE to stop): ");
                scanf("%s",aWord);
                printf("\n");
            }

            // add the entered word to the ladder (unless it is "DONE")
            if (strcmp(aWord,"DONE") != 0) {
                int currInd = findWord(words, aWord, 0, numWords-1);
                insertWordAtFront(&userLadder, words[currInd]);
                ladderHeight++;
            }
            printf("\n");
        }

        // Check if the built word ladder is complete and
        // display the word ladder appropriately.
        if (isLadderComplete(userLadder, finalWord)) {
            printf("Word Ladder complete!\n\n");
            displayCompleteLadder(userLadder);
            printf("\nWord Ladder height = \n%d\n", ladderHeight);
            printf("Can you find a shorter Word Ladder next time??? \n");
        } else {
            printf("The final Word Ladder is incomplete:\n");
            displayIncompleteLadder(userLadder);
            printf("Word Ladder height = %d\n\n", ladderHeight);
            printf("Can you complete the Word Ladder next time??? \n");
        }
        freeLadder(userLadder);
    }
    
    //----------------- ^^^ END OF GAME PLAY SECTION ^^^ -----------------
    
    
    //-------------- \/\/\/ TOP OF WORD LADDER SOLVER \/\/\/ --------------
    
    printf("\n\n");
    printf("-----------------------------------------\n");
    printf("Welcome to the CS 211 Word Ladder Solver!\n");
    printf("-----------------------------------------\n");
    printf("\n");
    

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // OPTIONAL EXTENTION TO FIND LONGEST WORD LADDER:
    //  program must end with finding the shortest word ladder
    //  (& the associated print statements); if you choose to
    //  extend your program to find the longest word ladder,
    //  put the long word ladder algorithm (& the associated
    //  print statements) BEFORE the short word ladder algorithm
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    
    // run the algorithm to find the shortest word ladder
    WordNode* myLadder = findShortestWordLadder(words, usedWord, numWords, wordSize, startWord, finalWord);

    // display word ladder and its height if one was found
    if (myLadder == NULL) {
        printf("There is no possible word ladder from %s to %s\n",startWord,finalWord);
    } else {
        printf("Shortest Word Ladder found!\n\n");
        displayCompleteLadder(myLadder);
        //printLadder(myLadder);
    }
    printf("\nWord Ladder height = %d\n\n",getLadderHeight(myLadder));

    //----------------- ^^^ END OF WORD LADDER SOLVER ^^^ -----------------
    
    
    //-------------- \/\/\/ TOP OF CLEAN-UP \/\/\/ --------------
    
    // TODO - Part of ALL Tasks:
    //      free all heap-allocated memory to avoid potential
    //      memory leaks. Since the word length for the word
    //      ladder is variable (i.e. set by a command-line
    //      argument or interactive user-input) any array
    //      whose size depends on the word length should be
    //      dynamically heap-allocated, and thus, must be
    //      tracked and freed before program termination.
    //      A big part of the memory management & freeing
    //      is handled by the following functions, but
    //      you may have introduced additional heap-memory
    //      allocations, especially as part of the game play.

    if (testingMode) {
    test_countWordsOfLength();
    test_strCmpCnt();
    test_checkForValidWord(words, numWords); 
    }
   
   
    // free the heap-allocated memory for the shortest ladder
    freeLadder(myLadder);
    // free the heap-allocated memory for the words array
    freeWords(words,numWords);
    free(usedWord);
    
    //----------------- ^^^ END OF CLEAN-UP ^^^ -----------------

    
    return 0;
}
