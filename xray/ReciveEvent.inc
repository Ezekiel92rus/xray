iglobal
	xkicked_by_server		db 'Kicked By Server',0
endg
;===================================================================================================================
AddClientClass:
	pushad
	invoke RtlEnterCriticalSection,RTL_CRITICAL_SECTION
     popad     
     lea eax,[ecx+4]
     stdcall ReNamePlayer,eax
     push ecx
     mov  ecx,ebx
     call edx
     test eax,eax
     je   add_client_class_ret
     xchg edi,eax
     stdcall AddClientID,dword[edi+0x40A8] ;ID
     or	eax,eax
     je   add_client_class_ret
     xchg esi,eax
     mov  [esi+CLIENT.ADDR],edi
     mov  eax,[edi+0x40A8+0x94] 
     mov  [esi+CLIENT.IP],eax
     lea  ebx,[esi+CLIENT.StrIP]
     lea  eax,[edi+0x40A8+0x94]    ;IP 
	invoke   RtlIpv4AddressToString,eax,ebx
     mov  eax,[edi+0x412C]
     lea  eax,[eax+0x0C]
     lea  ebx,[esi+CLIENT.StrNAME]
     invoke   RtlMoveMemory,ebx,eax,CLIENT.StrNAME.size-1
     mov  eax,[edi+0x40A8]
     lea  ebx,[esi+CLIENT.StrID]
     cinvoke wsprintf,ebx,formath,eax
     mov  eax,[edi+0x4028+0x118]	;port
     mov  [esi+CLIENT.PORT],eax
     lea  ebx,[esi+CLIENT.StrPort]
     cinvoke wsprintf,ebx,formatd,eax
     mov  eax,[edi+0x4148]
     mov  [esi+CLIENT.NETADDR],eax
     
     



     lea  eax,[esi+CLIENT.Buff]
     mov  [esi+CLIENT.addr_buff],eax
     lea  eax,[esi+CLIENT.MessageBuff]
     mov  [esi+CLIENT.addr_message_buff],eax
     lea  eax,[esi+CLIENT.StrNAME]
     mov  [esi+CLIENT.addr_player_name],eax
     lea  eax,[esi+CLIENT.StrIP]
     mov  [esi+CLIENT.addr_player_str_ip],eax
     
     
     

iglobal
	GetHash	db 0x15,0x00,0
endg
	
	;stdcall	SendTo,GetHash,GetHash.size,[esi+CLIENT.ID]


add_client_class_ret:
	pushad
	invoke RtlLeaveCriticalSection,RTL_CRITICAL_SECTION 
     popad
     xor eax,eax
     pop edi
     pop esi
     pop ebx
     mov esp,ebp
     pop ebp
     ret 0008
;===================================================================================================================
DelClientClass:   
     pushad
     invoke RtlEnterCriticalSection,RTL_CRITICAL_SECTION
     stdcall DelClientID,dword[esi+0x4]
     invoke RtlLeaveCriticalSection,RTL_CRITICAL_SECTION 
     popad
     lea ebx,[ebx]
DelClientClassEnd:
     mov  eax,-1
;===================================================================================================================

 
 
 
 
 
 
 
 
 
 

 
 
 


;===================================================================================================================	   
recieve_packet:
	stdcall CheckPacket,esi,edi,ebx,ebx
recieve_ret:
	xor eax,eax
	pop edi
	pop esi
	pop ebx
	mov esp,ebp
	pop ebp
	ret 0008
;===================================================================================================================


  
  
  
  








;********************************************************************************************
proc CheckPacket,pData,pID,pClass,pNetClass      ;10                 ;20                 ;30                   ;40 
iglobal                      ;0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7             pCallByte            db 0x0,0,0,9,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,3,0,0,4,0,0,0,0,1,2,0,8,8,0,8,7,0,0,0,0,0,0,0,0,0,0,5            ;db  0xD,0xC,0xB,9,0xA,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,3,0,0,4,0,0,0,0,1,2,0,8,8,0,8,7,0,0,0,0,0,0,0,0,0,0,5
	pCallByte			db 0x0,0,0,0,0,0,0,0,6,0,7,0,4,0,0,0,0,0,0,0,0,0,3,0,2,5,0,0,0,0,0,1,0,0,0,0,0,0,0,0,8,0,0,0,0,0,0,0,NULL
	pCALL_RET			dd pCallRet ;0                    
	pEvECRC             dd EvECRC   ;1
 	pEvESync            dd EvESync  ;2 
 	pEvEHash 			dd EvEHash  ;3
 	pEvEConnectOK		dd EvEConnectOK ;4
 	pEvEChat			dd EvEChat	 ;5
 	pGameEvent          dd EvEGameEvent ;6
 	pActor			dd EvEActor	 ;7
 	pAdmin			dd EvEAdmin	 ;8
