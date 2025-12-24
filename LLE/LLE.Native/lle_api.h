#pragma once


#ifndef LLE_NATIVE_EXPORT
#define LLE_NATIVE_API __declspec(dllimport)
#else
#define LLE_NATIVE_API __declspec(dllexport)
#endif