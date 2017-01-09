;stalker shadow of chernobyl 1.0006 protect
;darkz w24v@mail.ru 
;=======================================================================
format PE GUI 4.0
entry  main 
;=======================================================================
include 'win32ax.inc'	 ;*
include 'macro.inc'
include 'kglobals.inc'
include 'ReciveEvent.inc'
include 'SendEvent.inc'
include 'rc.inc'	;*
include 'xrproc.inc'
;=======================================================================


 
 
 
section '.code' code readable writable executable
main:	    
	  mov ebx,0x00400000
	  ;jmp start
	
	invoke Sleep,500
	
	invoke GetModuleHandle,NULL
	mov [hInstance],eax
	add eax,[eax+3Ch] 
	mov eax,[eax+50h] 
	mov [rSize],eax ;sizeofmodule
	
	invoke	 GetModuleFileName,NULL,CurrentDirectoryFile,CurrentDirectoryFile.size
	
	mov   [_PROCESSENTRY32.dwSize],sizeof.PROCESSENTRY32
	
	invoke CreateToolhelp32Snapshot, 0x2, NULL
	mov [hSnapshot], eax
	
	invoke Process32First,[hSnapshot], _PROCESSENTRY32
	jmp    @f
@next:
	invoke CloseHandle,   [hProcess]
	invoke Process32Next, [hSnapshot], _PROCESSENTRY32
	test eax,eax
	je quit
@@:	  
	invoke StrStrI, _PROCESSENTRY32.szExeFile,pName
	test eax, eax

je @next
	invoke OpenProcess,PROCESS_ALL_ACCESS,0,[_PROCESSENTRY32.th32ProcessID]
	mov  [hProcess],eax
	
	invoke GetProcessTimes,-1,_FILETIME,Buff,Buff,Buff
	invoke FileTimeToSystemTime,_FILETIME,_SYSTEMTIME

	movzx esi,[_SYSTEMTIME.wMinute]
	movzx edi,[_SYSTEMTIME.wSecond]

	invoke GetProcessTimes,[hProcess],_FILETIME,Buff,Buff,Buff
	invoke FileTimeToSystemTime,_FILETIME,_SYSTEMTIME

	movzx eax,[_SYSTEMTIME.wMinute]
	cmp eax,esi
jne @next

	movzx esi,[_SYSTEMTIME.wSecond]
	sub edi,esi
	cmp edi,5
ja @next 

       
 
alock_memory:
	invoke	VirtualAllocEx,[hProcess],0,[rSize],MEM_COMMIT+MEM_RESERVE,PAGE_EXECUTE_READWRITE
	mov	[RemoteThreadBaseAddress],eax
	test	eax,eax
	je	quit
	invoke	VirtualAlloc,NULL,[rSize],MEM_COMMIT+MEM_RESERVE,PAGE_EXECUTE_READWRITE  
	mov	[rAlock],eax 
	test	eax,eax
	je	quit
	invoke	RtlMoveMemory,[rAlock],[hInstance],[rSize]
	stdcall   RelocationImage,[hInstance],[rAlock],[RemoteThreadBaseAddress]
	mov	eax,[rAlock] 
	invoke	WriteProcessMemory,[hProcess],[RemoteThreadBaseAddress],eax,[rSize],Buff
	lea	eax,[start]
	sub	eax,[hInstance]
	mov	ecx,[RemoteThreadBaseAddress]
	add	eax,ecx 
	invoke	CreateRemoteThread,[hProcess],0,0x100000,eax,[RemoteThreadBaseAddress],0,Buff	
	invoke	CloseHandle,[hProcess]
	jmp	quit












	 







start:
	ACTCTX_FLAG_RESOURCE_NAME_VALID 	equ 00000008h
	ACTCTX_FLAG_HMODULE_VALID			equ 00000080h
	mov [hInstance],ebx
	invoke Sleep,2500
	mov [_ACTCTX.cbSize],sizeof.ACTCTX
	mov [_ACTCTX.dwFlags], ACTCTX_FLAG_RESOURCE_NAME_VALID or ACTCTX_FLAG_HMODULE_VALID	  
	mov [_ACTCTX.lpResourceName],1
	mov [_ACTCTX.lpApplicationName],0
	mov [_ACTCTX.lpSource],CurrentDirectoryFile
	mov eax,[hInstance]
	mov [_ACTCTX.hModule],eax
	invoke	CreateActCtx,_ACTCTX
	invoke	ActivateActCtx,eax,_ACTCTX
	invoke	SetPriorityClass,-1,REALTIME_PRIORITY_CLASS
	invoke	GetProcessAffinityMask, -1, lpProcessAffinityMask,lpSystemAffinityMask
     invoke	SetProcessAffinityMask ,-1, dword[lpSystemAffinityMask]   
	invoke DialogBoxParam,[hInstance],D_MAIN,0,dlg_proc,0
quit:	
	invoke TerminateProcess,-1,0
	
	
	
	
	
	
	

	
	
	
	
;=======================================================================
proc dlg_proc, hWnd, uMsg, wParam, lParam
iglobal
	lpProcessAffinityMask	dd	0
	lpSystemAffinityMask	dd	0
	formatd 			db '%d',0
	formath 			db '%08X',0
	formats 			db '%s',0
	sname				db 'name',0
	sip					db 'ip',0
	sping				db 'ping',0
	sfrags				db 'frags',0
	stime				db 'time',0
	snetwork				db 'network',0
	_HWND				dd 0
	hICON				dd 0
	hList				dd 0
	SHOW_HIDE				dd 0
	hList2				dd 0
	hMESSAGE				dd 0
	hMESSAGE_SEND			dd 0
	hIDC_LISTBOXCHAT		dd 0
	hIDC_CHECKFORUPDATES	dd 0
