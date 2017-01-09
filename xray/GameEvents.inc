
iglobal
     StrIDHex db '%08X',0
endg 




 
           

;===================================================================================================================
AddClientClass:
AddClientWtiteTo rb 0xF2
     lea  eax,[ecx+4]
     stdcall   ReNamePlayer,eax
     push ecx
     mov  ecx,ebx
     call edx
     test eax,eax
     je   add_client_class_ret
     xchg edi,eax
     stdcall AddClientID,dword [edi+0x000040A8]
     test eax,eax
     je   add_client_class_ret
     xchg esi,eax
     mov  [esi+CLIENT.ADDR],edi
     lea  ebx,[esi+CLIENT.StrIP]
     lea  eax,[edi+0x000040A8+0x94]    ;IP 
	invoke   RtlIpv4AddressToString,eax,ebx
     mov  eax,[edi+0x0000412C]
     lea  eax,[eax+0x0C]
     lea  ebx,[esi+CLIENT.StrNAME]
     invoke    RtlMoveMemory,ebx,eax,CLIENT.StrNAME.size
     mov  eax,[edi+0x000040A8]
     lea  ebx,[esi+CLIENT.StrID]
     cinvoke wsprintf,ebx,StrIDHex,eax
     invoke    SendMessage,[hList],LVM_INSERTITEM,0, lvi
add_client_class_ret:
     xor eax,eax
     pop edi
     pop esi
     pop ebx
     mov esp,ebp
     pop ebp
     ret 0008
;===================================================================================================================
DelClientClass:   
DelClientWtiteToA rb 0x71
     pushad
     stdcall   DelClientID,dword[eax+0x000040A8]
     invoke	SendMessage,[hList],LVM_DELETEITEM,NULL,0
     popad
DelClientWtiteToB rb 0x26
;===================================================================================================================
















		   
recieve_packet:
     mov ecx,eax
     mov    eax,[esi]
     cmp    al,0xE1
     jne    recieve_packet_block
     rol    eax,8
     cmp    al,0xC0
     jne    recieve_packet_block
	stdcall GetClientID,edi
     or	  eax,eax
     je 	  recieve_packet_block
	stdcall CheckPacket,esi,edi,eax,eax   ;ebx NetClass
	test   eax,eax
	je     recieve_packet_ok
	movzx  ecx,word[esi+1]
	cmp    ecx,250h
	ja     recieve_packet_ok  
	add    ecx,4
	stdcall xrCorePMSG,esi,ecx,3
recieve_packet_block:
     mov  eax,0xFFFFFFF
recieve_packet_ok:
     push edi
     push ecx
     push esi
     mov  ecx,ebx
recieve_packet_end:
	mov  eax,0xFFFFFFF
;===================================================================================================================


  
  
  
  












        






;********************************************************************************************
proc CheckPacket,pData,pID,xClass,pClass      ;10                 ;20                 ;30                 ;40 
iglobal                     ; 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8
    pCallByte            db 0x0,0,0,9,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,3,0,0,4,0,0,0,0,1,2,0,8,8,0,8,7,0,0,0,0,0,0,0,0,0,0,5
    pCALL_RET            dd pCallRet            ;0        
    pCALL_CREATE_OBJECT  dd pWeaponsCreate      ;1
    pCALL_CRC            dd pCRC                ;2
    pCALL_CD_KEY         dd pChreckForBanClient ;3
    pCALL_CHAT           dd pChat               ;4
    pCALL_KICK           dd pKickClinet         ;5
    pACTOR               dd pActor              ;6
    ;---------------------------------------------
    pMEDICK              dd pChreckForDead      ;7  0x25 Medick
    pCHANGE              dd pChreckForDead      ;8  0x21 PLAYER_CLASS 0x24,0x22 
    pDROP                dd pChreckForDead      ;9  0x03 Drop              
    ;---------------------------------------------  ;Fire 18 
endg    
;==================================================================================================================================
.packet_start:
      push    ebx ecx edx esi edi
              ;--------------------------------------------------
              push   .packet_handler  ;Установим наш обработчик
              push   dword[fs:0]
              mov    dword[fs:0], esp 
              ;--------------------------------------------------
      mov  eax,[xClass]
      mov  eax,[eax+CLIENTCLASS.ADDR]
      mov [pClass],eax
      mov    esi,[pData]
      movzx  edi,word[esi+1]    ;eax edx esi edi  
      lea    edi,[edi+esi+4]
      add    esi,6
      xor    edx,edx
      xor    ebx,ebx
      xor    ecx,ecx
