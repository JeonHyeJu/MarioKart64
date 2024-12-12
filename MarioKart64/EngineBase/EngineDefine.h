#pragma once

#define USEDLL

#ifdef USEDLL
#define ENGINEAPI __declspec(dllexport)
#else
#define ENGINEAPI 
#endif