endg
push	ebx esi edi
		cmp [uMsg],WM_INITDIALOG
		je	.wminitdialog
		cmp [uMsg],WM_COMMAND
		je	.wmcommand
		cmp [uMsg],WM_CLOSE
		je	.wmclose
		cmp [uMsg],WM_NOTIFY
		je	.wmnotify
		cmp [uMsg],WM_NCLBUTTONDBLCLK
		je	.HIDE_PROC
		cmp [uMsg],WM_SHELLNOTIFY
		je	.SHOW_PROC
		xor  eax,eax
		jmp .finish
.wmnotify:
		;Указатель на NMHDR
		xor  eax,eax
		mov	esi,[lParam]
		mov	ebx,[hList]
		cmp	dword [esi+nml.hwndFrom],ebx
		je	.list_1 
		mov	ebx,[hList2]
		cmp	dword [esi+nml.hwndFrom],ebx 
		jne	.exit_true
		cmp	[esi+NMHDR.code],LVN_ITEMCHANGED
		je	.LVN_ITEMCHANGED
		cmp	[esi+_LV_KEYDOWN.wVKey],VK_UP
		je	.LVN_2_UP  
		cmp	[esi+_LV_KEYDOWN.wVKey],VK_DOWN
		je	.LVN_2_DOWN  
		jmp	.exit_true
.list_1:
		cmp	[esi+NMHDR.code],NM_RCLICK
		je	.LIST_1_R_CLICK
		cmp	[esi+NMHDR.code],NM_CLICK
		je	.LIST_1_L_CLICK
		; Сообщение NM_CUSTOMDRAW?
		cmp	dword [esi+nml.code],NM_CUSTOMDRAW
		jne  .exit_true
		; Пред-отрисовка строки?
		cmp  dword [esi+nml.dwDrawStage],CDDS_PREPAINT
		jne  @f
		; Установить ответ окна
		invoke	SetWindowLong,[hWnd],DWL_MSGRESULT,CDRF_NOTIFYITEMDRAW
		jmp	.exit_true
		@@:
		; Требуется нарисовать строку?
		cmp	dword [esi+nml.dwDrawStage],CDDS_ITEMPREPAINT
		jne	.exit_true
		; Получить номер строки
		stdcall GetClientByNum,dword[esi+nml.dwItemSpec] 
		or	eax,eax
		je	.exit_true
		xchg edi,eax		      
		;cmp  [edi+CLIENT.GamedataCRC],0x46E2424B   ;0x4B42E246 
		;je   @f
		;cmp  [esi+nml.dwItemSpec],0
		;je   @f
		;invoke MessageBox,0,0,0,0
		;mov	dword [esi+nml.clrText],0x00fb559f ;CRC 
		;@@:
		jmp	.exit_true
