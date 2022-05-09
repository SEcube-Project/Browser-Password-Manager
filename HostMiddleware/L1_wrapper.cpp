#include <new>
#include <iostream>
#include "../Host Password Manager/sources/L1/L1.h"

extern "C" void *createL1Instance() {
    return new(std::nothrow) L1;
}

extern "C" void L1_SelectSECube_Indx(void *instance, char indx) {
    L1 *l1 = reinterpret_cast<L1 *>(instance);
    l1->L1SelectSEcube(indx);
}
