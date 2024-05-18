#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024
struct toDo;
struct action;
struct ifsent;

typedef struct
{
    int actionType;
    // 0 for buy, 1 for sell, 2 for go
    // the following are for buy
    char buyers[10][100];
    int numBuyers;
    char seller[100];
    // the following are for sell
    char sellers[10][100];
    int numSellers;
    char buyer[100];
    // the following are for buy and sell
    char quantity[10][4];
    char item[10][100];
    int numItems;
    // these are for go
    char goers[10][100];
    int numGoers;
    char location[100];
} action;
typedef struct
{
    int ifType;
    // 0 for at, 1 for has
    char location[100];
    char subjects[10][100];
    int numSubjects;
    // these were for at
    int hastype; // 0 for more, 1 for less, 2 for neutral
    char ifsubjects[10][100];
    int ifnumSubjects;
    char ifitems[10][100];
    int ifnumItems;
    char ifquantity[10][4];
    // these were for has
} ifsent;
typedef struct
{
    bool isif;
    // i need to have an array of actions
    action actions[10];
    // i need to have an array of ifs
    ifsent ifs[10];
    int actidx;
    int ifidx;
} toDo;

void tokenizer(char *buffer, char **words, int *numWords)
{
    char *token = strtok(buffer, " ");
    *numWords = 0;
    while (token != NULL)
    {
        strcpy(words[*numWords], token);
        (*numWords)++;
        token = strtok(NULL, " ");
    }
}
int nextKeyWord(char **words, int numWords, int *nxtindex, int *prindex)
{
    for (int i = *prindex; i < numWords; ++i)
    {
        if (strcmp(words[i], "if") == 0)
        {
            *nxtindex = i;
            return 0;
        }
        if (strcmp(words[i], "from") == 0)
        {
            *nxtindex = i;
            return 1;
        }
        if (strcmp(words[i], "has") == 0)
        {
            return 2;
        }
        if (strcmp(words[i], "at") == 0)
        {
            return 3;
        }

        if (strcmp(words[i], "buy") == 0 || strcmp(words[i], "sell") == 0 || strcmp(words[i], "go") == 0)
        {
            return 4;
        }
        if (strcmp(words[i], "to") == 0)
        {
            *nxtindex = i;
            return 6;
        }
    }
    return 5;
}
void sentenceWriter(char **words, int numWords)
{
    for (int i = 0; i < numWords; i++)
    {
        printf("~%s~\n", words[i]);
    }
}
int nextIfChecker(char **words, int numWords, int *previf, int *atIndex, int *hasIndex, int *prindex)
{

    for (int i = *previf; i < numWords; ++i)
    {
        if (strcmp(words[i], "has") == 0)
        {
            *hasIndex = i;
            return 1;
        }
        if (strcmp(words[i], "at") == 0)
        {
            *atIndex = i;
            return 2;
        }
        if (strcmp(words[i], "buy") == 0 || strcmp(words[i], "sell") == 0 || strcmp(words[i], "go") == 0)
        {

            return 3;
        }
    }
    return 90;
}
void atTopl(char **words, int *atIndex, int *previf, int *prindex, toDo todos[], int *toDoIndex)
{
    char iflocation[100];
    char ifsubjects[10][100];
    int ifnumSubjects = 0;

    if (strcmp(words[*previf], "if") == 0)
    {
        (*previf)++;
    }
    strcpy(iflocation, words[*atIndex + 1]);
    for (int i = *previf; i < *atIndex; ++i)
    {
        if (strcmp(words[i], "and") != 0)
        {
            strcpy(ifsubjects[ifnumSubjects], words[i]);
            ifnumSubjects++;
        }
    }
    *prindex = *atIndex + 2;
    *previf = *atIndex + 2;

    int tmpifidx = todos[*toDoIndex].ifidx;
    todos[*toDoIndex].ifs[tmpifidx].ifType = 0;
    strcpy(todos[*toDoIndex].ifs[tmpifidx].location, iflocation);
    todos[*toDoIndex].ifs[tmpifidx].numSubjects = ifnumSubjects;
    for (int i = 0; i < ifnumSubjects; ++i)
    {
        strcpy(todos[*toDoIndex].ifs[tmpifidx].subjects[i], ifsubjects[i]);
    }
    todos[*toDoIndex].ifidx++;

    // printf("Location: %s\n", iflocation);
    // printf("Subjects: ");
    // for (int i = 0; i < ifnumSubjects; ++i)
    // {
    //     printf("%s", ifsubjects[i]);
    //     if (i < ifnumSubjects - 1)
    //     {
    //         printf(", ");
    //     }
    // }
    // printf("\n\n");
}
int hasTopl(char **words, int *hasIndex, int *previf, int *prindex, int numWords, toDo todos[], int *toDoIndex)
{
    char ifsubjects[10][100];
    int ifnumSubjects = 0;
    char ifitems[10][100];
    int ifnumItems = 0;
    char ifquantity[10][4];
    char condition[5] = "";
    if (strcmp(words[*prindex], "if") == 0)
    {
        (*prindex)++;
    }

    int tmpifidx = todos[*toDoIndex].ifidx;
    todos[*toDoIndex].ifs[tmpifidx].ifType = 1;

    while (*prindex < *hasIndex)
    {

        if (strcmp(words[*prindex], "and") != 0)
        {
            strcpy(ifsubjects[ifnumSubjects], words[*prindex]);

            ifnumSubjects++;
            (*prindex)++;
        }
        (*prindex)++;
    }
    



    if (strcmp(words[*prindex], "more") == 0)
    {
        if (*prindex + 1 < numWords && strcmp(words[*prindex + 1], "than") != 0)
        {
            printf("INVALID\n");
            fflush(stdout);
            return -1;
        }

        (*prindex)++;
        (*prindex)++;
        if (*prindex == numWords)
        {
            printf("INVALID\n");
            fflush(stdout);
            return -1;
        }
        strcpy(condition, "more");

        todos[*toDoIndex].ifs[tmpifidx].hastype = 0;
    }

    else if (strcmp(words[*prindex], "less") == 0)
    {

        if (*prindex + 1 < numWords && strcmp(words[*prindex + 1], "than") != 0)
        {
            printf("INVALID\n");
            fflush(stdout);
            return -1;
        }
        (*prindex)++;
        (*prindex)++;
        if (*prindex == numWords)
        {
            printf("INVALID\n");
            fflush(stdout);
            return -1;
        }

        todos[*toDoIndex].ifs[tmpifidx].hastype = 1;
        strcpy(condition, "less");
    }
    else if (!isdigit(words[*prindex][0]))
    {
        //printf("%s\n", words[*prindex]);
        printf("INVALID\n");
        fflush(stdout);
        return -1;
    }
    else
    {
        todos[*toDoIndex].ifs[tmpifidx].hastype = 2;
    }
    while (*prindex < numWords)
    {

        if (isdigit(words[*prindex][0]))
        {

            strcpy(ifquantity[ifnumItems], words[*prindex]);

            (*prindex)++;
        }
        else
        {

            *previf = *prindex;
            break;
        }
        if (*prindex <= numWords)
        {
            strcpy(ifitems[ifnumItems], words[*prindex]);
            (*prindex)++;
            ifnumItems++;
        }
        else
        {
            printf("INVALID\n");
            fflush(stdout);
            return -1;
        }
        if (*prindex == numWords)
        {
            break;
        }
        if (strcmp(words[*prindex], "and") == 0)
        {
            if (*prindex == numWords)
            {
                printf("INVALID\n");
                fflush(stdout);
                return -1;
            }
            (*prindex)++;
        }
    }
    *previf = *prindex;
    ///////////////////////////

    todos[*toDoIndex].ifs[tmpifidx].ifnumSubjects = ifnumSubjects;
    for (int i = 0; i < ifnumSubjects; ++i)
    {
        strcpy(todos[*toDoIndex].ifs[tmpifidx].ifsubjects[i], ifsubjects[i]);
    }
    todos[*toDoIndex].ifs[tmpifidx].ifnumItems = ifnumItems;
    for (int i = 0; i < ifnumItems; ++i)
    {
        strcpy(todos[*toDoIndex].ifs[tmpifidx].ifquantity[i], ifquantity[i]);
        strcpy(todos[*toDoIndex].ifs[tmpifidx].ifitems[i], ifitems[i]);
    }
    todos[*toDoIndex].ifidx++;

    // printf("hasifSubjects: ");
    // for (int i = 0; i < ifnumSubjects; ++i)
    // {
    //     printf("%s", ifsubjects[i]);
    //     if (i < ifnumSubjects - 1)
    //     {
    //         printf(", ");
    //     }
    // }
    // printf("\ncondition: %s", condition);
    // printf("\nhasItems: ");

    // for (int i = 0; i < ifnumItems; ++i)
    // {
    //     printf("%s: ", ifquantity[i]);
    //     printf("%s(s)", ifitems[i]);
    //     if (i < ifnumItems - 1)
    //     {
    //         printf(", ");
    //     }
    // }
    // printf("\n\n");
    return 90;
}
int categorize(char **words, int numWords, int *prindex, toDo todos[], int *toDoIndex)
{

    // other than buy we can have sell, go to actions so we can categorize
    // iterate over words until you see buy or sell or go
    int buyIndex = -1;
    int sellIndex = -1;
    int goIndex = -1;

    for (int p = *prindex; p < numWords; ++p)
    {
        if (strcmp(words[p], "buy") == 0)
        {
            buyIndex = p;
            break;
        }
        if (strcmp(words[p], "sell") == 0)
        {
            sellIndex = p;
            break;
        }
        if (strcmp(words[p], "go") == 0)
        {
            goIndex = p;
            break;
        }
    }
    // the one having non negative index is the action
    if (buyIndex != -1)
    {
        char buyers[10][100];
        int numBuyers = 0;
        char quantity[10][4];
        char item[10][100];
        int numItems = 0;
        char seller[100] = "";

        // this is a buy action
        todos[*toDoIndex].actions[todos[*toDoIndex].actidx].actionType = 0;

        for (int i = *prindex; i < buyIndex; ++i)
        {
            if (strcmp(words[i], "and") != 0)
            {
                strcpy(buyers[numBuyers], words[i]);
                numBuyers++;
            }
        }
        *prindex = buyIndex + 1;
        int nextIndex = -1;
        int key = nextKeyWord(words, numWords, &nextIndex, prindex);
        if (key == 1)
        {
            
            // from var
            int fromIndex = nextIndex;
            //  iterate from buyIndex + 1 to fromIndex - 1
            for (int i = buyIndex + 1; i < fromIndex; ++i)
            {
                if (strcmp(words[i], "and") == 0)
                {
                    continue;
                }

                if (isdigit(words[i][0]))
                {
                    strcpy(quantity[numItems], words[i]);
                }
                else
                {
                    strcpy(item[numItems], words[i]);
                    numItems++;
                }
            }
            // the seller is the word after from
            strcpy(seller, words[fromIndex + 1]);
            *prindex = fromIndex + 2;

            // adding the information to todos list
            int tmpactidx = todos[*toDoIndex].actidx;

            todos[*toDoIndex].actions[tmpactidx].numBuyers = numBuyers;
            for (int i = 0; i < numBuyers; ++i)
            {
                strcpy(todos[*toDoIndex].actions[tmpactidx].buyers[i], buyers[i]);
            }
            todos[*toDoIndex].actions[tmpactidx].numItems = numItems;
            for (int i = 0; i < numItems; ++i)
            {
                strcpy(todos[*toDoIndex].actions[tmpactidx].quantity[i], quantity[i]);
                strcpy(todos[*toDoIndex].actions[tmpactidx].item[i], item[i]);
            }
            strcpy(todos[*toDoIndex].actions[tmpactidx].seller, seller);
            todos[*toDoIndex].actidx++;

            if (*prindex < numWords && strcmp(words[*prindex], "if") == 0)
            {
                int ifIndex = *prindex;

                int previf = ifIndex;
                int atIndex = -1;
                int hasIndex = -1;
                for (int i = ifIndex + 1; i < numWords; ++i)
                {
                    if (strcmp(words[i], "buy") == 0 || strcmp(words[i], "sell") == 0 || strcmp(words[i], "go") == 0)
                    {
                        
                        return -1;
                    }
                    else if (strcmp(words[i], "at") == 0)
                    {
                        atIndex = i;
                        break;
                    }
                    else if (strcmp(words[i], "has") == 0)
                    {
                        hasIndex = i;
                        break;
                    }
                }
                if (atIndex != -1)
                {
                    // find the location and the ones to be questioned if they are in the location
                    // and then check whether there are more conditions
                    // we will have possibly more than 1 subject and only 1 location

                    // now we have the location and the subjects to check
                    // lets see if there are more conditions
                    // since we will do this check more than once we can create a function for this
                    while (*prindex < numWords && previf < numWords)
                    {

                        int now = nextIfChecker(words, numWords, &previf, &atIndex, &hasIndex, prindex);
                        if (now == 3)
                        {
                            // this means that the sentence is done
                            (*toDoIndex)++;
                            return 0;
                        }
                        else if (now == 2)
                        {

                            atTopl(words, &atIndex, &previf, prindex, todos, toDoIndex);
                        }
                        else if (now == 1)
                        {
                            int tmp = hasTopl(words, &hasIndex, &previf, prindex, numWords, todos, toDoIndex);
                            if(tmp == -1){
                                return -1;
                            }
                            ///////////////
                        }
                    }
                }
                else if (hasIndex != -1)
                {
                    if (hasIndex + 2 >= numWords)
                    {
                        
                        return -1;
                    }
                    if (strcmp(words[hasIndex + 1], "more") == 0)
                    {
                        if (strcmp(words[hasIndex + 2], "than") != 0)
                        {
                            
                            return -1;
                        }
                        while (*prindex < numWords && previf < numWords)
                        {
                            int now = nextIfChecker(words, numWords, &previf, &atIndex, &hasIndex, prindex);
                            if (now == 3)
                            {
                                (*toDoIndex)++;
                                // it should return at the end
                                return 0;
                            }
                            else if (now == 2)
                            {
                                atTopl(words, &atIndex, &previf, prindex, todos, toDoIndex);
                            }
                            else if (now == 1)
                            {
                                // it's seen the has index
                                
                                int tmp = hasTopl(words, &hasIndex, &previf, prindex, numWords, todos, toDoIndex);
                                if(tmp == -1){
                                    return -1;
                                }
                            }
                        }
                    }
                    else if (strcmp(words[hasIndex + 1], "less") == 0)
                    {
                        if (strcmp(words[hasIndex + 2], "than") != 0)
                        {
                            
                            return -1;
                        }
                        while (*prindex < numWords && previf < numWords)
                        {
                            int now = nextIfChecker(words, numWords, &previf, &atIndex, &hasIndex, prindex);
                            if (now == 3)
                            {
                                (*toDoIndex)++;
                                // it should return at the end
                                return 0;
                            }
                            else if (now == 2)
                            {

                                atTopl(words, &atIndex, &previf, prindex, todos, toDoIndex);
                            }
                            else if (now == 1)
                            {
                                // it's seen the has index

                                int tmp = hasTopl(words, &hasIndex, &previf, prindex, numWords, todos, toDoIndex);
                            if(tmp == -1){
                                return -1;
                            }
                            }
                        }
                    }
                    else if (isdigit(words[hasIndex + 1][0]))
                    {
                        while (*prindex < numWords && previf < numWords)
                        {
                            int now = nextIfChecker(words, numWords, &previf, &atIndex, &hasIndex, prindex);
                            if (now == 3)
                            {
                                (*toDoIndex)++;
                                // it should return at the end
                                return 0;
                            }
                            else if (now == 2)
                            {

                                atTopl(words, &atIndex, &previf, prindex, todos, toDoIndex);
                            }
                            else if (now == 1)
                            {
                                // it's seen the has index

                               int tmp = hasTopl(words, &hasIndex, &previf, prindex, numWords, todos, toDoIndex);
                            if(tmp == -1){
                                return -1;
                            }
                            }
                        }
                    }
                    else
                    {
                        
                        return -1;
                    }
                }
            }
        }
        else if (key == 0)
        {
            // if var
            int ifIndex = nextIndex;
            for (int i = buyIndex + 1; i < ifIndex; ++i)
            {
                if (strcmp(words[i], "and") == 0)
                {
                    continue;
                }

                if (isdigit(words[i][0]))
                {
                    strcpy(quantity[numItems], words[i]);
                }
                else
                {
                    strcpy(item[numItems], words[i]);
                    numItems++;
                }
            }

            int tmpactidx = todos[*toDoIndex].actidx;

            todos[*toDoIndex].actions[tmpactidx].numBuyers = numBuyers;
            for (int i = 0; i < numBuyers; ++i)
            {
                strcpy(todos[*toDoIndex].actions[tmpactidx].buyers[i], buyers[i]);
            }
            todos[*toDoIndex].actions[tmpactidx].numItems = numItems;
            for (int i = 0; i < numItems; ++i)
            {
                strcpy(todos[*toDoIndex].actions[tmpactidx].quantity[i], quantity[i]);
                strcpy(todos[*toDoIndex].actions[tmpactidx].item[i], item[i]);
            }
            strcpy(todos[*toDoIndex].actions[tmpactidx].seller, "NULL");
            todos[*toDoIndex].actidx++;

            *prindex = ifIndex;
            int previf = ifIndex;
            int atIndex = -1;
            int hasIndex = -1;
            for (int i = ifIndex + 1; i < numWords; ++i)
            {
                if (strcmp(words[i], "buy") == 0 || strcmp(words[i], "sell") == 0 || strcmp(words[i], "go") == 0)
                {
                    
                    return -1;
                }
                else if (strcmp(words[i], "at") == 0)
                {
                    atIndex = i;
                    break;
                }
                else if (strcmp(words[i], "has") == 0)
                {
                    hasIndex = i;
                    break;
                }
            }
            if (atIndex != -1)
            {
                // find the location and the ones to be questioned if they are in the location
                // and then check whether there are more conditions
                // we will have possibly more than 1 subject and only 1 location

                // now we have the location and the subjects to check
                // lets see if there are more conditions
                // since we will do this check more than once we can create a function for this
                while (*prindex < numWords && previf < numWords)
                {

                    int now = nextIfChecker(words, numWords, &previf, &atIndex, &hasIndex, prindex);
                    if (now == 3)
                    {
                        (*toDoIndex)++;
                        return 0;
                    }
                    else if (now == 2)
                    {

                        atTopl(words, &atIndex, &previf, prindex, todos, toDoIndex);
                    }
                    else if (now == 1)
                    {
                        int tmp = hasTopl(words, &hasIndex, &previf, prindex, numWords, todos, toDoIndex);
                            if(tmp == -1){
                                return -1;
                            }
                        ///////////////
                    }
                }
            }
            else if (hasIndex != -1)
            {
                if (hasIndex + 2 >= numWords)
                {
                    
                    return -1;
                }
                if (strcmp(words[hasIndex + 1], "more") == 0)
                {
                    if (strcmp(words[hasIndex + 2], "than") != 0)
                    {
                        
                        return  -1;
                    }
                    while (*prindex < numWords && previf < numWords)
                    {
                        int now = nextIfChecker(words, numWords, &previf, &atIndex, &hasIndex, prindex);
                        if (now == 3)
                        {
                            (*toDoIndex)++;
                            // it should return at the end
                            return 0;
                        }
                        else if (now == 2)
                        {
                            atTopl(words, &atIndex, &previf, prindex, todos, toDoIndex);
                        }
                        else if (now == 1)
                        {
                            // it's seen the has index
                            int tmp = hasTopl(words, &hasIndex, &previf, prindex, numWords, todos, toDoIndex);
                            if(tmp == -1){
                                return -1;
                            }
                        }
                    }
                }
                else if (strcmp(words[hasIndex + 1], "less") == 0)
                {
                    if (strcmp(words[hasIndex + 2], "than") != 0)
                    {
                        
                        return -1;
                    }
                    while (*prindex < numWords && previf < numWords)
                    {
                        int now = nextIfChecker(words, numWords, &previf, &atIndex, &hasIndex, prindex);
                        if (now == 3)
                        {
                            (*toDoIndex)++;
                            // it should return at the end
                            return 0;
                        }
                        else if (now == 2)
                        {

                            atTopl(words, &atIndex, &previf, prindex, todos, toDoIndex);
                        }
                        else if (now == 1)
                        {
                            // it's seen the has index

                            int tmp = hasTopl(words, &hasIndex, &previf, prindex, numWords, todos, toDoIndex);
                            if(tmp == -1){
                                return -1;
                            }
                        }
                    }
                }
                else if (isdigit(words[hasIndex + 1][0]))
                {
                    while (*prindex < numWords && previf < numWords)
                    {
                        int now = nextIfChecker(words, numWords, &previf, &atIndex, &hasIndex, prindex);
                        if (now == 3)
                        {
                            (*toDoIndex)++;
                            // it should return at the end
                            return 0;
                        }
                        else if (now == 2)
                        {

                            atTopl(words, &atIndex, &previf, prindex, todos, toDoIndex);
                        }
                        else if (now == 1)
                        {
                            // it's seen the has index

                            int tmp = hasTopl(words, &hasIndex, &previf, prindex, numWords, todos, toDoIndex);
                            if(tmp == -1){
                                return -1;
                            }
                        }
                    }
                }
                else
                {
                    
                    return -1;
                }
            }
        }
        else if (key == 4)
        {
            // başka operasyon var
            *prindex = buyIndex + 1;
            while (*prindex < numWords)
            {
                if (isdigit(words[*prindex][0]))
                {
                    strcpy(quantity[numItems], words[*prindex]);
                    (*prindex)++;
                }
                else
                {
                    break;
                }
                if (*prindex < numWords)
                {
                    strcpy(item[numItems], words[*prindex]);
                    (*prindex)++;
                    numItems++;
                }
                else
                {
                    
                    return -1;
                }
            }
            int tmpactidx = todos[*toDoIndex].actidx;

            todos[*toDoIndex].actions[tmpactidx].numBuyers = numBuyers;
            for (int i = 0; i < numBuyers; ++i)
            {
                strcpy(todos[*toDoIndex].actions[tmpactidx].buyers[i], buyers[i]);
            }
            todos[*toDoIndex].actions[tmpactidx].numItems = numItems;
            for (int i = 0; i < numItems; ++i)
            {
                strcpy(todos[*toDoIndex].actions[tmpactidx].quantity[i], quantity[i]);
                strcpy(todos[*toDoIndex].actions[tmpactidx].item[i], item[i]);
            }
            strcpy(todos[*toDoIndex].actions[tmpactidx].seller, "NULL");
            todos[*toDoIndex].actidx++;
        }
        else if (key == 5)
        {
            *prindex = buyIndex + 1;
            while (*prindex < numWords)
            {
                if (isdigit(words[*prindex][0]))
                {
                    strcpy(quantity[numItems], words[*prindex]);
                    (*prindex)++;
                }
                else
                {
                    
                    return -1;
                }
                if (*prindex < numWords)
                {
                    strcpy(item[numItems], words[*prindex]);
                    (*prindex)++;
                    numItems++;
                }
                else
                {
                    
                    return -1;
                }
                if (*prindex < numWords)
                {
                    if (strcmp(words[*prindex], "and") == 0)
                    {
                        (*prindex)++;
                    }
                    else
                    {
                        
                        return -1;
                    }
                }
            }
            int tmpactidx = todos[*toDoIndex].actidx;

            todos[*toDoIndex].actions[tmpactidx].numBuyers = numBuyers;
            for (int i = 0; i < numBuyers; ++i)
            {
                strcpy(todos[*toDoIndex].actions[tmpactidx].buyers[i], buyers[i]);
            }
            todos[*toDoIndex].actions[tmpactidx].numItems = numItems;
            for (int i = 0; i < numItems; ++i)
            {
                strcpy(todos[*toDoIndex].actions[tmpactidx].quantity[i], quantity[i]);
                strcpy(todos[*toDoIndex].actions[tmpactidx].item[i], item[i]);
            }
            strcpy(todos[*toDoIndex].actions[tmpactidx].seller, "NULL");
            todos[*toDoIndex].actidx++;
        }
        // the seller is the word after from

        // printf("Buyers: ");
        // for (int i = 0; i < numBuyers; ++i)
        // {
        //     printf("%s", buyers[i]);
        //     if (i < numBuyers - 1)
        //     {
        //         printf(", ");
        //     }
        // }
        // printf("\nGoods: ");
        // for (int i = 0; i < numItems; ++i)
        // {
        //     printf("%s: ", quantity[i]);
        //     printf("%s(s)", item[i]);
        //     if (i < numItems - 1)
        //     {
        //         printf(", ");
        //     }
        // }

        // printf("\nSellers: %s", seller);
        // printf("\n\n");
        //*prindex = fromIndex + 2;
        return 0;
    }
    else if (sellIndex != -1)
    {

        char sellers[10][100]; // assume there are at most 10 sellers
        int numSellers = 0;
        char quantity[10][4];
        char item[10][100];
        int numItems = 0;
        char buyer[100] = "";
        
        // this is a sell action
        todos[*toDoIndex].actions[todos[*toDoIndex].actidx].actionType = 1;

        for (int i = *prindex; i < sellIndex; ++i)
        {
            if (strcmp(words[i], "and") != 0)
            {
                strcpy(sellers[numSellers], words[i]);
                numSellers++;
            }
        }
        *prindex = sellIndex + 1;
        int nextIndex = -1;
        int key = nextKeyWord(words, numWords, &nextIndex, prindex);
        if (key == 6)
        {
            
            // to var
            int toIndex = nextIndex;
            //  iterate from sellIndex + 1 to toIndex - 1
            for (int i = sellIndex + 1; i < toIndex; ++i)
            {
                if (strcmp(words[i], "and") == 0)
                {
                    continue;
                }

                if (isdigit(words[i][0]))
                {
                    strcpy(quantity[numItems], words[i]);
                }
                else
                {
                    strcpy(item[numItems], words[i]);
                    numItems++;
                }
            }
            // the buyer is the word after to
            strcpy(buyer, words[toIndex + 1]);
            *prindex = toIndex + 2;
            int tmpactidx = todos[*toDoIndex].actidx;

            todos[*toDoIndex].actions[tmpactidx].numSellers = numSellers;
            for (int i = 0; i < numSellers; ++i)
            {
                strcpy(todos[*toDoIndex].actions[tmpactidx].sellers[i], sellers[i]);
            }
            todos[*toDoIndex].actions[tmpactidx].numItems = numItems;
            for (int i = 0; i < numItems; ++i)
            {
                strcpy(todos[*toDoIndex].actions[tmpactidx].quantity[i], quantity[i]);
                strcpy(todos[*toDoIndex].actions[tmpactidx].item[i], item[i]);
            }
            strcpy(todos[*toDoIndex].actions[tmpactidx].buyer, buyer);
            todos[*toDoIndex].actidx++;

            if (*prindex < numWords && strcmp(words[*prindex], "if") == 0)
            {
                int ifIndex = *prindex;

                int previf = ifIndex;
                int atIndex = -1;
                int hasIndex = -1;
                for (int i = ifIndex + 1; i < numWords; ++i)
                {
                    if (strcmp(words[i], "buy") == 0 || strcmp(words[i], "sell") == 0 || strcmp(words[i], "go") == 0)
                    {
                        
                        return -1;
                    }
                    else if (strcmp(words[i], "at") == 0)
                    {
                        atIndex = i;
                        break;
                    }
                    else if (strcmp(words[i], "has") == 0)
                    {
                        hasIndex = i;
                        break;
                    }
                }
                if (atIndex != -1)
                {
                    // find the location and the ones to be questioned if they are in the location
                    // and then check whether there are more conditions
                    // we will have possibly more than 1 subject and only 1 location

                    // now we have the location and the subjects to check
                    // lets see if there are more conditions
                    // since we will do this check more than once we can create a function for this
                    while (*prindex < numWords && previf < numWords)
                    {

                        int now = nextIfChecker(words, numWords, &previf, &atIndex, &hasIndex, prindex);
                        if (now == 3)
                        {
                            (*toDoIndex)++;
                            return 0;
                        }
                        else if (now == 2)
                        {

                            atTopl(words, &atIndex, &previf, prindex, todos, toDoIndex);
                        }
                        else if (now == 1)
                        {
                            int tmp = hasTopl(words, &hasIndex, &previf, prindex, numWords, todos, toDoIndex);
                            if(tmp == -1){
                                return -1;
                            }
                            ///////////////
                        }
                    }
                }
                else if (hasIndex != -1)
                {
                    if (hasIndex + 2 >= numWords)
                    {
                        
                        return -1;
                    }
                    if (strcmp(words[hasIndex + 1], "more") == 0)
                    {
                        if (strcmp(words[hasIndex + 2], "than") != 0)
                        {
                            
                            return -1;
                        }
                        while (*prindex < numWords && previf < numWords)
                        {
                            int now = nextIfChecker(words, numWords, &previf, &atIndex, &hasIndex, prindex);
                            if (now == 3)
                            {
                                (*toDoIndex)++;
                                // it should return at the end
                                return 0;
                            }
                            else if (now == 2)
                            {
                                atTopl(words, &atIndex, &previf, prindex, todos, toDoIndex);
                            }
                            else if (now == 1)
                            {
                                // it's seen the has index
                                int tmp = hasTopl(words, &hasIndex, &previf, prindex, numWords, todos, toDoIndex);
                            if(tmp == -1){
                                return -1;
                            }
                            }
                        }
                    }
                    else if (strcmp(words[hasIndex + 1], "less") == 0)
                    {
                        if (strcmp(words[hasIndex + 2], "than") != 0)
                        {
                            
                            return -1;
                        }
                        while (*prindex < numWords && previf < numWords)
                        {
                            int now = nextIfChecker(words, numWords, &previf, &atIndex, &hasIndex, prindex);
                            if (now == 3)
                            {
                                (*toDoIndex)++;
                                // it should return at the end
                                return 0;
                            }
                            else if (now == 2)
                            {

                                atTopl(words, &atIndex, &previf, prindex, todos, toDoIndex);
                            }
                            else if (now == 1)
                            {
                                // it's seen the has index

                                int tmp = hasTopl(words, &hasIndex, &previf, prindex, numWords, todos, toDoIndex);
                            if(tmp == -1){
                                return -1;
                            }
                            }
                        }
                    }
                    else if (isdigit(words[hasIndex + 1][0]))
                    {
                        while (*prindex < numWords && previf < numWords)
                        {
                            int now = nextIfChecker(words, numWords, &previf, &atIndex, &hasIndex, prindex);
                            if (now == 3)
                            {
                                (*toDoIndex)++;
                                // it should return at the end
                                return 0;
                            }
                            else if (now == 2)
                            {

                                atTopl(words, &atIndex, &previf, prindex, todos, toDoIndex);
                            }
                            else if (now == 1)
                            {
                                // it's seen the has index

                               int tmp = hasTopl(words, &hasIndex, &previf, prindex, numWords, todos, toDoIndex);
                            if(tmp == -1){
                                return -1;
                            }
                            }
                        }
                    }
                    else
                    {
                        
                        return -1;
                    }
                }
            }
        }
        else if (key == 0)
        {
            // if var
            int ifIndex = nextIndex;
            for (int i = sellIndex + 1; i < ifIndex; ++i)
            {
                if (strcmp(words[i], "and") == 0)
                {
                    continue;
                }

                if (isdigit(words[i][0]))
                {
                    strcpy(quantity[numItems], words[i]);
                }
                else
                {
                    strcpy(item[numItems], words[i]);
                    numItems++;
                }
            }

            int tmpactidx = todos[*toDoIndex].actidx;

            todos[*toDoIndex].actions[tmpactidx].numSellers = numSellers;
            for (int i = 0; i < numSellers; ++i)
            {
                strcpy(todos[*toDoIndex].actions[tmpactidx].sellers[i], sellers[i]);
            }
            todos[*toDoIndex].actions[tmpactidx].numItems = numItems;
            for (int i = 0; i < numItems; ++i)
            {
                strcpy(todos[*toDoIndex].actions[tmpactidx].quantity[i], quantity[i]);
                strcpy(todos[*toDoIndex].actions[tmpactidx].item[i], item[i]);
            }
            strcpy(todos[*toDoIndex].actions[tmpactidx].buyer, "NULL");
            todos[*toDoIndex].actidx++;
            
            *prindex = ifIndex;
            int previf = ifIndex;
            int atIndex = -1;
            int hasIndex = -1;
            for (int i = ifIndex + 1; i < numWords; ++i)
            {
                if (strcmp(words[i], "buy") == 0 || strcmp(words[i], "sell") == 0 || strcmp(words[i], "go") == 0)
                {
                    
                    return -1;

                }
                else if (strcmp(words[i], "at") == 0)
                {
                    atIndex = i;
                    break;
                }
                else if (strcmp(words[i], "has") == 0)
                {
                    hasIndex = i;
                    break;
                }
            }
            if (atIndex != -1)
            {
                // find the location and the ones to be questioned if they are in the location
                // and then check whether there are more conditions
                // we will have possibly more than 1 subject and only 1 location

                // now we have the location and the subjects to check
                // lets see if there are more conditions
                // since we will do this check more than once we can create a function for this
                while (*prindex < numWords && previf < numWords)
                {

                    int now = nextIfChecker(words, numWords, &previf, &atIndex, &hasIndex, prindex);
                    if (now == 3)
                    {
                        (*toDoIndex)++;
                        return 0;
                    }
                    else if (now == 2)
                    {

                        atTopl(words, &atIndex, &previf, prindex, todos, toDoIndex);
                    }
                    else if (now == 1)
                    {
                        int tmp = hasTopl(words, &hasIndex, &previf, prindex, numWords, todos, toDoIndex);
                            if(tmp == -1){
                                return -1;
                            }
                        ///////////////
                    }
                }
            }
            else if (hasIndex != -1)
            {
                if (hasIndex + 2 >= numWords)
                {
                    
                    return -1;
                }
                if (strcmp(words[hasIndex + 1], "more") == 0)
                {
                    if (strcmp(words[hasIndex + 2], "than") != 0)
                    {
                        
                        return -1;
                    }
                    while (*prindex < numWords && previf < numWords)
                    {
                        int now = nextIfChecker(words, numWords, &previf, &atIndex, &hasIndex, prindex);
                        if (now == 3)
                        {
                            (*toDoIndex)++;
                            // it should return at the end
                            return 0;
                        }
                        else if (now == 2)
                        {
                            atTopl(words, &atIndex, &previf, prindex, todos, toDoIndex);
                        }
                        else if (now == 1)
                        {
                            // it's seen the has index
                            int tmp = hasTopl(words, &hasIndex, &previf, prindex, numWords, todos, toDoIndex);
                            if(tmp == -1){
                                return -1;
                            }
                        }
                    }
                }
                else if (strcmp(words[hasIndex + 1], "less") == 0)
                {
                    if (strcmp(words[hasIndex + 2], "than") != 0)
                    {
                        
                        return -1;
                    }
                    while (*prindex < numWords && previf < numWords)
                    {
                        int now = nextIfChecker(words, numWords, &previf, &atIndex, &hasIndex, prindex);
                        if (now == 3)
                        {
                            (*toDoIndex)++;
                            // it should return at the end
                            return 0;
                        }
                        else if (now == 2)
                        {

                            atTopl(words, &atIndex, &previf, prindex, todos, toDoIndex);
                        }
                        else if (now == 1)
                        {
                            // it's seen the has index

                            int tmp = hasTopl(words, &hasIndex, &previf, prindex, numWords, todos, toDoIndex);
                            if(tmp == -1){
                                return -1;
                            }
                        }
                    }
                }
                else if (isdigit(words[hasIndex + 1][0]))
                {
                    while (*prindex < numWords && previf < numWords)
                    {
                        int now = nextIfChecker(words, numWords, &previf, &atIndex, &hasIndex, prindex);
                        if (now == 3)
                        {
                            (*toDoIndex)++;
                            // it should return at the end
                            return 0;
                        }
                        else if (now == 2)
                        {

                            atTopl(words, &atIndex, &previf, prindex, todos, toDoIndex);
                        }
                        else if (now == 1)
                        {
                            // it's seen the has index

                            int tmp = hasTopl(words, &hasIndex, &previf, prindex, numWords, todos, toDoIndex);
                            if(tmp == -1){
                                return -1;
                            }
                        }
                    }
                }
                else
                {
                    
                    return -1;
                }
            }
        }
        else if (key == 4)
        {
            // başka operasyon var
            *prindex = sellIndex + 1;
            while (*prindex < numWords)
            {
                if (isdigit(words[*prindex][0]))
                {
                    strcpy(quantity[numItems], words[*prindex]);
                    (*prindex)++;
                }
                else
                {
                    break;
                }
                if (*prindex < numWords)
                {
                    strcpy(item[numItems], words[*prindex]);
                    (*prindex)++;
                    numItems++;
                }
                else
                {
                    
                    return -1;
                }
            }

            int tmpactidx = todos[*toDoIndex].actidx;

            todos[*toDoIndex].actions[tmpactidx].numSellers = numSellers;
            for (int i = 0; i < numSellers; ++i)
            {
                strcpy(todos[*toDoIndex].actions[tmpactidx].sellers[i], sellers[i]);
            }
            todos[*toDoIndex].actions[tmpactidx].numItems = numItems;
            for (int i = 0; i < numItems; ++i)
            {
                strcpy(todos[*toDoIndex].actions[tmpactidx].quantity[i], quantity[i]);
                strcpy(todos[*toDoIndex].actions[tmpactidx].item[i], item[i]);
            }
            strcpy(todos[*toDoIndex].actions[tmpactidx].buyer, "NULL");
            todos[*toDoIndex].actidx++;
        }
        else if (key == 5)
        {
            *prindex = sellIndex + 1;
            while (*prindex < numWords)
            {
                if (isdigit(words[*prindex][0]))
                {
                    strcpy(quantity[numItems], words[*prindex]);
                    (*prindex)++;
                }
                else
                {
                    
                    return -1;
                }
                if (*prindex < numWords)
                {
                    strcpy(item[numItems], words[*prindex]);
                    (*prindex)++;
                    numItems++;
                }
                else
                {
                    
                    return -1;
                }
                if (*prindex < numWords)
                {
                    if (strcmp(words[*prindex], "and") == 0)
                    {
                        (*prindex)++;
                    }
                    else
                    {
                        
                        return -1;
                    }
                }
            }

            int tmpactidx = todos[*toDoIndex].actidx;

            todos[*toDoIndex].actions[tmpactidx].numSellers = numSellers;
            for (int i = 0; i < numSellers; ++i)
            {
                strcpy(todos[*toDoIndex].actions[tmpactidx].sellers[i], sellers[i]);
            }
            todos[*toDoIndex].actions[tmpactidx].numItems = numItems;
            for (int i = 0; i < numItems; ++i)
            {
                strcpy(todos[*toDoIndex].actions[tmpactidx].quantity[i], quantity[i]);
                strcpy(todos[*toDoIndex].actions[tmpactidx].item[i], item[i]);
            }
            strcpy(todos[*toDoIndex].actions[tmpactidx].buyer, "NULL");
            todos[*toDoIndex].actidx++;

        }
        // the seller is the word after to
        // printf("Sellers: ");
        // for (int i = 0; i < numSellers; ++i)
        // {
        //     printf("%s", sellers[i]);
        //     if (i < numSellers - 1)
        //     {
        //         printf(", ");
        //     }
        // }
        // printf("\nGoods: ");
        // for (int i = 0; i < numItems; ++i)
        // {
        //     printf("%s: ", quantity[i]);
        //     printf("%s(s)", item[i]);
        //     if (i < numItems - 1)
        //     {
        //         printf(", ");
        //     }
        // }
        // printf("\nBuyer: %s", buyer);
        // printf("\n\n");
        //*prindex = toIndex + 2;
        return 0;
    }
    else if (goIndex != -1)
    {
        
        char goers[10][100]; // assume there are at most 10 goers
        int numGoers = 0;
        char location[100];
        // this is a go action
        todos[*toDoIndex].actions[todos[*toDoIndex].actidx].actionType = 2;

        for (int i = *prindex; i < goIndex; ++i)
        {
            if (strcmp(words[i], "and") != 0)
            {
                strcpy(goers[numGoers], words[i]);
                numGoers++;
            }
        }
        // the location is the second word after go
        strcpy(location, words[goIndex + 2]);
        *prindex = goIndex + 3;
        // check if there is an if statement

        int tmpactidx = todos[*toDoIndex].actidx;
        todos[*toDoIndex].actions[tmpactidx].numGoers = numGoers;
        for (int i = 0; i < numGoers; ++i)
        {
            strcpy(todos[*toDoIndex].actions[tmpactidx].goers[i], goers[i]);
        }
        strcpy(todos[*toDoIndex].actions[tmpactidx].location, location);
        todos[*toDoIndex].actidx++;

        int ifIndex = -1;
        if (goIndex + 3 < numWords && strcmp(words[goIndex + 3], "if") == 0)
        {
            ifIndex = goIndex + 3;
            int previf = goIndex + 3;
            int atIndex = -1;
            int hasIndex = -1;
            for (int i = goIndex + 4; i < numWords; ++i)
            {
                if (strcmp(words[i], "buy") == 0 || strcmp(words[i], "sell") == 0 || strcmp(words[i], "go") == 0)
                {
                    
                    return -1;
                }
                else if (strcmp(words[i], "at") == 0)
                {
                    atIndex = i;
                    break;
                }
                else if (strcmp(words[i], "has") == 0)
                {
                    hasIndex = i;
                    break;
                }
            }
            if (atIndex != -1)
            {
                // find the location and the ones to be questioned if they are in the location
                // and then check whether there are more conditions
                // we will have possibly more than 1 subject and only 1 location

                // now we have the location and the subjects to check
                // lets see if there are more conditions
                // since we will do this check more than once we can create a function for this
                while (*prindex < numWords && previf < numWords)
                {

                    int now = nextIfChecker(words, numWords, &previf, &atIndex, &hasIndex, prindex);
                    if (now == 3)
                    {
                        (*toDoIndex)++;
                        // printf("Goers: ");
                        // for (int i = 0; i < numGoers; ++i)
                        // {
                        //     printf("%s", goers[i]);
                        //     if (i < numGoers - 1)
                        //     {
                        //         printf(", ");
                        //     }
                        // }
                        // printf("\nLocation: %s", location);
                        // printf("\n\n");
                        // return 0;
                    }
                    else if (now == 2)
                    {

                        atTopl(words, &atIndex, &previf, prindex, todos, toDoIndex);
                    }
                    else if (now == 1)
                    {
                        int tmp = hasTopl(words, &hasIndex, &previf, prindex, numWords, todos, toDoIndex);
                            if(tmp == -1){
                                return -1;
                            }
                        ///////////////
                    }
                }
            }
            else if (hasIndex != -1)
            {

                if (hasIndex + 2 >= numWords)
                {
                    
                    return -1;
                }
                if (strcmp(words[hasIndex + 1], "more") == 0)
                {
                    if (strcmp(words[hasIndex + 2], "than") != 0)
                    {
                        
                        return -1;
                    }
                    while (*prindex < numWords && previf < numWords)
                    {
                        int now = nextIfChecker(words, numWords, &previf, &atIndex, &hasIndex, prindex);
                        if (now == 3)
                        {
                            (*toDoIndex)++;
                            // it should return at the end
                            return 0;
                        }
                        else if (now == 2)
                        {

                            atTopl(words, &atIndex, &previf, prindex, todos, toDoIndex);
                        }
                        else if (now == 1)
                        {
                            // it's seen the has index

                            int tmp = hasTopl(words, &hasIndex, &previf, prindex, numWords, todos, toDoIndex);
                            if(tmp == -1){
                                return -1;
                            }
                        }
                    }
                }
                else if (strcmp(words[hasIndex + 1], "less") == 0)
                {
                    if (strcmp(words[hasIndex + 2], "than") != 0)
                    {
                        
                        return -1;
                    }
                    while (*prindex < numWords && previf < numWords)
                    {
                        int now = nextIfChecker(words, numWords, &previf, &atIndex, &hasIndex, prindex);
                        if (now == 3)
                        {
                            (*toDoIndex)++;
                            // it should return at the end
                            return 0;
                        }
                        else if (now == 2)
                        {

                            atTopl(words, &atIndex, &previf, prindex, todos, toDoIndex);
                        }
                        else if (now == 1)
                        {
                            // it's seen the has index

                            int tmp = hasTopl(words, &hasIndex, &previf, prindex, numWords, todos, toDoIndex);
                            if(tmp == -1){
                                return -1;
                            }
                        }
                    }
                }
                else if (isdigit(words[hasIndex + 1][0]))
                {
                    while (*prindex < numWords && previf < numWords)
                    {
                        int now = nextIfChecker(words, numWords, &previf, &atIndex, &hasIndex, prindex);
                        if (now == 3)
                        {
                            (*toDoIndex)++;
                            // it should return at the end
                            return 0;
                        }
                        else if (now == 2)
                        {

                            atTopl(words, &atIndex, &previf, prindex, todos, toDoIndex);
                        }
                        else if (now == 1)
                        {
                            // it's seen the has index

                            int tmp = hasTopl(words, &hasIndex, &previf, prindex, numWords, todos, toDoIndex);
                            if(tmp == -1){
                                return -1;
                            }
                        }
                    }
                }
                else
                {
                    
                    return -1;
                }
            }
        }

        // printf("Goers: ");
        // for (int i = 0; i < numGoers; ++i)
        // {
        //     printf("%s", goers[i]);
        //     if (i < numGoers - 1)
        //     {
        //         printf(", ");
        //     }
        // }
        // printf("\nLocation: %s", location);
        // printf("\n\n");
        // return 0;
    }
    
    return -1;

}
int personIndex(char ****people, char name[100]){
    for(int i = 0; i < 20; i++){
        if(strcmp(people[i][0][0], name) == 0){
            return i;
        }
    }
    return -1;
}
bool hasChecker(ifsent ifer, char ****people){
    
    for(int i = 0; i < ifer.ifnumSubjects; i++){
        bool found = false;
        for(int j = 0; j < 20; j++){
            if(strcmp(people[j][0][0], ifer.ifsubjects[i]) == 0){
                found = true;
                for(int k = 0; k < ifer.ifnumItems; k++){
                    bool foundItem = false;
                    for(int l = 1; l < 20; l++){
                        if(strcmp(people[j][l][0], ifer.ifitems[k]) == 0){
                            foundItem = true;
                            if(atoi(people[j][l][1]) != atoi(ifer.ifquantity[k])){
                                return false;
                            }
                        }
                    }
                    if(!foundItem){
                        if(atoi(ifer.ifquantity[k]) == 0){continue;}
                        else{return false;}
                    }
                }
            }
        }
        if(!found){
            for (int k = 0; k < ifer.ifnumItems; k++){
                if(atoi(ifer.ifquantity[k]) == 0){continue;}
                else{return false;}
            }
        }
    }
    return true;
}
bool hasMoreChecker(ifsent ifer, char ****people){
    //for each person in the ifer.ifsubjects we will find the person in the people array
    //and  for that person we are going to iterate over ifer.ifitems
    //if the item is not found we will return false
    //if the item is found we will check if the quantity is more than the quantity in the people array
    //if it is we will continue
    //if it is not we will return false and break. thats all
    for(int i = 0; i < ifer.ifnumSubjects; i++){
        bool found = false;
        for(int j = 0; j < 20; j++){
            if(strcmp(people[j][0][0], ifer.ifsubjects[i]) == 0){
                found = true;
                for(int k = 0; k < ifer.ifnumItems; k++){
                    bool foundItem = false;
                    for(int l = 1; l < 20; l++){
                        if(strcmp(people[j][l][0], ifer.ifitems[k]) == 0){
                            foundItem = true;
                            if(atoi(people[j][l][1]) <= atoi(ifer.ifquantity[k])){
                                return false;
                            }
                        }
                    }
                    if(!foundItem){
                        return false;
                        
                    }
                }
            }
        }
        if(!found){
            return false;
        }
    }
    return true;
    
}
bool hasLessChecker(ifsent ifer, char ****people){
    //i want to print all the information in the ifer. tahts all

    for(int i = 0; i < ifer.ifnumSubjects; i++){
        bool found = false;
        for(int j = 0; j < 20; j++){
            if(strcmp(people[j][0][0], ifer.ifsubjects[i]) == 0){
                found = true;
                for(int k = 0; k < ifer.ifnumItems; k++){
                    bool foundItem = false;
                    for(int l = 1; l < 20; l++){
                        if(strcmp(people[j][l][0], ifer.ifitems[k]) == 0){
                            foundItem = true;
                            if(atoi(people[j][l][1]) >= atoi(ifer.ifquantity[k])){
                                return false;
                            }
                        }
                    }
                    if(!foundItem){
                        
                        if(atoi(ifer.ifquantity[k]) == 0){return false;}
                        continue;
                    }
                }
            }
        }
        if(!found){
            //if the person is not found we can assume that the person has 0 of each item
            for(int k = 0; k < ifer.ifnumItems; k++){
                if(atoi(ifer.ifquantity[k]) == 0){return false;}
            }
            continue;
        }
    }
    return true;
}
bool atChecker(ifsent ifer, char ****people){
    //for each person in the ifer.subjects we will find the person in the people array
    //and  for that person we are going to check the location which is people[j][0][1]
    //if the location is not the same as the ifer.iflocation we will return false
    //if it is we will continue. thats all
    for(int i = 0; i < ifer.numSubjects; i++){
        bool found = false;
        for(int j = 0; j < 20; j++){
            if(strcmp(people[j][0][0], ifer.subjects[i]) == 0){
                found = true;
                if(strcmp(people[j][0][1], ifer.location) != 0){
                    return false;
                }
            }
        }
        if(!found){
            if(strcmp(ifer.location, "NOWHERE") == 0){
                continue;
            }
            else{
                return false;
            }
        }
    }
    return true;
}