.packet_loop:
      movzx eax,byte[esi]
           cmp byte [esi],8
           jne .skip_packet
           or  edx,edx
           je  .data_packet
           mov  eax,[esi+2]
           sub  eax,edx
           cmp  eax,0x550
           ja   .bag_packet
.data_packet:
		            ;------------------------------------------------------------	
		             movzx eax,byte[esi+6]
                            cmp al,0x2E
                            je  .packet_ok
                            cmp  al,0x25
                            sete al
                            add  ebx,eax
                            inc  ecx
                            cmp  ebx,1
                            JG   .bag_packet
                            or   ebx,ebx
                            je   .packet_ok
                            cmp  ecx,ebx
                            JG   .bag_packet
                    ;------------------------------------------------------------
.packet_ok:
     mov  edx,[esi+2]
     movzx eax,byte[esi+6]
.skip_packet: 
                        cmp eax,0x2F+1
		                ja  .bag_packet
                        movzx  eax,byte[eax+pCallByte]
                        stdcall dword [pCALL_RET+eax*4],[pClass],esi
                        or eax,eax
                        jne .bag_packet
      movzx  eax,word[esi-2]
      lea    esi,[esi+eax+2]
      cmp    edi,esi
ja   .packet_loop
      xor  eax,eax
      jmp  .packet_ret
.bag_packet:
      mov eax,1
.packet_ret:
      pop  dword[fs:0]                ;Убираем обработчик
      add  esp,4
      pop  edi esi edx ecx ebx
      ret
            ;-----------------------------------------------------
.packet_handler:
            mov ecx,[esp+8];pFrame равно тому, что мы поместили в fs:[0]
            mov eax,dword[esp+0Ch];Context
            mov dword[eax+0C4h],ecx;ESP
            mov dword[eax+0B8h],.packet_ret;ThreadCont
            xor eax,eax ;Продолжить выполнение
            retn 4*4 
endp   
;********************************************************************************************












;********************************************************************************************
proc pCallRet,pClass,pData
     xor eax,eax
	ret
endp
;********************************************************************************************

;********************************************************************************************
proc pKickClinet,pClass,pData
     push ebx ecx edx esi edi
        mov  edi,[pClass]
        stdcall KickedByBattleEyE,dword[edi+0x40A8],Kicked_by_server
        mov eax,1
	 pop  edi esi edx ecx ebx
     ret
endp
;********************************************************************************************

;********************************************************************************************
proc pActor,pClass,pData
iglobal 
      pTextDisconnectClient     db '--[SERVER]: Disconnect Client %s',0
      pDataDisconnectBuff       db 0x13,0x00,0x24,0x00,0x00,0x00,'%c[0,255,255,0]'
      pTextBuffKickedReason     rb 250h
endg     
     
     
     push ebx ecx edx esi edi         
	               mov     edi, [pClass]                ;cmp dword[edi+0x4028+0x128],0
                   cmp dword[edi+0x4028+0x4],0x1F4      ; 500 max ping     
                   ja  .actor_bag
                   cmp dword[edi+0x4028+0x5C],0x50      ; 80 max packet  MRR     
                   ja  .actor_bag
                   cmp dword[edi+0x4028+0x64],0x78      ; 120 max packet  MSR
                   ja  .actor_bag
.actor_ok:
      xor   eax,eax
      jmp   .actor_ret
.actor_bag:
                  mov	  eax, [edi+0x4160]
     	          lea     eax, [eax+4]
                  cinvoke wsprintf,pTextBuffKickedReason,pTextDisconnectClient,eax
	              cmp     eax,0x1E0-7+16
			      ja      .actor_ok
                  add     eax,7+16
	              mov     esi,eax
                  ;stdcall KickedByBattleEyE,dword[edi+0x000040A8],BuffKickedReason
	              ;stdcall SendToAll,pDataDisconnectBuff,esi,-1
	              stdcall [xrCore.msg],pTextBuffKickedReason
	              add     esp,4
	              mov  eax,1
.actor_ret: 
	 pop  edi esi edx ecx ebx
     ret
endp
;********************************************************************************************

;********************************************************************************************
proc pChat,pClass,pData
          push   ebx ecx edx esi edi
          mov    esi,[pData]
          lea    esi,[esi+4]
          lea    edi,[esi]
          invoke lstrlen,esi
          test   eax,eax
          je     .chat_end
          add    esi,eax
          mov    byte [esi],':'
          invoke  SendMessage, [hIDC_LISTBOXCHAT], LB_ADDSTRING, 0, edi
	      invoke  SendMessage, [hIDC_LISTBOXCHAT], LB_SETTOPINDEX, edi, 0  
          mov    byte [esi],0
.chat_end:
          pop   edi esi edx ecx ebx
          xor   eax,eax
