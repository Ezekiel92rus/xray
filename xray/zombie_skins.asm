	format PE GUI 4.0 DLL
	include 'win32ax.inc'
	include 'kglobals.inc'
	include 'MACRO.INC'



section '.code' code readable writable executable
 main:
 mov	eax,[DllEntryPoint]

proc DllEntryPoint hinstDLL,fdwReason,lpvReserved
	mov eax, [fdwReason]
	.if eax = DLL_PROCESS_ATTACH
	invoke CreateThread,0,0x100000, LoadBaseHook, NULL ,0,ipThreadId
	invoke DisableThreadLibraryCalls, [hinstDLL]
	.endif
	mov	eax,TRUE
	ret
endp





proc LoadBaseHook

iglobal
	ipThreadId		dd	0
	ZombieMode		db	'! Load plugin Zombie Mode',0
endg
	invoke	Sleep,5000
	stdcall GetModuleAddress
	stdcall JumpTo,[xrGame.dll],0x356B78,EvE_zombie,1
	stdcall JumpTo,EvE_zombie_end,[xrGame.dll],0x356B7E,0
	stdcall [xrCore.msg],ZombieMode
	add	esp,4
	ret
endp
;--------------------------------------------------------------------------------------------------------------------
EvE_zombie:
iglobal
	 skin_ex_1		      db      'actors\stalker_mp\stalker_killer_head_1',0
	 skin_ex_2		      db      'actors\stalker_mp\stalker_killer_antigas',0
	 skin_ex_3		      db      'actors\stalker_mp\stalker_killer_head_3',0
	 skin_ex_4		      db      'actors\stalker_mp\stalker_killer_mask',0
	 skin_ex_5		      db      'actors\stalker_mp\stalker_sv_balon_10',0
	 skin_ex_6		      db      'actors\stalker_mp\stalker_sv_hood_9',0
	 skin_ex_7		      db      'actors\stalker_mp\stalker_sv_rukzak_3',0
	 skin_ex_8		      db      'actors\stalker_mp\stalker_sv_rukzak_2',0
	 skin_ex_9		      db      'actors\stalker_mp\stalker_killer_mask_de',0
	 skin_ex_10		      db      'actors\stalker_mp\stalker_killer_mask_us',0
	 skin_ex_11		      db      'actors\stalker_mp\stalker_killer_mask_fr',0
	 skin_ex_12		      db      'actors\stalker_mp\stalker_killer_mask_uk',0

	 skin_exz_1		      db      'actors\bandit\stalker_bandit_2',0
	 skin_exz_2		      db      'actors\bandit\stalker_bandit_3',0
	 skin_exz_3		      db      'actors\bandit\stalker_bandit_master',0
	 skin_exz_4		      db      'actors\bandit\stalker_bandit_drunk',0
	 skin_exz_5		      db      'actors\bandit\stalker_bandit_veteran',0
	 skin_exz_6		      db      'actors\bandit\stalker_bandit_borov',0
	 skin_exz_7		      db      'actors\stalker_zombi\stalker_zombie1',0
	 skin_exz_8		      db      'actors\stalker_zombi\stalker_zombie2',0
	 skin_exz_9		      db      'actors\stalker_zombi\stalker_zombie3',0
	 skin_exz_10		      db      'actors\stalker_zombi\stalker_zombie4',0
	 skin_exz_11		      db      'actors\stalker_zombi\stalker_zombie5',0
	 skin_exz_12		      db      'actors\stalker_zombi\stalker_zombie6',0

	 skin_addr_ex_1 	      dd      skin_ex_1
	 skin_addr_ex_2 	      dd      skin_ex_2
	 skin_addr_ex_3 	      dd      skin_ex_3
	 skin_addr_ex_4 	      dd      skin_ex_4
	 skin_addr_ex_5 	      dd      skin_ex_5
	 skin_addr_ex_6 	      dd      skin_ex_6
	 skin_addr_ex_7 	      dd      skin_ex_7
	 skin_addr_ex_8 	      dd      skin_ex_8
	 skin_addr_ex_9 	      dd      skin_ex_9
	 skin_addr_ex_10	      dd      skin_ex_10
	 skin_addr_ex_11	      dd      skin_ex_11
	 skin_addr_ex_12	      dd      skin_ex_12

	 skin_addr_exz_1	      dd      skin_exz_1
	 skin_addr_exz_2	      dd      skin_exz_2
	 skin_addr_exz_3	      dd      skin_exz_3
	 skin_addr_exz_4	      dd      skin_exz_4
	 skin_addr_exz_5	      dd      skin_exz_5
	 skin_addr_exz_6	      dd      skin_exz_6
	 skin_addr_exz_7	      dd      skin_exz_7
	 skin_addr_exz_8	      dd      skin_exz_8
	 skin_addr_exz_9	      dd      skin_exz_9
	 skin_addr_exz_10	      dd      skin_exz_10
	 skin_addr_exz_11	      dd      skin_exz_11
	 skin_addr_exz_12	      dd      skin_exz_12

	 skin_add_byte_to_end	      db      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x64,0x65,0x66,0x61,0x75,0x6C,0x74,0x00,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x00,0x24,0x65,0x64,0x69,0x74,0x6F,0x72,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00
	 skin_count  = 12
