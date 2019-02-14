

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Fri Jul 23 11:06:43 2010
 */
/* Compiler settings for _part.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef ___part_h__
#define ___part_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IBitmapHandler_FWD_DEFINED__
#define __IBitmapHandler_FWD_DEFINED__
typedef interface IBitmapHandler IBitmapHandler;
#endif 	/* __IBitmapHandler_FWD_DEFINED__ */


#ifndef __Ipart_FWD_DEFINED__
#define __Ipart_FWD_DEFINED__
typedef interface Ipart Ipart;
#endif 	/* __Ipart_FWD_DEFINED__ */


#ifndef __CBitmapHandler_FWD_DEFINED__
#define __CBitmapHandler_FWD_DEFINED__

#ifdef __cplusplus
typedef class CBitmapHandler CBitmapHandler;
#else
typedef struct CBitmapHandler CBitmapHandler;
#endif /* __cplusplus */

#endif 	/* __CBitmapHandler_FWD_DEFINED__ */


#ifndef __Cpart_FWD_DEFINED__
#define __Cpart_FWD_DEFINED__

#ifdef __cplusplus
typedef class Cpart Cpart;
#else
typedef struct Cpart Cpart;
#endif /* __cplusplus */

#endif 	/* __Cpart_FWD_DEFINED__ */


/* header files for imported files */
#include "prsht.h"
#include "mshtml.h"
#include "mshtmhst.h"
#include "exdisp.h"
#include "objsafe.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IBitmapHandler_INTERFACE_DEFINED__
#define __IBitmapHandler_INTERFACE_DEFINED__

/* interface IBitmapHandler */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IBitmapHandler;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1ADB3D4C-8316-428E-9F89-598E91ECAEE0")
    IBitmapHandler : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateBitmapFileFromResource( 
            /* [in] */ DWORD resID,
            /* [retval][out] */ BSTR *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Dispose( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IBitmapHandlerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBitmapHandler * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBitmapHandler * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBitmapHandler * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IBitmapHandler * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IBitmapHandler * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IBitmapHandler * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IBitmapHandler * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateBitmapFileFromResource )( 
            IBitmapHandler * This,
            /* [in] */ DWORD resID,
            /* [retval][out] */ BSTR *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Dispose )( 
            IBitmapHandler * This);
        
        END_INTERFACE
    } IBitmapHandlerVtbl;

    interface IBitmapHandler
    {
        CONST_VTBL struct IBitmapHandlerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBitmapHandler_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBitmapHandler_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBitmapHandler_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IBitmapHandler_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IBitmapHandler_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IBitmapHandler_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IBitmapHandler_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IBitmapHandler_CreateBitmapFileFromResource(This,resID,retval)	\
    ( (This)->lpVtbl -> CreateBitmapFileFromResource(This,resID,retval) ) 

#define IBitmapHandler_Dispose(This)	\
    ( (This)->lpVtbl -> Dispose(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IBitmapHandler_INTERFACE_DEFINED__ */


#ifndef __Ipart_INTERFACE_DEFINED__
#define __Ipart_INTERFACE_DEFINED__

/* interface Ipart */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_Ipart;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("319430FB-809A-4464-9BAC-1E345B4A243F")
    Ipart : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ToolbarCallback0( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ToolbarEnable0( 
            /* [retval][out] */ long *status) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IpartVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            Ipart * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            Ipart * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            Ipart * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            Ipart * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            Ipart * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            Ipart * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            Ipart * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ToolbarCallback0 )( 
            Ipart * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ToolbarEnable0 )( 
            Ipart * This,
            /* [retval][out] */ long *status);
        
        END_INTERFACE
    } IpartVtbl;

    interface Ipart
    {
        CONST_VTBL struct IpartVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define Ipart_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define Ipart_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define Ipart_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define Ipart_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define Ipart_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define Ipart_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define Ipart_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define Ipart_ToolbarCallback0(This)	\
    ( (This)->lpVtbl -> ToolbarCallback0(This) ) 

#define Ipart_ToolbarEnable0(This,status)	\
    ( (This)->lpVtbl -> ToolbarEnable0(This,status) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __Ipart_INTERFACE_DEFINED__ */



#ifndef __part_LIBRARY_DEFINED__
#define __part_LIBRARY_DEFINED__

/* library part */
/* [helpstring][uuid][version] */ 


EXTERN_C const IID LIBID_part;

EXTERN_C const CLSID CLSID_CBitmapHandler;

#ifdef __cplusplus

class DECLSPEC_UUID("2C2A7FEE-CAEA-4C70-AA26-E486DC1B24C9")
CBitmapHandler;
#endif

EXTERN_C const CLSID CLSID_Cpart;

#ifdef __cplusplus

class DECLSPEC_UUID("4481A85F-5E87-4B1C-9A5E-8E2D32E1BC2A")
Cpart;
#endif
#endif /* __part_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


