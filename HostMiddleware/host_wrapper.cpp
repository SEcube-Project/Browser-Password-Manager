#include "../Host Password Manager/sources/L0/L0.h"

extern "C" int HOST_get_SerialSize() {
    return L0Communication::Size::SERIAL;
}
