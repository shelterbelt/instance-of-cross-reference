// Majic Software File System Operations Dynamic Link Library
// Version 1.0.0.1

#ifndef __UTILFSO_H
#define __UTILFSO_H


// Constant Definitions
#define UTILFSO_NULL_CHAR		_T('\0')
#define UTILFSO_NULL_TEXT		TEXT("")
#define UTILFSO_PERIOD			TEXT(".")
#define UTILFSO_PERIOD_CHAR		_T('.')
#define UTILFSO_SLASH			TEXT("\\")
#define UTILFSO_SLASH_CHAR		_T('\\')
#define UTILFSO_SPACE			TEXT(" ")
#define UTILFSO_PATH_ALLFILES	TEXT("*.*")


// File Manipulation Methods
AFX_EXT_CLASS BOOL CopySpecificFile(CString szFilePath, CString szDestinationPath);
AFX_EXT_CLASS void DeleteAllFilesFromDirectory(CString szPath);
AFX_EXT_CLASS BOOL DeleteSpecificFile(CString szFilePath);

// File Name Manipulation Methods
AFX_EXT_CLASS CString BrowseForPath(CString szDialogCaption, HWND hWnd);
AFX_EXT_CLASS CString BrowseForFile(CString szDialogTitle, char BASED_CODE szFilter[], CWnd* wndParent);
AFX_EXT_CLASS CString ChangeFileExtension(CString szFileName, CString szNewExtension);
AFX_EXT_CLASS CString DetermineFileExtension(CString szFileName);
AFX_EXT_CLASS CString ExtractFileNameFromPath(CString szPath);
AFX_EXT_CLASS CString ExtractPathFromFilePath(CString szPath);
AFX_EXT_CLASS BOOL ExtractPathFromString(CString szLineToSearch, CString szPathIdentifier, CString szPathTerminator, CString *szFoundPath);
AFX_EXT_CLASS BOOL GetShortFormatPath(CString* szPath);
AFX_EXT_CLASS CString ValidatePath(CString szPath);
AFX_EXT_CLASS BOOL VerifyFilePathExists(CString szFilePath);
AFX_EXT_CLASS BOOL IsDirectoryEmpty(CString szPath);

// Process and Thread Methods
AFX_EXT_CLASS BOOL ExecuteProcess(CString szExecutable, CString szArguments, BOOL bVisible);


#endif /* __UTILFSO_H */