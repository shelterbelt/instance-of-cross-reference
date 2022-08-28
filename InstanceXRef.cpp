// InstanceXRef.cpp : Defines the entry point for the Instance Of Cross Reference application.
//

#include "stdafx.h"
#include "InstanceXRef.h"


/* main - The application entry point. */
int main(int argc, TCHAR* argv[])
{
	/* Local Variable Declarations and Initializations */

	UINT uError = INSTXREF_ERROR_NOERROR;	// Method return code

	DSPARAMS dsScanParameters;				// Structure holding command line parameters

	CList<CString, CString> FileList;		// Linked list of paths to the files to evaluate


	// Clear data structures before processing to ensure clean data.
	ClearParametersStructure(&dsScanParameters);
	FileList.RemoveAll();


	/* Load scan parameters. */

	// Extract the parameters from the command line argument array.
	dsScanParameters = ExtractCommandLineParameters(argc, argv);

	// Check if an error was encountered while loading the parameters structure.
	if (dsScanParameters.uError != INSTXREF_ERROR_NOERROR)
	{
		// If an error was encountered, write the appropriate message to standard output.
		switch (dsScanParameters.uError)
		{
		case INSTXREF_ERROR_XPARAMS:				// The parameters are not formatted correctly.
			cout << INSTXREF_MESSAGE_XPARAMS;
			break;
		default:									// An unknown error was encountered.
			cout << INSTXREF_MESSAGE_UKERROR;
			break;
		}

		// If an error occurred, return and signal error.
		return INSTXREF_RETURN_PROGERROR;
	}


	/* Load the files to search. */

	// Extract the path to each file to search from the pseudo-Visual Studio 6 project file.
	uError = ExtractFilesToScan(dsScanParameters.szFilesetFile, &FileList);

	// Check if an error was encountered while loading the files to scan list.
	if (uError != INSTXREF_ERROR_NOERROR)
	{
		// If an error was encountered, write the appropriate message to standard output.
		switch (uError)
		{
		case INSTXREF_ERROR_NOFILE:					// The pseudo-Visual Studio 6 project file does not exist.
			cout << INSTXREF_MESSAGE_NOFS;
			break;
		case INSTXREF_ERROR_XFORMAT:				// The pseudo-Visual Studio 6 project file is not formatted correctly.
			cout << INSTXREF_MESSAGE_BADFS;
			break;
		default:									// An unknown error was encountered.
			cout << INSTXREF_MESSAGE_UKERROR;
			break;
		}

		// If an error occurred, return and signal error.
		return INSTXREF_RETURN_PROGERROR;
	}


	/* Scan each file for the text being searched for. */

	// The scan engine will cycle the scan process over each file to scan.
	ScanEngine(&FileList, dsScanParameters);

	// Return and signal success.
	return INSTXREF_RETURN_PROGSUCESS;
}

/* ClearParametersStructure - Reset the parameters structure to a neutral state. */
void ClearParametersStructure(DSPARAMS* dsParameters)
{
	// Set all elements of the parameter structure to null or default values.
	if ( dsParameters != NULL )
	{
		dsParameters->bIgnoreComments = false;
		dsParameters->bMatchCase = false;
		dsParameters->bPrefixMatching = false;
		dsParameters->szFilesetFile = NULL;
		dsParameters->szSearchPattern.Empty();
		dsParameters->uError = INSTXREF_ERROR_NOERROR;
	}
}

