/*
 * ================================================================================
 *
 *       Filename:  timer.c
 *
 *    Description:  
 *
 *        Version:  3.0
 *        Created:  01/13/2014 
 *       Compiler:  mingw32-g++ 
 *
 * ================================================================================
 */
#include "timer.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
	                  LPSTR lpCmdLine, int nCmdShow
	                )
{
  MSG msg;
  // Run app only once
  if (FindWindow(NULL, LPCSTR("SEM Filament Timer"))) return 0;
  
  // Set time data storage in %TEMP% directory
  data_file_name();
 
  // Load previously saved data or zero the timer
  LoadTime();

  // Create dialog main window 
  // Set icon, title and clock dislay.
  hwnd = CreateDialog(  GetModuleHandle(NULL), 
                        MAKEINTRESOURCE(IDD_MAIN), NULL, 
                        DlgProc );
  //hicon_off = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_OFF));
  hicon_on = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_ON));
  greyed = CreateSolidBrush(RGB(0xA5, 0xC0, 0xA4));
  // greyed = CreateSolidBrush(RGB(0xd4, 0xd0, 0xc8));
  // greyed = CreateSolidBrush(RGB(0xC0, 0xC0, 0xC0));
  SendMessage (hwnd, WM_SETICON, (WPARAM) ICON_SMALL, (LPARAM) hicon_on);
  SetWindowText(hwnd, LPCSTR("SEM Filament Timer"));
  // Set font in clock dislay.
  hdc = GetDC(NULL);
  lfHeight = -MulDiv(36, GetDeviceCaps(hdc, LOGPIXELSY), 72);
  ReleaseDC(NULL, hdc);
  hf = CreateFont(lfHeight, 0, 0, 0, 900, 0, 0, 0, 0, 0, 0, 0, 0,"Times New Roman");
  SendDlgItemMessage(hwnd, IDC_HH, WM_SETFONT, (WPARAM)hf, TRUE);
  SendDlgItemMessage(hwnd, IDC_MM, WM_SETFONT, (WPARAM)hf, TRUE);
  SendDlgItemMessage(hwnd, IDC_SS, WM_SETFONT, (WPARAM)hf, TRUE);
  SendDlgItemMessage(hwnd, IDC_COLON1, WM_SETFONT, (WPARAM)hf, TRUE);
  SendDlgItemMessage(hwnd, IDC_COLON2, WM_SETFONT, (WPARAM)hf, TRUE);
  SetDlgItemText(hwnd,IDC_COLON1,":");
  SetDlgItemText(hwnd,IDC_COLON2,":");
  DisplayTime(StoredTime);
  //  CheckDlgButton(hwnd, IDC_CHECK1, BST_CHECKED); 
  ShowWindow(hwnd, nCmdShow); // SW_SHOW);
  UpdateWindow(hwnd);
  // SetWindowLong(hwnd, GWL_STYLE, 0);  // Remove all decorations

  while(GetMessage(&msg, NULL, 0, 0))
      {
          if(!IsDialogMessage(hwnd, &msg))
          {
              TranslateMessage(&msg);
              DispatchMessage(&msg);
          }
      }
  return 0;
}