.chat_ret:
	  ret
endp
;********************************************************************************************


;********************************************************************************************
proc pWeaponsCreate,pClass,pData
iglobal
   wStr           db 'Player: %s need change weapon %s',0
   wStrData       db 0x13,0x00,0x24,0x00,0x00,0x00
   wCollor        db '%c[0,255,0,255]'
   wWeaponsBuff   rb 250h
endg  
            push  ebx ecx edx esi edi
            xor eax,eax
		    mov esi,[pData]
            cmp byte [esi+0xA],2
            jne  .by_weapon_loop_ok
.by_weapon: 
            movzx  ebx,byte[esi+0x10]
.by_weapon_loop:
            dec    ebx
            cmp    ebx,30h
            ja     .by_weapon_loop_ok
            movzx  ecx,byte[esi+0x11+ebx*2]
            cmp    ecx,41h
            ja     .by_weapon_loop_bag
            mov    edx,[pWEAPONS+ecx*4]
            cmp    byte[edx-1],cl
            jne    .by_weapon_loop
            cmp    byte[edx-2],1    
            jne    .by_weapon_loop
            nop
            mov    edi,[pClass]
            mov    eax,[edi+0x4160]
	        lea    eax,[eax+4]
            cinvoke wsprintf,wWeaponsBuff,wStr,eax,edx
            cmp     eax,0x1E0-7+16
			ja      .by_weapon_loop_bag
            add     eax,7+16
            stdcall SendTo,addr wStrData,eax,dword[edi+0x40A8]
.by_weapon_loop_bag:
            mov eax,1
.by_weapon_loop_ok:
            add eax,eax
.by_weapon_loop_end:
            pop  edi esi edx ecx ebx
            ret
endp
;********************************************************************************************

;***************************************************************************************************************
proc pChreckForBanClient,pClass,pData
iglobal
    Kicked_by_server       db 'Kicked by server',0
    BuffIPADDR             rb 30h
    KeyOK                  db 0x1E,0x00,0
    BuffBanClient          rb 2500h
endg  
      push    ebx ecx edx esi edi
      mov     esi,[pData]
      mov     edi,[pClass]
                      lea esi,[esi+2]
                      invoke lstrlen,esi
                      cmp eax,0x48
                      jne .kick_client
                      lea  eax,[edi+0x000040A8+0x4] ;HASH
                      invoke RtlMoveMemory,eax,esi,0x20
                      lea  eax,[edi+0x000040A8+0x94]    ;IP 
	                  invoke  RtlIpv4AddressToString,eax,BuffIPADDR
                      invoke  StrStr,BuffBanClient,BuffIPADDR
                      test  eax,eax
                      jne  .kick_client
                      lea     eax,[edi+0x000040A8+0x4]
                      invoke  StrStr,BuffBanClient,eax
                      test  eax,eax
                      jne  .kick_client 
                      stdcall SendTo,KeyOK,2,dword[edi+0x000040A8]
                      xor     eax,eax
                      jmp     .chreck_for_ban_ret
.kick_client:
                      stdcall KickedByBattleEyE,dword[edi+0x40A8],Kicked_by_server
.chreck_for_ban_ret: 
                      mov eax,1
      pop     edi esi edx ecx ebx
	  ret
endp
;***************************************************************************************************************

;***************************************************************************************************************
proc pCRC,pClass,pData
       push    ebx ecx edx esi edi
       xor     eax,eax
       mov     esi,[pData]
       mov     edi,[pClass]
               mov ecx,[esi+2]
               mov [edi+0x41B4],ecx
               cmp ecx,0x46E2424B
               je  .crc_ret
               stdcall KickedByBattleEyE,dword[edi+0x40A8],Kicked_by_server
               mov eax,1
.crc_ret:
       pop     edi esi edx ecx ebx
	   ret
endp
;***************************************************************************************************************

 
;********************************************************************************************
proc pChreckForDead,pClass,pData
              push    ebx ecx edx esi edi
              xor  eax,eax
              mov  ebx,[pClass]
              mov  ebx,[ebx+0x4160]
		    cmp  byte [ebx+0x63],4
              sete al
              pop     edi esi edx ecx ebx
	  ret
endp
;********************************************************************************************




















;***************************************************************************************************************
proc AddClientID,pID
iglobal
;======================================================================================================== 
        PLAYERMAXCOUNT = 31     
struct  CLIENTCLASS
        ID              dd 0
        ADDR            dd 0
        StrNAME         rb 10h
        StrID           rb 8+1
        StrIP           rb 10h
        Buff            rb 250h
ends    ;================================================================================================
virtual at 0
        CLIENT  CLIENTCLASS