.wminitdialog:
		mov	eax,[hWnd]
		mov	[_HWND],eax
		invoke LoadIcon, [hInstance],17
		mov	[hICON],eax
		invoke	SendMessage,[hWnd],WM_SETICON,0,[hICON]
		invoke	SendMessage,[hWnd],WM_SETTEXT,0,chat_xray_version_msg+15
		invoke	GetDlgItem,[hWnd],IDC_LISTVIEW
		mov	[hList],eax  
		invoke	GetDlgItem,[hWnd],IDC_LISTVIEW2
		mov	[hList2],eax
		invoke	GetDlgItem,[hWnd],IDC_MESSAGE
		mov	[hMESSAGE],eax
		invoke	GetDlgItem,[hWnd],IDC_MESSAGE_SEND
		mov	[hMESSAGE_SEND],eax
		invoke	GetDlgItem,[hWnd],IDC_LISTBOXCHAT
		mov	[hIDC_LISTBOXCHAT],eax
		invoke	GetDlgItem,[hWnd],IDC_CHECKFORUPDATES
		mov	[hIDC_CHECKFORUPDATES],eax
		invoke	ShowWindow,[hList2],SW_HIDE
		invoke	ShowWindow,[hIDC_CHECKFORUPDATES],SW_HIDE
		invoke	ShowWindow,[hIDC_LISTBOXCHAT],SW_HIDE
		invoke	SendMessage,[hList2],LVM_SETEXTENDEDLISTVIEWSTYLE,0,LVS_EX_CHECKBOXES+LVS_EX_FULLROWSELECT
		mov [lvc.mask],LVCF_TEXT+LVCF_WIDTH+LVCF_SUBITEM+LVCF_FMT;
		mov [lvc.fmt],LVCFMT_LEFT		
		mov [lvc.cx],180 
		invoke SendMessage,[hList2],LVM_INSERTCOLUMN,0,lvc
		mov [lvi.mask],LVIF_TEXT+LVIF_STATE
		mov    esi,0x41
		@@:
		mov    eax,[WEAPONS+esi*4]
		mov    [lvi.pszText],eax
		invoke SendMessage,[hList2],LVM_INSERTITEM,0, lvi 
		dec    esi
		cmp    esi,-1
		jne    @b
		invoke RtlZeroMemory,lvi,sizeof.LV_ITEM
		;===============================================================================================
		;xrNetServer.IClient::_SendTo_LL+3A 
		;=============================================================================================== 
		invoke	 timeSetEvent,0x3E8,0x1000000,TimerProc,NULL,1	;0x3E8*3		    
		stdcall    INSERTCOLUMN,[hList],sname,sip,sping,sfrags,stime,snetwork,150,95,44,50,70,60
		invoke	 RtlInitializeCriticalSection,RTL_CRITICAL_SECTION
		stdcall    GetModuleAddress
		stdcall    JumpTo,[XR_WSAGetOverlappedResult],NULL,_WSAGetOverlappedResult,0
		stdcall    JumpTo,_WSAGetOverlappedResultEnd,5,[XR_WSAGetOverlappedResult],0
		stdcall    JumpTo,[XR_WSARecvFrom],NULL,_WSARecvFrom,0
		stdcall    JumpTo,_WSARecvFromEnd,5,[XR_WSARecvFrom],0
		stdcall    JumpTo,[xrNetServer.dll],0x9E97,AddClientClass,0
		stdcall    JumpTo,[xrNetServer.dll],0x9EBA,DelClientClass,1
		stdcall    JumpTo,DelClientClassEnd,[xrNetServer.dll],0x9EC0,0
		stdcall    JumpTo,[xrNetServer.dll],0x9FBF,DossProtect,0
		stdcall    JumpTo,DossProtectExit,[xrNetServer.dll],0x9FE1,0
		stdcall    JumpTo,DossProtectOut,[xrNetServer.dll],0x9FC4,0
		stdcall    JumpTo,[xrNetServer.dll],0x9F95,recieve_packet,0
		stdcall    JumpTo,[xrGame.dll],0x2060AB,Medic_1,1
		stdcall    JumpTo,Medic_1_Exit,[xrGame.dll],0x20629B,0
		stdcall    JumpTo,[xrGame.dll],0x35657C,Medic_2,1
		stdcall    JumpTo,Medic_2_Exit,[xrGame.dll],0x356562,0
		stdcall    JumpTo,[xrGame.dll],0x3564E5,Medic_3,1
		stdcall    JumpTo,Medic_3_Exit,[xrGame.dll],0x356562,0
		stdcall    JumpTo,[xr_3da.exe],0x4CDA8,SetHeight,1  
		stdcall    JumpTo,SetHeight_end,[xr_3da.exe],0x4CDAE,0 
		stdcall    JumpTo,[xrGame.dll],0x1A9293,ServerSendPacket,2
		stdcall    JumpTo,ServerSendPacketRet,[xrGame.dll],0x1A929A,0
		stdcall    JumpTo,ServerSendPacketBag,[xrGame.dll],0x1A9371,0
		stdcall    JumpTo,[xrGame.dll],0x35470C,EvEGameType,0
		stdcall    JumpTo,EvEGameTypeEnd,[xrGame.dll],0x354711,0
		stdcall    JumpTo,[xrGame.dll],0x35782A,EvEGameLevel,0
		stdcall    JumpTo,EvEGameLevelEnd,[xrGame.dll],0x35782F,0
		stdcall    JumpTo,[xrGame.dll],0x357097,EvECDKey,0
		stdcall    JumpTo,EvECDKeyEnd,[xrGame.dll],0x35709C,0
		stdcall    JumpTo,[xrGame.dll],0x2D6D51,EvEBattlEye,3
		stdcall    JumpTo,EvEBattlEyeEnd,[xrGame.dll],0x2D6D59,0
		stdcall    JumpTo,[xrGame.dll],0x21D3B6,RemoveHabarTimerA,1
		stdcall    JumpTo,RemoveHabarTimerAEnd,[xrGame.dll],0x21D3BB,0
		stdcall    JumpTo,[xrGame.dll],0x252B1B,RemoveHabarTimerB,0
		stdcall    JumpTo,RemoveHabarTimerBEnd,[xrGame.dll],0x252B20,0
		stdcall    JumpTo,[xrGame.dll],0x20CD57,RemoveHabarTimerC,1
		stdcall    JumpTo,RemoveHabarTimerCEnd,[xrGame.dll],0x20CD5D,0
		stdcall    JumpTo,[xrGame.dll],0x1C96E9,RemoveHabarTimerD,1
		stdcall    JumpTo,[xrGame.dll],0x221EC7,RemoveHabarTimerE,1
		stdcall    JumpTo,RemoveHabarTimerEEnd,[xrGame.dll],0x221ECD,0
		
		;invoke     ShowWindow,dword[0x00503C88+0xF0],SW_MINIMIZE
		stdcall    JumpTo,[xr_3da.exe],0x786F0,DrawConsole,1
		stdcall    JumpTo,DrawConsoleRet,[xr_3da.exe],0x786DE,0
		stdcall    JumpTo,DrawConsoleOK,[xr_3da.exe],0x786F6,0
		
		;---------------------------------------------------------------------------------------------------------------
		stdcall JumpTo,[xr_3da.exe],0x5A3A0,quit,0
		mov dword[0x00503FD4],1 ; Thread XZ
		mov	esi,[xrCore.dll]
		add	esi,0x1CC85
		invoke	VirtualProtect,esi,1,PAGE_EXECUTE_READWRITE,Buff
		invoke	RtlFillMemory,esi,1,0x90	
		mov ecx,[XR_CONSOLE]
		stdcall [XR_ADD_COMAND],addr_sv_rename
		mov ecx,[XR_CONSOLE]
		stdcall [XR_ADD_COMAND],addr_add_ip
		mov ecx,[XR_CONSOLE]
		stdcall [XR_ADD_COMAND],addr_del_ip
		mov ecx,[XR_CONSOLE]
		stdcall [XR_ADD_COMAND],addr_get_all_ip
		mov ecx,[XR_CONSOLE]
		stdcall [XR_ADD_COMAND],addr_sv_timehabar
.PluginFind:
iglobal
		MaskFind	db '*.plugin',0
		FileData	WIN32_FIND_DATA
		hFind		dd   0 
endg	     
		invoke FindFirstFile,MaskFind, FileData
		mov    [hFind],eax
		cmp	eax,INVALID_HANDLE_VALUE
		je	.PluginFindFileEnd
		lea	eax,[FileData.cFileName]
		invoke	LoadLibrary,eax
.PluginFindNextFile:
		lea	eax,[FileData]
		invoke	FindNextFile,[hFind],eax
		or	eax,eax
		jne	.PluginFindNextFile
