#ifndef FILEFINDER_H
#define FILEFINDER_H

#include "iofunctions.h"

#include <boost/filesystem.hpp>





class FileFinder
{

public:
	FileFinder()
	{

	}

	void FindFiles( const std::string & folder , const stringlist & ext)
	{
		if ( folder.empty() )
			return;

		std::string mask_folder(folder + "*.*" );

		HANDLE hFindFile = INVALID_HANDLE_VALUE;
		WIN32_FIND_DATAA findData;

		hFindFile = FindFirstFileA( mask_folder.c_str() , &findData );
		if ( hFindFile != INVALID_HANDLE_VALUE )
		{
		do 
		{
			if ( !strncmp( findData.cFileName, "." , 1 ) || !strncmp( findData.cFileName , ".." , 2 ) )
				continue;

			if ( (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
			{
				std::string sub_folder( IO::addFolderName( folder , findData.cFileName ) );
				IO::addBackspace( sub_folder );
				FindFiles( sub_folder , ext );
			}

			// Get PAHT to Directory
			if ( ! (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
			{
				std::string file_name( findData.cFileName );
				
				auto file_ext = boost::filesystem::extension( file_name );
				if ( IO::isPresentInList( ext , file_ext ) ) 
				{
					file_list_.push_back( IO::make_file_path( folder , file_name ) );
				}
			}				

			//SearchFiles(
		}while(FindNextFileA(hFindFile, &findData) );

		FindClose(hFindFile);
			
		}

	}

	stringlist getFileNames() const
	{
		return file_list_;
	}

private:
	stringlist file_list_;

};

#endif