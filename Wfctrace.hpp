#if ! defined( WFC_TRACE_CLASS_HEADER )

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
** $Workfile: WFCTrace.hpp $
** $Revision: 1.1 $
** $Modtime: 6/10/96 5:00a $
*/

#define WFC_TRACE_CLASS_HEADER

#if defined( _DEBUG )

#define WFCTRACEINIT( string ) CWfcTrace ___trace( string );
#define WFCTRACE( string )     { ___trace.Output( string ); }
#define WFCTRACEVAL( string, value ) { ___trace.Output( string, value ); }
#define WFCTRACEDUMP( variable ) { ___trace.Output( #variable, variable ); }

#else

#define WFCTRACEINIT( string )       {;}
#define WFCTRACE( string )           {;}
#define WFCTRACEVAL( string, value ) {;}
#define WFCTRACEDUMP( variable )     {;}

#endif // _DEBUG

class CWfcTrace : public CObject
{
   protected:

      CString m_FunctionName;

      void m_Indent( void ) const;

   public:

      static int Indent;
      static BOOL Tracing;

      CWfcTrace( const CString& function_name );

      virtual ~CWfcTrace();

      virtual void AssertValid() const;
      virtual void Output( const CString& message ) const;
      virtual void Output( const CString& message, const int integer      ) const;
      virtual void Output( const CString& message, const UINT integer     ) const;
      virtual void Output( const CString& message, const long a_long      ) const;
      virtual void Output( const CString& message, const ULONG a_long     ) const;
      virtual void Output( const CString& message, const double a_double  ) const;
      virtual void Output( const CString& message, const CString& string  ) const;
      virtual void Output( const CString& message, const CObject * object ) const;
      virtual void Output( const CString& message, const CObject& object  ) const;
};

#endif // WFC_TRACE_CLASS_HEADER
