//  : Declaration of the Cpart

#pragma once
#include "resource.h"       // main symbols

class CBitmapHandler;

#define ID_SLDWORKS_EVENTS 0

//IDL SECTION
// Ipart
[
	object,
	uuid("319430FB-809A-4464-9BAC-1E345B4A243F"),
	dual,	
	helpstring("Ipart Interface"),
	pointer_default(unique)
]
__interface Ipart : IDispatch
{
	[id(3), helpstring("method ToolbarCallback0")] HRESULT ToolbarCallback0();
	[id(4), helpstring("method ToolbarEnable0")] HRESULT ToolbarEnable0([out, retval] long* status);
};


//C++ HEADER SECTION
// Cpart

[
	coclass,
	threading(apartment),
	vi_progid("part.part"),
	progid("part.part.1"),
	version(1.0),
	uuid("4481A85F-5E87-4B1C-9A5E-8E2D32E1BC2A"),
	helpstring("part Class")
]
class ATL_NO_VTABLE Cpart : 
	public Ipart,
	public ISwAddin,
	public IDispEventImpl<ID_SLDWORKS_EVENTS, Cpart, &__uuidof(DSldWorksEvents), &LIBID_SldWorks, ID_SLDWORKS_TLB_MAJOR, ID_SLDWORKS_TLB_MINOR>
{
	typedef IDispEventImpl<ID_SLDWORKS_EVENTS, Cpart, &__uuidof(DSldWorksEvents), &LIBID_SldWorks, ID_SLDWORKS_TLB_MAJOR, ID_SLDWORKS_TLB_MINOR> CSldWorksEvents;

private:
	CComPtr<ISldWorks> iSwApp;
	CComPtr<ICommandManager> iCmdMgr;
	long addinID;
	long toolbarID;
	long m_swMajNum;
	long m_swMinNum;

public:
	Cpart()
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

	//Utility Methods
public:
	CComPtr<ISldWorks> GetSldWorksPtr() { return iSwApp != NULL ? iSwApp : NULL; }
	//These methods will connect and disconnect this addin to the SolidWorks events
	VARIANT_BOOL AttachEventHandlers();
	VARIANT_BOOL DetachEventHandlers();

	int GetSldWorksTlbMajor() {return (m_swMajNum >= ID_SLDWORKS_TLB_MAJOR ) ? m_swMajNum : 0;}
	int GetSldWorksTlbMinor() {return m_swMinNum;}

	
public:
	//UI Creation
	//These methods control the User Interface for this addin
	void AddCommandManager();
	void RemoveCommandManager();
	


	//Event Handlers
	//These are the methods that are called when certain SolidWorks events are fired
	STDMETHOD(OnDocChange)(void);
	STDMETHOD(OnModelDocChange)(void);
	STDMETHOD(OnDocLoad)(BSTR docTitle, BSTR docPath);
	STDMETHOD(OnFileNew)(LPDISPATCH newDoc, long docType, BSTR templateName);
	STDMETHOD(OnFileOpenPostNotify)(BSTR fileName);

	//Event Sinks
	//The SINK_MAP connects the specified SolidWorks event to a specific event handler
	BEGIN_SINK_MAP(Cpart)
		SINK_ENTRY_EX(ID_SLDWORKS_EVENTS, __uuidof(DSldWorksEvents), swAppActiveDocChangeNotify, OnDocChange)
		SINK_ENTRY_EX(ID_SLDWORKS_EVENTS, __uuidof(DSldWorksEvents), swAppDocumentLoadNotify, OnDocLoad)
		SINK_ENTRY_EX(ID_SLDWORKS_EVENTS, __uuidof(DSldWorksEvents), swAppFileNewNotify2, OnFileNew)
		SINK_ENTRY_EX(ID_SLDWORKS_EVENTS, __uuidof(DSldWorksEvents), swAppActiveModelDocChangeNotify, OnModelDocChange)
		SINK_ENTRY_EX(ID_SLDWORKS_EVENTS, __uuidof(DSldWorksEvents), swAppFileOpenPostNotify, OnFileOpenPostNotify)
	END_SINK_MAP( )

	// ISwAddin Methods
	//These are the methods inherited from the ISwAddin interface that 
	//need to be implemented in order to connect the addin to SolidWorks
public:
	STDMETHOD(ConnectToSW)(LPDISPATCH ThisSW, long Cookie, VARIANT_BOOL * IsConnected);
	STDMETHOD(DisconnectFromSW)(VARIANT_BOOL * IsDisconnected);

public:
	//UI Callbacks
	//These are the methds that are called by the addin created User Interface
	STDMETHOD(ToolbarCallback0)(void);
	STDMETHOD(ToolbarEnable0)(long* status);
};

