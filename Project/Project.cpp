/*
FILE          : project.cpp
PROJECT       : Final Project
PROGRAMMER    : Helly Shah (8958841)
                Jasmi Devjibhai Vekariya (8960251)
FIRST VERSION : August 6, 2024
DESCRIPTION   : The below program delineates the code which calculates the different statistics about the items to be delivered through courier company.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HASH_TABLE_SIZE 127

typedef struct Parcel
{
    char* destination;
    int weight;
    float value;
    Parcel* left;
    Parcel* right;
} Parcel;

unsigned long djb2(char* str);
void insertTheParcel(Parcel** root, char* destination, int weight, float value);
void displayTheParcel(Parcel* root);
void displayTheParcelsByWeight(Parcel* root, int weight, int higher);
void displayTheTotalLoadAndValuation(Parcel* root, int* totalWeight, float* totalValue);
void displayTheCheapestAndMostExpensive(Parcel* root, Parcel** cheapest, Parcel** expensive);
void displayTheLightestAndHeaviest(Parcel* root, Parcel** lightest, Parcel** heaviest);
void freeTheTree(Parcel* root);

int main(void)
{
    Parcel* hashTable[HASH_TABLE_SIZE] = { NULL };

    FILE* file = fopen("couriers.txt", "r");

    if (!file)
    {
        printf("Error opening file.\n");
        return 1;
    }

    char destination[21];
    int weight;
    float value;

    while (fscanf(file, "%20[^,],%d,%f\n", destination, &weight, &value) == 3)
    {
        unsigned long hashIndex = djb2(destination) % HASH_TABLE_SIZE;
        insertTheParcel(&hashTable[hashIndex], destination, weight, value);
    }

    fclose(file);

    int choice;
    char country[21];
    char input[100];
    int searchWeight;

    while (1)
    {
        printf("Menu:\n");
        printf("1. Enter country name and display all the parcels details\n");
        printf("2. Enter country and weight pair\n");
        printf("3. Display the total parcel load and valuation for the country\n");
        printf("4. Enter the country name and display cheapest and most expensive parcel’s details\n");
        printf("5. Enter the country name and display lightest and heaviest parcel for the country\n");
        printf("6. Exit the application\n");
        printf("Enter your choice: ");
        fgets(input, sizeof(input), stdin);
        choice = atoi(input);

        unsigned long hashIndex = 0;
        switch (choice) {
        case 1:
        {
            printf("Enter country name: ");
            fgets(country, sizeof(country), stdin);
            country[strcspn(country, "\n")] = 0;
            hashIndex = djb2(country) % HASH_TABLE_SIZE;
            displayTheParcel(hashTable[hashIndex]);
            break;
        }
        case 2:
        {
            printf("Enter country name: ");
            fgets(country, sizeof(country), stdin);
            country[strcspn(country, "\n")] = 0;
            printf("Enter weight: ");
            fgets(input, sizeof(input), stdin);
            searchWeight = atoi(input);
            hashIndex = djb2(country) % HASH_TABLE_SIZE;
            displayTheParcelsByWeight(hashTable[hashIndex], searchWeight, 1); // Assuming higher weights
            break;
        }
        case 3:
        {
            printf("Enter country name: ");
            fgets(country, sizeof(country), stdin);
            country[strcspn(country, "\n")] = 0;
            hashIndex = djb2(country) % HASH_TABLE_SIZE;
            int totalWeight = 0;
            float totalValue = 0.0;
            displayTheTotalLoadAndValuation(hashTable[hashIndex], &totalWeight, &totalValue);
            printf("Total weight: %d, Total value: %.2f\n", totalWeight, totalValue);
            break;
        }
        case 4:
        {
            printf("Enter country name: ");
            fgets(country, sizeof(country), stdin);
            country[strcspn(country, "\n")] = 0;
            hashIndex = djb2(country) % HASH_TABLE_SIZE;
            Parcel* cheapest = NULL, * expensive = NULL;
            displayTheCheapestAndMostExpensive(hashTable[hashIndex], &cheapest, &expensive);
            if (cheapest && expensive) {
                printf("Cheapest: %s, %d, %.2f\n", cheapest->destination, cheapest->weight, cheapest->value);
                printf("Most Expensive: %s, %d, %.2f\n", expensive->destination, expensive->weight, expensive->value);
            }
            break;
        }
        case 5:
        {
            printf("Enter country name: ");
            fgets(country, sizeof(country), stdin);
            country[strcspn(country, "\n")] = 0;
            hashIndex = djb2(country) % HASH_TABLE_SIZE;
            Parcel* lightest = NULL, * heaviest = NULL;
            displayTheLightestAndHeaviest(hashTable[hashIndex], &lightest, &heaviest);
            if (lightest && heaviest) {
                printf("Lightest: %s, %d, %.2f\n", lightest->destination, lightest->weight, lightest->value);
                printf("Heaviest: %s, %d, %.2f\n", heaviest->destination, heaviest->weight, heaviest->value);
            }
            break;
        }
        case 6:
            for (int i = 0; i < HASH_TABLE_SIZE; ++i) {
                if (hashTable[i]) {
                    freeTheTree(hashTable[i]);
                }
            }
            return 0;
        default:
            printf("Invalid choice.\n");
            break;
        }
    }

    return 0;
}

unsigned long djb2(char* str) 
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) 
    {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

void insertTheParcel(Parcel** root, char* destination, int weight, float value) 
{
    if (*root == NULL) 
    {
        *root = (Parcel*)malloc(sizeof(Parcel));
        (*root)->destination = strdup(destination);
        (*root)->weight = weight;
        (*root)->value = value;
        (*root)->left = (*root)->right = NULL;
    }

    else if (weight < (*root)->weight) 
    {
        insertTheParcel(&(*root)->left, destination, weight, value);
    }

    else 
    {
        insertTheParcel(&(*root)->right, destination, weight, value);
    }
}

void displayTheParcel(Parcel* root) 
{
    if (root != NULL) 
    {
        displayTheParcel(root->left);
        printf("Destination: %s, Weight: %d, Value: %.2f\n", root->destination, root->weight, root->value);
        displayTheParcel(root->right);
    }
}

void displayTheParcelsByWeight(Parcel* root, int weight, int higher) 
{
    if (root != NULL) 
    {
        displayTheParcelsByWeight(root->left, weight, higher);
        if ((higher && root->weight > weight) || (!higher && root->weight < weight)) 
        {
            printf("Destination: %s, Weight: %d, Value: %.2f\n", root->destination, root->weight, root->value);
        }

        displayTheParcelsByWeight(root->right, weight, higher);
    }
}

void displayTheTotalLoadAndValuation(Parcel* root, int* totalWeight, float* totalValue) 
{
    if (root != NULL) 
    {
        *totalWeight += root->weight;
        *totalValue += root->value;

        displayTheTotalLoadAndValuation(root->left, totalWeight, totalValue);
        displayTheTotalLoadAndValuation(root->right, totalWeight, totalValue);
    }
}

void displayTheCheapestAndMostExpensive(Parcel* root, Parcel** cheapest, Parcel** expensive) 
{
    if (root != NULL) 
    {
        if (*cheapest == NULL || root->value < (*cheapest)->value) 
        {
            *cheapest = root;
        }

        if (*expensive == NULL || root->value > (*expensive)->value) 
        {
            *expensive = root;
        }

        displayTheCheapestAndMostExpensive(root->left, cheapest, expensive);
        displayTheCheapestAndMostExpensive(root->right, cheapest, expensive);
    }
}

void displayTheLightestAndHeaviest(Parcel* root, Parcel** lightest, Parcel** heaviest) 
{
    if (root != NULL) 
    {
        if (*lightest == NULL || root->weight < (*lightest)->weight) 
        {
            *lightest = root;
        }

        if (*heaviest == NULL || root->weight > (*heaviest)->weight) 
        {
            *heaviest = root;
        }

        displayTheLightestAndHeaviest(root->left, lightest, heaviest);
        displayTheLightestAndHeaviest(root->right, lightest, heaviest);
    }
}

void freeTheTree(Parcel* root) 
{
    if (root != NULL) 
    {
        freeTheTree(root->left);
        freeTheTree(root->right);
        free(root->destination);
        free(root);
    }
}