bool buyController(action acti){
    //in this function we will check whether the seller is in the buyers array or not
    //if the seller is NULL we will return true
    //if the seller is not NULL and in the buyers array we will return false
    //if the seller is not NULL and not in the buyers array we will continue, thats all
    if(strcmp(acti.seller, "NULL") == 0){
        return true;
    }
    else{
        for(int i = 0; i < acti.numBuyers; i++){
            if(strcmp(acti.buyers[i], acti.seller) == 0){
                return false;
            }
        }
    }
    return true;
    

}
bool sellController(action acti){
    if(strcmp(acti.buyer, "NULL") == 0){
        return true;
    }
    else{
        for(int i = 0; i < acti.numSellers; i++){
            if(strcmp(acti.sellers[i], acti.buyer) == 0){
                return false;
            }
        }
    }
    return true;
}
bool goController(action acti){
    if (strcmp(acti.location, "NOWHERE") == 0)
    {
        return false;
    }
    return true;
}
int whichindex(char ****people, int person){
    for(int i = 1; i < 20; ++i){
        if(strcmp(people[person][i][0], "") == 0){return i;}
    }
    return -1;
}
void buyer(action acti, char ****people, int *prsidx){
    //we have two possibilities
    //first, the seller is NULL 
    //in this case we will iterate over acti.buyers[] and for all of them ve will iterate over item[][]
    //for each item we will find whether they have that item or not,
    //if they have that item already we will increase the quantity as specified in acti.quantity[][] array
    //else if they do not have that item we will set it to zero and increase as specified in acti.quantitiy[][] array
    //second, the seller is not NULL
    //in this case we will almost do the same thing, but in order to increase the quantity we will check whether the seller
    //has that item more than the specified quantity for each item, thats all
    if(strcmp(acti.seller, "NULL") == 0){
        for(int i = 0; i < acti.numBuyers; ++i){
            //lets find the person in the people array
            int tmpprs = personIndex(people, acti.buyers[i]);
            int tmpit = -1;
            if(tmpprs == -1){
                (*prsidx)++;
                strcpy(people[*prsidx][0][0], acti.buyers[i]);
                tmpprs = *prsidx;
                tmpit = 1;        
            }
            for(int j = 0; j < acti.numItems; ++j){
                bool found = false;
                for(int k = 1; k < 20; ++k){
                    if(strcmp(people[tmpprs][k][0], acti.item[j]) == 0){
                        found = true;
                        sprintf(people[tmpprs][k][1], "%d", atoi(people[tmpprs][k][1]) + atoi(acti.quantity[j]));

                        
                    }
                }
                if(!found){
                    if(tmpit == -1){
                        tmpit = whichindex(people, tmpprs);
                    }
                    strcpy(people[tmpprs][tmpit][0], acti.item[j]);
                    strcpy(people[tmpprs][tmpit][1], acti.quantity[j]);
                    tmpit++;
                }
            }
        }
    }
    else{
        //////////////
        //here we need to check whether the seller has enough amount of specified item first.
        //if not we will not add any item to any buyer
        int tmpsell = personIndex(people, acti.seller);
        if(tmpsell == -1){
            return;
            //meaning that cannot sell any item
        }
        
        for(int i = 0; i < acti.numItems; ++i){
            //for each item we will check whether the seller has enough quantity or not
            int totamount  = acti.numBuyers * atoi(acti.quantity[i]);
            bool found = false; // the item is in the sellers account
            for(int k = 1; k < 20; k++){
                if(strcmp(people[tmpsell][k][0], acti.item[i])==0){
                    found = true;
                    if(atoi(people[tmpsell][k][1]) >= totamount){
                        sprintf(people[tmpsell][k][1], "%d", atoi(people[tmpsell][k][1]) - totamount);

                        //if not, it means that the seller does not have enough quantity
                        //here we need to serve all buyers that item for specified quantity
                        for (int h = 0; h < acti.numBuyers; ++h){
                            int tmpprs = personIndex(people, acti.buyers[h]);
                            if(tmpprs == -1){
                                (*prsidx)++;
                                strcpy(people[*prsidx][0][0], acti.buyers[h]);
                                tmpprs = *prsidx;   
                            }
                            bool find = false;
                            for(int l = 1; l < 20; ++l){
                                if(strcmp(people[tmpprs][l][0], acti.item[i])==0){
                                    find = true;
                                    sprintf(people[tmpprs][l][1], "%d", atoi(people[tmpprs][l][1]) + atoi(acti.quantity[i]));

                                }
                            }
                            if(!find){
                                int tmpit = whichindex(people, tmpprs);
                                strcpy(people[tmpprs][tmpit][0], acti.item[i]);
                                strcpy(people[tmpprs][tmpit][1], acti.quantity[i]);
                            }
                        }
                        break;
                    }
                }

            }
            if(!found){
                continue;//meaning that the seller does not even had any of that item
            }
        }

    }
}

