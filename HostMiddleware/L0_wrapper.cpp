#include <new>
#include <iostream>
#include <string>
#include <sstream>
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

    return devicelist.size();
}

extern "C" int L0_get_DevicePathSize(void *instance) {

    L0 *l0 = reinterpret_cast<L0 *>(instance);

    std::vector<std::pair<std::string, std::string>> devicelist;
    l0->GetDeviceList(devicelist);

    size_t maxlen = 0;
    for (auto dev : devicelist) {
        auto ln = dev.first.length();
        maxlen = ln > maxlen ? ln : maxlen;
    }   
    
    return maxlen;
}

extern "C" void L0_getDeviceList(void *instance, char *devicepath, char *serialno, int size) {

    L0 *l0 = reinterpret_cast<L0 *>(instance);
    std::vector<std::pair<std::string, std::string>> devicelist;
    l0->GetDeviceList(devicelist);

    for (int i = 0, ssize = 0; i < size && i < devicelist.size(); i++) {
        std::stringstream ss;
        ss << devicelist[i].first << "\n";
        auto sstr = ss.str();
        sstr.copy(devicepath + ssize, sstr.size());
        ssize += sstr.size();

        ss.str("");
        ss << devicelist[i].second << "\n";
        sstr = ss.str();
        sstr.copy(serialno + i * sstr.size(), sstr.size());
    }

}