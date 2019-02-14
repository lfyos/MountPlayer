

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


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

MIDL_DEFINE_GUID(IID, IID_IBitmapHandler,0x1ADB3D4C,0x8316,0x428E,0x9F,0x89,0x59,0x8E,0x91,0xEC,0xAE,0xE0);


MIDL_DEFINE_GUID(IID, IID_Ipart,0x319430FB,0x809A,0x4464,0x9B,0xAC,0x1E,0x34,0x5B,0x4A,0x24,0x3F);


MIDL_DEFINE_GUID(IID, LIBID_part,0x59632D95,0xD5B9,0x48B6,0xA7,0x58,0x9F,0x83,0x1B,0x9A,0xCF,0x8A);


MIDL_DEFINE_GUID(CLSID, CLSID_CBitmapHandler,0x2C2A7FEE,0xCAEA,0x4C70,0xAA,0x26,0xE4,0x86,0xDC,0x1B,0x24,0xC9);


MIDL_DEFINE_GUID(CLSID, CLSID_Cpart,0x4481A85F,0x5E87,0x4B1C,0x9A,0x5E,0x8E,0x2D,0x32,0xE1,0xBC,0x2A);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



