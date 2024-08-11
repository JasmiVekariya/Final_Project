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