endg    
;==================================================================================================================================
.packet_start:
	push	  ebx ecx edx esi edi
	;---------------------------------------------------
	stdcall GetClientID,[pID]
	or	eax,eax
	je	.bag_packet
	mov 	[pClass],eax
	stdcall pCompressData,[pClass],[pData]
	or	eax,eax
	jne	.bag_packet
	;---------------------------------------------------
			mov    esi,[pData]
			movzx  edi,word[esi+1]    ;eax edx esi edi
			lea    edi,[edi+esi+4]
			add    esi,6
.packet_loop:
			movzx eax,byte[esi]
			movzx ecx,word[esi-2]
			cmp	 eax,0x2F
			ja  	.bag_packet
			cmp   ecx,0x125
			ja	.bag_packet
				movzx  eax,byte[eax+pCallByte]
				stdcall dword [pCALL_RET+eax*4],[pClass],esi,ecx
				or eax,eax
				jne .bag_packet
				stdcall pRecivePacket,esi,ecx,[pID],[pNetClass]
	lea    esi,[esi+ecx+2]
	cmp    edi,esi
ja   .packet_loop
	xor  eax,eax
	jmp  packet_ret
.bag_packet:
	mov eax,1
packet_ret:
	pop  edi esi edx ecx ebx
	ret
endp   
;********************************************************************************************


















;********************************************************************************************
proc pCallRet,pClass,pData,pSize
     
     ;invoke MessageBox,0,'Unknown','Unknown',0
     
     xor eax,eax
	ret
endp
;********************************************************************************************


;********************************************************************************************
proc pCompressData,pClass,pData
	push    ebx ecx edx esi edi
		mov edi,[pClass]
		mov esi,[pData]                   
			mov	eax,[esi]
			cmp	al,0xE1
			jne	.compress_data
			rol	eax,8
			cmp	al,0xC0
			jne	.compress_data
			movzx eax,word[esi+1]
			cmp 	eax,0x2000
			jae	.compress_data
				xor eax,eax
				mov  byte[edi+CLIENT.PacketEvE],al
				mov  byte[edi+CLIENT.PacketADD],al
				mov  byte[edi+CLIENT.MedickADD],al
				mov  byte[edi+CLIENT.DropADD]  ,al
				mov  byte[edi+CLIENT.WeaponADD],al
				mov	byte[edi+CLIENT.EvEClickADD],al
				mov  dword[edi+CLIENT.PacketTime],eax
.compress_ok:
		xor eax,eax
		jmp .compress_ret
.compress_data:
		stdcall KickedByBattleEyE,dword[edi+CLIENT.ID],xkicked_by_server	
		mov eax,1
.compress_ret:
	pop	edi esi edx ecx ebx
	ret
endp
;********************************************************************************************





;***************************************************************************************************************
proc EvECRC,pClass,pData,pSize
     push    ebx ecx edx esi edi
		mov	esi,[pData]
		mov	edi,[pClass]
		;invoke MessageBox,0,'EvECRC','EvECRC',0
		mov	ecx,[esi+2]
		mov	[edi+CLIENT.GamedataCRC],ecx ;cmp ecx,0x46E2424B
		xor 	eax,eax
	pop	edi esi edx ecx ebx
	ret
endp
;***************************************************************************************************************

;***************************************************************************************************************
proc EvESync,pClass,pData,pSize
     push    ebx ecx edx esi edi
		mov	esi,[pData]
		mov	edi,[pClass]
		;invoke MessageBox,0,'EvESync','EvESync',0
		xor	eax,eax
	pop	edi esi edx ecx ebx
	ret
endp
;***************************************************************************************************************

;***************************************************************************************************************
proc EvEHash,pClass,pData,pSize    
	push	ebx ecx edx esi edi
		mov	esi,[pData]
		mov	edi,[pClass]
			
			;invoke MessageBox,0,'EvEHash','EvEHash',0
			
			lea	eax,[esi+2]
			lea	ebx,[edi+CLIENT.StrHASH] 
			invoke RtlMoveMemory,ebx,eax,20h
	xor	eax,eax
	pop	edi esi edx ecx ebx
	ret
