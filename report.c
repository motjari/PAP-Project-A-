#include <stdio.h>
#include <string.h>
#include "reports.h"
#include "employees.h"
#include "budget.h"
#include "suppliers.h"
#include "asset.h"

int  readInt(const char *prompt, int min, int max);  
void pauseScreen(void);                              
void printLine(char c, int n);                       

extern struct Employee employees[]; 
extern int employeeCount;            

extern struct Budget budget;         

extern struct Supplier suppliers[];  
extern int supplierCount;             

extern struct Asset assets[];         
extern int assetCount;        

static void printHeading(const char *title)
{
    int lineLength = (int)strlen(title) + 8;

    printf("\n");
    printLine('=', lineLength);
    printf("    %s\n", title);
    printLine('=', lineLength);
}

void employeeReport(void)
{
    int i;
    double totalSalary;
    double highestSalary;
    double lowestSalary;

    printHeading("EMPLOYEE REPORT");

    if (employeeCount <= 0) {
        printf("No employees registered.\n");
        return;
    }
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

void supplierReport(void)
{
    int i;

    printHeading("SUPPLIER REPORT");

    if (supplierCount <= 0) {
        printf("No suppliers registered.\n");
        return;
    }
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
