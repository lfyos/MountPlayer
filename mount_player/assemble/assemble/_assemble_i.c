

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


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


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_Iassemble,0x77024056,0xA800,0x4920,0x82,0xA9,0xEA,0x0D,0xA0,0x1C,0xAC,0x03);


MIDL_DEFINE_GUID(IID, IID_IBitmapHandler,0x496D7CF8,0x5F2B,0x4F06,0xA0,0xA9,0x60,0x3C,0xC8,0x9A,0x3B,0xDA);


MIDL_DEFINE_GUID(IID, LIBID_assemble,0x54B0978C,0xEA63,0x4F80,0x86,0x1E,0x5D,0x43,0xE8,0x1A,0xBB,0x7B);


MIDL_DEFINE_GUID(CLSID, CLSID_Cassemble,0x4663216B,0x881A,0x41FB,0xA5,0x07,0x45,0x59,0x6D,0xAD,0x92,0xAC);


MIDL_DEFINE_GUID(CLSID, CLSID_CBitmapHandler,0xF92552A6,0xD0AA,0x4DC2,0x9F,0xCA,0xBE,0x92,0x6B,0xDE,0x15,0x91);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



