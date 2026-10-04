#include <stdio.h>
#include <string.h>
#include "reports.h"
#include "employees.h"
#include "budget.h"
#include "suppliers.h"
#include "assets.h"


/* ------------------------------------------------------------
   Functions that already exist in main.c (we just use them)
   ------------------------------------------------------------ */
int  readInt(const char *prompt, int min, int max);  /* asks for a valid whole number */
void pauseScreen(void);                              /* waits for the user to press ENTER */
void printLine(char c, int n);                       /* prints a character n times */


/* ------------------------------------------------------------
   Data that belongs to the other modules.
   "extern" means: this variable is created in another .c file,
   and we are only borrowing it here.
   ------------------------------------------------------------ */
extern struct Employee employees[];   /* list of employees      */
extern int employeeCount;             /* how many employees     */

extern struct Budget budget;          /* all department budgets */

extern struct Supplier suppliers[];   /* list of suppliers      */
extern int supplierCount;             /* how many suppliers     */

extern struct Asset assets[];         /* list of assets         */
extern int assetCount;                /* how many assets        */


/* ------------------------------------------------------------
   printHeading
   Prints a title with a line of '=' above and below it.
   strlen() gives the length of the title so the lines fit it.
   ------------------------------------------------------------ */
static void printHeading(const char *title)
{
    int lineLength = (int)strlen(title) + 8;

    printf("\n");
    printLine('=', lineLength);
    printf("    %s\n", title);
    printLine('=', lineLength);
}


/* ------------------------------------------------------------
   employeeReport
   Shows: number of employees, average, highest and lowest salary.
   ------------------------------------------------------------ */
void employeeReport(void)
{
    int i;
    double totalSalary;
    double highestSalary;
    double lowestSalary;

    printHeading("EMPLOYEE REPORT");

    /* If nobody has been added yet, there is nothing to report. */
    if (employeeCount <= 0) {
        printf("No employees registered.\n");
        return;
    }

    /* Start with the first employee as both the highest and lowest. */
    totalSalary   = employees[0].salary;
    highestSalary = employees[0].salary;
    lowestSalary  = employees[0].salary;

    /* Go through the remaining employees one by one. */
    for (i = 1; i < employeeCount; i++) {
        totalSalary = totalSalary + employees[i].salary;

        if (employees[i].salary > highestSalary) {
            highestSalary = employees[i].salary;
        }
        if (employees[i].salary < lowestSalary) {
            lowestSalary = employees[i].salary;
        }
    }

    printf("Total Employees : %d\n", employeeCount);
    printf("Average Salary  : N$%.2f\n", totalSalary / employeeCount);
    printf("Highest Salary  : N$%.2f\n", highestSalary);
    printf("Lowest Salary   : N$%.2f\n", lowestSalary);
}


/* ------------------------------------------------------------
   budgetReport
   Shows: total allocated budget, total spent, what is left,
   and which departments spent more than they were given.
   ------------------------------------------------------------ */
void budgetReport(void)
{
    int i;
    int overCount = 0;            /* how many departments are over budget */
    double totalAllocated = 0;
    double totalSpent = 0;
    double amountOver;

    printHeading("BUDGET REPORT");

    if (budget.deptCount <= 0) {
        printf("No department budgets captured.\n");
        return;
    }

    /* Add up the budget and spending of every department. */
    for (i = 0; i < budget.deptCount; i++) {
        totalAllocated = totalAllocated + budget.budgetAllowed[i];
        totalSpent     = totalSpent     + budget.budgetSpent[i];
    }

    printf("Total Allocated Budget : N$%.2f\n", totalAllocated);
    printf("Total Expenditure      : N$%.2f\n", totalSpent);
    printf("Remaining Budget       : N$%.2f\n", totalAllocated - totalSpent);

    /* Now list the departments that went over their budget. */
    printf("\nDepartments Exceeding Budget:\n");

    for (i = 0; i < budget.deptCount; i++) {
        if (budget.budgetSpent[i] > budget.budgetAllowed[i]) {
            amountOver = budget.budgetSpent[i] - budget.budgetAllowed[i];
            printf("  - %-20s over by N$%.2f\n", budget.budgetDept[i], amountOver);
            overCount++;
        }
    }

    if (overCount == 0) {
        printf("  None. All departments are within budget.\n");
    }
}


/* ------------------------------------------------------------
   supplierReport
   Shows every registered supplier in a table.
   ------------------------------------------------------------ */
void supplierReport(void)
{
    int i;

    printHeading("SUPPLIER REPORT");

    if (supplierCount <= 0) {
        printf("No suppliers registered.\n");
        return;
    }

    /* Table column titles. %-20s means "left-align in 20 characters". */
    printf("%-5s %-20s %-26s %-14s %-14s\n",
           "ID", "Name", "Email", "Telephone", "Town");
    printLine('-', 82);

    for (i = 0; i < supplierCount; i++) {
        printf("%-5d %-20s %-26s %-14s %-14s\n",
               suppliers[i].id,
               suppliers[i].name,
               suppliers[i].email,
               suppliers[i].phone,
               suppliers[i].town);
    }

    printf("\nTotal Suppliers: %d\n", supplierCount);
}


/* ------------------------------------------------------------
   assetReport
   Shows every registered asset in a table, plus their total value.
   ------------------------------------------------------------ */
void assetReport(void)
{
    int i;
    double totalValue = 0;

    printHeading("ASSET REPORT");

    if (assetCount <= 0) {
        printf("No assets registered.\n");
        return;
    }

    printf("%-5s %-18s %-12s %-14s %-14s %-10s\n",
           "ID", "Name", "Type", "Value (N$)", "Department", "Condition");
    printLine('-', 78);

    for (i = 0; i < assetCount; i++) {
        printf("%-5d %-18s %-12s %-14.2f %-14s %-10s\n",
               assets[i].id,
               assets[i].name,
               assets[i].type,
               assets[i].value,
               assets[i].department,
               assets[i].condition);

        totalValue = totalValue + assets[i].value;
    }

    printf("\nTotal Assets      : %d\n", assetCount);
    printf("Total Asset Value : N$%.2f\n", totalValue);
}


/* ------------------------------------------------------------
   reportsMenu
   The Reports sub-menu. It keeps repeating until the user
   chooses option 5 (Back to Main Menu).
   readInt() already rejects letters and out-of-range numbers,
   so every choice that reaches the switch is between 1 and 5.
   ------------------------------------------------------------ */
void reportsMenu(void)
{
    int choice;

    do {
        printHeading("REPORTS MENU");
        printf("1. Employee Report\n");
        printf("2. Budget Report\n");
        printf("3. Supplier Report\n");
        printf("4. Asset Report\n");
        printf("5. Back to Main Menu\n");

        choice = readInt("Enter your choice: ", 1, 5);

        switch (choice) {
            case 1:
                employeeReport();
                pauseScreen();
                break;
            case 2:
                budgetReport();
                pauseScreen();
                break;
            case 3:
                supplierReport();
                pauseScreen();
                break;
            case 4:
                assetReport();
                pauseScreen();
                break;
            case 5:
                /* nothing to do - the loop ends */
                break;
        }
    } while (choice != 5);
}
