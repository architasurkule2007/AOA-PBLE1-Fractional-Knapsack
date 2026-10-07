#include <stdio.h>

#define MAX 50

/* Package structure */
typedef struct {
    int    id;
    float  value;
    float  weight;
    float  ratio;      /* value / weight */
    float  taken;      /* fraction of package selected (0 to 1) */
} Package;

Package pkg[MAX];
int n = 0;
float capacity = 0;
int entered = 0, ratioDone = 0, sorted = 0, solved = 0;
float totalValue = 0, totalWeight = 0;

void enterDetails(void) {
    int i;
    printf("Enter number of packages (max %d): ", MAX);
    scanf("%d", &n);
    if (n <= 0 || n > MAX) {
        printf("Invalid number of packages.\n");
        n = 0;
        return;
    }
    for (i = 0; i < n; i++) {
        pkg[i].id = i + 1;
        printf("Package %d - Value: ", i + 1);
        scanf("%f", &pkg[i].value);
        printf("Package %d - Weight: ", i + 1);
        scanf("%f", &pkg[i].weight);
        pkg[i].ratio = 0;
        pkg[i].taken = 0;
    }
    printf("Enter maximum carrying capacity of the vehicle: ");
    scanf("%f", &capacity);
    entered = 1; ratioDone = sorted = solved = 0;
    printf("Package details saved.\n");
}

void displayDetails(void) {
    int i;
    if (!entered) { printf("Please enter package details first (option 1).\n"); return; }
    printf("\n%-10s %-10s %-10s", "Package", "Value", "Weight");
    if (ratioDone) printf(" %-12s", "Value/Weight");
    printf("\n----------------------------------------\n");
    for (i = 0; i < n; i++) {
        printf("%-10d %-10.2f %-10.2f", pkg[i].id, pkg[i].value, pkg[i].weight);
        if (ratioDone) printf(" %-12.2f", pkg[i].ratio);
        printf("\n");
    }
    printf("Vehicle capacity = %.2f\n", capacity);
}

void calculateRatio(void) {
    int i;
    if (!entered) { printf("Please enter package details first (option 1).\n"); return; }
    for (i = 0; i < n; i++)
        pkg[i].ratio = pkg[i].value / pkg[i].weight;
    ratioDone = 1;
    printf("Value/Weight ratio calculated for all packages.\n");
    displayDetails();
}

void sortByRatio(void) {
    int i, j;
    Package temp;
    if (!ratioDone) { printf("Please calculate Value/Weight ratio first (option 3).\n"); return; }
    for (i = 0; i < n - 1; i++)
        for (j = 0; j < n - i - 1; j++)
            if (pkg[j].ratio < pkg[j + 1].ratio) {   /* decreasing order */
                temp = pkg[j]; pkg[j] = pkg[j + 1]; pkg[j + 1] = temp;
            }
    sorted = 1; solved = 0;
    printf("Packages sorted in decreasing order of Value/Weight ratio.\n");
    displayDetails();
}

void findMaxValue(void) {
    int i;
    float remaining;
    if (!sorted) { printf("Please sort the packages first (option 4).\n"); return; }
    remaining = capacity;
    totalValue = 0; totalWeight = 0;
    for (i = 0; i < n; i++) pkg[i].taken = 0;

    for (i = 0; i < n && remaining > 0; i++) {
        if (pkg[i].weight <= remaining) {          /* whole package fits */
            pkg[i].taken = 1.0f;
            remaining   -= pkg[i].weight;
            totalWeight += pkg[i].weight;
            totalValue  += pkg[i].value;
        } else {                                   /* take only a fraction */
            pkg[i].taken = remaining / pkg[i].weight;
            totalValue  += pkg[i].value * pkg[i].taken;
            totalWeight += remaining;
            remaining    = 0;
        }
    }
    solved = 1;
    printf("Maximum value obtained = %.2f\n", totalValue);
}

void displaySelected(void) {
    int i;
    if (!solved) { printf("Please find the maximum value first (option 5).\n"); return; }
    printf("\n%-10s %-10s %-10s %-10s %-12s\n", "Package", "Value", "Weight", "Fraction", "Value taken");
    printf("-----------------------------------------------------------\n");
    for (i = 0; i < n; i++)
        if (pkg[i].taken > 0)
            printf("%-10d %-10.2f %-10.2f %-10.2f %-12.2f\n", pkg[i].id, pkg[i].value,
                   pkg[i].weight, pkg[i].taken, pkg[i].value * pkg[i].taken);
    printf("-----------------------------------------------------------\n");
    printf("Total weight used     : %.2f / %.2f\n", totalWeight, capacity);
    printf("Maximum value obtained: %.2f\n", totalValue);
    printf("Time complexity: O(n log n) with an efficient sort,\n");
    printf("(O(n^2) for the bubble sort used here); selection loop is O(n).\n");
}

int main(void) {
    int choice;
    do {
        printf("\n===== SMART DELIVERY PLANNING - FRACTIONAL KNAPSACK =====\n");
        printf("1. Enter Package Details\n2. Display Package Details\n3. Calculate Value/Weight Ratio\n");
        printf("4. Sort Packages by Ratio\n5. Find Maximum Value\n6. Display Selected Packages\n7. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) break;
        switch (choice) {
            case 1: enterDetails();     break;
            case 2: displayDetails();   break;
            case 3: calculateRatio();   break;
            case 4: sortByRatio();      break;
            case 5: findMaxValue();     break;
            case 6: displaySelected();  break;
            case 7: printf("Exiting program.\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 7);
    return 0;
}
