#include <new>
#include <iostream>
#include "../Host Password Manager/sources/L0/L0.h"

float cmult(float a, int times) {
    float c;
    
    c = a * (float)times;
    // std::cout << "#CC: a: %.1f, b: %d, c: %.1f" a, times, c << std::endl;

    return c;
}

extern "C" void *createL0Instance() {
    return new(std::nothrow) L0;
}

extern "C" int L0_getDeviceListSize(void *instance) {

    L0 *l0 = reinterpret_cast<L0 *>(instance);

    std::vector<std::pair<std::string, std::string>> devicelist;
    l0->GetDeviceList(devicelist);

    for (auto &&i : devicelist) {
        std::cout << i.first << " " << i.second << std::endl;
    }

    return devicelist.size();
}
