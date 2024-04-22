#include <wfc.h>

#pragma hdrstop

/*
** Author: Samuel R. Blackburn
** CI$: 76300,326
** Internet: sblackbu@csc.com
**
** You can use it any way you like as long as you don't try to sell it.
**
** Any attempt to sell WFC in source code form must have the permission
** of the original author. You can produce commercial executables with
** WFC but you can't sell WFC.
**
** Copyright, 1996, Samuel R. Blackburn
**
** $Workfile: WfcTrace.cpp $
** $Revision: 1.1 $
** $Modtime: 7/25/96 9:31a $
*/


int  CWfcTrace::Indent  = 0;
BOOL CWfcTrace::Tracing = TRUE;

CWfcTrace::CWfcTrace( const CString& function_name )
{
   m_FunctionName = function_name;

   if ( Tracing == TRUE )
   {
      m_Indent();
      TRACE( "Entering " + m_FunctionName + "\n" );
   }

   Indent += 2;
}

CWfcTrace::~CWfcTrace()
{
   ASSERT_VALID( this );

   Indent -= 2;

   if ( Tracing == TRUE )
   {
      m_Indent();
      TRACE( "Leaving  " + m_FunctionName + "\n" );
   }

   m_FunctionName.Empty();
}

void CWfcTrace::AssertValid() const
{
   CObject::AssertValid();
}

void CWfcTrace::m_Indent( void ) const
{
   ASSERT_VALID( this );

   if ( Tracing == TRUE )
   {
      if ( Indent > 0 )
      {
         CString spaces( "" );

         int index = 0;

         while( index < Indent )
         {
            spaces += " ";
            index++;
         }

         TRACE( spaces );
      }
   }
}

void CWfcTrace::Output( const CString& message ) const
{
   ASSERT_VALID( this );

   if ( Tracing == TRUE )
   {
      m_Indent();

      CString output_string( m_FunctionName );

      output_string += " : ";
      output_string += message;

      if ( output_string.GetLength() > 510 )
      {
            output_string = output_string.Left( 510 );
      }

      output_string += "\n";

      TRACE0( output_string );
   }
}

void CWfcTrace::Output( const CString& message, const int integer ) const
{
   ASSERT_VALID( this );

   char temp_string[ 11 ];

   ::memset( temp_string, 0x00, sizeof( temp_string ) );
   ::itoa( integer, temp_string, 10 );

   Output( message + temp_string );
}

void CWfcTrace::Output( const CString& message, const UINT integer ) const
{
   ASSERT_VALID( this );

   Output( message, (ULONG) integer );
}

void CWfcTrace::Output( const CString& message, const long a_long ) const
{
   ASSERT_VALID( this );

   char temp_string[ 11 ];

   ::memset( temp_string, 0x00, sizeof( temp_string ) );
   ::ltoa( a_long, temp_string, 10 );

   Output( message + temp_string );
}

void CWfcTrace::Output( const CString& message, const ULONG a_long ) const
{
   ASSERT_VALID( this );

   char temp_string[ 11 ];

   ::memset( temp_string, 0x00, sizeof( temp_string ) );
   ::ultoa( a_long, temp_string, 10 );

   Output( message + temp_string );
}

void CWfcTrace::Output( const CString& message, const double a_double ) const
{
   ASSERT_VALID( this );

   int decimal = 0;
   int sign    = 0;

   char *string_p = (char *) NULL;

   string_p = ::_fcvt( a_double, 7, &decimal, &sign );

   CString string( "" );

   if ( sign != 0 )
   {
      string = "-";
   }

   if ( decimal < 1 )
   {
      string += ".";

      int index = 0;

      while( index > decimal )
      {
         string += "0";
         index--;
      }

      string += string_p;
   }
   else if ( decimal >= (int) ::strlen( string_p ) )
   {
      string += string_p;
      string += ".";
   }
   else
   {
      int index = 0;

      while( index < decimal )
      {
         string += string_p[ index ];
         index++;
      }

      string += ".";

      index = decimal;

      while( index < (int) ::strlen( string_p ) )
      {
         string += string_p[ index ];
         index++;
      }
   }

   Output( message + string );
}

void CWfcTrace::Output( const CString& message, const CString& string ) const
{
   ASSERT_VALID( this );

   Output( message + string );
}

void CWfcTrace::Output( const CString& message, const CObject * object_p ) const
{
   ASSERT_VALID( this );
   ASSERT_VALID( object_p );

#if defined( _DEBUG )

   if ( object_p == NULL )
   {
      Output( message + "NULL POINTER!" );
   }
   else
   {
      Output( message, *object_p );
   }
#endif // _DEBUG
}

void CWfcTrace::Output( const CString& message, const CObject& object ) const
{
   ASSERT_VALID( this );

   Output( message + " dump:" );

#if defined( _DEBUG )

   object.Dump( afxDump );

   CRuntimeClass * runtime_data_p = object.GetRuntimeClass();

   if ( ::strcmp( runtime_data_p->m_lpszClassName, "CByteArray" ) == 0 )
   {
      // Dump da bytes

      CByteArray * bytes_p = (CByteArray *) &object;

      CString spaces( "" );

      if ( Indent > 0 )
      {
         int index = 0;

         while( index < Indent )
         {
            spaces += " ";
            index++;
         }
      }

      afxDump.HexDump( spaces, bytes_p->GetData(), bytes_p->GetSize(), 20 );
   }

#endif // _DEBUG

   Output( "End of " + message + " dump." );
}
