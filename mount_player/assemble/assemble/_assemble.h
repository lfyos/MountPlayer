

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Sun Nov 21 22:06:48 2010
 */
/* Compiler settings for _assemble.idl:
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

#ifndef ___assemble_h__
#define ___assemble_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __Iassemble_FWD_DEFINED__
#define __Iassemble_FWD_DEFINED__
typedef interface Iassemble Iassemble;
#endif 	/* __Iassemble_FWD_DEFINED__ */


#ifndef __IBitmapHandler_FWD_DEFINED__
#define __IBitmapHandler_FWD_DEFINED__
typedef interface IBitmapHandler IBitmapHandler;
#endif 	/* __IBitmapHandler_FWD_DEFINED__ */


#ifndef __Cassemble_FWD_DEFINED__
#define __Cassemble_FWD_DEFINED__

#ifdef __cplusplus
typedef class Cassemble Cassemble;
#else
typedef struct Cassemble Cassemble;
#endif /* __cplusplus */

#endif 	/* __Cassemble_FWD_DEFINED__ */


#ifndef __CBitmapHandler_FWD_DEFINED__
#define __CBitmapHandler_FWD_DEFINED__

#ifdef __cplusplus
typedef class CBitmapHandler CBitmapHandler;
#else
typedef struct CBitmapHandler CBitmapHandler;
#endif /* __cplusplus */

#endif 	/* __CBitmapHandler_FWD_DEFINED__ */


/* header files for imported files */
#include "prsht.h"
#include "mshtml.h"
#include "mshtmhst.h"
#include "exdisp.h"
#include "objsafe.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __Iassemble_INTERFACE_DEFINED__
#define __Iassemble_INTERFACE_DEFINED__

/* interface Iassemble */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_Iassemble;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("77024056-A800-4920-82A9-EA0DA01CAC03")
    Iassemble : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ToolbarCallback0( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ToolbarEnable0( 
            /* [retval][out] */ long *status) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IassembleVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            Iassemble * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            Iassemble * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            Iassemble * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            Iassemble * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            Iassemble * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            Iassemble * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            Iassemble * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ToolbarCallback0 )( 
            Iassemble * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ToolbarEnable0 )( 
            Iassemble * This,
            /* [retval][out] */ long *status);
        
        END_INTERFACE
    } IassembleVtbl;

    interface Iassemble
    {
        CONST_VTBL struct IassembleVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define Iassemble_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define Iassemble_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define Iassemble_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define Iassemble_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define Iassemble_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define Iassemble_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define Iassemble_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define Iassemble_ToolbarCallback0(This)	\
    ( (This)->lpVtbl -> ToolbarCallback0(This) ) 

#define Iassemble_ToolbarEnable0(This,status)	\
    ( (This)->lpVtbl -> ToolbarEnable0(This,status) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __Iassemble_INTERFACE_DEFINED__ */


#ifndef __IBitmapHandler_INTERFACE_DEFINED__
#define __IBitmapHandler_INTERFACE_DEFINED__

/* interface IBitmapHandler */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IBitmapHandler;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("496D7CF8-5F2B-4F06-A0A9-603CC89A3BDA")
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



#ifndef __assemble_LIBRARY_DEFINED__
#define __assemble_LIBRARY_DEFINED__

/* library assemble */
/* [helpstring][uuid][version] */ 


EXTERN_C const IID LIBID_assemble;

EXTERN_C const CLSID CLSID_Cassemble;

#ifdef __cplusplus

class DECLSPEC_UUID("4663216B-881A-41FB-A507-45596DAD92AC")
Cassemble;
#endif

EXTERN_C const CLSID CLSID_CBitmapHandler;

#ifdef __cplusplus

class DECLSPEC_UUID("F92552A6-D0AA-4DC2-9FCA-BE926BDE1591")
CBitmapHandler;
#endif
#endif /* __assemble_LIBRARY_DEFINED__ */

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


