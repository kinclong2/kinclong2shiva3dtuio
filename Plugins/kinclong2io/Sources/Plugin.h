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
#ifndef __kinclong2io_Plugin_h__
#define __kinclong2io_Plugin_h__
//-----------------------------------------------------------------------------
#include "S3DXPlugin.h"
//-----------------------------------------------------------------------------

#define PLUGIN_AIPACKAGES_COUNT 1

//-----------------------------------------------------------------------------
// Class declaration
//-----------------------------------------------------------------------------

S3DX_DECLARE_PLUGIN ( kinclong2io )
{
    S3DX_DECLARE_AIENGINEAPI        ( ) ;
    S3DX_DECLARE_PLUGIN_INSTANCE    ( kinclong2io ) ;

public :

    //-------------------------------------------------------------------------
    //  Constructor / Destructor
    //-------------------------------------------------------------------------
                                    kinclong2io                    ( ) ;
                                   ~kinclong2io                    ( ) ;

    //-------------------------------------------------------------------------
    //  Plugin content directory
    //-------------------------------------------------------------------------
            void                    SetContentsDirectory        ( const char *_pDirectory ) ;

    //-------------------------------------------------------------------------
    //  AI packages
    //-------------------------------------------------------------------------
            S3DX::uint32            GetAIPackageCount           ( ) const ;
    const   S3DX::AIPackage        *GetAIPackageAt              ( S3DX::uint32 _iIndex ) const ;


    //-------------------------------------------------------------------------
    //  User functions
    //-------------------------------------------------------------------------
    inline  const char             *GetContentsDirectory        ( ) const { return aContentsDirectory ; }

private :

    //-------------------------------------------------------------------------
    //  Variables
    //-------------------------------------------------------------------------
	S3DX::AIPackage *   aAIPackages			[PLUGIN_AIPACKAGES_COUNT] ;
    char                aContentsDirectory  [ 1024 ] ;
} ;


//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
