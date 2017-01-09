




;=====================================================================
ServerSendPacket:
	mov eax,[esp+08]
	cmp byte[eax],8
	jne ServerSendPacketOk
	cmp byte[eax+06],0x22
	jne ServerSendPacketOk
	stdcall VisualChreck								
	or eax,eax											
	je	ServerSendPacketOk								
	stdcall GetClientByNum,0
	or	eax,eax
	je	ServerSendPacketBag
	xor eax,eax
ServerSendPacketBag:
	mov  eax,0xFFFFFFF
ServerSendPacketOk:
	cmp dword [esi+0x0000439C],00
ServerSendPacketRet:
	mov  eax,0xFFFFFFF
;=====================================================================













;*************************************************************************************************************** 
proc VisualChreck
	push   ebx ecx edx esi edi
	;--------------------------------------------------
		push   visual_handler  ;Установим наш обработчик
		push   dword[fs:0]
		mov    dword[fs:0], esp 
		;--------------------------------------------------
		mov esi,[xrGame.dll]
		cmp dword[esi+0x53C520],1
		je  .chreck_visual_loop_ok
		xor esi,esi
.chreck_visual_loop:
		stdcall GetClientByNum,esi
		or	eax,eax
		je	.chreck_visual_loop_ok
		mov  edi,[eax+CLIENT.ADDR]
			;------------------------------
			mov eax,[edi+0x4160]
			cmp  byte [eax+0x63],4
			jne .chreck_visual_loop_next
			mov  eax,[edi+0x0000414C]
			or   eax,eax
			je   .chreck_visual_loop_next
			cmp  dword[eax+0x71],'ROTC'
			jne  .chreck_visual_loop_next
			cmp  byte [eax+0x220],3
			mov  byte [eax+0x220],0
			je   .chreck_visual_loop_bag
				;------------------------------
.chreck_visual_loop_next:
	add  esi,1
	jmp  .chreck_visual_loop
.chreck_visual_loop_ok:    
		xor eax,eax
		jmp chreck_visual_loop_end
.chreck_visual_loop_bag:
		mov eax,1
chreck_visual_loop_end:
		pop  dword[fs:0]                ;Убираем обработчик
		add  esp,4
		pop  edi esi edx ecx ebx
		ret
            ;-----------------------------------------------------
visual_handler:
            mov ecx,[esp+8];pFrame равно тому, что мы поместили в fs:[0]
            mov eax,dword[esp+0Ch];Context
            mov dword[eax+0C4h],ecx;ESP
            mov dword[eax+0B8h],chreck_visual_loop_end;ThreadCont
            xor eax,eax ;Продолжить выполнение
            retn 4*4 
endp
;*************************************************************************************************************** 





























;=====================================================================
EvEGameType:
	lea esi,[esp+0x18+2]
	pushad
 	invoke lstrlen,esi
	inc	eax
	cmp	eax,GameType.size
	ja	@f
	invoke RtlMoveMemory,GameType,esi,eax
	@@:
	popad
	lea eax,[esp+0x18]
	push eax
EvEGameTypeEnd:
	mov  eax,0xFFFFFFF
;=====================================================================






;=====================================================================
EvEGameLevel:
	pushad
	lea esi,[esi+2]
	invoke lstrlen,esi
     inc	eax
     cmp	eax,GameLevel.size
     ja	@f
     invoke RtlMoveMemory,GameLevel,esi,eax
	@@:
	popad
	push 0x08
	mov edx,esi
	push edx
EvEGameLevelEnd:
	mov  eax,0xFFFFFFF
;=====================================================================



;=====================================================================
EvECDKey:
	lea ecx,[esp+0x14]
	push ecx
	cmp  byte[ecx],0x15
	jne	EvECDKeyEnd	
	mov  dword[ecx],0x00010017
EvECDKeyEnd:
	mov  eax,0xFFFFFFF
;=====================================================================




;=====================================================================
EvEBattlEye:
	lea ecx,[esp+0x14-0xC]
	cmp dword[ecx+0x8],1
	jne @f
	mov dword[ecx+0x8],0
	@@:
	cmp [edx],ebx
	push ebx
	push 0x00000188