endg
	pushad
	lea	edi,[esp+0x98]
	cmp	dword[edi+2],'mp_a'
	jne	.skin_loop_end
	mov	ecx,100h
.skin_actor_loop:
	add	edi,1
	dec	ecx
	je	.skin_loop_end
	cmp	dword[edi],'acto'
	jne	.skin_actor_loop
.skin_loop_ok:
	mov	ecx,-1
.skin_loop:
	add	ecx,1
	cmp	ecx,skin_count
	je	.skin_loop_end
	mov	ebx,[skin_addr_ex_1+ecx*4]
	mov	edx,[skin_addr_exz_1+ecx*4]
	stdcall _lstrcmp,edi,ebx
	or	eax,eax
	je	.skin_loop
	stdcall _lstcpy,edi,edx
	add	edi,eax
	stdcall _memcpy,edi,skin_add_byte_to_end,skin_add_byte_to_end.size
.skin_loop_end:
EvE_zombie_ret:
	popad
	mov ecx,[edi+0x40A8]
EvE_zombie_end:
	mov	eax,0xFFFFFFFF
;--------------------------------------------------------------------------------------------------------------------











































;***************************************************************************************************************
proc JumpTo,ModuleBase,ModuleOffset,ToJump,Nops
;-----------------------------------------------------------------------------------------------------------
locals
  _lpflOldProtect dd 0
  _jump dd 0xE9,NULL
endl
       mov eax,[ModuleBase]
       cmp word[eax],'MZ'
je @f
       mov esi,[ModuleOffset]
       add esi,[ToJump]
       mov edi,[ModuleBase]
       sub esi,5
       sub esi,edi
       mov [_jump+1],esi
       jmp JumpToEnd
@@:
       mov edi,[ModuleBase]
       add edi,[ModuleOffset]
       mov esi,[ToJump]
       sub esi,5
       sub esi,edi
       mov [_jump+1],esi
JumpToEnd:
       invoke VirtualProtect,edi,1,PAGE_EXECUTE_READWRITE,addr _lpflOldProtect
       invoke RtlMoveMemory,edi,addr _jump,5
       add edi,5
       invoke RtlFillMemory,edi,[Nops],0x90
      ret
endp						   
;***************************************************************************************************************

;***************************************************************************************************************
proc GetModuleAddress
iglobal
	xrNetServer				db 'xrNetServer.dll',0
	xrGame					db 'xrGame.dll',0
	xrCore					db 'xrCore.dll',0
	xray.exe				dd 0
	xrNetServer.dll 			dd 0
	xrGame.dll				dd 0
	xrCore.dll				dd 0
	xr_3da.exe				dd 0
	xrCore.msg				dd 0
