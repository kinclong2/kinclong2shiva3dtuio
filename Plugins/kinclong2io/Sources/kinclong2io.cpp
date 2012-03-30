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
#include "kinclong2io.h"
//-----------------------------------------------------------------------------

#include "tuio_parser.h" // Ketik manual, 29 Mar 2012, Gamelan Lor

#ifdef S3DX_DLL
	kinclong2ioAPI kinclong2io ;
#endif

#define _CRTDBG_MAP_ALLOC

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h>
#include "winsock2.h"
#include <malloc.h>
#include <windows.h>

//#define DEBUGGING			// uncomment untuk debugging dengan log file
#define NAMA_LOG_FILE			"TUIO.log"
#define THE_BUFFER_SIZE					1024*10

//
//
//
WSADATA wsaData;
SOCKET RecvSocket;
sockaddr_in RecvAddr;
char RecvBuf[THE_BUFFER_SIZE];
int  BufLen = THE_BUFFER_SIZE;
sockaddr_in SenderAddr;
int SenderAddrSize = sizeof(SenderAddr);
struct FEEDBACK fb;
int iOldAliveCnt = 0;
FILE *fLog;

//-----------------------------------------------------------------------------
//  Callbacks
//-----------------------------------------------------------------------------

int Callback_kinclong2io_InitTUIO ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "kinclong2io.InitTUIO" ) ;

    // Input Parameters 
    int iInputCount = 0 ;
    S3DX::AIVariable nPort = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;


//-----------------------------------------------
	// Initialize Winsock
	WSAStartup(MAKEWORD(2,2), &wsaData);

	//-----------------------------------------------
	// Create a receiver socket to receive datagrams
	RecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//-----------------------------------------------
	// Bind the socket to any address and the specified port.
	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons((u_short) nPort.GetNumberValue());
	RecvAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	//
	// If iMode!=0, non-blocking mode is enabled.
	u_long iMode=1;
	ioctlsocket(RecvSocket, FIONBIO, &iMode);

	//
	bind(RecvSocket, (SOCKADDR *) &RecvAddr, sizeof(RecvAddr));

#ifdef DEBUGGING
	//
	fLog = fopen(NAMA_LOG_FILE, "wb");

#endif

    S3DX_API_PROFILING_STOP( ) ;
    return 0;
}

//-----------------------------------------------------------------------------

int Callback_kinclong2io_GetTouchInput ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "kinclong2io.GetTouchInput" ) ;

    // Output Parameters 
    S3DX::AIVariable nTouchCnt ;

	//
	nTouchCnt.SetNumberValue(0);

	// Receive data
	memset( RecvBuf, 0x0, THE_BUFFER_SIZE);
	BufLen = THE_BUFFER_SIZE;
	int iLenRcv = 0;
	

	//
	iLenRcv = recvfrom(RecvSocket, 
						RecvBuf,
						BufLen,
						0,
						(SOCKADDR *)&SenderAddr,
						&SenderAddrSize);

	//
	int nError = WSAGetLastError();
	if( (nError != WSAEWOULDBLOCK) )
	{	
		//	
		if( iLenRcv != SOCKET_ERROR )
		{	
			if( iLenRcv > 0)
			{
				//memset( &fb, 0x0, sizeof(fb) ); // cleanup
				Parse( RecvBuf, iLenRcv, &fb, sizeof(fb) );

				/*
				if( iOldAliveCnt != fb.alive_count )
				{
					if( iOldAlive
				}
				*/
				iOldAliveCnt = fb.alive_count;

				//
				nTouchCnt.SetNumberValue( iOldAliveCnt );

#ifdef DEBUGGING
				//
				fwrite(RecvBuf, sizeof(char), iLenRcv, fLog);
				fflush(fLog);
#endif
			}
		}
	}

    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = nTouchCnt ;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}

//-----------------------------------------------------------------------------

int Callback_kinclong2io_GetTouchID ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "kinclong2io.GetTouchID" ) ;

    // Input Parameters 
    int iInputCount = 0 ;
    S3DX::AIVariable nIndex = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;

    // Output Parameters 
    S3DX::AIVariable nTouchID ;

	nTouchID.SetNumberValue( 0 );
	if( fb.alive_count > 0 )
	{
		if( nIndex < (UINT) fb.alive_count )
			nTouchID.SetNumberValue( fb.alive_sid[ nIndex ] );
	}

    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = nTouchID ;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}

//-----------------------------------------------------------------------------

int Callback_kinclong2io_GetTouchx ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "kinclong2io.GetTouchx" ) ;

    // Input Parameters 
    int iInputCount = 0 ;
    S3DX::AIVariable nTouchID = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;

    // Output Parameters 
    S3DX::AIVariable nX ;


	nX.SetNumberValue( -1 );
	if( fb.alive_count > 0 )
	{
		for(int i = 0; i < fb.alive_count; i++ )
		{
			if( fb.alive_sid[ i ] == nTouchID.GetNumberValue() )
				nX.SetNumberValue( fb.sets[ i ].x );
		}
	}


    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = nX ;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}