.PluginFindFileEnd: 
		jmp .exit_true
.wmcommand:
		cmp [wParam],IDC_IMAGE	   
		je	.IDC_IMAGE
		cmp [wParam],IDC_CHECKFORUPDATES
		je	.IDC_CHECKFORUPDATES
		cmp [wParam],IDC_MESSAGE_SEND
		je	.message_send
		cmp [wParam],id_kicked
		je	.kicked
		cmp [wParam],id_banned
		je	.banned
		cmp [wParam],id_reconnect
		je	.reconnect
		cmp [wParam],id_sendmessage_by_id
		je	.sendmessage_by_id
		cmp [wParam],id_testers_mp_agroprom
		jb  @f
		cmp [wParam],id_testers_mp_workshop
		ja  @f
		mov eax,[wParam]
		movzx eax,al
		mov eax,[addr_agroprom+eax*4]	      
		stdcall [ChangeGameLevel],eax	 
		@@:
		cmp [wParam],id_dm
		jb  @f
		cmp [wParam],id_ah
		ja  @f
		mov eax,[wParam]
		movzx eax,al
		mov eax,[addr_deathmatch+eax*4]   
		stdcall [ChangeGameType],eax			    
		@@:
.exit_true:
		mov eax,TRUE  
		jmp .finish
.exit_false:
		mov eax,FALSE  
		jmp .finish
.wmclose:					   
		invoke	EndDialog,[hWnd],0
.finish:					  
		pop	edi esi ebx		      
		ret 
;================================================================================================
.IDC_IMAGE:
		cmp    [SHOW_HIDE],SW_SHOW
		je	   .SW_HIDE
		invoke	ShowWindow,[hList],SW_SHOW
		invoke	ShowWindow,[hMESSAGE],SW_SHOW
		invoke	ShowWindow,[hMESSAGE_SEND],SW_SHOW
		
		invoke	ShowWindow,[hList2],SW_HIDE
		invoke	ShowWindow,[hIDC_LISTBOXCHAT],SW_HIDE
		invoke	ShowWindow,[hIDC_CHECKFORUPDATES],SW_HIDE
		mov    [SHOW_HIDE],SW_SHOW
		jmp    .exit_true  
.SW_HIDE:		    
		invoke	ShowWindow,[hList],SW_HIDE
		invoke	ShowWindow,[hMESSAGE],SW_HIDE
		invoke	ShowWindow,[hMESSAGE_SEND],SW_HIDE
		
		invoke	ShowWindow,[hList2],SW_SHOW
		invoke	ShowWindow,[hIDC_LISTBOXCHAT],SW_SHOW
		invoke	ShowWindow,[hIDC_CHECKFORUPDATES],SW_SHOW
		mov    [SHOW_HIDE],SW_HIDE
		jmp    .exit_true 
.IDC_CHECKFORUPDATES:	    
		stdcall checkforupdates
		test	eax,eax
		je	@f
		invoke	SetDlgItemText,[hWnd],IDC_CHECKFORUPDATES,UPDATE_OK
		jmp    .exit_true
		@@:
		invoke	SetDlgItemText,[hWnd],IDC_CHECKFORUPDATES,UPDATE_BAG
		jmp    .exit_true
.LVN_ITEMCHANGED:
		mov	eax,[lParam]
		mov	dword ecx,[eax+_NML.iItem]
		cmp	ecx,0x41
		ja	.exit_true
		mov	eax,dword [eax+_NML.uNewState]
		shr	eax, 12
		test	eax,eax
		je	.exit_true
		cmp	eax,LVIS_SELECTED
		je	@f
		mov	eax,[WEAPONS+ecx*4]
		mov	byte [eax-2],0
		jmp	.exit_true
		@@:
		mov	eax,[WEAPONS+ecx*4]
		mov	byte [eax-2],1
		jmp	.exit_true
.LIST_1_L_CLICK:
		WM_UPDATEUISTATE = 0x0128
		UISF_HIDEFOCUS = 0x10001
		invoke	SendMessage,[hList],WM_UPDATEUISTATE,UISF_HIDEFOCUS,0
		jmp    .exit_true   
.LIST_1_R_CLICK: 
		invoke	SendMessage,[hList],LVM_GETSELECTEDCOUNT,0,0
		test eax,eax
		je   .exit_true 
		stdcall CreateMenuMouse,[hWnd]
		jmp    .exit_true
.LVN_2_UP:
		invoke SendMessage,[hList2],WM_VSCROLL ,SB_LINEUP,0
		jmp    .exit_true
.LVN_2_DOWN:
		invoke SendMessage,[hList2],WM_VSCROLL ,SB_LINEDOWN,0
		jmp    .exit_true
.HIDE_PROC:
iglobal
	PROC_SHOW_HIDE dd 0
endg		
		mov  [PROC_SHOW_HIDE],1
		mov	[node.cbSize],sizeof.NOTIFYICONDATA
		mov	eax,[hWnd]
		mov	[node.hWnd],eax
		mov	[node.uID],NULL
		mov	[node.uFlags],NIF_ICON+NIF_MESSAGE+NIF_TIP
		mov	[node.uCallbackMessage],WM_SHELLNOTIFY
		mov	eax,[hICON]
		mov	[node.hIcon],eax
		invoke Shell_NotifyIcon, NIM_ADD,node
		invoke SendMessage, [hWnd], WM_SYSCOMMAND, SC_MINIMIZE, 0
		invoke	ShowWindow,[hWnd],SW_HIDE
		jmp	.exit_true
.SHOW_PROC:
		cmp	[lParam],0x203
		jne	.exit_true
		mov  [PROC_SHOW_HIDE],0
		invoke SendMessage, [hWnd], WM_SYSCOMMAND, SC_RESTORE, 0
		invoke	ShowWindow,[hWnd],SW_SHOW
		invoke Shell_NotifyIcon, NIM_DELETE,node
		jmp	.exit_true
