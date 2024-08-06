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

struct Parcel 
{
    char* destination;
    int weight;
    float value;
    Parcel* left;
    Parcel* right;
};

struct HashTableEntry 
{
    Parcel* root;
};