/* ExtractCommandLineParameters - Load a data structure with the command line parameters passed to the application. */
DSPARAMS ExtractCommandLineParameters(int argc, TCHAR* argv[])
{
	/* Local Variable Declarations and Initializations */

	CString szParameterValue;		// CString version of a parameter value used for comparison.

	DSPARAMS dsCommandLineParams;	// Structure holding command line parameters


	// Clear data structures before processing to ensure clean data.
	ClearParametersStructure(&dsCommandLineParams);


	// Verify that the minimum number of parameters were passed to the application.
	if ( argc < INSTXREF_PROPER_NUM_PARAMS )
	{
		// If less than the minimum number of parameters are passed, set an error signifying the parameters
		// are not formatted correctly and don't attempt to load the parameters.
		dsCommandLineParams.uError = INSTXREF_ERROR_XPARAMS;
	}
	else
	{
		/* If the proper number of values is passed, load each value to the proper structure value. */

		// Load the path to the list of files to scan.
		if ( VerifyFilePathExists(argv[1]) )
		{
			dsCommandLineParams.szFilesetFile = argv[1];
		}
		else
		{
			// If the fileset file does not exist, set an error signifying the parameters
			// are not formatted correctly.
			dsCommandLineParams.uError = INSTXREF_ERROR_XPARAMS;
		}

		// Load the flag specifying whether comments should be ignored during scanning.
		szParameterValue = argv[2];
		// If T is passed, set the parameter to TRUE.
		if ( szParameterValue == INSTXREF_TRUE_PARAMETER )
		{
			dsCommandLineParams.bIgnoreComments = true;
		}
		else
		{
			// If F is passed, set the parameter to FALSE.
			if ( szParameterValue == INSTXREF_FALSE_PARAMETER )
			{
				dsCommandLineParams.bIgnoreComments = false;
			}
			else
			{
				// If neither T or F is passed for this parameter, set an error flag.
				dsCommandLineParams.uError = INSTXREF_ERROR_XPARAMS;		
			}
		}

		// Load the flag specifying whether the exact case should be matched during scanning.
		szParameterValue = argv[3];
		// If T is passed, set the parameter to TRUE.
		if ( szParameterValue == INSTXREF_TRUE_PARAMETER )
		{
			dsCommandLineParams.bMatchCase = true;
		}
		else
		{
			// If F is passed, set the parameter to FALSE.
			if ( szParameterValue == INSTXREF_FALSE_PARAMETER )
			{
				dsCommandLineParams.bMatchCase = false;
			}
			else
			{
				// If neither T or F is passed for this parameter, set an error flag.
				dsCommandLineParams.uError = INSTXREF_ERROR_XPARAMS;		
			}
		}

		// Load the flag specifying whether the full word should be returned when a match is found.
		szParameterValue = argv[4];
		// If T is passed, set the parameter to TRUE.
		if ( szParameterValue == INSTXREF_TRUE_PARAMETER )
		{
			dsCommandLineParams.bPrefixMatching = true;
		}
		else
		{
			// If F is passed, set the parameter to FALSE.
			if ( szParameterValue == INSTXREF_FALSE_PARAMETER )
			{
				dsCommandLineParams.bPrefixMatching = false;
			}
			else
			{
				// If neither T or F is passed for this parameter, set an error flag.
				dsCommandLineParams.uError = INSTXREF_ERROR_XPARAMS;		
			}
		}

		// Load the string to search for in the files to scan.  No validation is performed.
		dsCommandLineParams.szSearchPattern = argv[5];
	}

	// Return the structure to the calling method.
	return dsCommandLineParams;
}

/* ExtractFilesToScan - Load the paths to the files to scan to a linked list. */
UINT ExtractFilesToScan(TCHAR* szFilesetFile, CList<CString, CString>* FileList)
{
	/* Local Variable Declarations and Initializations */

	int nStartIndex = 0;									// Index of a specific character in the fetched line of the file

	UINT uError = INSTXREF_ERROR_NOERROR;					// Return value specifying whether an error was encountered during processing

	TCHAR szFileLine[INSTXREF_LINE_BUFFER_SIZE] = {_T('\0')};	// Line of text fetched from the pseudo-Visual Studio 6 project file

	CString szSubstringFileLine;							// Object version of the fetched line used to extract the file path

	FILE* fFileSetFile = NULL;								// The file containing the list of files to scan


	// Open the control script file to as "Read-Only".
	fFileSetFile = fopen(szFilesetFile, _T("r"));
	if (fFileSetFile == (FILE *)NULL)
	{
		// If the file failed to open, exit the function a signal an error.
		uError = INSTXREF_ERROR_NOFILE;
	}
	else
	{
		// Extract the path of each file to scan from the fileset file until until EOF.
		while ( fgets(szFileLine, INSTXREF_LINE_BUFFER_SIZE, fFileSetFile) != NULL )
		{
			// Extract the file path from the line.
			szSubstringFileLine = szFileLine;
			nStartIndex = 0;
			nStartIndex = szSubstringFileLine.Find(TEXT("="), nStartIndex);
			szSubstringFileLine.Delete(0, nStartIndex+1);
			nStartIndex = szSubstringFileLine.Find(TEXT("\n"), nStartIndex);
			if ( nStartIndex != -1)
			{
				szSubstringFileLine.Delete(nStartIndex, 1);
			}

			// Add the file path to the list of files to scan.
			if ( FileList != NULL )
			{
				FileList->AddTail(szSubstringFileLine);
			}
		}
	}


	// Close the control script file.
	if ( fFileSetFile != NULL )
	{
		fclose (fFileSetFile);
	}

	return uError;
}