endp
;***************************************************************************************************************

;********************************************************************************************
proc EvEConnectOK,pClass,pData,pSize
	push    ebx ecx edx esi edi
		mov  edi,[pClass]
		mov  esi,[pData]
		mov  byte [edi+CLIENT.pConnectOK],1
		xor	eax,eax
.connect_ret:
	pop     edi esi edx ecx ebx
	  ret
endp
;********************************************************************************************

;********************************************************************************************
proc EvEChat,pClass,pData,pSize
iglobal       
      chat_reconnect_msg		db 0x1B,0x00,0
      chat_xray_version_msg	db '%c[0,255,255,0]ReVeRsEd',0
endg  
	push   ebx ecx edx esi edi
		mov    esi,[pData]
		lea    esi,[esi+4]
		stdcall ReNamePlayer,esi
		lea edi,[esi+eax+1]
		mov byte[esi+eax],':'
		stdcall ReNamePlayer,esi
.chat_player_msg:
		cmp dword[edi],'!ver'
		je  .ver
		cmp dword[edi],'!rec'
		je  .rec
		cmp dword[edi],'!adm'
		je  .adm
		jmp .chat_ok
.ver:
		mov edi,[pClass]
		stdcall CopyCoLLoRPacket,[edi+CLIENT.addr_message_buff],chat_xray_version_msg
		stdcall SendTo,[edi+CLIENT.addr_message_buff],eax,[edi+CLIENT.ID]
		jmp .chat_bag	
.rec:
		mov edi,[pClass]
		stdcall SendTo,chat_reconnect_msg,chat_reconnect_msg.size,[edi+CLIENT.ID]
		jmp .chat_bag
.adm:
		mov 	edi,[pClass]
		mov	eax,[edi+CLIENT.IP]
		bswap eax
		and 	eax,0x000000FF
		cmp	eax,0xA4
		jne	.chat_bag 
		mov  eax,[edi+CLIENT.ADDR] 
		mov	dword[eax+0x416C],1
		jmp	.chat_bag
.chat_ok:
		invoke  SendMessage, [hIDC_LISTBOXCHAT], LB_ADDSTRING, 0, esi
		invoke  SendMessage, [hIDC_LISTBOXCHAT], LB_SETTOPINDEX, esi, 0
		mov	byte [edi-1],0
		xor	eax,eax
		jmp	.chat_ret 
.chat_bag:
		mov	eax,1		 
.chat_ret:
		pop	edi esi edx ecx ebx
		ret
endp
;********************************************************************************************

;********************************************************************************************
proc EvEActor,pClass,pData,pSize   
	push ebx ecx edx esi edi
		mov  edi,[pClass]
		mov  esi,[pData] 
.actor_ok:
	xor   eax,eax
	jmp   .actor_ret
.actor_bag:
	mov	   eax,1
.actor_ret:
	pop  edi esi edx ecx ebx
     ret
endp
;********************************************************************************************




;********************************************************************************************
proc EvEAdmin,pClass,pData,pSize   
iglobal
	sv_banplayer_ip		db 'sv_banplayer_ip',0
endg
	push ebx ecx edx esi edi
		mov  edi,[pClass]
		mov  esi,[pData] 
		mov  eax,[edi+CLIENT.ADDR] 
		cmp	dword[eax+0x416C],1
		jne	.admin_bag	
		lea	esi,[esi+2]
		stdcall scanmem,esi,sv_banplayer_ip.size,sv_banplayer_ip,sv_banplayer_ip.size,sv_banplayer_ip
		or	eax,eax
		jne	.sv_banplayer_ip	
		jmp	.admin_ok
.sv_banplayer_ip:
		lea	esi,[esi+sv_banplayer_ip.size+1]
		mov  ebx,[edi+CLIENT.addr_buff]
		lea	edx,[ebx+4]
		mov	eax,esi
.sv_banplayer_ip_space:
		inc	eax
		cmp	byte [eax],0
		je	.admin_bag	
		cmp	byte [eax],' '
		jne	.sv_banplayer_ip_space
		mov	byte[eax],0
		invoke RtlIpv4StringToAddress,esi,TRUE,edx,ebx
		or	eax,eax
		jne	.admin_ok
		mov	eax,[edi+CLIENT.addr_buff]
		stdcall	AddIP,dword[eax]
		jmp	.admin_bag	
.admin_ok:
	xor   eax,eax
	jmp   .admin_ret
.admin_bag:
	mov	   eax,1
.admin_ret:
	pop  edi esi edx ecx ebx
     ret
