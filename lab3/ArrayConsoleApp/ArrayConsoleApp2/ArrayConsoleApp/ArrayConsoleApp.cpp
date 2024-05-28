
#include <iostream>
#include <stdio.h>
#include <ios>

/* 1. специфичные заголовки для использования анклава */
#include "sgx_urts.h" 
#include "sgx_tseal.h"
#include "Enclave1_u.h" 
#define ENCLAVE_FILE L"Enclave1.signed.dll"


/*const char* SupernaturalData[] = {
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

void getDB(char* buffer, const size_t len, const int i)
{
    if (i < 0 || i >= 11) {
        memset(buffer, 0, len);
        printf_s("Only positive numbers from 0 to 11\n");
        return;
        
    }
    else {
        memcpy_s(buffer, len, SupernaturalData[i], 45);
        return;
    }
}*/

int main()
{

    /* 2 активация анклава*/
    sgx_enclave_id_t eid;
    sgx_status_t ret = SGX_SUCCESS;
    sgx_launch_token_t token = { 0 };
    int updated = 0;

    ret = sgx_create_enclave(ENCLAVE_FILE, SGX_DEBUG_FLAG, &token, &updated, &eid, NULL);
    if (ret != SGX_SUCCESS) {
        printf("App: error %#x, failed to create enclave.\n", ret);
        return -1;
    }


    int required_index = -1;
    char buffer[1024] = { 0 };
    printf_s("Enter the index of the array: ");
    int result = scanf_s("%d", &required_index);
    getDB(eid, buffer, 1024, required_index);
    printf_s("Value: %s\n", buffer);
    std::system("pause");
    return 0;

    /* 3. Выгрузить анклав */
    if (SGX_SUCCESS != sgx_destroy_enclave(eid))
        return -1;

}