void seller(action acti, char ****people, int *prsidx){
    //here we have two possibilities
    //first, the buyer is NULL
    //in this case we will iterate over acti.items[] and for each item we will iterate over sellers[] array
    //for each seller we will find whether they have that item as much as at least the specified quantity in the quantity[] array
    //if they ALL have enough items the action can be done
    //
    //if the action can be done we will do the iteration once again and decrease the quantity of the item in the sellers account 
    //second, the buyer is not NULL
    //we will do the same thing but this time the buyers account will be increased.
    for(int i = 0; i < acti.numSellers; ++i){
        int tmpsell = personIndex(people, acti.sellers[i]);
        if(tmpsell == -1){
            return;
            //meaning that cannot sell any item
        }
        for(int j = 0; j < acti.numItems; j++){
            bool found = false; // the item is in the sellers account
            for(int k = 1; k < 20; k++){
                if(strcmp(people[tmpsell][k][0], acti.item[j]) == 0){
                    found = true;
                    if(atoi(people[tmpsell][k][1]) >= atoi(acti.quantity[j])){
                        break;
                    }
                    else{
                        return;
                    }
                }
            }
            if(!found){
                return;
            }
        }
    }
    //if we are here, it means that all sellers have enough quantity of the items
    //now we can decrease the quantity of the items in the sellers account
    for(int i = 0; i < acti.numSellers; ++i){
        int tmpsell = personIndex(people, acti.sellers[i]);
        for(int j = 0; j < acti.numItems; ++j){
            for(int k = 1; k < 20; k++){
                if(strcmp(people[tmpsell][k][0], acti.item[j]) == 0){
                    sprintf(people[tmpsell][k][1], "%d", atoi(people[tmpsell][k][1]) - atoi(acti.quantity[j]));

                }
            }
        }
    }

    if(strcmp(acti.buyer, "NULL") != 0){
        //now we can increase the quantity of the items in the buyers account
        //first check whether the buyer is in the people array or not
        //if not add it to the people array
        //then check whether the item is in the buyers account or not
        //if it is increase the quantity
        //if not add it to the buyers account and increase the quantity
       
        int tmpprs = personIndex(people, acti.buyer);
        if(tmpprs == -1){
            (*prsidx)++;
            strcpy(people[*prsidx][0][0], acti.buyer);
            tmpprs = *prsidx;
        }
        for(int j = 0; j < acti.numItems; ++j){
            bool found = false;
            for(int k = 1; k < 20; k++){
                if(strcmp(people[tmpprs][k][0], acti.item[j]) == 0){
                    found = true;
                    sprintf(people[tmpprs][k][1], "%d", atoi(people[tmpprs][k][1]) + acti.numSellers * atoi(acti.quantity[j]));

                }
            }
            if(!found){
                int tmpit = whichindex(people, tmpprs);
                strcpy(people[tmpprs][tmpit][0], acti.item[j]);
                sprintf(people[tmpprs][tmpit][1], "%d", atoi(acti.quantity[j]) * acti.numSellers);
            }
        }
        
    }
    

}