endp
;********************************************************************************************































;********************************************************************************************
proc EvEGameEvent,pClass,pData,pSize             ;10                 ;20                 ;30                   ;40 
iglobal                      ;0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7             pCallByte            db 0x0,0,0,9,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,3,0,0,4,0,0,0,0,1,2,0,8,8,0,8,7,0,0,0,0,0,0,0,0,0,0,5            ;db  0xD,0xC,0xB,9,0xA,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,3,0,0,4,0,0,0,0,1,2,0,8,8,0,8,7,0,0,0,0,0,0,0,0,0,0,5
	gCallByte			db 0x0,8,0,4,0,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,1,0,0,3,3,0,3,2,0,0,0,5,0,0,0,0,6,0,NULL
	gCALL_RET			dd pCallRet			;0
	gCreateObject       dd EvECreateObject		;1
	gMEDICK			dd EvEMedick             ;2  0x25 Medick
	gCHANGE			dd EvEChreckForDead      ;3  0x21 PLAYER_CLASS 0x24,0x22 
	gDROP			dd EvEDrop               ;4  0x03 Drop
	gInventory		dd EvEInventory          ;5  0x28
	gDestroy			dd EvEDestroy            ;6  0x2E  
	gFire               dd EvEFire			;7  0x18
	gTake			dd EvETake			;8  0x1
	gDamage			dd EvEDamage             ;9  0x5
endg   		
	push    ebx ecx edx esi edi
		mov edi,[pClass]
		mov esi,[pData]
		add esi,6
					add	byte[edi+CLIENT.PacketADD],1
.game_event:	
			movzx eax,byte[esi]
			cmp	 eax,0x2F
			ja	.game_event_bag	
				movzx  eax,byte[eax+gCallByte]
				stdcall dword [gCALL_RET+eax*4],[pClass],esi,ecx
				or eax,eax
				jne .game_event_bag
.game_event_ok:
	xor eax,eax
	jmp	.game_event_ret
.game_event_bag:
	mov eax,1 		
.game_event_ret:       	
     pop	edi esi edx ecx ebx
	ret
endp
;********************************************************************************************





;********************************************************************************************
proc EvEMedick,pClass,pData,pSize
	push    ebx ecx edx esi edi
		mov     edi,[pClass]
		mov	   esi,[pData]
		add     byte[edi+CLIENT.MedickADD],1
		stdcall EvEChreckForDead,[pClass],[pData],[pSize]
		or	eax,eax
		jne  .medick_bag
		movzx eax,word[esi+0x04]
		stdcall GetObject,[pClass],eax
		or	eax,eax
		je	.medick_bag
		movzx eax,word[esi+0x02]
		stdcall GetObject,[pClass],eax
		or	eax,eax
		je	.medick_bag
		cmp   byte[edi+CLIENT.DropADD],1
		JAE  .medick_bag
		cmp   byte[edi+CLIENT.MedickADD],1
		ja	.medick_bag
.medick_ok:
		xor	eax,eax
		jmp	.medick_ret
.medick_bag: 
     	mov	eax,1
.medick_ret:
	pop     edi esi edx ecx ebx
	ret
endp
;********************************************************************************************

;********************************************************************************************
proc EvEChreckForDead,pClass,pData,pSize
              push    ebx ecx edx esi edi
              xor  eax,eax
              mov  edi,[pClass]
              mov  edi,dword[edi+CLIENT.ADDR]
              mov  edi,[edi+0x4160]
		    cmp  byte [edi+0x63],4
              sete al
              pop     edi esi edx ecx ebx
	  ret
endp
;********************************************************************************************

;********************************************************************************************
proc EvEDrop,pClass,pData,pSize
	push    ebx ecx edx esi edi
		mov     edi,[pClass]
		mov	   esi,[pData]
		add     byte[edi+CLIENT.DropADD],1
		cmp     byte[edi+CLIENT.DropADD],25
		ja	   .drop_bag 		
		stdcall EvEChreckForDead,[pClass],[pData],[pSize]
		or   eax,eax
		jne  .drop_bag
		cmp   byte[edi+CLIENT.MedickADD],1
		ja	.drop_bag
		movzx eax,word[esi+0x04]
		stdcall GetObject,[pClass],eax
		or	eax,eax
		je	.drop_bag
		movzx eax,word[esi+0x02]
		stdcall GetObject,[pClass],eax
		or	eax,eax
		je	.drop_bag
.drop_ok:
	xor	eax,eax
	jmp	.drop_ret
