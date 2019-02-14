// assemble.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"

// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{54B0978C-EA63-4F80-861E-5D43E81ABB7B}", 
		 name = "assemble", 
		 helpstring = "assemble 1.0 Type Library",
		 resource_name = "IDR_SAMPLE") ];