;================================================================================================
.message_send:	
	stdcall GetClientByNum,0
	or	eax,eax
	je	.exit_true
	xchg	edi,eax
	lea	esi,[edi+CLIENT.Buff]
	invoke RtlZeroMemory,esi,CLIENT.Buff.size
	invoke GetDlgItemText,[hWnd],IDC_MESSAGE,esi,250
	stdcall[ChatSend],esi
	jmp	.exit_true
.kicked: 
	invoke SendMessage,[hList],LVM_GETSELECTIONMARK,0,0
	cmp eax,-1
	je .exit_true
	or eax,eax
	je .exit_true
	stdcall GetClientByNum,eax
	or eax,eax
	je .exit_true
	xchg	edi,eax
	lea	esi,[edi+CLIENT.Buff]
	invoke RtlZeroMemory,esi,CLIENT.Buff.size
	invoke GetDlgItemText,[hWnd],IDC_MESSAGE,esi,250
	stdcall KickedByBattleEyE,dword[edi+CLIENT.ID],esi
	jmp .exit_true	
.banned:     
	invoke SendMessage,[hList],LVM_GETSELECTIONMARK,0,0
	cmp eax,-1
	je .exit_true
	or eax,eax
	je .exit_true
	stdcall GetClientByNum,eax
	or eax,eax
	je .exit_true
	xchg edi,eax
	stdcall AddIP,dword[edi+CLIENT.IP]
	lea esi,[edi+CLIENT.Buff]
	invoke RtlZeroMemory,esi,CLIENT.Buff.size
	invoke GetDlgItemText,[hWnd],IDC_MESSAGE,esi,250
	stdcall KickedByBattleEyE,dword[edi+CLIENT.ID],esi
	jmp .exit_true	
.reconnect: 
	invoke SendMessage,[hList],LVM_GETSELECTIONMARK,0,0
	cmp eax,-1
	je .exit_true
	or eax,eax
	je .exit_true
	stdcall GetClientByNum,eax
	or eax,eax
	je .exit_true
	xchg edi,eax
	lea esi,[edi+CLIENT.Buff]
	mov word[esi],0x001B
	stdcall SendTo,esi,2,dword[edi+CLIENT.ID]
	jmp .exit_true
.sendmessage_by_id:	   
	invoke SendMessage,[hList],LVM_GETSELECTIONMARK,0,0
	cmp eax,-1
	je .exit_true
	or eax,eax
	je .exit_true
	stdcall GetClientByNum,eax
	or eax,eax
	je .exit_true
	xchg edi,eax
	lea esi,[edi+CLIENT.Buff]
	invoke RtlZeroMemory,esi,CLIENT.Buff.size
	invoke GetDlgItemText,[hWnd],IDC_MESSAGE,esi,250
	stdcall CopyCoLLoRPacket,esi,0
	stdcall SendTo,esi,eax,dword[edi+CLIENT.ID]
	jmp .exit_true
endp
;============================================================================================================



















	
	
	



;***************************************************************************************************************
proc _WSARecvFrom,_SOCKET,_lpBuffers,_dwBufferCount,_lpNumberOfBytesSent,_dwFlags,_lpTo,_iToLen,_lpOverlapped,_lpCompletionRoutine	;stdcall eax,[_SOCKET],[_lpBuffers],[_dwBufferCount],[_lpNumberOfBytesSent],[_dwFlags],[_lpTo],[_iToLen],[_lpOverlapped],[_lpCompletionRoutine]
	stdcall _CALLWSARecvFrom,[_SOCKET],[_lpBuffers],[_dwBufferCount],[_lpNumberOfBytesSent],[_dwFlags],[_lpTo],[_iToLen],[_lpOverlapped],[_lpCompletionRoutine]
	or	eax,eax
	jne	_WSARecvFromRet
	mov	eax,[_lpTo]
	stdcall GetIP,dword[eax+4]
	or	eax,eax
	je	_WSARecvFromRet
	mov	ecx,[_lpNumberOfBytesSent]
	or	ecx,ecx
	je	_WSARecvFromRet
	or	eax,-1
	mov	dword[ecx],eax	
_WSARecvFromRet:	
	ret
_CALLWSARecvFrom:
	push ebp
     mov  ebp,esp
_WSARecvFromEnd:
	mov eax,0xFFFFFFFF
endp	
;***************************************************************************************************************
	











;***************************************************************************************************************
proc _WSAGetOverlappedResult,_SOCKET,_lpOverlapped,_lpcbTransfer,_fWait,_lpdwFlags
	stdcall _CALLWSAGetOverlappedResult,[_SOCKET],[_lpOverlapped],[_lpcbTransfer],[_fWait],[_lpdwFlags]
	mov	ecx,eax
	or	eax,eax
	je	.wsa_getoverlappedresult_ret
	mov	eax,[_lpcbTransfer]
	or	eax,eax
	je	.wsa_getoverlappedresult_ret	
	mov	eax,[eax+0x0C]		;ADDR SOCKADDR
	or	eax,eax
	je	.wsa_getoverlappedresult_ret
	stdcall GetIP,dword[eax+0x08]
	or	eax,eax
	je	.wsa_getoverlappedresult_ret
.wsa_set_zero:
	mov	eax,[_lpcbTransfer]
	mov	dword[eax],0
.wsa_getoverlappedresult_ret:
	mov	eax,ecx
	ret
_CALLWSAGetOverlappedResult:
	push ebp
     mov  ebp,esp
_WSAGetOverlappedResultEnd:
	mov eax,0xFFFFFFFF
endp
;***************************************************************************************************************