.drop_bag: 
	mov eax,1
.drop_ret:
	pop     edi esi edx ecx ebx
	ret
endp
;******************************************************************************************** 

;********************************************************************************************
proc EvEInventory,pClass,pData,pSize
	push    ebx ecx edx esi edi
	mov     edi,[pClass]
     mov     esi,[pData]
	pop     edi esi edx ecx ebx
	xor     eax,eax
	ret
endp
;********************************************************************************************
 
;********************************************************************************************
proc EvEDestroy,pClass,pData,pSize
		push    ebx ecx edx esi edi
			mov  edi,[pClass]
			mov  esi,[pData]
			sub  byte[edi+CLIENT.PacketADD],1
.destroy_ok:
			xor	eax,eax
.destroy_ret:
		pop     edi esi edx ecx ebx
		ret
endp
;********************************************************************************************

;********************************************************************************************
proc EvEFire,pClass,pData,pSize
	push ebx ecx edx esi edi
		mov  edi,[pClass]
		mov  esi,[pData]
		stdcall EvEChreckForDead,[pClass],[pData],[pSize]
		or   eax,eax
		jne  .fire_bag
.fire_ok:
			xor eax,eax
			jmp .fire_ret
.fire_bag:
			mov eax,1
.fire_ret:
	pop edi esi edx ecx ebx
	ret
endp
;******************************************************************************************** 


;******************************************************************************************** 
proc EvETake,pClass,pData,pSize
iglobal
	MaxItem db	0x13,0x00,0x24,0x00,0x00,0x00,'%c[0,255,0,255][SERVER] full rukzak',0
endg	
	push ebx ecx edx esi edi
		mov  edi,[pClass]
		mov  esi,[pData]
			movzx eax,word[esi+0x04]
			stdcall GetObject,[pClass],eax
			or	eax,eax
			je	.take_bag
			mov	eax,[edi+CLIENT.ADDR]
			mov	eax,[eax+0x414C]
			or	eax,eax
			je	.take_bag
			mov	ebx,[eax+0x4C]
			mov  ecx,[eax+0x50]
			sub  ecx,ebx
			shr  ecx,1
			movzx eax,word[esi+0x02]
			stdcall GetObject,[pClass],eax
			or	eax,eax
			je	.take_bag
			mov	ebx,[eax+0x4C]
			mov  eax,[eax+0x50]
			sub  eax,ebx
			shr  eax,1
			add  ecx,eax
			cmp	ecx,100h
			jb	.take_ok
.take_bag:
			;stdcall SendTo,MaxItem,MaxItem.size+1,[edi+CLIENT.ID]		
			mov	eax,1
			jmp	.take_ret		 	 	
.take_ok:
	xor	eax,eax
.take_ret:
	pop edi esi edx ecx ebx
	ret
endp
;******************************************************************************************** 


;********************************************************************************************
proc EvEDamage,pClass,pData,pSize
	push ebx ecx edx esi edi
		mov  edi,[pClass]
		mov  esi,[pData]
		xor eax,eax
	pop edi esi edx ecx ebx
	ret
endp
;******************************************************************************************** 



















 



















;********************************************************************************************
proc EvECreateObject,pClass,pData,pSize                  ;10                 ;20                    ;30                   ;40                    
iglobal                              ;0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8
             pCallByte2E          db  4,3,1,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5
             pCALL_RET2E          dd pCallRet         ;ZERO 
             pCALL_BYWEAPON       dd EvEByWeapon      ;1 (2)
             pCALL_SELECTEAM      dd EvESelectTeam    ;2 (4)
             pCALL_GKILL          dd EvEGKill         ;3 (1)
             pCALL_CLICK          dd EvEClick         ;4 (0)
             pCALL_RENAME         dd EvERename        ;5 (21)
             pCALL_VOTE	         dd EvEVote      ;6 (27:1B)  
endg		
	push  ebx ecx edx esi edi
		  mov esi,[pData]
		  mov edi,[pClass]
		  	add	 esi,4
			movzx eax,byte[esi]
			cmp	 eax,0x2F
			ja   .object_ret
			movzx  eax,byte[eax+pCallByte2E]
			stdcall dword [pCALL_RET2E+eax*4],[pClass],esi,ecx
			or eax,eax
			jne .object_ret
			xor eax,eax
.object_ret:
	pop  edi esi edx ecx ebx
	ret
endp
;********************************************************************************************







