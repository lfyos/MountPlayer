// BitmapHandler.cpp : Implementation of CBitmapHandler

#include "stdafx.h"
#include "BitmapHandler.h"

//Public Methods
STDMETHODIMP CBitmapHandler::CreateBitmapFileFromResource(DWORD resID, BSTR* retval)
{
	HBITMAP image = (HBITMAP)::LoadImage(_AtlBaseModule.GetModuleInstance(), 
								MAKEINTRESOURCE(resID), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

	CComBSTR fname =  CreateUniqueFileName();
	if (fname.Length() == 0)
		return E_FAIL;

	BOOL saved = SaveHBitmapToDisk(fname, image, NULL);
	if (!saved)
		return E_FAIL;

	createdFiles.push_front(CString(fname));

	*retval = fname.Detach();
	return S_OK;
}

STDMETHODIMP CBitmapHandler::Dispose()
{
	CleanFilesFromDisk();
	return S_OK;
}

//Protected 'Worker' Methods
CComBSTR CBitmapHandler::CreateUniqueFileName()
{
	TCHAR tempDir[MAX_PATH];
	TCHAR tmpName[MAX_PATH];
	DWORD dw = ::GetTempPath(sizeof(tempDir)/sizeof(TCHAR), tempDir);
	if (dw == 0)
		return _T("");
	UINT ui = ::GetTempFileName(tempDir, _T("swt"), 0, tmpName);
	if (ui == 0)
		return _T("");

	return CComBSTR(tmpName);
}

BOOL CBitmapHandler::SaveHBitmapToDisk(LPCTSTR filename, HBITMAP bitmap, HPALETTE palette = NULL)
{
	BOOL written = FALSE;

	//Create the Picture Description
	PICTDESC pictureDesc;
	pictureDesc.cbSizeofstruct = sizeof(PICTDESC);
	pictureDesc.picType = PICTYPE_BITMAP;
	pictureDesc.bmp.hbitmap = bitmap;
	pictureDesc.bmp.hpal = palette;

	//Create the IPicture
	IPicture *iPic;
	HRESULT success = ::OleCreatePictureIndirect(&pictureDesc, IID_IPicture, false, reinterpret_cast<void**>(&iPic));
	if (!SUCCEEDED(success))
		return FALSE;

	//Create the IStream
	IStream *stream;
	success = ::CreateStreamOnHGlobal(NULL, true, &stream);
	if (!SUCCEEDED(success))
		return FALSE;

	//Write the IPicture into the IStream
	LONG bytes_streamed;
	success = iPic->SaveAsFile(stream, true, &bytes_streamed);
	if (!SUCCEEDED(success))
		return FALSE;

    //Create the file
    HANDLE tempFile = ::CreateFile(filename, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (tempFile)
    {
		HGLOBAL mem = NULL;
        GetHGlobalFromStream(stream, &mem);
        LPVOID bitmapData = GlobalLock(mem);

        //Write the stream to the file
        DWORD bytes_written;
        written = ::WriteFile(tempFile, bitmapData, bytes_streamed, &bytes_written, NULL);
		if (!written || (bytes_written != bytes_streamed))
			written = FALSE;

        ::GlobalUnlock(mem);
        ::CloseHandle(tempFile);
    }
      
    stream->Release();
    iPic->Release();

	return written;   
}

BOOL CBitmapHandler::CleanFilesFromDisk()
{
	CString file;
	while(! createdFiles.empty())
	{
		file = createdFiles.front();
		createdFiles.pop_front();
		BOOL deleted = ::DeleteFile(file);
	}
	return TRUE;
}
