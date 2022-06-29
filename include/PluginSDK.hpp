#pragma once

#ifdef _WIN32
#define PLUGIN_FUNC extern "C" __declspec(dllexport)
#else
#define PLUGIN_FUNC extern "C"
#endif