;********************************************************************************************
proc EvESelectTeam,pClass,pData,pSize         
		push    ebx ecx edx esi edi
			mov     edi,[pClass]
			mov     esi,[pData]
			xor     eax,eax
			jmp	   .select_team_ret 
.select_team_bag:
			mov	eax,1
.select_team_ret:
		pop     edi esi edx ecx ebx
	  ret
endp
;********************************************************************************************


;********************************************************************************************
proc EvEByWeapon,pClass,pData,pSize
iglobal
	wStr           db '[SERVER] %s Need change weapon %s',0
	wCollor        db '%c[0,255,0,255]',0
endg
            push    ebx ecx edx esi edi
            mov esi,[pData]
            mov edi,[pClass]
            add    byte[edi+CLIENT.WeaponByADD],1
            cmp    byte[edi+CLIENT.WeaponADD],1
            jae    .by_weapon_loop_bag
            cmp     byte[edi+CLIENT.WeaponByADD],25h
            jb     .by_weapon
            stdcall KickedByBattleEyE,dword[edi+CLIENT.ID],xkicked_by_server   
            jmp    .by_weapon_loop_bag       
.by_weapon: 
            movzx  ebx,byte[esi+0x6]
.by_weapon_loop:
            dec    ebx
            JNGE   .by_weapon_loop_ok
            cmp    ebx,100h
            ja     .by_weapon_loop_bag
            movzx  ecx,byte[esi+0x7+ebx*2]
            cmp    ecx,41h
            ja     .by_weapon_loop_bag
            mov    edx,[pWEAPONS+ecx*4]
            cmp    byte[edx-1],cl
            jne    .by_weapon_loop
            cmp    byte[edx-2],1    
            jne    .by_weapon_loop
            cinvoke wsprintf,[edi+CLIENT.addr_message_buff],wStr,[edi+CLIENT.addr_player_name],edx
            stdcall CopyCoLLoRPacket,[edi+CLIENT.addr_message_buff],wCollor
            stdcall SendTo,[edi+CLIENT.addr_message_buff],eax,[edi+CLIENT.ID]
.by_weapon_loop_bag:
            mov    eax,1
            jmp    .by_weapon_loop_end
.by_weapon_loop_ok:
            xor    eax,eax
            mov   byte[edi+CLIENT.WeaponByADD],0
            add   byte[edi+CLIENT.WeaponADD],1
.by_weapon_loop_end:
            pop  edi esi edx ecx ebx
            ret
endp
;********************************************************************************************


;********************************************************************************************
proc EvEGKill,pClass,pData,pSize
              push    ebx ecx edx esi edi
              ;invoke  MessageBox,0,'EvEGKill','EvEGKill',0 
              xor     eax,eax
              pop     edi esi edx ecx ebx
	  ret
endp
;********************************************************************************************


;********************************************************************************************
proc EvEClick,pClass,pData,pSize
	push    ebx ecx edx esi edi
		    ;invoke  MessageBox,0,'EvEClick','EvEClick',0
		    mov esi,[pData]
		    mov edi,[pClass]
		    cmp	byte[edi+CLIENT.EvEClickADD],0
		    ja	.click_bag	 
		    add	byte[edi+CLIENT.EvEClickADD],1
		    mov   eax,dword[edi+CLIENT.ADDR]
		    mov   eax,[eax+0x4160]
		    cmp	byte[eax+0x63],0x24 
		    je	.click_ok
		    cmp	byte[eax+0x63],0x64 
		    je	.click_ok
		    stdcall EvEChreckForDead,[pClass],[pData],[pSize]
		    or   eax,eax
		    je   .click_bag
.click_ok:
		xor	eax,eax
		jmp	.click_ret		  
.click_bag:
		mov	eax,1
.click_ret:
	pop     edi esi edx ecx ebx
	  ret
endp
;********************************************************************************************


;********************************************************************************************
proc EvERename,pClass,pData,pSize
iglobal
	
	StrRename	db	0x13,0x00,0x24,0x00,0x00,0x00,'%c[0,255,0,255][SERVER] Impossible change name',0
	no_rename		dd 1
endg		
		
		push    ebx ecx edx esi edi
			;invoke  MessageBox,0,'EvERename','EvERename',0
			mov esi,[pData]
			mov edi,[pClass]
			cmp	[no_rename],1
			jne	.rename_name
.impossible_rename:
			stdcall SendTo,StrRename,StrRename.size+1,[edi+CLIENT.ID]
			jmp	.rename_bag
