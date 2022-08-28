// InstanceXRef.h : Include file for the Instance Of Cross Reference application.
//


/* Structure Definitions */

// Command Line Parameters Structure
typedef struct
{
	TCHAR* szFilesetFile;		// Path to the pseudo-Visual Studio 6 file containing the list of files to evaluate
	bool bMatchCase;			// Flag specifying whether the exact case of the search string should be matched
	bool bIgnoreComments;		// Flag specifying whether text in comments should be ignored
	bool bPrefixMatching;		// Flag specifying whether the search text identifies only the beginning of a string to search, and the remaining characters in the word when a match is identified should be returned
	CString szSearchPattern;	// The text to attempt to find in the files to evaluate
	UINT  uError;				// Flag specifying whether an error occured while loading the structure values
} DSPARAMS, *LPDSPARAMS;


/* Application Global Methods */

void ClearParametersStructure(DSPARAMS* dsParameters);
DSPARAMS ExtractCommandLineParameters(int argc, TCHAR* argv[]);
UINT ExtractFilesToScan(TCHAR* szFilesetFile, CList<CString, CString>* FileList);
UINT ScanEngine(CList<CString, CString>* FileList, DSPARAMS dsCommands);
void ConvertStringToLowerCase(TCHAR* szString);
UINT ParseLine(TCHAR* szSearchFile, TCHAR * szLine, TCHAR * szSearchString, bool* bIsComment, bool bIgnoreComments, bool bPrefixMatching);
