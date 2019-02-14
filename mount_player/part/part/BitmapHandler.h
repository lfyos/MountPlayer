// BitmapHandler.h : Declaration of the CBitmapHandler

#pragma once
#include "resource.h"       // main symbols
#include <list>

// IBitmapHandler
[
	object,
	uuid("1ADB3D4C-8316-428E-9F89-598E91ECAEE0"),
	dual,	helpstring("IBitmapHandler Interface"),
	pointer_default(unique)
]
__interface IBitmapHandler : IDispatch
{
	[id(1), helpstring("method CreateBitmapFileFromResource")] HRESULT CreateBitmapFileFromResource([in] DWORD resID, [out, retval] BSTR* retval);
	[id(2), helpstring("method Dispose")] HRESULT Dispose();
};

// CBitmapHandler
[
	coclass,
	threading(apartment),
	vi_progid("part.BitmapHandler"),
	progid("part.BitmapHandler.1"),
	version(1.0),
	uuid("2C2A7FEE-CAEA-4C70-AA26-E486DC1B24C9"),
	helpstring("BitmapHandler Class")
]
class ATL_NO_VTABLE CBitmapHandler : 
	public IBitmapHandler
{
public:
	CBitmapHandler()
	{
	}

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}

public:
	STDMETHOD(CreateBitmapFileFromResource)(DWORD resID, BSTR* retval);
	STDMETHOD(Dispose)();

protected:
	CComBSTR CreateUniqueFileName();
	BOOL SaveHBitmapToDisk(LPCTSTR filename, HBITMAP hBmp, HPALETTE hPal);
	BOOL CleanFilesFromDisk();
	
private:
	std::list<CString> createdFiles;
};