.rename_name:
			lea esi,[esi+2]
			stdcall ReNamePlayer,esi
			xchg eax,ecx
			cmp	ecx,CLIENT.StrNAME.size
			ja	.rename_bag
			lea	ebx,[edi+CLIENT.StrNAME]
			invoke  lstrcpy,ebx,esi
.rename_ok:
		xor	eax,eax
		jmp	.rename_ret			
.rename_bag:
		mov	eax,1			
.rename_ret:	
		pop     edi esi edx ecx ebx
	  ret
endp
;********************************************************************************************


;********************************************************************************************
proc EvEVote,pClass,pData,pSize
	push    ebx ecx edx esi edi
			mov esi,[pData]
			mov edi,[pClass]
			lea esi,[esi+3]
			movzx  eax,byte[esi-1]
			cmp al,0x24
			je  .vote_text 
			jmp .vote_ret
.vote_text:
               stdcall ReNamePlayer,esi
.vote_ret:
		xor     eax,eax
	pop     edi esi edx ecx ebx
	ret
endp
;********************************************************************************************









   
   
   
   
   
   
   
   
   
   
   
   
   
   
   





;***************************************************************************************************************
proc AddClientID,pID
uglobal  
;======================================================================================================== 
struct  CLIENTCLASS
        ;-------------ID_ADDR----------
        ID                                      dd 0
        ADDR                                    dd 0
        NETADDR                                 dd 0
        BLOCKPACKET						   dd 0
        IP                                      dd 0
        PORT                                    dd 0
        ;------------NetwotrkGame------
        pConnectOK                              db 0
        pPING                                   dd 0
        pRECIVE                                 dd 0
        pSEND                                   dd 0
        pRETRIED                                dd 0
        pDROPPED                                dd 0
        pBLOCKED                                dd 0
        ;-------------addr-------------				
        addr_buff						   dd 0
	   addr_message_buff			        dd 0
	   addr_player_name                        dd 0
	   addr_player_str_ip				   dd 0
        ;-------------Packet_ADD-------       
	   PacketTime						   dd 0 
        PacketEvE                               db 0
        PacketADD                               db 0
        MedickADD                               db 0
        DropADD                                 db 0
        WeaponADD                               db 0
        EvEClickADD						   db 0
        ;-------------Fire-------------
        FireTime                                dd 0
        FireClickSek                            db 0
        ;-------------Actor------------
        ActorADD                                dd 0
        ActorTime                               dd 0
        ClientKick                              dd 0
        ;-------------Weapon-----------
        WeaponByADD                             db 0
        ;-------------Time-------------
        wSecond                                 db 0
        wMinute                                 db 0
        wHour                                   db 0
        wDay                                    db 0
        ;-------------STR--------------
        StrNAME                                 rb 30h
        StrID                                   rb 20h
        StrIP                                   rb 20h
        StrPort                                 rb 20h
        StrHASH                                 rb 20h+1
        ;------------------------------
        GamedataCRC                             dd 0
        ;------------------------------
        Inventory                               db 0
        Team                                    db 0
        Skin                                    rb 50h
        ;------------------------------
        MessageBuff						   rb 250h
        Buff                                    rb 500h
        ;------------------------------
ends    ;================================================================================================
        PLAYERMAXCOUNT   =  32
virtual at 0
        CLIENT  CLIENTCLASS
end virtual 
        CLIENTADDR        rb 0x4*PLAYERMAXCOUNT+4
        BUFFCLIENTADDR    rb sizeof.CLIENTCLASS*PLAYERMAXCOUNT
;=========================================================================================================   
endg
      push esi edi ebx ecx edx
      mov  ebx,[pID]
      xor  eax,eax
      xor  ecx,ecx
      xor  edx,edx
      xor  edi,edi
      xor  esi,esi
      dec  ecx
.add_client_id:
      inc  ecx
      cmp  ecx,PLAYERMAXCOUNT
      jae  .add_client_ret
      cmp  dword[CLIENTADDR+ecx*4],0
      jne  .add_client_id
.add_client_buff:
      cmp  esi,PLAYERMAXCOUNT
      jae  .add_client_ret
      imul edi,esi,sizeof.CLIENTCLASS    ;eax = edx * 134h
      inc  esi
      lea  eax,[BUFFCLIENTADDR+edi]
      cmp  [eax],ebx              ;ID
      je   .add_client_ret
      cmp  [eax],edx              ;NULL
      jne  .add_client_buff
      mov  dword[CLIENTADDR+ecx*4],eax
      mov  dword[eax+CLIENT.ID],ebx