;***************************************************************************************************************
proc ___WSAGetOverlappedResult,_SOCKET,_lpOverlapped,_lpcbTransfer,_fWait,_lpdwFlags
	stdcall _CALLWSAGetOverlappedResult,[_SOCKET],[_lpOverlapped],[_lpcbTransfer],[_fWait],[_lpdwFlags]
	or	eax,eax
	je	.wsa_getoverlappedresult_ret
	mov	eax,[_lpcbTransfer]
	or	eax,eax
	je	.wsa_getoverlappedresult_ret	
	mov	ecx,[eax+0x0C]		;ADDR SOCKADDR
	or	ecx,ecx
	je	.wsa_getoverlappedresult_ret
	lea	ecx,[ecx+4]
	stdcall GetIP,dword[ecx+0x4]
	or	eax,eax
	jne	.wsa_set_zero
	mov	eax,[_lpcbTransfer]
	mov	eax,[eax+0x34]		;addr recv
	or	eax,eax
	je	.wsa_getoverlappedresult_ret
	cmp	byte [eax],0x7F
	je	.wsa_getoverlappedresult_ret
	cmp	byte [eax],0x3F
	je	.wsa_getoverlappedresult_ret  
	cmp	byte [eax],0x80
	je	.wsa_getoverlappedresult_ret
	cmp	byte [eax],0x88
	je	.wsa_getoverlappedresult_ret	   
	cmp	byte [eax],0xFE
	je	.wsa_getoverlappedresult_ret 
	cmp	byte [eax],0x39
	je	.wsa_getoverlappedresult_ret 
	cmp	byte [eax],0x3D
	je	.wsa_getoverlappedresult_ret
	cmp	dword [eax+0x15],'ToCo'
	je	.wsa_getoverlappedresult_ret 
	cmp	byte [eax],80
	je	.wsa_getoverlappedresult_ret
	stdcall GetClientBySADDR,ecx
	or	eax,eax
	jne	.wsa_getoverlappedresult_ret	
	stdcall AddIP,dword[ecx+0x4]
.wsa_set_zero:
	mov	eax,[_lpcbTransfer]
	mov	dword[eax],0
	mov	eax,[fs:+18h]
	mov	dword[eax+34h],0x2746
	;xor	eax,eax 
.wsa_getoverlappedresult_ret:
	ret
_CALLWSAGetOverlappedResult:
	push ebp
     mov  ebp,esp
_WSAGetOverlappedResultEnd:
	mov eax,0xFFFFFFFF
endp
;***************************************************************************************************************








;***************************************************************************************************************
proc AddCommand,addrcommand    
iglobal
	XR_ADD_COMAND					dd 0x0046ABB0
	XR_CONSOLE					= 0x00503BBC


	addr_sv_rename					dd addr_sv_rename_call,sv_rename,0x11111111,0
	addr_sv_rename_call				dd NULL,AddCommand
	sv_rename						db 'sv_rename',0
	
	
	TimeHabarRemove 			dd 0xEA60
	addr_sv_timehabar				dd addr_sv_timehabar_call,sv_timehabar,0x11111111,0
	addr_sv_timehabar_call			dd NULL,AddCommand
	sv_timehabar					db 'sv_removehabartime',0
	
	
	
	
	
	
	
	
	
	addr_add_ip				dd addr_add_ip_call,add_ip,0x11111111,0
	addr_add_ip_call			dd NULL,AddCommand
	add_ip					db 'add_ip',0
	addr_del_ip				dd addr_del_ip_call,del_ip,0x11111111,0
	addr_del_ip_call			dd NULL,AddCommand
	del_ip					db 'del_ip',0
	addr_get_all_ip 		dd addr_get_all_ip_call,get_all_ip,0x11111111,0
	addr_get_all_ip_call		dd NULL,AddCommand
	get_all_ip				db 'get_all_ip',0
	IPADDRBUFF				rb 10h
endg
	mov edi,[addrcommand]
	or  ecx,ecx
	je  .add_command_ret
	xchg esi,ecx
	mov eax,[esi+4]
	invoke StrStr,sv_rename,eax
	or  eax,eax
	jne .sv_rename
	mov eax,[esi+4]
	invoke StrStr,add_ip,eax
	or  eax,eax
	jne .add_ip
	mov eax,[esi+4]
	invoke StrStr,del_ip,eax
	or  eax,eax
	jne .del_ip
	mov eax,[esi+4]
	invoke StrStr,get_all_ip,eax
	or  eax,eax
	jne .get_all_ip
	mov eax,[esi+4]
	invoke StrStr,sv_timehabar,eax
	or  eax,eax
	jne .sv_timehabar
	jmp .add_command_ret 
	   
	   




.sv_rename:
	cmp byte[edi],0
	je  .add_command_ret
	movzx eax,byte[edi]
	xor eax,'0'
	cmp eax,1
	ja  .add_command_ret
	mov [no_rename],eax
	jmp .add_command_ret
.add_ip:	
	cmp byte[edi],0
	je  .add_command_ret 
	invoke RtlIpv4StringToAddress,edi,TRUE,IPADDRBUFF+4,IPADDRBUFF
	or eax,eax
	jne .add_command_ret
	stdcall AddIP,dword[IPADDRBUFF]
	jmp .add_command_ret
.del_ip:
	cmp byte[edi],0
	je  .add_command_ret 
	invoke RtlIpv4StringToAddress,edi,TRUE,IPADDRBUFF+4,IPADDRBUFF
	or eax,eax
	jne .add_command_ret
	stdcall DelIP,dword[IPADDRBUFF]
	jmp .add_command_ret
.get_all_ip:
	mov word[IPADDRBUFF],'! '
	xor esi,esi
	xor edi,edi
	dec esi