void addToLocArr(char ***locations, char location[100], char personName[100]){
    //add the location to the locations array
    //location array contains lines, in each line there is a location followed by the people in this location
    //if the location is not in the locations array we will add it to the locations array
    //if it is in the locations array we will add the person to the people in that location, thats all
    bool found = false;
    for(int j = 0; j < 20; j++){
        if(strcmp(locations[j][0], location) == 0){
            found = true;
            for(int k = 1; k < 20; k++){
                if(strcmp(locations[j][k], "") == 0){
                    strcpy(locations[j][k], personName);
                    break;
                }
            }
        }
    }
    if(!found){
        for(int j = 0; j < 20; j++){
            if(strcmp(locations[j][0], "") == 0){
                strcpy(locations[j][0], location);
                strcpy(locations[j][1], personName);
                break;
            }
        }
    }
}
void rmvFromLocArr(char ***locations, char location[100], char personName[100]){
    //now we need to remove the person from the previous location
    for(int j = 0; j < 20; j++){
        if(strcmp(locations[j][0], location) == 0){
            for(int k = 1; k < 20; k++){
                if(strcmp(locations[j][k], personName) == 0){
                    strcpy(locations[j][k], "");
                    break;
                }
            }
        }
    }
}
void goer(action acti, char ****people, char ***locations, int *prsidx){
    //for each acti.goers[] we will find the person in the people array
    //if not in the people array we will add it to the people array and set its location to acti.location
    //if it is in the people array and its location is not NOWHERE then we will find the previous location in the locations array
    //and remove the person from that location
    //then we will set the location of the person to the specified location in the acti.location
    //and add the location to the locations array
    
    for(int i = 0; i < acti.numGoers; ++i){
        int tmpprs = personIndex(people, acti.goers[i]);
        if(tmpprs == -1){
            (*prsidx)++;
            strcpy(people[*prsidx][0][0], acti.goers[i]);
            tmpprs = *prsidx;
            strcpy(people[tmpprs][0][1], acti.location);
            addToLocArr(locations, acti.location, acti.goers[i]);
            
        }
        else{
            if (strcmp(people[tmpprs][0][1], "NOWHERE") != 0)
            {
                rmvFromLocArr(locations, people[tmpprs][0][1], acti.goers[i]);
                addToLocArr(locations, acti.location, acti.goers[i]);
                strcpy(people[tmpprs][0][1], acti.location);
            }
            else{
                addToLocArr(locations, acti.location, acti.goers[i]);
                strcpy(people[tmpprs][0][1], acti.location);
            }
        }
    }
        
    
}
int qvalid(char **words, int numWords){
    //we will iterate over the words array
    //we will count the counts of the words "who", "where", "total"
    //if there is more than one "who" or "where" or "total" we will return -1
    //if there is no "who" or "where" or "total" we will return -1
    //if the first word is not "who" or the last word is not "where" or the last word is not "total" OR the second from last word is not "total" we will return -1
    //thats all
    int who = 0;
    int where = 0;
    int total = 0;
    for(int i = 0; i < numWords; i++){
        if(strcmp(words[i], "who") == 0){
            who++;
        }
        else if(strcmp(words[i], "where") == 0){
            where++;
        }
        else if(strcmp(words[i], "total") == 0){
            total++;
        }
    }
    if(who > 1 || where > 1 || total > 1){
        return -1;
    }
    if(who == 0 && where == 0 && total == 0){

        return -1;
    }
    
   
    if(who+where+total > 1){

        return -1;
    }
    if (who == 1){
        if(strcmp(words[0], "who") != 0){
            return -1;
        }
        if(strcmp(words[1], "at") != 0){

            return -1;
        }
        return 1;
    }
    else if(where == 1){
        if (strcmp(words[numWords - 2], "where") != 0 ){
            return -1;
        }
        return 2;
    }
    else if(total == 1){
        if(strcmp(words[numWords - 3], "total") != 0 && strcmp(words[numWords - 2], "total") != 0){
            return -1;
        }
        return 3;
    }
    return 90;
}
int question(char **words, int numWords, char ****people, char ***locations){
    //first check validity
    //if the sentence is not valid we will return -14
    int isvalid = qvalid(words, numWords);
    if(isvalid == -1){
        
        
        return -1;
    }
    if(isvalid == 1){
        //who
        if(numWords != 4){
            
            
            return -1;
        }
        char location[100];
        strcpy(location, words[2]);
        //we will seperate the peoples names with and, if there is no such location we will print "NOBODY", if there is noone at the location we will print nobody
        //and first i want to store the people in an array
        char peopleAtLoc[100][100];
        for(int i = 0; i < 20; i++){
            strcpy(peopleAtLoc[i], "");
        }
        bool locfound = false;
        int foundpers = 0;
        for(int i = 0; i < 20; i++){
            if(strcmp(locations[i][0], location) == 0){
                locfound = true;
                for(int j = 1; j < 20; j++){
                    if(strcmp( locations[i][j], "") != 0){
                        strcpy(peopleAtLoc[foundpers], locations[i][j]);
                        foundpers++;
                    }
                }
            }
        }
        if(!locfound){
            printf("NOBODY\n");
            fflush(stdout);
            return 0;
        }
        if(foundpers == 0){
            printf("NOBODY\n");
            fflush(stdout);
            return 0;
        }
        //now we will print the people at the location seperated by "and"
        for(int i = 0; i < foundpers; i++){
            printf("%s", peopleAtLoc[i]);
            if(i < foundpers - 1){
                printf(" and ");
            }
        } 
        printf("\n");
        fflush(stdout);


    }
    else if(isvalid == 2){
        //where
        if(numWords != 3){
            
            return -1;
        }
        char person[100];
        strcpy(person, words[0]);
        //we will find the location of the person
        //if the person is not in the people array we will print "NOWHERE"
        //if the person is in the people array but its location is "NOWHERE" we will print "NOWHERE"
        //if the person is in the people array and its location is not "NOWHERE" we will print the location
        bool found = false;
        for(int i = 0; i < 20; i++){
            if(strcmp(people[i][0][0], person) == 0){
                found = true;
                if(strcmp(people[i][0][1], "NOWHERE") == 0){
                    printf("NOWHERE\n");
                    fflush(stdout);
                    return 0;
                }
                else{
                    printf("%s\n", people[i][0][1]);
                    fflush(stdout);
                    return 0;
                }
            }
        }
        if(!found){
            printf("NOWHERE\n");
            fflush(stdout);
            return 0;
        }
    }
    else if(isvalid == 3){
        //total
        //we have 2 possibilities, either the third from the last word is total or the second from last word is total
        //if the third from the last word is total then the second from the last is the item we look for
        //in this case we will go over all peoples inventories and count the total amount of the item
        //if the item is not found in any inventory we will print 0
        //else if the second from the last word is total then the first word is the subject we look for
        //in this case we will find the person in the people array
        //if the person is not in the people array we will print "NOTHING"
        //if the person is in the people array we will iterate over the items in the persons inventory
        //and will print out all the items and their quantities
        if(strcmp(words[numWords - 3], "total") == 0){
            char tmppeople[100][100];
            for(int i = 0; i < 20; i++){
                strcpy(tmppeople[i], "");
            }
            int numPeople = 0;
            for(int i = 0; i < numWords - 3; i++){
                if(strcmp(words[i], "and") == 0){
                    if(strcmp(words[i+1], "total") == 0 || strcmp(words[i+1], "and") == 0){
                        
                        return -1;
                    }
                    continue;
                }
                strcpy(tmppeople[numPeople], words[i]);
                numPeople++;
            }
            char item[100];
            strcpy(item, words[numWords - 2]);
            int total = 0;
            //we will find the people in the people array and for each person we will iterate over the items in the inventory
            //if the item is found we will increase the total amount, thats all
            for(int i = 0; i < numPeople; i++){
                bool found = false;
                for(int j = 0; j < 20; j++){
                    if(strcmp(people[j][0][0], tmppeople[i]) == 0){
                        found = true;
                        for(int k = 1; k < 20; k++){
                            if(strcmp(people[j][k][0], item) == 0){
                                total += atoi(people[j][k][1]);
                                break;
                            }
                        }
                    }
                }
                if(!found){
                    continue;
                }
            }
            printf("%d\n", total);
            fflush(stdout);
            return 0;
        }
        else if(strcmp(words[numWords - 2], "total") == 0){
            char person[100];
            int totfound = 0;
            strcpy(person, words[0]);
            bool found = false;
            for(int i = 0; i < 20; i++){
                if(strcmp(people[i][0][0], person) == 0){
                    found = true;
                    for(int j = 1; j < 20; j++){
                        if(strcmp(people[i][j][0], "") == 0){
                            break;
                        }
                        if(atoi(people[i][j][1]) != 0){
                            printf("%s %s", people[i][j][1], people[i][j][0]);fflush(stdout);
                            totfound++;
                            //if there are more items we will print "and"
                            if(strcmp(people[i][j+1][0], "") != 0){
                                printf(" and ");
                                
                            }
                        }

                           
                        
                    }
                    if(totfound == 0){
                        printf("NOTHING");
                        return 0;
                    }
                    printf("\n");
                    fflush(stdout);
                }
            }
            if(!found){
                printf("NOTHING\n");
                fflush(stdout);
                return 0;
            }
        }
    }
    return 0;


}
int manipulate(toDo todos[], int toDoIndex, char ****people, char ***locations, int *prsidx){
    for (int i = 0; i <= toDoIndex; ++i){
        //first, check the validity
        bool isvalid = true;
        for(int j = 0; j < todos[i].actidx; j++){
            if(todos[i].actions[j].actionType == 0){
                //buy
                isvalid = buyController(todos[i].actions[j]);
                if(!isvalid){
                    

                    return -1;
                }
            }
            else if(todos[i].actions[j].actionType == 1){
                //sell
                isvalid = sellController(todos[i].actions[j]);
                if(!isvalid){
                    
                    return -1;
                }
            }
            else if(todos[i].actions[j].actionType == 2){
                //go
                isvalid = goController(todos[i].actions[j]);
                if(!isvalid){
                    
                    return -1;
                }
            }
        }
        //after this for loop, we are sure that the structure is valid
    }
    
    // we will iterate over the toDos
    // for each toDo we will check the if statements first
    // in case there is only one todo and it does not have an if statement we will just execute the action
    // if there is an if statement we will check the conditions
    // for each condition we will pass to a checker function
    // if the checker function returns false we will break and continue to the next toDo
    // if all the conditions are satisfied we will execute the action, thats enough now

    for (int i = 0; i <= toDoIndex; ++i){
        bool act = true;
        if (todos[i].ifidx > 0){
            for (int j = 0; j < todos[i].ifidx; j++){
                if (todos[i].ifs[j].ifType == 0){
                    act = atChecker(todos[i].ifs[j], people);
                    if (!act)
                        break;
                }
                else if (todos[i].ifs[j].ifType == 1)
                {
                    if (todos[i].ifs[j].hastype == 0){ // more
                        act = hasMoreChecker(todos[i].ifs[j], people);
                        if (!act)
                            break;
                    }
                    else if (todos[i].ifs[j].hastype == 1){ // less
                        act = hasLessChecker(todos[i].ifs[j], people);
                        if (!act)
                            break;
                    }
                    else if (todos[i].ifs[j].hastype == 2){ // neutral
                        act = hasChecker(todos[i].ifs[j], people);
                        if (!act)
                            break;
                    }
                }
            }
        }
        //after this for loop if act is still true, then, we can execute the action
        
        if(act){
            //now it is time for manipulations
            for(int j = 0; j < todos[i].actidx; j++){
                //iteration over actions
                if(todos[i].actions[j].actionType == 0){
                    //buy
                    buyer(todos[i].actions[j], people, prsidx);
                }
                else if(todos[i].actions[j].actionType == 1){
                    //sell
                    seller(todos[i].actions[j], people, prsidx);
                }
                else if(todos[i].actions[j].actionType == 2){
                    //go
                    goer(todos[i].actions[j], people, locations, prsidx);
                }

            }
        }
    }

    



    return 0;

}
int main()
{   
	
    // Create a buffer to store the input with fgets
    
    //char buffer[BUFFER_SIZE];
    //char people[20][20][2][50];
    char ****people;

    // Allocate memory for the first dimension
    people = malloc(20 * sizeof(char ***));

    for (int i = 0; i < 20; i++) {
        // Allocate memory for the second dimension
        people[i] = malloc(20 * sizeof(char **));

        for (int j = 0; j < 20; j++) {
            // Allocate memory for the third dimension
            people[i][j] = malloc(2 * sizeof(char *));
            

            for (int k = 0; k < 2; k++) {
                // Allocate memory for the fourth dimension
                people[i][j][k] = malloc(50 * sizeof(char));
                
            }
        }
    }
    for (int i = 0; i < 20; i++)
    {
        strcpy(people[i][0][1], "NOWHERE");
        for(int k = 0; k < 20; ++k){
            strcpy(people[i][k][0], "");
        }
    }
    //char locations[30][20][50];
    int m = 20, n = 20, p = 50;
    char ***locations;
    locations = malloc(m * sizeof(char **));

    for (int i = 0; i < m; ++i) {
        // Allocate memory for n pointers to char
        locations[i] = malloc(n * sizeof(char *));
        for (int j = 0; j < n; ++j) {
            // Allocate memory for p chars
            locations[i][j] = malloc(p * sizeof(char));
        }
    }


    for (int i = 0; i < 20; i++)
    {
        for(int k = 0; k < 20; ++k){
            strcpy(locations[i][k], "");
        }
    }
    int prsidx = 0;
    while (1)
    {
        printf(">> ");
        fflush(stdout);
        char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strcmp(buffer, "exit") == 0)
        {
            break;
        }
        // now we have our sentence in the buffer lets split
        
        //char words[100][100];
        char **words;

        // Allocate memory for 100 pointers to char
        words = malloc(100 * sizeof(char *));

        for (int i = 0; i < 100; i++) {
            // Allocate memory for an array of 100 chars
            words[i] = malloc(100 * sizeof(char));

        }
        int numWords;
        tokenizer(buffer, words, &numWords);
        bool isQuestion = false;
        // to store each to-do, we wiill have a struct
        if(strcmp(words[numWords - 1], "?") == 0){
            isQuestion = true;
        }
        // sentenceWriter(words, numWords);
        int prindex = 0;
        // i will have an array of toDos
        toDo toDos[10];
        // i will pass a pointer to toDos to the function "categorize"
        // each toDo will represent a sentence
        for (int i = 0; i < 10; i++)
        {
            toDos[i].actidx = 0;
            toDos[i].ifidx = 0;
        }
        int toDoIndex = 0;
        bool isvalid = true;
        if(!isQuestion){
            while (prindex < numWords)
            {
                if (numWords - prindex < 3)
                {
                    printf("INVALID\n");
                    isvalid = false;
                    break;
                }

                int temp = categorize(words, numWords, &prindex, toDos, &toDoIndex);
                if (temp == -1)
                {
                    printf("INVALID\n");
                    isvalid = false;
                    break;
                }
            }
            if (isvalid)
            {
                int still = manipulate(toDos, toDoIndex, people, locations, &prsidx);
                if (still != -1)
                {
                    printf("OK\n");
                }
                else
                {
                    printf("INVALID\n");
                }
            }
        }
        else{
            int still = question(words, numWords, people, locations);
            if (still == -1)
            {
                printf("INVALID\n");
            }
        }
        for (int i = 0; i < 100; i++) {
            free(words[i]);
        }
        free(words);
        free(buffer);
        //now i want to give the information of all the locations and people
        // for (int i = 0; i < 5; ++i){
        //     printf("person name: %s and his location: %s ", people[i][0][0], people[i][0][1]);
        //     for(int j = 1; j < 100; ++j){
        //         if(strcmp(people[i][j][0], "") != 0){
        //             printf("-----the material: %s and its quantity: %s\n" , people[i][j][0], people[i][j][1]);
        //         }
        //     }
        //     printf("\n");
        // }
        // printf("\n");
        // for(int i = 0; i < 5 ; i++){
        //     printf("the loc: %s ---> ", locations[i][0]);
        //     for(int k = 1; k < 100; ++k){
        //         if(strcmp(locations[i][k], "") != 0){
        //             printf("%s, ", locations[i][k]);
        //         }

        //     }
        //     printf("\n");
        // }
        //after categorizing the sentences we will manipulate the information accordingly
        //i will create a function for this
        //lets call the function manipulate
        //but we need some arrays to store the information
        //an array of people, an array of locations
        //which is -> char people[100][100][2][100];
        //each line of people will have lines in it having 2 strings, the zeroth line of the person will have the name of the person and the location of the person
        //after that the lines will have items and quantities
        //initially we will set all the locations to NOWHERE

        //manipulate(toDos, toDoIndex, people, locations);
        //if i want to prompt all the information in toDos
    //     for (int i = 0; i <= toDoIndex; i++)
    //     {
    //         printf("To-Do %d\n", i + 1);
    //         for (int j = 0; j < toDos[i].actidx; j++)
    //         {   
                
    //             if (toDos[i].actions[j].actionType == 0)
    //             {
    //                 printf("action is buy\n");
    //                 printf("Buyers: ");
    //                 for (int k = 0; k < toDos[i].actions[j].numBuyers; ++k)
    //                 {
    //                     printf("%s", toDos[i].actions[j].buyers[k]);
    //                     if (k < toDos[i].actions[j].numBuyers - 1)
    //                     {
    //                         printf(", ");
    //                     }
    //                 }
    //                 printf("\nGoods: ");
    //                 for (int k = 0; k < toDos[i].actions[j].numItems; ++k)
    //                 {
    //                     printf("%s: ", toDos[i].actions[j].quantity[k]);
    //                     printf("%s(s)", toDos[i].actions[j].item[k]);
    //                     if (k < toDos[i].actions[j].numItems - 1)
    //                     {
    //                         printf(", ");
    //                     }
    //                 }
    //                 printf("\nSeller: %s", toDos[i].actions[j].seller);
    //                 printf("\n\n");
    //             }
    //             else if (toDos[i].actions[j].actionType == 1)
    //             {
    //                 printf("action is sell\n");
    //                 printf("Sellers: ");
    //                 for (int k = 0; k < toDos[i].actions[j].numSellers; ++k)
    //                 {
    //                     printf("%s", toDos[i].actions[j].sellers[k]);
    //                     if (k < toDos[i].actions[j].numSellers - 1)
    //                     {
    //                         printf(", ");
    //                     }
    //                 }
    //                 printf("\nGoods: ");
    //                 for (int k = 0; k < toDos[i].actions[j].numItems; ++k)
    //                 {
    //                     printf("%s: ", toDos[i].actions[j].quantity[k]);
    //                     printf("%s(s)", toDos[i].actions[j].item[k]);
    //                     if (k < toDos[i].actions[j].numItems - 1)
    //                     {
    //                         printf(", ");
    //                     }
    //                 }
    //                 printf("\nBuyer: %s", toDos[i].actions[j].buyer);
    //                 printf("\n\n");
    //             }
    //             else if (toDos[i].actions[j].actionType == 2)
    //             {
    //                 printf("action is go\n");
    //                 printf("Goers: ");
    //                 for (int k = 0; k < toDos[i].actions[j].numGoers; ++k)
    //                 {
    //                     printf("%s", toDos[i].actions[j].goers[k]);
    //                     if (k < toDos[i].actions[j].numGoers - 1)
    //                     {
    //                         printf(", ");
    //                     }
    //                 }
    //                 printf("\nLocation: %s", toDos[i].actions[j].location);
    //                 printf("\n\n");
    //             }
    //             else
    //             {
    //                 printf("the actual type is %d", toDos[i].actions[j].actionType);
    //             }
                

    //         }
    //         //now we should prompt the if statements if exist
    //         for (int j = 0; j < toDos[i].ifidx; j++)
    //         {
    //             printf("if statement %d\n", j + 1);
    //             if (toDos[i].ifs[j].ifType == 0)
    //             {
    //                 printf("at\n");
    //                 printf("Location: %s\n", toDos[i].ifs[j].location);
    //                 printf("Subjects: ");
    //                 for (int k = 0; k < toDos[i].ifs[j].numSubjects; ++k)
    //                 {
    //                     printf("%s", toDos[i].ifs[j].subjects[k]);
    //                     if (k < toDos[i].ifs[j].numSubjects - 1)
    //                     {
    //                         printf(", ");
    //                     }
    //                 }
    //                 printf("\n\n");
    //             }
    //             else if (toDos[i].ifs[j].ifType == 1)
    //             {
    //                 printf("has\n");
    //                 //we will print subjects and items with their quantities
    //                 //we will not print the location
    //                 printf("Subjects: ");
    //                 for (int k = 0; k < toDos[i].ifs[j].ifnumSubjects; ++k)
    //                 {
    //                     printf("%s", toDos[i].ifs[j].ifsubjects[k]);
    //                     if (k < toDos[i].ifs[j].ifnumSubjects - 1)
    //                     {
    //                         printf(", ");
    //                     }
    //                 }
    //                 printf("\nGoods: ");
    //                 for (int k = 0; k < toDos[i].ifs[j].ifnumItems; ++k)
    //                 {
    //                     printf("%s: ", toDos[i].ifs[j].ifquantity[k]);
    //                     printf("%s(s)", toDos[i].ifs[j].ifitems[k]);
    //                     if (k < toDos[i].ifs[j].ifnumItems - 1)
    //                     {
    //                         printf(", ");
    //                     }
    //                 }
    //                 printf("\n\n");
    //             }
    //         }
    //         printf("statement ended ----------- \n\n\n\n");
    //     }
    }
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            free(locations[i][j]);
        }
        free(locations[i]);
    }
    free(locations);
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            for (int k = 0; k < 2; k++) {
                free(people[i][j][k]);
            }
            free(people[i][j]);
        }
        free(people[i]);
    }
    free(people);
    return 1;
}