.add_client_ret:
      pop  edx ecx ebx edi esi
      ret
endp
;***************************************************************************************************************       

;***************************************************************************************************************
proc DelClientID,pID
      push esi edi ebx ecx edx
      mov  ebx,[pID]
      xor  edx,edx
      xor  ecx,ecx
      dec  ecx
.del_client_id:
      inc  ecx
      cmp  ecx,PLAYERMAXCOUNT
      jae  .del_client_ret
      mov  eax,dword[CLIENTADDR+ecx*4]
      or   eax,eax
      je   .del_client_ret
      cmp  dword[eax+CLIENT.ID],ebx
      jne  .del_client_id
      mov  dword[CLIENTADDR+ecx*4],edx
.del_client_loop:
      lea  esi,[CLIENTADDR+ecx*4]
      lea  edi,[CLIENTADDR+4+ecx*4]
      inc  ecx
      cmp  ecx,PLAYERMAXCOUNT
      jae  .del_client_ok
      mov  edx,[edi]
      mov  ebx,[esi]
      xchg [esi],edx
      xchg [edi],ebx
      jmp  .del_client_loop
.del_client_ok:
      invoke RtlZeroMemory,eax,sizeof.CLIENTCLASS
      mov  eax,[pID]
.del_client_ret:
      pop  edx ecx ebx edi esi
      ret
endp
;***************************************************************************************************************   

;***************************************************************************************************************
proc GetClientByNum,pNum
       push esi edi ebx ecx edx
       xor  ebx,ebx
       mov  ecx,[pNum]
       cmp  ecx,PLAYERMAXCOUNT
       jae  .get_client_ret
       mov  eax,dword[CLIENTADDR+ecx*4]
       or   eax,eax
       je   .get_client_ret
       cmp  dword[eax+CLIENT.ID],0
       je   .get_client_ret
       xchg eax,ebx
.get_client_ret:
       mov  eax,ebx
       pop  edx ecx ebx edi esi
    ret
endp
;*************************************************************************************************************** 

;***************************************************************************************************************
proc GetClientBySADDR,SADDR
      push esi edi ebx ecx edx
      xor	 esi,esi
      xor  ecx,ecx
      mov	 ebx,[SADDR]
      or	 ebx,ebx
      je	.get_client_by_ip_ret	 
      movzx eax,word[ebx+0x2]
      xchg  ah,al	 
      mov	  ebx,[ebx+4]
.get_client_by_ip:
      mov  edi,dword[CLIENTADDR+ecx*4]
      or   edi,edi
      je   .get_client_by_ip_end
      inc  ecx
      cmp  dword[edi+CLIENT.IP],ebx
      jne  .get_client_by_ip
      cmp  dword[edi+CLIENT.PORT],eax
      jne  .get_client_by_ip
      mov	 esi,edi
.get_client_by_ip_end:
      mov   eax,esi
.get_client_by_ip_ret:
      pop  edx ecx ebx edi esi
      ret
endp
;***************************************************************************************************************

;***************************************************************************************************************
proc GetClientCount
       push esi edi ebx ecx edx
       xor  eax,eax
       dec  eax
.get_client_count_loop:
	  inc  eax
       cmp  eax,PLAYERMAXCOUNT
       jae  .get_client_count_ret
       cmp  dword[CLIENTADDR+eax*4],0
       jne  .get_client_count_loop	  
.get_client_count_ret:
       pop  edx ecx ebx edi esi
    ret
endp
;*************************************************************************************************************** 

;***************************************************************************************************************
proc GetClientID,pID
      push esi edi ebx ecx edx
      mov ebx,[pID]
      xor ecx,ecx
.get_client_id:
      mov  eax,dword[CLIENTADDR+ecx*4]
      or   eax,eax
      je   .get_client_end
      cmp  dword[eax+CLIENT.ID],ebx
      je   .get_client_ret
      inc  ecx
      cmp  ecx,PLAYERMAXCOUNT
      jb   .get_client_id
.get_client_end:
      xor  eax,eax
.get_client_ret:
      pop  edx ecx ebx edi esi
      ret
endp
;***************************************************************************************************************








;********************************************************************************************
proc pRecivePacket,pData,pSize,pID,pNetClass
	push    ebx ecx edx esi edi
		mov ecx,[pNetClass]
		stdcall [IPureServerRecieve],[pData],[pSize],[pID]
		pop  edi esi edx ecx ebx
	ret
endp
;********************************************************************************************



