.get_all_ip_loop:
	inc esi
	cmp esi,MAXADDRIP
	jae .add_command_ret
	lea eax,[IPBUFF+esi*4]
	cmp [eax],edi
	je .add_command_ret
	invoke	RtlIpv4AddressToString,eax,IPADDRBUFF+2
	stdcall [xrCore.msg],IPADDRBUFF
	add esp,4
	jmp .get_all_ip_loop
.sv_timehabar:
	cmp byte[edi],0
	je  .add_command_ret
	stdcall StrToDexEAX,edi
	imul eax,eax,1000   ;eax = edx * 134h
	cmp	eax,0xEA60*3
	ja	.add_command_ret
	mov	[TimeHabarRemove],eax
	jmp	.add_command_ret
.add_command_ret:
	ret
endp  
;***************************************************************************************************************










	

	
	
	
	
	
	
	
	











;***************************************************************************************************************
proc AddIP,pIP
uglobal
	MAXADDRIP = 1000
	IPBUFF	   rb 0x4*MAXADDRIP
endg  
     push esi edi ebx ecx edx
     mov ebx,[pIP]
     xor eax,eax
     xor edx,edx
     xor ecx,ecx
     dec ecx
.add_ip_loop:
     inc ecx
     cmp ecx,MAXADDRIP
     jae .add_ip_ret
     cmp dword[IPBUFF+ecx*4],ebx
     je .ip_ok
     cmp dword[IPBUFF+ecx*4],edx
     jne .add_ip_loop
     mov dword[IPBUFF+ecx*4],ebx
.ip_ok:
     xchg eax,ebx
.add_ip_ret:
     pop  edx ecx ebx edi esi
     ret
endp
;***************************************************************************************************************

;***************************************************************************************************************
proc GetIP,pIP
     push esi edi ebx ecx edx
     mov ebx,[pIP]
     xor eax,eax
     xor edx,edx
     xor ecx,ecx
     dec ecx
.get_ip_loop:
     inc ecx
     cmp ecx,MAXADDRIP
     jae .get_ip_ret
     cmp dword[IPBUFF+ecx*4],edx
     je  .get_ip_ret
     cmp dword[IPBUFF+ecx*4],ebx
     jne .get_ip_loop
     xchg eax,ebx
.get_ip_ret:
     pop  edx ecx ebx edi esi
     ret
endp
;***************************************************************************************************************

;***************************************************************************************************************
proc DelIP,pIP
      push esi edi ebx ecx edx
      mov ebx,[pIP]
      xor eax,eax
      xor edx,edx
      xor ecx,ecx
      dec ecx
.del_ip_loop:
      inc ecx
      cmp ecx,MAXADDRIP
      jae .del_ip_ret
      cmp dword[IPBUFF+ecx*4],ebx
      jne .del_ip_loop
      mov dword[IPBUFF+ecx*4],edx
.del_loop:
      lea  esi,[IPBUFF+ecx*4]
      lea  edi,[IPBUFF+4+ecx*4]
      inc  ecx
      cmp  ecx,MAXADDRIP
      jae  .del_ip_ok
      mov  edx,[edi]
      mov  ebx,[esi]
      xchg [esi],edx
      xchg [edi],ebx
      jmp  .del_loop
.del_ip_ok:
      mov eax,[pIP]
.del_ip_ret:
      pop  edx ecx ebx edi esi
      ret
endp
;***************************************************************************************************************




			 


































;============================================================================================================
;---------------------------------------------------------------------------------------------------------------

iglobal
	ip_attack		db '! ip attack %s ',0
endg
DossProtect:
     lea edx,[esp+0x0C]
     push edx
     pushad
     xor eax,eax
     mov ecx,0x40
	       stdcall GetIP,dword[edx]
	       or eax,eax
	       jne DossProtectExit 
	       compare:
	       cmp byte[ebp+0x1CB],NULL
	       jne _xor
	       @@:
	       cmp dword eax,0x20
	       je Doss
	       inc eax
	       inc ebp
	       dec ecx
	       test ecx,ecx
	       jne compare
	       popad
	       DossProtectOut:
	       mov  eax,0xFFFFFFF
	       _xor:
	       xor eax,eax										    ;
	       jmp @b
Doss:
      stdcall AddIP,dword[edx]	  
      lea esi,[edx-0x31C]
      stdcall [xrCore.msg],ip_attack,esi
      add esp,8
      popad
      pop edx
DossProtectExit:
      mov  eax,0xFFFFFFF
;---------------------------------------------------------------------------------------------------------------



































;==================================================
iglobal
	Medic_Bag_1		db '! Medic_1',0
	Medic_Bag_2		db '! Medic_2',0
	Medic_Bag_3		db '! Medic_3',0
endg
;-----------------------------------------
Medic_1:
    stdcall [xrCore.msg],Medic_Bag_1
    add esp,4
Medic_1_Exit:
    mov  eax,0xFFFFFFF
;-----------------------------------------
Medic_2:
    stdcall [xrCore.msg],Medic_Bag_2
    add esp,4
Medic_2_Exit:
    mov  eax,0xFFFFFFF
;-----------------------------------------
Medic_3:
    stdcall [xrCore.msg],Medic_Bag_3
    add esp,4
Medic_3_Exit:
    mov  eax,0xFFFFFFF
;-----------------------------------------





;-----------------------------------------
RemoveHabarTimerA:
	mov	ecx,[TimeHabarRemove]
	mov	[esi+0x00000390],ecx
	cmp eax,[esi+0x00000390]
RemoveHabarTimerAEnd:
	mov  eax,0xFFFFFFF
;-----------------------------------------
RemoveHabarTimerB:
	cmp eax,[TimeHabarRemove]
RemoveHabarTimerBEnd:
	mov  eax,0xFFFFFFF
