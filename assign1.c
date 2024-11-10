#include <stdio.h>
void program1();
void main();


void main()
{
    char select[50];

    printf("\nChoose a program (Type 1-4)...\n");
    scanf("%s",&select);

    switch (select[0])
    {
    case '1':
        program1();
        break;

    case '2':
        printf("\nNot yet implemented\n");
        break;

    case '3':
        printf("\nNot yet implemented\n");
        break;

    case '4':
        printf("\nNot yet implemented\n");
        break;

    default:
        printf("\nUnknown command.\n");
        main();
    }

}

void program1()
{
    int limit;

    printf("\nTimes table program.\nEnter the limit for the times table. For example, type 10 to get a table that goes up to 10.\n");

    scanf("%d", &limit); // Recieve number from user, store in limit variable

    // Define array (two dimensional)
    int table[limit][limit];
    int i;
    int j;
    for (i = 1; i < (limit + 1); i++)
    {
        for (j = 1; j < (limit + 1); j++)
        {
            table[i][j] = (j * i); // Fill our array with values using for loops
            // attempting to build an array larger than 17 causes a 'segmentation fault', i'm not sure why it is happening.
        }
        
    }
    
    i = 1;
    j = 1;
    while (i < (limit + 1))
    {
        while (j < (limit + 1))
        {
            printf("%d ", table[i][j]); //printing the array to the screen, using while loops this time
            j++;
        }
        printf("\n");
        i++;
        j = 1;
    }
    
    
}