/* ScanEngine - Cycle through each file in a list, and run the scan process over each. */
UINT ScanEngine(CList<CString, CString>* FileList, DSPARAMS dsCommands)
{
	/* Local Variable Declarations and Initializations */

	bool bIsComment = false;							// Flag passed to, and received from, ParseLine specifying whether the text
														// currently being searched is part of a C-style comment

	TCHAR* szSearchPattern = NULL;						// Array of characters to search for in the files to scan
	TCHAR szLine[INSTXREF_LINE_BUFFER_SIZE] = {_T('\0')};	// Array of characters from files to scan to search
	TCHAR* szOutputMessage = NULL;						// String used to write messages to standard output

    CString szFilePath;									// Path to the file currently being searched

	FILE* fSearchFile = NULL;							// The current file to search

	POSITION currentFile = NULL;						// Pointer to the current file path in the file list.


	// Load the current search string to the active search string.
	szSearchPattern = NULL;
	szSearchPattern = dsCommands.szSearchPattern.GetBuffer(sizeof(dsCommands.szSearchPattern));

	// Only attempt to seach for the search string if the current search string is not blank.
	if ( szSearchPattern != NULL )
	{
		/* Loop through each file to scan. */

		// Get the position of the first file path in the list.
		currentFile = FileList->GetHeadPosition();

		// Run the scan process for each file in the list.
		while ( currentFile != NULL )
		{
			// Clear the file path to ensure clean data.
			szFilePath.Empty();

			// Get the path to the file to scan and increment to the position to the next file in the list.
			szFilePath = FileList->GetNext(currentFile);

			// If a path is retrieved, attempt to open the file.
			if ( !szFilePath.IsEmpty() )
			{
				// Verify the file to scan exits.
				if ( VerifyFilePathExists(szFilePath) )
				{
					// Open the file to search to as "Read-Only".
					fSearchFile = fopen(szFilePath.GetBuffer(sizeof(szFilePath)), _T("r"));
					if (fSearchFile == (FILE *)NULL)
					{
						// If the file failed to open, write an error to standard output.
						sprintf(szOutputMessage, TEXT("%s%s%s"), INSTXREF_MESSAGE_FILE, szFilePath, INSTXREF_MESSAGE_XEXIST);
						cout << szOutputMessage;
					}
					else
					{
						// Write the header message identifying the file being scanned to standard output.
						cout << TEXT("\n") << szFilePath + INSTXREF_MESSAGE_CURFILE;

						// If the file opened successfully, parse each line in the file until EOF.
						while ( fgets(szLine, INSTXREF_LINE_BUFFER_SIZE, fSearchFile) != NULL )
						{
							// If the control script specifies to ignore the case of szSearchPattern, 
							// convert all characters of both szLine and szSearchPattern to the same 
							// case before comparing the two strings.
							if(dsCommands.bMatchCase != true)
							{
								ConvertStringToLowerCase(szLine);
								ConvertStringToLowerCase(szSearchPattern);
							}

							// Search szLine for any instance of szSearchPattern.
							ParseLine(szFilePath.GetBuffer(sizeof(szFilePath)), szLine, szSearchPattern, &bIsComment, dsCommands.bIgnoreComments, dsCommands.bPrefixMatching);
						}
					}
				}
				else
				{
					// If the file does not exist, write a message stating so to standard output.
					cout << TEXT("\n") << szFilePath + INSTXREF_MESSAGE_XEXIST;
				}

				// Close the file before openning the next file.
				if ( fSearchFile != NULL )
				{
					fclose (fSearchFile);
				}
			}
		}
	}

	// Once each file is scanned, return and signal no error was encounterred.
	return INSTXREF_ERROR_NOERROR;
}

/* ConvertStringToLowerCase - Convert every character of a string to lower case. */
void ConvertStringToLowerCase(TCHAR* szString)
{
	/* Local Variable Declarations and Initializations */

	TCHAR *pStringCharacter;   // Pointer to the array of characters in szString

	if ( szString != NULL )
	{
		// Set pStringCharacter to the address of the first character in the string.
		pStringCharacter = szString;

		// Loop through szString character-by-character until the end of the string is reached.
		while(*pStringCharacter != _T('\0'))
		{
			// Convert the current character of szString to lower case.
			*pStringCharacter = (TCHAR) tolower(*pStringCharacter);
			// Increment the string pointer to the next character in szString.
			pStringCharacter++;

		}
	}
}