;-----------------------------------------
RemoveHabarTimerC:
	mov	ecx,[TimeHabarRemove]
	mov	[esi+0x000000B0],ecx
	cmp eax,[esi+0x000000B0]
RemoveHabarTimerCEnd:
	mov  eax,0xFFFFFFF
;-----------------------------------------
RemoveHabarTimerD:
	mov	dword[esi+0x000001C0],0xBB8
	cmp eax,[esi+0x000001C0]
	jna RemoveHabarTimerRetD
	mov byte[esi+0x000004D0],01
	mov eax,1
	pop esi
	ret  
RemoveHabarTimerRetD:
	xor	eax,eax
	pop	esi
	ret 
;-----------------------------------------
RemoveHabarTimerE:
	mov	ecx,[TimeHabarRemove]
	mov	[esi+0x00000490],ecx
	cmp eax,[esi+0x00000490]
RemoveHabarTimerEEnd:
	mov  eax,0xFFFFFFF
;-----------------------------------------
;==================================================

DrawConsole:
DrawConsoleA equ 0x4786DE
;==================================================
mov	ecx,[0x503B78]
mov	ecx,[ecx+0x45F4]
or	ecx,ecx
je	DrawConsoleRet
mov ecx,[0x503BBC]
DrawConsoleOK:
	mov  eax,0xFFFFFFF
DrawConsoleRet:
	mov  eax,0xFFFFFFF
;==================================================






































































;=======================================================================
SetHeight:
iglobal
	SEHxformat	db '! --------------REG_LOAD-------------',0xA,'--','xrGame.dll=%08X',0xA,'--','xrCore.dll=%08X',0xA,'--','xrNetServer.dll=%08X',0xA,'*','-----------------------',0xA,'--','EAX=%08X',0xA,'--','EBX=%08X',0xA,'--','ECX=%08X',0xA,'--','EDX=%08X',0xA,'--','ESI=%08X',0xA,'--','EDI=%08X',0xA,'--','EBP=%08X',0xA,'--','ESP=%08X',0xA,'--','EIP=%08X',0xA,'*','-----------------------',0xA,'! ------------REG_LOAD_END-----------',NULL    
endg
			push esi
			push edi
			push   SetHeight_thread_handler  ;Установим наш обработчик
			push   dword[fs:0]
			mov    dword[fs:0], esp 
			;--------------------------------------------------
			mov eax,[ecx]
			mov edx,[eax]
			call edx
			;--------------------------------------------------
SetHeight_thread_handler_end:
			pop  dword[fs:0]		;Убираем обработчик
			add  esp,4
			pop  edi
			pop  esi
SetHeight_end:
			mov  eax,0xFFFFFFF
			;-----------------------------------------------------
SetHeight_thread_handler:
			mov ecx,[esp+8];pFrame равно тому, что мы поместили в fs:[0]
			mov eax,dword[esp+0Ch];Context
			;-----------------------------------------------------------
			pushad
			mov esi,eax
			cinvoke wsprintf,Buff,SEHxformat,[xrGame.dll],[xrCore.dll],[xrNetServer.dll],dword[esi+0B0h],dword[esi+0A4h],dword[esi+0ACh],dword[esi+0A8h],dword[esi+0A0h],dword[esi+09Ch],dword[esi+0B4h],dword[esi+0C4h],dword[esi+0B8h]
			stdcall  [xrCore.msg],Buff
			add esp,4
			stdcall [xrFlushLog]
			popad
			;-----------------------------------------------------------
			mov dword[eax+0C4h],ecx;ESP
			mov dword[eax+0B8h],SetHeight_thread_handler_end;ThreadCont
			xor eax,eax ;Продолжить выполнение
			retn 4*4 
;=======================================================================
 


























































  












;======================================================================= 
section '.edata' export data readable
export 'xray.exe',\
	 AddIP,'AddIP',\
	 GetIP,'GetIP',\
	 DelIP,'DelIP',\
	 AddClientClass,'AddClientClass',\
	 DelClientClass,'DelClientClass',\
	 EvEHash,'EvEHash',\
	 EvEConnectOK,'EvEConnectOK',\
	 EvEChat,'EvEChat',\
	 EvEActor,'EvEActor',\
	 GetClientByNum,'GetClientByNum',\
	 GetClientCount,'GetClientCount',\
	 GetClientID,'GetClientID',\
	 SEvEGetCDKey,'SEvEGetCDKey',\			;Send
	 EvECDKey,'EvECDKey'					;Send
	 
	 
	 
	 
	 
	 
	 
include 'idata.inc'
include 'data.inc'
;=======================================================================
uglobal
Buff			rb 2500h
endg
IncludeAllGlobals
section '.rsrc' resource data readable
;-----------------------------------------------------------------------
  directory RT_DIALOG,dialogs,\
	    RT_MANIFEST,manifest,\
	    RT_BITMAP,bitmaps,\
	    RT_ICON,icons,\
	    RT_GROUP_ICON,group_icons	  
	
	
	
  resource  manifest,\
		 1,LANG_NEUTRAL,winxp
  resource  bitmaps,\
		 IDC_IMAGE,LANG_NEUTRAL,pict
  resource  icons,\
		   5,LANG_NEUTRAL,icon_data
  resource  group_icons,\
		   17,LANG_NEUTRAL,main_icon


  
resdata winxp
	    file 'winxpstyle2.xml'
endres	


bitmap pict,'bitmap.bmp'
icon main_icon,icon_data,'_1.ico'

;-----------------------------------------------------------------------
include "dialogs.tab" ;*
;-----------------------------------------------------------------------
include "dialogs.dat" ;*
;=======================================================================
;---------------------------------------------------------------------------------------------------------------------------
section '.reloc' fixups data readable writable discardable
