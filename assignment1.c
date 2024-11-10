#include <stdio.h>
void program1();


void main()
{
    char select[50];

    printf("\nChoose a program (Type 1-4)...\n");
    scanf("%s",&select);

    if (select[0] == '1')
    {
        program1();
    }
}

void program1()
{
    int multiplier;
    int limit;

    printf("\nTimes table program.\nEnter the number of the times table that you want...\n");

    scanf("%d", &multiplier); //

    printf("\nHow big do you want the times table to be?\n");
    scanf("%d", &limit);
    // Define array
    int table[limit];
    int i;
    for (i = 0; i > limit; i++)
    {
        table[i] = (multiplier * i ); // Fill our array with values
    }


    int 

}