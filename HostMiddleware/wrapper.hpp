#ifndef _WRAPPER_HPP_
#define _WRAPPER_HPP_

#ifdef _WIN32
#include <Windows.h>
#define __MODIFIER __declspec(dllexport)
#else
#define __MODIFIER 
#endif

#define EXPORT_FUNC(_type, _name) extern "C" __MODIFIER _type _name

#endif