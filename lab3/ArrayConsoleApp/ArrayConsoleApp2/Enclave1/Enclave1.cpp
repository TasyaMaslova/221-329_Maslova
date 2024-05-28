#include "Enclave1_t.h"

#include "sgx_trts.h"

#include <string.h>
#include <stdio.h>
//#include <corecrt_memcpy_s.h>
//#include <iostream>
//#include <memory.h>
//#include <ios>



const char* SupernaturalData[] = {
    "Dean Winchester Jensen Ackles 1978",
    "Sam Winchester Jared Padalecki 1982",
    "Castiel Misha Collins 1974",
    "Crowley Mark Sheppard 1964",
    "Jack Alexander Calvert 1990",
    "Rowena Ruth Connell 1979",
    "Bobby Singer Jim Beaver 1950",
    "Lucifer Mark Pellegrino 1965",
    "Gabriel Richard Speight Jr. 1970",
    "Charlie Brandbury Felicia Day 1979",
    "Death Julian Richings 1955",
};

//void getDB(char* buffer, size_t len, int i)
//{
//    if (i < 0 || i >= 11) {
//        memset(buffer, 0, len);
//        snprintf(buffer, len, "Only positive numbers from 0 to 11\n");
//        return;
//
//    }
//    else {
//        memcpy_s(buffer, len, SupernaturalData[i], 45);
//        return;
//    }
//}

void getDB(char* buffer, size_t len, int i) {
    if (i < 0 || i >= sizeof(SupernaturalData) / sizeof(char*)) {
        snprintf(buffer, len, "Error: an element with such an index was not found");
    }
    else {
        snprintf(buffer, len, "%s", SupernaturalData [i]);
    }
}