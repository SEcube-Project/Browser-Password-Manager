#include <new>
#include <iterator>
#include <iostream>
#include "../Host Password Manager/sources/L1/L1.h"

extern "C" void *createL1Instance() {
    return new(std::nothrow) L1;
}

extern "C" void L1_SelectSECube_Indx(void *instance, char indx) {
    L1 *l1 = reinterpret_cast<L1 *>(instance);
    l1->L1SelectSEcube(indx);
}

extern "C" int L1_Login(void *instance, uint8_t pin[32], uint8_t is_admin, uint8_t do_force) {

    L1 *l1 = reinterpret_cast<L1 *>(instance);

    std::array<uint8_t, L1Parameters::Size::PIN> ppin;
    std::copy(pin, pin + 32, ppin.begin());

    try {
        l1->L1Login(ppin, is_admin ? SE3_ACCESS_ADMIN : SE3_ACCESS_USER, do_force == 1);
    } catch (...) {
        return false;
    }

    return l1->L1GetSessionLoggedIn();
}

extern "C" int L1_Logout(void *instance) {

    L1 *l1 = reinterpret_cast<L1 *>(instance);
    l1->L1Logout();

    return !l1->L1GetSessionLoggedIn();
}
