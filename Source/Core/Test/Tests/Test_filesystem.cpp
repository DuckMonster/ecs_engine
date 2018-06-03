#include <CorePCH.h>
#include "../Test.h"
#include "Core/OS/Directory.h"
#include "Core/OS/File.h"

namespace
{
	bool Directory()
	{
		FDirectory directory;
		FDirectory workDir = FDirectory::GetWorkingDirectory();

		Debug_Log( "Working Directory: %s", workDir.ToString().c_str() );

		TEST_EXPR( FDirectory().GetType() == EDirectoryType::Relative );
		TEST_EXPR( FDirectory( "/Hello" ).GetType() == EDirectoryType::LocalRoot );
		TEST_EXPR( FDirectory( "C:\\Some/System/Path" ).GetType() == EDirectoryType::SystemRoot );

		TEST_EXPR( FDirectory( "Some/Path\\somefile.exe" ) == "Some/Path/" );
		TEST_EXPR( FDirectory( "somefile.exe" ) == "" );
		TEST_EXPR( FDirectory( "\\Other/Path/file.exe" ).GetExtractedFileName() == "file.exe" );

		TEST_EXPR( ( FDirectory( "/Local/Root/Path" ) + FDirectory( "/Other\\Local//Root//" ) ) == "/Local/Root/Path/Other/Local/Root/" );
		TEST_EXPR( ( FDirectory( "C:/System/Root/Path" ) + FDirectory( "Some\\Relative" ) ) == "C:\\System/Root/Path/Some//Relative" );
		TEST_EXPR( ( FDirectory( "C:/System/Root/Path" ) + FDirectory( "D:/Other/Root" ) ) == "" );

		{
			std::string filename;
			TEST_EXPR( FDirectory::RemoveFile( "C:/Dir/file.exe", &filename ) == "C:/Dir/" );
			TEST_EXPR( filename == "file.exe" );

			TEST_EXPR( FDirectory::RemoveFile( "\\Dir/.git\\.exe.com", &filename ) == "\\Dir/.git\\" );
			TEST_EXPR( filename == ".exe.com" );
		}

		FDirectory::SetLocalDirectory( "C:/MyGame" );
		TEST_EXPR( FDirectory::GetLocalDirectory() == "C:/MyGame" );
		TEST_EXPR( FDirectory::GetCurrentDirectory() == "C:/MyGame" );

		{
			FDirectoryScope scope( "Some/Relative/Path" );
			TEST_EXPR( FDirectory::GetCurrentDirectory() == "C:/MyGame/Some/Relative/Path" );
			FDirectoryScope scope2( "More/Relative" );
			TEST_EXPR( FDirectory::GetCurrentDirectory() == "C:/MyGame/Some/Relative/Path/More/Relative" );

			{
				FDirectoryScope scope3( "/Local/Root/Path" );
				TEST_EXPR( FDirectory::GetCurrentDirectory() == "C:/MyGame/Local/Root/Path" );

				FDirectoryScope scope4( "G:/New/System/Root" );
				TEST_EXPR( FDirectory::GetCurrentDirectory() == "G:/New/System/Root" );
			}

			TEST_EXPR( FDirectory::GetCurrentDirectory() == "C:/MyGame/Some/Relative/Path/More/Relative" );
		}

		TEST_EXPR( FDirectory::GetCurrentDirectory() == "C:/MyGame" );

		return true;
	}

	bool File()
	{
		FDirectoryScope scope1("C:/MyGame/");

		FFile myFile;
		TEST_EXPR( FFile( "Hello/World\\bro.exe" ) == "C:/MyGame/Hello/World/bro.exe" );
		TEST_EXPR( FFile( "Hello/World\\bro.exe" ).GetFileName() == "bro.exe" );

		{
			FDirectoryScope scope2("Relative/");
			TEST_EXPR( FFile( "Hello/bro.exe" ) == "C:/MyGame/Relative/Hello/bro.exe" );
			FDirectoryScope scope3("MoreRelative/");
			TEST_EXPR( FFile( "tree.fbx" ) == "C:/MyGame/Relative/MoreRelative/tree.fbx" );
			TEST_EXPR( FFile( "/Local/Root/tree.fbx" ) == "C:/MyGame/Local/Root/tree.fbx" );
			TEST_EXPR( FFile( "D:/OS/Root/tree.fbx" ) == "D:/OS/Root/tree.fbx" );
		}

		return true;
	}

	DEFINE_TEST( Directory );
	DEFINE_TEST( File );
}