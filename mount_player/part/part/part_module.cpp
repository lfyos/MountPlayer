// part.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"

// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{59632D95-D5B9-48B6-A758-9F831B9ACF8A}", 
		 name = "part", 
		 helpstring = "part 1.0 Type Library",
		 resource_name = "IDR_SAMPLE") ];
