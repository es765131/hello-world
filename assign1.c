#include <stdio.h>
void program1(); //Define our functions early, so that the compiler knows the other two functions exist when they are called in main().
void program2();
void main();


void main()
{
    char select[50];

    printf("\nChoose a program (Type 1-2)...\n"); // Print information to the console, \n adds a line break.
    scanf("%s",&select); // Wait for user input, then set it aside in the select char array.
    switch (select[0]) //Switch statement is best for this scenario, especially if we plan to add more programs later on.
    {
    case '1': // If the first char is 1, then true.
        program1(); // Execute program 1.
        break; // Break out of the switch case.

    case '2':
        program2();
        break;

    default: // Acts as input validation. If anything other than 1 or 2 is entered as the first character, ask the user again.
        printf("\nUnknown command.\n");
        main(); // This sends us back to the start of main() function.
        break;
    }
    return;

}

void program1()
{
    int limit;

    printf("\nTimes table program.\nEnter the limit for the times table. For example, type 10 to get a table that goes up to 10.\n");

    scanf("%d", &limit); // Recieve number from user, store in limit variable

    // Define int array (two dimensional) and our two iterators.
    int table[limit][limit];
    int i;
    int j;
    for (i = 1; i < (limit + 1); i++)   // For loop, iterates through the rows of our array. 
    {                                   // We add one to the limit so that i is equal to the limit before the loop ends.
                                        // Otherwise, the loop will stop one literation earlier than intended.
        for (j = 1; j < (limit + 1); j++) // Another for loop, nested in the first. This iterates through the columns of our array.
        {
            table[i][j] = (j * i); // Fill our array with values using for loops
            // attempting to build an array larger than 17 causes a 'segmentation fault', i'm not sure why it is happening. A 16x16 array is good enough.
        }
        
    }
    
    i = 1;
    j = 1;
    while (i < (limit + 1))     // Similar to the for loops above, just using a while loop this time.
    {                           // The only real difference is that we must give our iterators a value before the while loop, and must iterate the iterator ourselves.
        while (j < (limit + 1))
        {
            printf("%d ", table[i][j]); //printing the array to the screen, using while loops this time

            j++; // Iterate our iterator.
        }
        printf("\n");
        i++;
        j = 1;
    }
    return;
    
    
}

void program2()
{
    float num1; // Define our floats
    float num2;

    printf("\nDivision program.\nEnter a number... (For example, type 20.055)\n");
    scanf("%f", &num1);
    printf("\nEnter another number... (For example, type 1.5)\n");
    scanf("%f", &num2); // Gather user input. %f is used for float variables.

    printf("%f divided by %f is:\n", num1,num2); // Put this in a nice readable form.
    printf("%f ", (num1 / num2)); // Use the / operator to divide the two numbers. Print the result.
    
    if (num1 > num2) // Is num1 is larger than num2?
    {
        printf("\nThis fraction is top-heavy!\n");
    }
    else if (num2 > num1)// Is num2 is larger than num1?
    {
        printf("\nThis fraction is bottom-heavy!\n");
    }
    else // Must be that the numbers are equal.
    {
        printf("\nThis fraction is equal!\n");
    }
    
    return;
}