/////////////////////////////////////////////////////////////////////////////
//  CallBack routine 
/////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK DlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
  int buttonID;              //int  event;
	switch(Message)
	{
    case WM_CLOSE: 
        SaveTime(&ElapsedTime);
        // Sleep(1000);
        DestroyWindow(hwnd); 
        break;
    case WM_DESTROY:
        // Save timer data before closing window         
        //Shell_NotifyIcon(NIM_DELETE, &niData);
        DestroyWindow(hwnd);
        PostQuitMessage(0);
        break;
    case WM_TIMER:
            if(wParam == main_timer_id)
            {     //call display time routine
                ElapsedTime = StoredTime + (GetTickCount() - StartTime)/1000.0f;
                DisplayTime(ElapsedTime);
            }
            if(wParam == save_timer_id)
            {   // save time in file every 160 seconds
                SaveTime(&ElapsedTime);
            }
          break;

    case WM_COMMAND:
        {
          buttonID = LOWORD (wParam);
          // event = HIWORD (wParam);
          switch (buttonID)
          {
            case IDC_START:                       // Start/stop button
                  if (!main_timer_id)             // If stoped
                  {
                    SendDlgItemMessage(hwnd, IDC_START,   // change button text
                                       WM_SETTEXT, 0, 
                                       (LPARAM)"Stop");
                    main_timer_id = SetTimer(hwnd, 1, 1000, (TIMERPROC) NULL);
                    StartTime = GetTickCount();
                    ElapsedTime = StoredTime + (GetTickCount() - StartTime)/1000.0f;
                    if (!save_timer_id)         //start the save metronome
                    {
                      save_timer_id = SetTimer(hwnd, 2, 160000, (TIMERPROC) NULL);
                    }
                  }
                  else
                  {
                    if(KillTimer(hwnd, main_timer_id)) 
                    {
                      SendDlgItemMessage(hwnd, IDC_START, 
                                         WM_SETTEXT, 0, 
                                         (LPARAM)"Start");
                      main_timer_id = 0;
                      StoredTime = ElapsedTime;
                      SaveTime(&ElapsedTime);
                      KillTimer(hwnd, save_timer_id);
                      save_timer_id = 0;
                    }
                  }
                    SetDlgItemText(hwnd,IDC_COLON1,":");
                    SetDlgItemText(hwnd,IDC_COLON2,":");
                    DisplayTime(ElapsedTime);
                  break; 

            case IDC_RESET:                //reset counter and delete data 
                    StoredTime = 0.0;
                    ElapsedTime = 0.0;
                    StartTime = GetTickCount();
                    DisplayTime(ElapsedTime);
                    SaveTime(&ElapsedTime);
                    break; 
          } // -- end switch buttonID
        } // -- end case WM_COMMAND:
        break;  

    case WM_CTLCOLORSTATIC:  // grey digits when stoped
        {
          if (
              (HWND)lParam != GetDlgItem(hwnd, IDC_COLON1)
                              &&
              (HWND)lParam != GetDlgItem(hwnd, IDC_COLON2)
             )
          {
          HDC hdcStatic = (HDC)wParam;
          if (!main_timer_id)
          {
            SetTextColor(hdcStatic, RGB(0xAB, 0xA8, 0xA7));
          }
          else
          {
            SetTextColor(hdcStatic, RGB(0, 0, 0));
          }
          SetBkColor(hdcStatic, RGB(0xA5, 0xC0, 0xA4));
          //  Other colours
          //  SetTextColor(hdcStatic, RGB(0xCF, 0xD9, 0xFF));
          //  SetBkColor(hdcStatic, RGB(0xd4, 0xd0, 0xc8));
          //  SetBkColor(hdcStatic, RGB(0xC0, 0xC0, 0xC0)); //win98 grey
          return (INT_PTR)greyed;
          }
        }
    default:	return FALSE;
  } // -- end switch message
  return 0;
}

void DisplayTime(double time)
{
    sprintf(ss, "%02.0f", fmod(time,60.0)); 
    sprintf(mm, "%02.0f", floor(fmod(time,3600.0)/60.0)); 
    sprintf(hh, "%03.0f", floor(time/3600.0)); 
    SetDlgItemText(hwnd,IDC_SS, (LPCSTR) ss);
    SetDlgItemText(hwnd,IDC_MM, (LPCSTR) mm);
    SetDlgItemText(hwnd,IDC_HH, (LPCSTR) hh);
    return;
}

void SaveTime(double * time)
{
  pfile = fopen(filename, "wb");
  if (pfile != NULL)
  {
    fwrite(time, sizeof(double), 1, pfile);
    fclose(pfile);
  }
}

void LoadTime()
{
  pfile = fopen(filename, "rb");
  if (pfile != NULL)
  {
    fread(&StoredTime, sizeof(double), 1, pfile);
    ElapsedTime = StoredTime;     /* In case it exits before start counting */
    fclose(pfile);
  }
  else
  {
    StoredTime = 0.0;
  }
}

void data_file_name(void)
{
  // DWORD dwRetVal = 0;
  TCHAR tmp_path[MAX_PATH];
  GetTempPath(MAX_PATH, tmp_path);
  //if (dwRetVal > MAX_PATH || (dwRetVal == 0))
  sprintf(filename, "%s%s", tmp_path, DATA_FILE);
}
