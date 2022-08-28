// InstanceXRefConstants.h : Include file for constants defined for Instance Of Cross Reference application.
//


/* Application Processing Constants */

#define INSTXREF_PROPER_NUM_PARAMS	6				// The minimum number of parameters that must be passed to the application
#define INSTXREF_LINE_BUFFER_SIZE	300				// Maximum number of characters to read from a file at one time
#define INSTXREF_TRUE_PARAMETER		TEXT("T")		// Value to signify TRUE for a command line parameter
#define INSTXREF_FALSE_PARAMETER	TEXT("F")		// Value to signify FALSE for a command line parameter


/* Application Return Values */

#define INSTXREF_RETURN_PROGSUCESS	0x00			// The application completed without error
#define INSTXREF_RETURN_PROGERROR	0x01			// The application encountered an error


/* Error Codes */

#define INSTXREF_ERROR_NOERROR		0x00			// No error occured
#define INSTXREF_ERROR_XPARAMS		0x01			// The command line parameters were not formatted correctly
#define INSTXREF_ERROR_NOFILE		0x02			// The file being opened does not exist
#define INSTXREF_ERROR_XFORMAT		0x03			// The file data is not formatted correctly
#define INSTXREF_ERROR_CPPCOMMENT	0x04			// The remainder of the line is a C++ style comment


/* Console Display Text */

// Result Messages
#define INSTXREF_MESSAGE_CURFILE	TEXT(" is being scanned:\n")


/* Error Messages */

#define INSTXREF_MESSAGE_UKERROR	TEXT("An unknown error occured.")
// Parameter Format Error Messages
#define INSTXREF_MESSAGE_XPARAMS	TEXT("The parameters were not entered correctly.\nThe format is InstanceXRef [file listing the files to scan in VS6 format] [match case (T/F)] [ignore comments (T/F)] [run prefix matching (T/F)] [text to search for]\n")
// File Access Error Messages
#define INSTXREF_MESSAGE_FILE		TEXT("File ")
#define INSTXREF_MESSAGE_XEXIST		TEXT(" does not exist.")
// Fileset File Error Messages
#define INSTXREF_MESSAGE_BADFS		TEXT("The list of files to scan is not formatted correctly.\n")
#define INSTXREF_MESSAGE_NOFS		TEXT("The list of files to scan does not exist.\n")
