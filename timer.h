#pragma once
#include <windows.h>    
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#define DATA_FILE "time_counter.dat"

//-----------------------
// Function Prototypes
//-----------------------
BOOL CALLBACK DlgProc(HWND, UINT, WPARAM,  LPARAM );
void DisplayTime(double);
void data_file_name(void);
void SaveTime( double * );
void LoadTime(void);
//void ShowContextMenu(HWND hWnd);  

//-----------------------
// Global Vartiables
//-----------------------  
//int counter;
TCHAR filename[MAX_PATH];
FILE * pfile;
DWORD StartTime;
double ElapsedTime, StoredTime;
char hh[5], mm[3], ss[3];
UINT_PTR main_timer_id, save_timer_id;
HINSTANCE hInstance;
HWND hwnd;
HICON hicon_on, hicon_off; 
// NOTIFYICONDATA niData; 
// Font related
HFONT hf;
HDC hdc;
HBRUSH greyed;
long lfHeight;



//-----------------------
// Resources
//-----------------------
// #define IDI_ICON_OFF 100
#define IDI_ICON_ON 1501
#define IDD_MAIN 1502
#define IDC_START 1511
// #define IDC_STOP  1512
#define IDC_RESET 1513
#define IDC_STATIC 1515
// #define IDC_CHECK1 1516
#define IDC_COLON1  1520
#define IDC_COLON2  1530
#define IDC_HH  1540
#define IDC_MM  1550
#define IDC_SS  1560

//#define HOOK_TRAY_ICON_ID 2001
//#define MY_TRAY_ICON_MESSAGE WM_APP+1
//#define SWM_HIDE WM_APP+2
//#define SWM_SHOW WM_APP+3
//#define SWM_DISABLE WM_APP+4
//#define SWM_ENABLE WM_APP+5
//#define ID_HELP WM_APP+6
//#define ID_ABOUT WM_APP+7         
// #define CLICKED 0
