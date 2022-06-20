#include "../Host Password Manager/sources/L0/L0.h"
#include "../Host Password Manager/sources/L1/L1.h"
#include "wrapper.hpp"

EXPORT_FUNC(int, HOST_get_SerialSize)() {
    return L0Communication::Size::SERIAL;
}

EXPORT_FUNC(int, HOST_get_PinSize)() {
    return L1Parameters::Size::PIN;
}
