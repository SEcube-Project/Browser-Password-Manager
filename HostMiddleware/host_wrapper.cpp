#include "../Host Password Manager/sources/L0/L0.h"
#include "../Host Password Manager/sources/L1/L1.h"

extern "C" int HOST_get_SerialSize() {
    return L0Communication::Size::SERIAL;
}

extern "C" int HOST_get_PinSize() {
    return L1Parameters::Size::PIN;
}
