
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "MovieTheaterLib.h"
#include "QueueLib.h"
#include "BSTLib.h"
#include "ListLib.h"
#include "StackLib.h"



void PrintReceipts(SNODE **StackTop)
{

    if ((*StackTop)==NULL)
    {
        printf("No receipts");
    }
    else
    {
        SNODE *TempPtr = NULL;
        printf("Today's receipts\n");
        TempPtr = *StackTop;

        while (TempPtr != NULL)
        {
            printf("Receipt # %d\n\t%s\n", (*StackTop)->ReceiptNumber, (*StackTop)->MovieTheaterName);
            LNODE *LinkedList = TempPtr->TicketList;
            while (LinkedList!= NULL)
            {
                char Ticket[4];
                ReturnAndFreeLinkedListNode(&((TempPtr)->TicketList), Ticket);
                printf("%s\n", Ticket);
            }
            pop(StackTop);
            TempPtr = *StackTop;
        }
    }
}



BNODE *PickAndDisplayTheater(BNODE *BSTRoot, char MovieTheaterMap[][MAXCOLS], int *MapRow, int *MapCol)
{
    BNODE *MyTheater = NULL;
    char zip[6] = {};
    char MyDims[6] = {};

    char filename[100];
    char *token = NULL;
    int movieBool;

    printf("\n\nPick a theater by entering the zipcode\n\n");

    InOrder(BSTRoot);

    printf("Enter zip");
    scanf("%s", zip);

    MyTheater = SearchForBNODE(BSTRoot, zip);

    if (MyTheater == NULL)
    {
        printf("Theater not found");
    }
    else
    {
        strcpy(filename, MyTheater->FileName);

        strcpy(MyDims, MyTheater->Dimensions);
        token = strtok(MyDims, "x");
        *MapRow = atoi(token);
        token = strtok(NULL, "x");
        *MapCol = atoi(token);

        movieBool = ReadMovieTheaterFile(MovieTheaterMap, filename, *MapRow, *MapCol);

        if (movieBool == 0)
        {
            printf("Unable to print that seat map at this time. Check back later.\n");
        }
        else
        {
            PrintSeatMap(MovieTheaterMap, *MapRow, *MapCol);
        }
    }

    return MyTheater;
}

void ReadFileIntoQueue(FILE *QueueFile, QNODE **QH, QNODE **QT)
{
    char name[100];
    while (fgets(name, sizeof (name)-1, QueueFile))
    {
        int length = strlen(name);
        if (name[length - 1] == '\n')
        {
            name[length - 1] = '\0';
        }
        enQueue(name, QH, QT);
    }
}

void ReadFileIntoBST(FILE *BSTFile, BNODE **BSTnode)
{
    char line[1000];
    char MTN[100];
    char ZC[100];
    char FN[100];
    char DIM[100];
    char *token = NULL;
    while (fgets(line, sizeof(line)-1, BSTFile) != NULL)
    {

        token = strtok(line, "|");
        strcpy(MTN, token);
        token = strtok(NULL, "|");
        strcpy(ZC, token);
        token = strtok(NULL, "|");
        strcpy(FN, token);
        token = strtok(NULL, "|");
        strcpy(DIM, token);

        AddBSTNode(BSTnode, MTN, ZC, FN, DIM);
    }
}

void get_command_line_parameter(char *argv[], char ParamName[], char ParamValue[])
{
    int i = 0;

    while (argv[++i] != NULL)
    {
        if (!strncmp(argv[i], ParamName, strlen(ParamName)))
        {
            strcpy(ParamValue, strchr(argv[i], '=') + 1);
        }
    }
}

int PrintMenu()
{
    int choice = 0;

    printf("\n\n1.	Sell tickets to next customer\n\n");
    printf("2.	See who's in line\n\n");
    printf("3.	See the seat map for a given theater\n\n");
    printf("4.	Print today's receipts\n\n");
    printf("Choice : ");
    scanf("%d", &choice);

    while (choice < 1 || choice > 4)
    {
        printf("Invalid choice.  Please reenter. ");
        scanf("%d", &choice);
    }

    return choice;
}