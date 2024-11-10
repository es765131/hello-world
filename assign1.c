#include <stdio.h>
void program1();
void program2();
void main();


void main()
{
    char select[50];

    printf("\nChoose a program (Type 1-2)...\n");
    scanf("%s",&select); // Wait for user input, then set it aside in the select char array.
    switch (select[0])
    {
    case '1': // If the first char is 1, then true.
        program1(); // Execute program 1.
        break;

    case '2':
        program2();
        break;

    default:
        printf("\nUnknown command.\n");
        main();
        break;
    }
    return;

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
    return;
    
    
}

void program2()
{
    float num1;
    float num2;

    printf("\nDivision program.\nEnter a number... (For example, type 20.055)\n");
    scanf("%f", &num1);
    printf("\nEnter another number... (For example, type 1.5)\n");
    scanf("%f", &num2);

    printf("%f divided by %f is:\n", num1,num2);
    printf("%f ", (num1 / num2));
    
    if (num1 > num2)
    {
        printf("\nThis fraction is top-heavy!\n");
    }
    else if (num2 > num1)
    {
        printf("\nThis fraction is bottom-heavy!\n");
    }
    else
    {
        printf("\nThis fraction is equal!\n");
    }
    
    return;
}