/* ParseLine - Search for the search text in a line of text character by character. */
UINT ParseLine(TCHAR* szSearchFile, TCHAR * szLine, TCHAR * szSearchString, bool* bIsComment, bool bIgnoreComments, bool bPrefixMatching)
{
   /* Local Variable Declarations and Initializations */

   TCHAR* pLineCharacter = NULL;			// Pointer to the array of characters in szLine
   TCHAR* pSearchStringCharacter = NULL;	// Pointer to the array of characters in szSearchString

   CString szMatchResult;					// The located text string


   // Only attempt to parse the string if both the line and search string have values.
   if ( (szLine != NULL) &&
	    (szSearchString != NULL) )
   {
	   // Set pointers to the address of the first character in the respective string.
	   pLineCharacter = szLine;
	   pSearchStringCharacter = szSearchString;

	   // Loop through szLine character-by-character until the end of the string is reached.
	   while(*pLineCharacter != _T('\0'))
	   {
		  // Clear the result strings to ensure clean data.
		  szMatchResult.Empty();

		  // If bIgnoreComments == true, then check if the text to search is part
		  // of a comment and ignore matches within comment text.  Otherwise, signal
		  // success if a match is found within comment text.
		  if (bIgnoreComments == true)
		  {
			 // If the current character and the next character are a C++ style comment, return without scanning 
			 // the line any further.
			 if ( (*pLineCharacter == _T('/')) && ((*(pLineCharacter+1)) == _T('/')) )
			 {
				 return INSTXREF_ERROR_CPPCOMMENT;
			 }

			 // Check the current character and the next character to determine if a comment is beginning.
			 if ( (*pLineCharacter == _T('/')) && ((*(pLineCharacter+1)) == _T('*')) )
			 {
				// If a comment is beginning, set a flag to specify that subsequent characters are part of a comment.
				*bIsComment = true;
				// Advance the line character forward two positions to the first character after the '/*' combination.
				pLineCharacter=pLineCharacter+2;
			 }

			 // If the current text is part of a comment, check the current character and 
			 // the next character to determine if the comment is ending.
			 if (*bIsComment == true)
			 {
				if ( (*pLineCharacter == _T('*')) && ((*(pLineCharacter+1)) == _T('/')) )
				{
				   // If a comment is ending, set a flag to specify that subsequent characters are not part of a comment.
				   *bIsComment = false;
				}
			 }
		  }

		  // Compare the two strings character-by-character to determine if szSearchString is a part of szLine.
		  if (*pSearchStringCharacter != _T('\0'))
		  {
			 // If szSearchString has not been fully matched, compare the current character of the two strings.
			 if (*pLineCharacter == *pSearchStringCharacter)
			 {
				// If the characters of both strings match, increment both strings to the next character.
				pLineCharacter++;
				pSearchStringCharacter++;
			 }
			 else
			 {
				// If the two characters do not match, increment szLine to the next character and reset 
				// pSearchStringCharacter to the beginning of szSearchString to start the search over starting 
				// at the new position of szLine.
				pLineCharacter++;
				pSearchStringCharacter = szSearchString;
			 }
		  }
		  else
		  {
			 // If szSearchString has been fully matched, extract the remaining elements of the matched item and log the match.
			 if (*bIsComment != true)
			 {
				// Load the matched string to the result string for display.
				szMatchResult = szSearchString;

				// If requested, extract the remaining characters of the matched string to return the full string (allows for full searching using prefix matching).
				if (bPrefixMatching == true)
				{
					// Grab the remaining characters until the end of the word is reached.
					while ( (*pLineCharacter != _T(' ')) &&
							(*pLineCharacter != _T(',')) &&
							(*pLineCharacter != _T(';')) &&
							(*pLineCharacter != _T(')')) &&
							(*pLineCharacter != _T('/')) &&
						    (*pLineCharacter != _T('\0')) &&
							(*pLineCharacter != _T('\t')) &&
							(*pLineCharacter != _T('\n')) &&
							(*pLineCharacter != NULL) )
					{
						szMatchResult += *pLineCharacter;
						pLineCharacter++;
					}
				}

				// Write the match details to the standard output.
				cout << szMatchResult << TEXT("\n");
			 }
			 else
			 {
				// If the match was made in text that is part of a comment, ignore the 
				// match and continue searching at the next position of szLine.
				pLineCharacter++;
			 }

			 // Reset the search string pointer to resume searching.
			 pSearchStringCharacter = szSearchString;
		  }
	   }
   }

   // If every character of szLine is searched and no match is made, return FALSE to
   // the calling method to signal szSearchString was not found in szLine.
   return INSTXREF_ERROR_NOERROR;
}
