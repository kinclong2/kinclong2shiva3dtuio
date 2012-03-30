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
#include "PrecompiledHeader.h"
//-----------------------------------------------------------------------------
#include <string.h>
//-----------------------------------------------------------------------------
S3DX_IMPLEMENT_AIVARIABLE_STRING_POOL   ( 524288 ) ;
S3DX_IMPLEMENT_AIENGINEAPI              ( kinclong2io ) ;
S3DX_IMPLEMENT_PLUGIN                   ( kinclong2io ) ;

//-----------------------------------------------------------------------------
//  AI Packages
//-----------------------------------------------------------------------------
#include "kinclong2io.h"

//-----------------------------------------------------------------------------
//  Constructor / Destructor
//-----------------------------------------------------------------------------

kinclong2io::kinclong2io ( )
{
    S3DX_REGISTER_PLUGIN  ( "kinclong2io" ) ;
    aContentsDirectory[0] = '\0' ;
	
	//Instanciate AI Packages
    S3DX::uint32 iAIPackageIndex = 0 ;
    if ( iAIPackageIndex < PLUGIN_AIPACKAGES_COUNT ) aAIPackages [iAIPackageIndex++] = new kinclong2ioPackage ( ) ;

	for ( ; iAIPackageIndex < PLUGIN_AIPACKAGES_COUNT; iAIPackageIndex ++ )
	{
        aAIPackages[iAIPackageIndex] = NULL  ;		
	}

}

//-----------------------------------------------------------------------------

kinclong2io::~kinclong2io ( )
{
	for ( S3DX::uint32 iAIPackageIndex = 0 ; iAIPackageIndex < PLUGIN_AIPACKAGES_COUNT; iAIPackageIndex ++ )
	{
		if ( aAIPackages [iAIPackageIndex] )
		{
			delete aAIPackages [iAIPackageIndex] ; 
			aAIPackages[iAIPackageIndex] = NULL  ;
		}
	}
}


//-----------------------------------------------------------------------------
//  Plugin content directory
//-----------------------------------------------------------------------------

        void                    kinclong2io::SetContentsDirectory  ( const char *_pDirectory ) { strcpy ( aContentsDirectory, _pDirectory ) ; }

//-----------------------------------------------------------------------------
//  AI packages
//-----------------------------------------------------------------------------

        S3DX::uint32            kinclong2io::GetAIPackageCount     ( )                      const { return PLUGIN_AIPACKAGES_COUNT ; }
const   S3DX::AIPackage        *kinclong2io::GetAIPackageAt        ( S3DX::uint32 _iIndex ) const { return (_iIndex < PLUGIN_AIPACKAGES_COUNT) ? aAIPackages[_iIndex] : NULL ; }

