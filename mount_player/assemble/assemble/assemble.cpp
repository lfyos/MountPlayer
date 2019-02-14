// assemble.cpp : Implementation of Cassemble

#include "stdafx.h"
#include "assemble.h"
#include "BitmapHandler.h"
// Cassemble
#include "assemble_node.h"
//This method will add the command manager to the SolidWorks UI
void Cassemble::AddCommandManager()
{
	CComPtr<ICommandGroup> icmdGroup;
	CComObject<CBitmapHandler> *iBmp;
	CComObject<CBitmapHandler>::CreateInstance(&iBmp);
	long cmdIndex0, cmdIndex1;

	const int numDocumentTypes = 3;

	int docTypes[numDocumentTypes];

	docTypes[0] = swDocASSEMBLY;
        docTypes[1] = swDocDRAWING,
        docTypes[2] = swDocPART;


	CComBSTR title;
	title.LoadString(IDS_TOOLBAR_TITLE);

	CComBSTR hint;
	hint.LoadString(IDS_MENU_HINT);

	iCmdMgr->CreateCommandGroup(1,title,title,hint,-1,&icmdGroup);
	
	CComBSTR smallImageFile;
	iBmp->CreateBitmapFileFromResource(IDB_TOOLBAR_SMALL, &smallImageFile);
	icmdGroup->put_SmallIconList(smallImageFile);

	CComBSTR largeImageFile;
	iBmp->CreateBitmapFileFromResource(IDB_TOOLBAR_LARGE, &largeImageFile);
	icmdGroup->put_LargeIconList(largeImageFile);

	CComBSTR largeIconFile;
	iBmp->CreateBitmapFileFromResource(IDB_ICON_LARGE, &largeIconFile);
	icmdGroup->put_LargeMainIcon(largeIconFile);

	CComBSTR smallIconFile;
	iBmp->CreateBitmapFileFromResource(IDB_ICON_SMALL, &smallIconFile);
	icmdGroup->put_SmallMainIcon(smallIconFile);


	CComBSTR tip;
	CComBSTR callback;
	CComBSTR enable;
	VARIANT_BOOL cmdActivated;

	callback.LoadString(IDS_TOOLBAR_CALLBACK0);
	enable.LoadString(IDS_TOOLBAR_ENABLE0);
	tip.LoadString(IDS_TOOLBAR_TIP0);
	hint.LoadString(IDS_TOOLBAR_HINT0);
	icmdGroup->AddCommandItem(tip,-1,hint,tip,0,callback,enable,0,&cmdIndex0);


	icmdGroup->put_HasToolbar(true);
	icmdGroup->put_HasMenu(true);
	icmdGroup->Activate(&cmdActivated);



	for(int i=0; i < numDocumentTypes; i++)
	{
		CComPtr<ICommandTab> pTab = NULL;

		long TabCount, docType = docTypes[i];

		iCmdMgr->GetCommandTabCount(docType, &TabCount);

		// check if tab exists
	    iCmdMgr->GetCommandTab(docType, title, &pTab);

		if(pTab == NULL)
		{

						  // if not, add one
			  iCmdMgr->AddCommandTab(docType, title, &pTab);

			  CComPtr<ICommandTabBox> pBox;

			  pTab->AddCommandTabBox(&pBox);
			  
			  // create 2 commands on this tab
			  long CommandIDCount = 2;
			  
			  
			  long CommandIDs[2];
			  long TextDisplayStyles[2];
			  
			  long cmdID = 0;

			  // the 2 command buttons have different text styles
			  icmdGroup->get_CommandID(cmdIndex0, &cmdID);      
			  CommandIDs[0] = cmdID;
			  TextDisplayStyles[0] = swCommandTabButton_TextHorizontal;

			  icmdGroup->get_ToolbarId(&cmdID);
			  CommandIDs[1] = cmdID;
			  TextDisplayStyles[1] = swCommandTabButton_TextHorizontal;

			  VARIANT_BOOL vbResult = VARIANT_FALSE;

			  pBox->IAddCommands(CommandIDCount, CommandIDs, TextDisplayStyles, &vbResult);

			  CommandIDCount = 1;

			  icmdGroup->get_ToolbarId(&cmdID);
			  CommandIDs[0] = cmdID;
			  TextDisplayStyles[0] = swCommandTabButton_TextBelow | swCommandTabButton_ActionFlyout;


			  CComPtr<ICommandTabBox> pBox1;

			  pTab->AddCommandTabBox(&pBox1);

			  pBox1->IAddCommands(CommandIDCount, CommandIDs, TextDisplayStyles, &vbResult);
				  
			  CComPtr<ICommandTabBox> pBoxNew1; 

			  pTab->AddSeparator(pBox1, cmdID, &pBoxNew1);

		}
	
	}

	//Clean up
	iBmp->Dispose();
	iBmp->Release();
}


