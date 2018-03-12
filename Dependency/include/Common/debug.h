#pragma once
#include <stdarg.h>
#include <set>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef WIN32_LEAN_AND_MEAN
#include "hash.h"

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef DEBUG_BUF_SIZE
#define DEBUG_BUF_SIZE 4096
#endif

namespace Debug
{
	__forceinline void ConsoleLogV( const char* file, long line, const char* format, va_list list )
	{
		//--------------------------------------------------- Trim path to find last file
		const char* filePtr = file;
		int lastSplit = 0;
		int i = 0;

		while ( *filePtr )
		{
			if ( *filePtr == '\\' || *filePtr == '/' )
				lastSplit = i;

			++i;
			++filePtr;
		}

		filePtr = file + lastSplit + 1;
		//---------------------------------------------------

		static char buffer[DEBUG_BUF_SIZE];
		int cursor = 0;

		// File and line header
		cursor += sprintf_s( buffer, DEBUG_BUF_SIZE, "(%s:%d)> \t", filePtr, line );

		// Message
		cursor += vsprintf_s( buffer + cursor, DEBUG_BUF_SIZE - cursor, format, list );

		// Newline and null-terminator
		buffer[cursor] = '\n';
		buffer[cursor + 1] = '\0';

		// Print to console
		printf( buffer );
		// Print to output window
		OutputDebugStringA( buffer );
	}

	__forceinline void ConsoleLog( const char* file, long line, const char* format, ... )
	{
		// Message
		va_list args;
		va_start( args, format );
		ConsoleLogV( file, line, format, args );
		va_end( args );
	}

	__forceinline void ConsoleLogMinimal( const char* format, ... )
	{
		static char buffer[DEBUG_BUF_SIZE];
		int cursor = 0;

		// Message
		va_list args;
		va_start( args, format );
		cursor += vsprintf_s( buffer, DEBUG_BUF_SIZE, format, args );
		va_end( args );

		// Newline and null-terminator
		buffer[cursor] = '\n';
		buffer[cursor + 1] = '\0';

		// Print to console
		printf( buffer );
		// Print to output window
		OutputDebugStringA( buffer );
	}

	__forceinline bool CheckEnsureOnce( const char* file, long line, const char* exprStr )
	{
		static std::set<Hash::Type> sEnsureSet;

		struct EnsureHashable
		{
			char fileName[20];
			long line;
			char expr[20];
		} hashable;

		memcpy( hashable.fileName, file, min( 20, strlen( file ) ) );
		memcpy( hashable.expr, exprStr, min( 20, strlen( exprStr ) ) );
		hashable.line = line;

		Hash::Type hash = Hash::FNV( hashable );

		if ( sEnsureSet.find( hash ) == sEnsureSet.end() )
		{
			sEnsureSet.emplace( hash );
			return true;
		}

		return false;
	}

	__forceinline bool DoEnsure( bool expr, const char* file, long line, const char* exprStr )
	{
		if ( !expr )
		{
			ConsoleLog( file, line, "ENSURE: %s", exprStr );

			if ( CheckEnsureOnce( file, line, exprStr ) )
			{
				if ( IsDebuggerPresent() )
					DebugBreak();
				else
				{
					static char buffer[DEBUG_BUF_SIZE];
					sprintf_s( buffer, DEBUG_BUF_SIZE, "Ensure hit at [ %s : %d ]!\n\n%s", file, line, exprStr );
					MessageBoxA( 0, buffer, "Ensure", MB_OK );
				}
			}
		}

		return expr;
	}

	__forceinline bool DoEnsure_Msg( bool expr, const char* file, long line, const char* exprStr, const char* msgFormat, ... )
	{
		if ( !expr )
		{
			static char sBuffer[DEBUG_BUF_SIZE];

			va_list vl;
			va_start( vl, msgFormat );
			vsprintf_s( sBuffer, DEBUG_BUF_SIZE, msgFormat, vl );
			va_end( vl );

			ConsoleLog( file, line, "ENSURE: %s || %s", exprStr, sBuffer );

			if ( CheckEnsureOnce( file, line, exprStr ) )
			{
				if ( IsDebuggerPresent() )
					DebugBreak();
				else
				{
					static char buffer[DEBUG_BUF_SIZE];
					sprintf_s( buffer, DEBUG_BUF_SIZE, "Ensure hit at [ %s : %d ]!\n\n%s\nMsg: %s", file, line, exprStr, sBuffer );
					MessageBoxA( 0, buffer, "Ensure", MB_OK );
				}
			}
		}

		return expr;
	}
}

#if defined(DEBUG)
#define Debug_Log(format, ...) Debug::ConsoleLog( __FILE__, __LINE__, format, __VA_ARGS__ )
#define Debug_Log_Minimal(format, ...) Debug::ConsoleLogMinimal(format, __VA_ARGS__)
#define Debug_LogV(format, list) Debug::ConsoleLogV( __FILE__, __LINE__, format, list )
#define Ensure(expr) ( Debug::DoEnsure( !!(expr), __FILE__, __LINE__, #expr ) )
#define EnsureMsg(expr, format, ...) ( Debug::DoEnsure_Msg( !!(expr), __FILE__, __LINE__, #expr, format, __VA_ARGS__ ) )
#else
#define Debug_Log(format, ...)
#define Debug_Log_Minimal(format, ...)
#define Debug_LogV(format, list)
#define Ensure(expr) ( expr )
#define EnsureMsg(expr, format, ...) ( expr )
#endif