/****************************************************************************
 *                                                                          *
 * File    : main.c                                                         *
 *                                                                          *
 * Purpose : Generic dialog based Win32 application.                        *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

/* 
 * Either define WIN32_LEAN_AND_MEAN, or one or more of NOCRYPT,
 * NOSERVICE, NOMCX and NOIME, to decrease compile time (if you
 * don't need these defines -- see windows.h).
 */

#define WIN32_LEAN_AND_MEAN
/* #define NOCRYPT */
/* #define NOSERVICE */
/* #define NOMCX */
/* #define NOIME */

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <tchar.h>
#include "main.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
////////////////////////////
#include <CommDlg.h> 
////////////////////////////

#define NELEMS(a)  (sizeof(a) / sizeof((a)[0]))


/** Prototypes **************************************************************/

static INT_PTR CALLBACK MainDlgProc(HWND, UINT, WPARAM, LPARAM);

/** Global variables ********************************************************/

static HANDLE ghInstance;

CHAR FileName[MAX_PATH];
HWND WinLook;
FILE* OpenedFile;
FILE* WritedFile;
signed short bufferData[1]; 

// pick a program file
INT ChooseProgram(void)
  { 
	OPENFILENAME  ofn;   
     memset(&ofn,0,sizeof(ofn));
    ofn.lStructSize     = sizeof(ofn);
    ofn.hwndOwner       = NULL;
    ofn.hInstance       = ghInstance;
    ofn.lpstrFilter     = "WAV Files\0*.wav\0\0";    
    ofn.lpstrFile       = FileName;
    ofn.nMaxFile        = MAX_PATH;
    ofn.lpstrTitle      = "Please Select A File To Open";
    ofn.Flags           = OFN_NONETWORKBUTTON |
                          OFN_FILEMUSTEXIST |
                          OFN_HIDEREADONLY;
    if (!GetOpenFileName(&ofn))
      return(0);
	  return 1; 
}




void FileConvert(FILE* OpenedFile){
float Formatted12 = 0.0f;
	
unsigned int counterWr=0;


    char text[20];
    GetWindowTextA(GetDlgItem(GetActiveWindow(), 4005), text, 20);
    double CoefMul = strtod(text,NULL);
    GetWindowTextA(GetDlgItem(GetActiveWindow(), 4007), text, 20);
    double TimeCoef = 1.0/strtod(text,NULL);


WritedFile = fopen("result.txt","w");
        fseek(OpenedFile, 0x2c, 0);
		if(fread(bufferData,2,1,OpenedFile)){
		
Formatted12 =(float)bufferData[0] * CoefMul;
		fprintf(WritedFile,"%f %f",(double)counterWr*TimeCoef,Formatted12);
		counterWr++;
	}
		


while (!feof(OpenedFile)){
		
	if(fread(bufferData,2,1,OpenedFile)){
	fprintf(WritedFile,"\n");
	Formatted12 = (float)bufferData[0] * CoefMul;
	fprintf(WritedFile,"%f %f",(double)counterWr*TimeCoef,Formatted12);
	counterWr++;
		}
	}

MessageBox(
        NULL,
		"File converted! See the result.txt file",
		"File converted",
        MB_ICONINFORMATION | MB_OK
    );

return;
}


/****************************************************************************
 *                                                                          *
 * Function: WinMain                                                        *
 *                                                                          *
 * Purpose : Initialize the application.  Register a window class,          *
 *           create and display the main window and enter the               *
 *           message loop.                                                  *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
    INITCOMMONCONTROLSEX icc;
    WNDCLASSEX wcx;

    ghInstance = hInstance;

    /* Initialize common controls. Also needed for MANIFEST's */
    /*
     * TODO: set the ICC_???_CLASSES that you need.
     */
    icc.dwSize = sizeof(icc);
    icc.dwICC = ICC_WIN95_CLASSES /*|ICC_COOL_CLASSES|ICC_DATE_CLASSES|ICC_PAGESCROLLER_CLASS|ICC_USEREX_CLASSES|... */;
    InitCommonControlsEx(&icc);

    /* Load Rich Edit control support */
    /*
     * TODO: uncomment one of the lines below, if you are using a Rich Edit control.
     */
    // LoadLibrary(_T("riched32.dll"));  // Rich Edit v1.0
    // LoadLibrary(_T("riched20.dll"));  // Rich Edit v2.0, v3.0

    /*
     * TODO: uncomment line below, if you are using the Network Address control (Windows Vista+).
     */
    // InitNetworkAddressControl();

    /* Get system dialog information */
    wcx.cbSize = sizeof(wcx);
    if (!GetClassInfoEx(NULL, MAKEINTRESOURCE(32770), &wcx))
        return 0;

    /* Add our own stuff */
    wcx.hInstance = hInstance;
    wcx.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDR_ICO_MAIN));
    wcx.lpszClassName = _T("Samples_Class");
    if (!RegisterClassEx(&wcx))
        return 0;

    /* The user interface is a modal dialog box */
    return DialogBox(hInstance, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)MainDlgProc);
}

/****************************************************************************
 *                                                                          *
 * Function: MainDlgProc                                                    *
 *                                                                          *
 * Purpose : Process messages for the Main dialog.                          *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static INT_PTR CALLBACK MainDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_INITDIALOG:
            /*
             * TODO: Add code to initialize the dialog.
             */
            return TRUE;

        case WM_SIZE:
            /*
             * TODO: Add code to process resizing, when needed.
             */
            return TRUE;

        case WM_COMMAND:
            switch (GET_WM_COMMAND_ID(wParam, lParam))
            {
                /*
                 * TODO: Add more control ID's, when needed.
                 */
                case IDOK:
                    EndDialog(hwndDlg, TRUE);
                    return TRUE;
				case 4002:
					 //     EndDialog(hwndDlg, TRUE);
 					if(ChooseProgram()) {
					WinLook = GetDlgItem(hwndDlg,4003);
					SetWindowText(WinLook,(LPCSTR)FileName);
                    OpenedFile = fopen(FileName,"rb");
					FileConvert(OpenedFile);
					;
                    return TRUE;
          			  }
                    }
 		break;

        case WM_CLOSE:
            EndDialog(hwndDlg, 0);
            return TRUE;

        /*
         * TODO: Add more messages, when needed.
         */
    
}
    return FALSE;
}