void Cassemble::RemoveCommandManager()
{
	VARIANT_BOOL cmdRemoved; 
	iCmdMgr->RemoveCommandGroup(1,&cmdRemoved);
}



//Event Handlers
//Called when the active document in SolidWorks changes
STDMETHODIMP Cassemble::OnDocChange(void)
{
	// TODO: Add your implementation code here
	return S_OK;
}

//Called after a document is opened
STDMETHODIMP Cassemble::OnFileOpenPostNotify(BSTR fileName)
{
	HRESULT hres = S_OK;
	// TODO: Add your implementation code here
   return hres;
}

//Called when a new document is created or a document is loaded
STDMETHODIMP Cassemble::OnDocLoad(BSTR docTitle, BSTR docPath)
{

	return S_OK;
}

//Called when the active model document changes in SolidWorks
STDMETHODIMP Cassemble::OnModelDocChange(void)
{
	// TODO: Add your implementation code here
	return S_OK;
}

//Called when a new file is created
STDMETHODIMP Cassemble::OnFileNew(LPDISPATCH newDoc, long docType, BSTR templateName)
{
	HRESULT hres = S_OK;
	// TODO: Add your implementation code here
   return hres;
}

//Utility Methods

//Set up the addin to catch SolidWorks events
VARIANT_BOOL Cassemble::AttachEventHandlers()
{
	VARIANT_BOOL attached = VARIANT_TRUE;
	this->m_libid = LIBID_SldWorks;
	this->m_wMajorVerNum = GetSldWorksTlbMajor();
	this->m_wMinorVerNum = 0;

	CSldWorksEvents::_tih.m_wMajor = this->m_wMajorVerNum;

	// Connect to the SldWorks event sink
	HRESULT success = this->DispEventAdvise(iSwApp, &__uuidof(DSldWorksEvents));

	if (success != S_OK)
		return VARIANT_FALSE;

	return attached;
}


//Stop listening for SolidWorks Events
VARIANT_BOOL Cassemble::DetachEventHandlers()
{
	VARIANT_BOOL detached = VARIANT_TRUE;

	// Disconnect from the SldWorks event sink
	HRESULT success = this->DispEventUnadvise(iSwApp, &__uuidof(DSldWorksEvents));

	CSldWorksEvents::_tih.m_plibid = &GUID_NULL;

	if (success != S_OK)
		return VARIANT_FALSE;

	return detached;
}


// ISwAddin Methods
//This is the starting point for the addin.
STDMETHODIMP Cassemble::ConnectToSW(LPDISPATCH ThisSW, long Cookie, VARIANT_BOOL * IsConnected)
{
	ThisSW->QueryInterface(__uuidof(ISldWorks), (void**)&iSwApp);
	addinID = Cookie;
	iSwApp->GetCommandManager(Cookie,&iCmdMgr);

	VARIANT_BOOL status = VARIANT_FALSE;

	iSwApp->SetAddinCallbackInfo((long)_AtlBaseModule.GetModuleInstance(), static_cast<Iassemble*>(this), addinID, &status);
	//Get the current type library version.
	{
		USES_CONVERSION;
		CComBSTR bstrNum;
		std::string strNum;
		char *buffer;

		iSwApp->RevisionNumber(&bstrNum);

		strNum = W2A(bstrNum);
		m_swMajNum = strtol(strNum.c_str(), &buffer, 10 );

		m_swMinNum=0;

	}
	//Create the addin's UI
	AddCommandManager();
	//Listen for events
	*IsConnected = AttachEventHandlers();
	*IsConnected = VARIANT_TRUE;
	return S_OK;
}

STDMETHODIMP Cassemble::DisconnectFromSW(VARIANT_BOOL * IsDisconnected)
{
	//Remove the addin's UI
	RemoveCommandManager();
	//Stop listening for events
	*IsDisconnected = DetachEventHandlers();

	iCmdMgr.Release();
	//Make sure you release the SolidWorks pointer last
	iSwApp.Release();

	return E_NOTIMPL;
}

// Iassemble Methods
//Menu and Toolbar Callbacks

STDMETHODIMP Cassemble::ToolbarCallback0(void)
{
	// TODO: Add your implementation code here
//	::WinExec("Notepad.exe", SW_SHOW);

	CString file_name;
	{	
		assemble_node a(iSwApp);
		file_name = a.file_name+_T(".assemble");
		ofstream f(file_name);

		a.output_data(&f,a.get_directory_name().GetLength());
		f.close();
	}
	::MessageBox(NULL,file_name,_T("�����ļ�����"),MB_OK);
	return S_OK;
}

STDMETHODIMP Cassemble::ToolbarEnable0(long* status)
{
	// TODO: Add your implementation code here
	*status = 1;
	return S_OK;
}