//-----------------------------------------------------------------------------

int Callback_kinclong2io_GetTouchy ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "kinclong2io.GetTouchy" ) ;

    // Input Parameters 
    int iInputCount = 0 ;
    S3DX::AIVariable nTouchID = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;

    // Output Parameters 
    S3DX::AIVariable nY ;


	nY.SetNumberValue( -1 );
	if( fb.alive_count > 0 )
	{
		for(int i = 0; i < fb.alive_count; i++ )
		{
			if( fb.alive_sid[ i ] == nTouchID.GetNumberValue() )
				nY.SetNumberValue( fb.sets[ i ].y );
		}
	}


    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = nY ;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}

//-----------------------------------------------------------------------------

int Callback_kinclong2io_DeInitTUIO ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "kinclong2io.DeInitTUIO" ) ;


	//
	WSACleanup();

#ifdef DEBUGGING

	//
	fclose(fLog);

#endif

    S3DX_API_PROFILING_STOP( ) ;
    return 0;
}

//-----------------------------------------------------------------------------
//  Constructor / Destructor
//-----------------------------------------------------------------------------

kinclong2ioPackage::kinclong2ioPackage ( )
{
#ifdef S3DX_DLL
    kinclong2io.pfn_kinclong2io_InitTUIO      = Callback_kinclong2io_InitTUIO      ;
    kinclong2io.pfn_kinclong2io_GetTouchInput = Callback_kinclong2io_GetTouchInput ;
    kinclong2io.pfn_kinclong2io_GetTouchID    = Callback_kinclong2io_GetTouchID    ;
    kinclong2io.pfn_kinclong2io_GetTouchx     = Callback_kinclong2io_GetTouchx     ;
    kinclong2io.pfn_kinclong2io_GetTouchy     = Callback_kinclong2io_GetTouchy     ;
    kinclong2io.pfn_kinclong2io_DeInitTUIO    = Callback_kinclong2io_DeInitTUIO    ;
	
#endif
}

//-----------------------------------------------------------------------------

kinclong2ioPackage::~kinclong2ioPackage ( )
{

}

//-----------------------------------------------------------------------------
//  Functions table
//-----------------------------------------------------------------------------

static S3DX::AIFunction aMyFunctions [ ] =
{
    { "InitTUIO"     , Callback_kinclong2io_InitTUIO     , ""         , "nPort"   , "Setup UDP socket receiver"   , 0 }, 
    { "GetTouchInput", Callback_kinclong2io_GetTouchInput, "nTouchCnt", ""        , "Get and Parse UDP packet"    , 0 }, 
    { "GetTouchID"   , Callback_kinclong2io_GetTouchID   , "nTouchID" , "nIndex"  , "Get TouchID from array index", 0 }, 
    { "GetTouchx"    , Callback_kinclong2io_GetTouchx    , "nX"       , "nTouchID", "Get Touch X position"        , 0 }, 
    { "GetTouchy"    , Callback_kinclong2io_GetTouchy    , "nY"       , "nTouchID", "Get Touch Y position"        , 0 }, 
    { "DeInitTUIO"   , Callback_kinclong2io_DeInitTUIO   , ""         , ""        , "Free Allocated Memory"       , 0 }    
    //{ NULL, NULL, NULL, NULL, NULL, 0}
} ;

//-----------------------------------------------------------------------------
//  Constants table
//-----------------------------------------------------------------------------

static S3DX::AIConstant aMyConstants [ ] =
{
	{ NULL, 0, NULL, 0}
} ;

//-----------------------------------------------------------------------------
//  Accessors
//-----------------------------------------------------------------------------

const char *kinclong2ioPackage::GetName ( ) const
{
    return "kinclong2io" ;
}

//-----------------------------------------------------------------------------

S3DX::uint32 kinclong2ioPackage::GetFunctionCount ( ) const
{
	if ( aMyFunctions[0].pName == NULL )
	{
		return 0 ;
	}
	else
	{
		return sizeof( aMyFunctions ) / sizeof( S3DX::AIFunction ) ;
	}
}

//-----------------------------------------------------------------------------

S3DX::uint32 kinclong2ioPackage::GetConstantCount ( ) const
{
	if ( aMyConstants[0].pName == NULL )
	{
		return 0 ;
	}
	else
	{
		return sizeof( aMyConstants ) / sizeof( S3DX::AIConstant ) ;
	}
}

//-----------------------------------------------------------------------------

const S3DX::AIFunction *kinclong2ioPackage::GetFunctionAt ( S3DX::uint32 _iIndex ) const
{
    return &aMyFunctions[ _iIndex ] ;
}

//-----------------------------------------------------------------------------

const S3DX::AIConstant *kinclong2ioPackage::GetConstantAt ( S3DX::uint32 _iIndex ) const
{
    return &aMyConstants[ _iIndex ] ;
}
