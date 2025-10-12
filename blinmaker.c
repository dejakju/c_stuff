/*
 * Name:    dejakju@gmail.com
 * Date:    2023-08-03
 * Update:  2023-08-10
 * Desc.:   A 'C' port of the fantastic 'blinmaker' inspired by Boris (https://www.youtube.com/watch?v=FMIZEfjYmtM)
 */

/*
**  An actual recipie which works (for 6-8 pancakes, depending on the thickness):
**
**      150g  Flour
**        3   Eggs
**      125ml Milk
**      125ml Mineral Water
**            A pinch of salt
**            Sugar at will
**
*/

#include <stdio.h>

int main(void)
{

    int eggsAmount = 0;
    int eggsMin = 3;
    int milkAmount = 0;
    int milkMin = 125;
    int flourAmount = 0;
    int flourMin = 150;


    printf("Welcome!\nBlinmaker is starting up...\n\n");

    printf("How many eggs do you have? ");
    scanf("%d", &eggsAmount);

    printf("How much milk (ml) do you have? ");
    scanf("%d", &milkAmount);

    printf("How much flour (g) do you have? ");
    scanf("%d", &flourAmount);

    if (eggsAmount < eggsMin || milkAmount < milkMin || flourAmount < flourMin)
    {
        printf("\nNo blin today :(\n");
        return 0;
    }    

    eggsAmount = eggsAmount / eggsMin;
    milkAmount = milkAmount / milkMin;
    flourAmount = flourAmount / flourMin;

    printf("OK, so you have %d portions of eggs, %d portions of milk and %d portions of flour...\n", eggsAmount, milkAmount, flourAmount);

    int smallest = 0;
    if (eggsAmount <= milkAmount)
    {
        smallest = eggsAmount;
    }
    else if (milkAmount <= flourAmount)
    {
        smallest = milkAmount;
    }
    else if (flourAmount <= eggsAmount)
    {
        smallest = flourAmount;
    }

    printf("So, in order to make %d blins you'll have to prepare:\n\n", (smallest * 4));

    printf("%8d    eggs\n", smallest * eggsMin);
    printf("%8dml  milk\n", smallest * milkMin);
    printf("%8dml  mineral water\n", smallest * milkMin);
    printf("%8dg   flour\n", smallest * flourMin);
    printf("            a pinch of salt\n");
    printf("            sugar at will\n");

    printf("\nBlinmaker is shutting down... and stay cheeki breeki!\n");

    return 0;
}
