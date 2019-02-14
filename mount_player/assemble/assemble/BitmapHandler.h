// BitmapHandler.h : Declaration of the CBitmapHandler

#pragma once
#include "resource.h"       // main symbols
#include <list>

// IBitmapHandler
[
	object,
	uuid("496D7CF8-5F2B-4F06-A0A9-603CC89A3BDA"),
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
	vi_progid("assemble.BitmapHandler"),
	progid("assemble.BitmapHandler.1"),
	version(1.0),
	uuid("F92552A6-D0AA-4DC2-9FCA-BE926BDE1591"),
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

