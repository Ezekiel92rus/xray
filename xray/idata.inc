section '.idata' import data readable writeable

  library kernel32,'kernel32.dll',\
          user32,'user32.dll',\
          shell32,'shell32.dll',\
          Winmm,'Winmm.dll',\
          WININET,'WININET.dll',\
          ntdll,'ntdll.dll'

  import kernel32,\
         ActivateActCtx,'ActivateActCtx',\
         CloseHandle,'CloseHandle',\
         CreateActCtx,'CreateActCtxA',\
         CreateFile,'CreateFileA',\
         CreateRemoteThread,'CreateRemoteThread',\
         CreateToolhelp32Snapshot,'CreateToolhelp32Snapshot',\
         FileTimeToSystemTime,'FileTimeToSystemTime',\
         FindFirstFile,'FindFirstFileA',\
         FindNextFile,'FindNextFileA',\
         GetModuleFileName,'GetModuleFileNameA',\
         GetModuleHandle,'GetModuleHandleA',\
         GetProcAddress,'GetProcAddress',\
         GetProcessAffinityMask,'GetProcessAffinityMask',\
         GetProcessTimes,'GetProcessTimes',\
         LoadLibrary,'LoadLibraryA',\
         OpenProcess,'OpenProcess',\
         Process32First,'Process32First',\
         Process32Next,'Process32Next',\
         RtlFillMemory,'RtlFillMemory',\
         RtlMoveMemory,'RtlMoveMemory',\
         RtlZeroMemory,'RtlZeroMemory',\
         SetPriorityClass,'SetPriorityClass',\
         SetProcessAffinityMask,'SetProcessAffinityMask',\
         Sleep,'Sleep',\
         TerminateProcess,'TerminateProcess',\
         VirtualAlloc,'VirtualAlloc',\
         VirtualAllocEx,'VirtualAllocEx',\
         VirtualFree,'VirtualFree',\
         VirtualProtect,'VirtualProtect',\
         WriteFile,'WriteFile',\
         WriteProcessMemory,'WriteProcessMemory',\
         lstrcpy,'lstrcpyA',\
         lstrlen,'lstrlenA'

  import user32,\
         AppendMenu,'AppendMenuA',\
         CreatePopupMenu,'CreatePopupMenu',\
         DialogBoxParam,'DialogBoxParamA',\
         EndDialog,'EndDialog',\
         GetCursorPos,'GetCursorPos',\
         GetDlgItem,'GetDlgItem',\
         GetDlgItemText,'GetDlgItemTextA',\
         LoadIcon,'LoadIconA',\
         SendMessage,'SendMessageA',\
         SetDlgItemText,'SetDlgItemTextA',\
         SetWindowLong,'SetWindowLongA',\
         ShowWindow,'ShowWindow',\
         TrackPopupMenu,'TrackPopupMenu',\
         wsprintf,'wsprintfA'

  import shell32,\
         Shell_NotifyIcon,'Shell_NotifyIconA',\
         StrStr,'StrStrA',\
         StrStrI,'StrStrIA'

  import Winmm,\
         timeSetEvent,'timeSetEvent'

  import WININET,\
         InternetCloseHandle,'InternetCloseHandle',\
         InternetOpen,'InternetOpenA',\
         InternetOpenUrl,'InternetOpenUrlA',\
         InternetReadFile,'InternetReadFile'

  import ntdll,\
         RtlEnterCriticalSection,'RtlEnterCriticalSection',\
         RtlInitializeCriticalSection,'RtlInitializeCriticalSection',\
         RtlIpv4AddressToString,'RtlIpv4AddressToStringA',\
         RtlIpv4StringToAddress,'RtlIpv4StringToAddressA',\
         RtlLeaveCriticalSection,'RtlLeaveCriticalSection'