endg
	mov [xr_3da.exe],0x00400000
	invoke GetModuleHandle,xrNetServer
	mov [xrNetServer.dll],eax
	invoke GetModuleHandle,xrGame
	mov [xrGame.dll],eax
	invoke GetModuleHandle,xrCore
	mov [xrCore.dll],eax
	mov ecx,eax
	add eax,0x18640
	mov [xrCore.msg],eax
.get_base:
	mov	esi,[xrNetServer.dll]
	add	esi,0x9F95
	mov	edi,[esi+1]
	add	edi,5
	add	esi,edi
.get_base_b:
	dec	esi
	cmp	dword[esi],'mode'
	jne	.get_base_b
.get_base_a:
	dec	esi
	cmp	word[esi],'MZ'
	jne	.get_base_a
	mov	[xray.exe],esi
	ret
endp
;***************************************************************************************************************

;***************************************************************************************************************

proc GetADDRExportByName,hModule,sName
     push    ebx ecx edx esi edi
     xor     edx,edx
     mov     esi, [hModule]
     mov     edi,esi
	 add esi, [esi+0x3C] ; Start of PE header
	 mov esi, [esi+0x78] ; RVA of export dir
	 add esi, edi
	 mov ebx, [esi+0x14] ; NumberOfFunctions
	 or  ebx,ebx
	 je  .get_export_ret
.get_export_loop:
	 mov edx,[esi+0x1C]
	 add edx,[hModule]
	 mov edx,[edx+ebx*4-4]
	 add edx,edi		;ADDR
	 mov ecx,[esi+0x24]	  ;RVA of EOT
	 add ecx,edi
	 movzx ecx,word[ecx+ebx*2-2]
	 mov eax, [esi+0x20]
	 add eax, edi
	 mov eax, [eax+ecx*4]
	 add eax,edi
	 stdcall _lstrcmp,eax,[sName]
	 or  eax,eax
	 jne .get_export_ret
	 xor edx,edx
	 dec ebx
	 jne .get_export_loop
.get_export_ret:
	 mov eax,edx
     pop     edi esi edx ecx ebx
     ret
endp
;***************************************************************************************************************

;***************************************************************************************************************
proc _memcpy,lpStrA:dword,lpStrB:dword,lpSize:dword
     push    ecx esi edi
     mov     edi,[lpStrA]
     mov     esi,[lpStrB]
     mov     ecx,[lpSize]
	dec	ecx
     JNG	   .memcpy_loop_ret  
.memcpy_loop:
     mov     al,[esi+ecx]
     mov     [edi+ecx],al
     dec     ecx
     jnl     .memcpy_loop
.memcpy_loop_ret:
     mov  eax,[lpSize]
     pop  edi esi ecx
     ret
endp
;***************************************************************************************************************

;***************************************************************************************************************
proc _lstcpy,lpStrA:dword,lpStrB:dword
     push    ecx esi edi
     mov     edi,[lpStrA]
     mov     esi,[lpStrB]
     mov     ecx,-1
.lstrcpy_loop:
     inc     ecx
     mov     al,[esi+ecx]
     mov     [edi+ecx],al
     or      al,al
     jne     .lstrcpy_loop
.lstrcpy_ret:
     xchg eax,ecx
     pop  edi esi ecx
     ret
endp
;***************************************************************************************************************

;***************************************************************************************************************
proc _lstrcmp,lpStrA:dword,lpStrB:dword
     push    edx ecx esi edi
     mov     edi,[lpStrA]
     mov     esi,[lpStrB]
     xor     eax,eax
     xor     edx,edx
     mov     ecx,-1
.lstrcmp_loop:
     inc     ecx
     mov     dl,[esi+ecx]
     cmp     [edi+ecx],dl
     jne     .lstrcmp_ret
     or      edx,edx
     jne     .lstrcmp_loop
.lstrcmp_ok:
     xchg    eax,ecx
.lstrcmp_ret:
     pop  edi esi ecx edx
     ret
endp
;***************************************************************************************************************

 .end main


IncludeAllGlobals
section '.reloc' fixups data writable discardable