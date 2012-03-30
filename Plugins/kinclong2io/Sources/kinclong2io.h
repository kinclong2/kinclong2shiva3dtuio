/**
The MIT License (MIT)

Copyright (c) 2012 Danang Suharno, Yogyakarta 55143, Indonesia, kinclong2@gmail.com

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.

*/

//-----------------------------------------------------------------------------
#ifndef __kinclong2io_h__
#define __kinclong2io_h__
//-----------------------------------------------------------------------------
#include "S3DXAIPackage.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Package declaration
//-----------------------------------------------------------------------------
class kinclong2ioPackage : public S3DX::AIPackage
{
public :

    //-------------------------------------------------------------------------
    //  Constructor / Destructor
	//-------------------------------------------------------------------------

	kinclong2ioPackage         ( ) ;
                               ~kinclong2ioPackage         ( ) ;

	//-------------------------------------------------------------------------
    //  Accessors
	//-------------------------------------------------------------------------

    const   char               *GetName             ( ) const ;
            S3DX::uint32        GetFunctionCount    ( ) const ;
            S3DX::uint32        GetConstantCount    ( ) const ;
    const   S3DX::AIFunction   *GetFunctionAt       ( S3DX::uint32 _iIndex ) const ;
    const   S3DX::AIConstant   *GetConstantAt       ( S3DX::uint32 _iIndex ) const ;

} ;

//-----------------------------------------------------------------------------
// Package API declaration
//-----------------------------------------------------------------------------
class kinclong2ioAPI
{
public :

    //-------------------------------------------------------------------------
    //  API Constructor
	//-------------------------------------------------------------------------
                                kinclong2ioAPI       ( ) 
                                {
                                    pfn_kinclong2io_InitTUIO      = NULL ;
                                    pfn_kinclong2io_GetTouchInput = NULL ;
                                    pfn_kinclong2io_GetTouchID    = NULL ;
                                    pfn_kinclong2io_GetTouchx     = NULL ;
                                    pfn_kinclong2io_GetTouchy     = NULL ;
                                    pfn_kinclong2io_DeInitTUIO    = NULL ;

                                }

	//-------------------------------------------------------------------------
	//  API Callbacks 
	//-------------------------------------------------------------------------

    S3DX::AICallback        pfn_kinclong2io_InitTUIO ;
    S3DX::AICallback        pfn_kinclong2io_GetTouchInput ;
    S3DX::AICallback        pfn_kinclong2io_GetTouchID ;
    S3DX::AICallback        pfn_kinclong2io_GetTouchx ;
    S3DX::AICallback        pfn_kinclong2io_GetTouchy ;
    S3DX::AICallback        pfn_kinclong2io_DeInitTUIO ;

	//-------------------------------------------------------------------------
	//  API Functions 
	//-------------------------------------------------------------------------

    inline void                 InitTUIO      ( const S3DX::AIVariable& nPort ) { S3DX_DECLARE_VIN_01( nPort ) ; if ( pfn_kinclong2io_InitTUIO ) pfn_kinclong2io_InitTUIO ( 1, vIn, NULL );  }
    inline S3DX::AIVariable     GetTouchInput (  ) { S3DX::AIVariable vOut ; if ( pfn_kinclong2io_GetTouchInput ) pfn_kinclong2io_GetTouchInput ( 0, NULL, &vOut ); return vOut ; }
    inline S3DX::AIVariable     GetTouchID    ( const S3DX::AIVariable& nIndex ) { S3DX_DECLARE_VIN_01( nIndex ) ; S3DX::AIVariable vOut ; if ( pfn_kinclong2io_GetTouchID ) pfn_kinclong2io_GetTouchID ( 1, vIn, &vOut ); return vOut ; }
    inline S3DX::AIVariable     GetTouchx     ( const S3DX::AIVariable& nTouchID ) { S3DX_DECLARE_VIN_01( nTouchID ) ; S3DX::AIVariable vOut ; if ( pfn_kinclong2io_GetTouchx ) pfn_kinclong2io_GetTouchx ( 1, vIn, &vOut ); return vOut ; }
    inline S3DX::AIVariable     GetTouchy     ( const S3DX::AIVariable& nTouchID ) { S3DX_DECLARE_VIN_01( nTouchID ) ; S3DX::AIVariable vOut ; if ( pfn_kinclong2io_GetTouchy ) pfn_kinclong2io_GetTouchy ( 1, vIn, &vOut ); return vOut ; }
    inline void                 DeInitTUIO    (  ) { if ( pfn_kinclong2io_DeInitTUIO ) pfn_kinclong2io_DeInitTUIO ( 0, NULL, NULL );  }

 	//-------------------------------------------------------------------------
	//  API Constants 
	//-------------------------------------------------------------------------


} ;

extern kinclong2ioAPI kinclong2io;

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