EvEBattlEyeEnd:
	mov  eax,0xFFFFFFF
;=====================================================================





























;********************************************************************************************
proc SCallRet,pClass,pData,pSize
     
     ;invoke MessageBox,0,'Unknown','Unknown',0
     
     xor eax,eax
	ret
endp
;********************************************************************************************









;********************************************************************************************
proc SEvEGetCDKey,pClass,pData,pSize
	push    ebx ecx edx esi edi
			mov  edi,[pClass]
			mov  esi,[pData]
			mov  dword[esi],0x00010017
.get_key_ret:	
			xor  eax,eax
	pop     edi esi edx ecx ebx
	ret
endp
;********************************************************************************************

;********************************************************************************************
proc SEvEGetAuth,pClass,pData,pSize
	push    ebx ecx edx esi edi
              mov  edi,[pClass]
              mov  esi,[pData]
              xor  eax,eax
	pop     edi esi edx ecx ebx
	  ret
endp
;********************************************************************************************

;********************************************************************************************
proc SEvEAutoChreck,pClass,pData,pSize
	push    ebx ecx edx esi edi
			mov  edi,[pClass]
			mov  esi,[pData]
			xor  eax,eax
	pop     edi esi edx ecx ebx
	  ret
endp
;********************************************************************************************

;********************************************************************************************
proc SEvEGameType,pClass,pData,pSize
iglobal
	GameType    rb 50h
endg
	push    ebx ecx edx esi edi
		mov  edi,[pClass]
		mov  esi,[pData]
		add  esi,2
		invoke lstrlen,esi
		inc	eax
		cmp	eax,GameType.size
		ja	.game_type_ret
		invoke RtlMoveMemory,GameType,esi,eax
.game_type_ret:
		xor  eax,eax
		pop     edi esi edx ecx ebx
	  ret
endp
;********************************************************************************************

;********************************************************************************************
proc SEvEGameLevel,pClass,pData,pSize
iglobal
	GameLevel   rb 50h
endg
	push    ebx ecx edx esi edi
              mov  edi,[pClass]
              mov  esi,[pData]
              add  esi,2
              invoke lstrlen,esi
              inc	eax
              cmp	eax,GameLevel.size
              ja	.game_level_ret
              invoke RtlMoveMemory,GameLevel,esi,eax
.game_level_ret:
              xor  eax,eax
	pop     edi esi edx ecx ebx
	  ret
endp
;********************************************************************************************

;********************************************************************************************
proc SEvEXZ,pClass,pData,pSize
	push    ebx ecx edx esi edi
			xor  eax,eax
	pop     edi esi edx ecx ebx
	ret
endp
;********************************************************************************************

;********************************************************************************************
proc SEvESpawnObject,pClass,pData,pSize
	push    ebx ecx edx esi edi
			xor  eax,eax
	pop     edi esi edx ecx ebx
	ret
endp
;********************************************************************************************

;********************************************************************************************
proc SEvEXZ_1,pClass,pData,pSize
	push    ebx ecx edx esi edi
			xor  eax,eax
	pop     edi esi edx ecx ebx
	ret
endp
;********************************************************************************************

;********************************************************************************************
proc SEvESpectator,pClass,pData,pSize
	push	ebx ecx edx esi edi
		mov  edi,[pClass]
		mov  esi,[pData]
.spectator_ret:
		xor  eax,eax
	pop	edi esi edx ecx ebx
	ret
endp
;********************************************************************************************

;********************************************************************************************
proc SEvEXZ_2,pClass,pData,pSize
	push    ebx ecx edx esi edi
			xor  eax,eax
	pop     edi esi edx ecx ebx
	ret
endp
;********************************************************************************************

;********************************************************************************************
proc SEvEGameEvE,pClass,pData,pSize
	push    ebx ecx edx esi edi
			xor  eax,eax
	pop     edi esi edx ecx ebx
	ret
endp
;********************************************************************************************