end virtual
        CLIENTADDR        rb 0x4*PLAYERMAXCOUNT
        BUFFCLIENTADDR    rb sizeof.CLIENTCLASS*PLAYERMAXCOUNT
;=========================================================================================================   
endg
      push esi edi ebx ecx edx
      mov  ebx,[pID]
      xor  eax,eax
      xor  ecx,ecx
      dec  ecx
.add_client_id:
      inc  ecx
      cmp  ecx,PLAYERMAXCOUNT
      JGE  .add_client_ret
      cmp  dword[CLIENTADDR+ecx*4],0
      jne  .add_client_id
      imul eax,ecx,sizeof.CLIENTCLASS     ;eax = ecx * 134h
      lea  eax,[eax+BUFFCLIENTADDR]
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
      xor  eax,eax
      xor  ecx,ecx
      dec  ecx
.del_client_id:
      inc  ecx
      cmp  ecx,PLAYERMAXCOUNT
      JGE  .del_client_ret
      mov  eax,dword[CLIENTADDR+ecx*4]
      or   eax,eax
      je   .del_client_ret
      cmp  dword[eax+CLIENT.ID],ebx
      jne  .del_client_id
.del_client_addr:
      lea  esi,[CLIENTADDR+ecx*4]
      inc  ecx
      cmp  ecx,PLAYERMAXCOUNT
      JGE  .del_client_addr_zero
      lea  edi,[CLIENTADDR+ecx*4]
      mov  ebx,[esi]
      mov  edx,[edi]
      mov [esi],edx
      mov [edi],ebx
      jmp .del_client_addr
.del_client_addr_zero:
      mov dword[esi],0
      invoke RtlZeroMemory,eax,sizeof.CLIENTCLASS
      xchg eax,edi
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
       JGE  .get_client_ret
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




;***************************************************************************************************************
proc AddHASH,sHASH,sIP,sNAME
iglobal
;=========================================================================================================
  HASHMAXCOUNT = 20
struct  HASHCLASS
        StrHASH              rb 20h+1
        StrIPADDR            rb 10h+1
        StrPLAYERNAME        rb 16h+1
ends
virtual at 0
HASH  HASHCLASS
end virtual
HASHADDR          rb 0x4*HASHMAXCOUNT
BUFFHASHADDR      rb sizeof.HASHCLASS*HASHMAXCOUNT
;=========================================================================================================   
endg
      push esi edi ebx ecx edx
      xor  eax,eax
      xor  ecx,ecx
      dec  ecx
.add_hash:
      inc  ecx
      cmp  ecx,HASHMAXCOUNT
      JGE  .add_hash_ret
      cmp  dword[HASHADDR+ecx*4],0
      jne  .add_hash
      imul eax,ecx,sizeof.HASHCLASS    ;eax = ecx * 134h
      lea  eax,[eax+BUFFHASHADDR]
      mov  dword[HASHADDR+ecx*4],eax
      mov  edi,eax
      lea  eax,[edi+HASH.StrHASH]
      invoke RtlMoveMemory,eax,dword[sHASH],0x20
      lea  eax,[edi+HASH.StrIPADDR]
      invoke RtlMoveMemory,eax,dword[sIP],0x10
      lea  eax,[edi+HASH.StrPLAYERNAME]
      invoke RtlMoveMemory,eax,dword[sNAME],0x16
.add_hash_ret:
      pop  edx ecx ebx edi esi
      ret
endp
;***************************************************************************************************************

;***************************************************************************************************************
proc GetHASH,StrHASH,StrIP,StrNAME
      push esi edi ebx ecx edx
      xor ecx,ecx
.get_hash:
      mov  edi,dword[HASHADDR+ecx*4]
      or   edi,edi
      je   .get_hash_end
      lea   esi,[edi+HASH.StrHASH]
      push  ecx
      invoke StrStr,esi,[StrHASH]
      pop   ecx
      or   eax,eax
      mov  eax,edi
      jne   .get_hash_ret
      lea   esi,[edi+HASH.StrIPADDR]
      push  ecx
      invoke StrStr,esi,[StrIP]
      pop   ecx
      or   eax,eax
      mov  eax,edi
      jne   .get_hash_ret
      lea   esi,[edi+HASH.StrPLAYERNAME]
      push  ecx
      invoke StrStr,esi,[StrNAME]
      pop   ecx
      or   eax,eax
      mov  eax,edi
      jne   .get_hash_ret
      inc  ecx
      cmp  ecx,HASHMAXCOUNT
      jb   .get_hash
.get_hash_end:
      xor  eax,eax
.get_hash_ret:
      pop  edx ecx ebx edi esi
      ret
endp
;*************************************************************************************************************** 





