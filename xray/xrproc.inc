




;*************************************************************************************************************** 
proc xrCorePMSG,pID,xDATA,xSIZE,xCOLLOR
        push   ebx ecx edx esi edi
                cmp [xSIZE],CLIENT.Buff.size/3-5   
                ja  .xr_core_pmsg_ret
                stdcall GetClientID,[pID]
                or eax,eax
                je .xr_core_pmsg_ret
                xchg edi,eax
                lea edi,[edi+CLIENT.Buff]
                mov eax,[xCOLLOR]      
                cmp eax,1
                jne @f
                mov word [edi],0x2021
@@:
                cmp eax,2
                jne @f
                mov word [edi],0x202D
@@:
                cmp eax,3
                jne @f
                mov word [edi],0x2023
@@:
                add  edi,2
                stdcall HexToStr,[xDATA],edi,[xSIZE],1
                sub  edi,2
                stdcall [xrCore.msg],edi
                add esp,4
                invoke RtlZeroMemory,edi,CLIENT.Buff.size
.xr_core_pmsg_ret:
        pop   edi esi edx ecx ebx
        ret
endp
;*************************************************************************************************************** 










;*************************************************************************************************************** 
proc DisconnectClientEx,rID,rDATA
        push    ebx ecx edx esi edi
                stdcall GetClientID,[rID]
                or      eax,eax
                je      .DisconnectClientRet
                xchg edi,eax
                mov  [edi+CLIENT.BLOCKPACKET],1
                mov     ecx,[edi+CLIENT.NETADDR]
                stdcall [DisconnectClient],dword[edi+CLIENT.ADDR],[rDATA]    
.DisconnectClientRet:
        pop     edi esi edx ecx ebx
        ret
endp
;*************************************************************************************************************** 








;*************************************************************************************************************** 
proc KickedByBattleEyE,rID,rDATA
        push    ebx ecx edx esi edi     
                stdcall GetClientID,[rID]
                or      eax,eax
                je      .KickedByBattleEyERet
                xchg edi,eax
                mov  [edi+CLIENT.BLOCKPACKET],1
                stdcall [KickedClient],[rID],[rDATA]  
.KickedByBattleEyERet:
        pop     edi esi edx ecx ebx
     ret
endp
;*************************************************************************************************************** 

;*************************************************************************************************************** 
proc scanmem SRCdata:dword, SRCsize:dword, PTRdata:dword,PTRsize:dword, MSKdata:dword
;********************************************************************************************
        push    esi edi ebx ecx edx
 
        ; Длина паттерна больше длины данных?
        mov     eax,[PTRsize]
        cmp     eax,[SRCsize]
        ; Да, возврат -1
        ja      .scanmem_not_found
 
        mov     esi,[SRCdata]
        mov     edi,[PTRdata]
        mov     edx,[MSKdata]
        mov     ebx,esi
        add     ebx,[SRCsize]
        sub     ebx,[PTRsize]
.scanmem_loop:
        xor     ecx,ecx
.scanmem_test_char:
        or      edx,edx
        jz      .scanmem_no_mask
        cmp     byte [edx+ecx],0
        jz      .scanmem_char_equal
 
.scanmem_no_mask:
        mov     al,[esi+ecx]
        cmp     al,[edi+ecx]
        jne     .scanmem_next_pattern
.scanmem_char_equal:
        inc     ecx
        cmp     ecx,[PTRsize]
        jb      .scanmem_test_char
        jmp     .scanmem_found
.scanmem_next_pattern:
        inc     esi
        cmp     esi,ebx
        jbe     .scanmem_loop
 
.scanmem_not_found:
        ; Строка не найдена
        xor      eax,eax
        jmp     .scanmem_ret
 
.scanmem_found:
        ; Строка найдена
        mov     eax,esi
 
.scanmem_ret:
        pop    edx ecx ebx edi esi
        ret
endp
;*************************************************************************************************************** 

;*************************************************************************************************************** 
proc scanbytemem SRCdata:dword, SRCsize:dword, PTRdata:dword,PTRsize:dword,RCsize:dword
;********************************************************************************************
       push     esi edi ebx ecx edx
       mov      eax,[PTRsize]
       add      eax,[RCsize]
       cmp      eax,[SRCsize]
       ja       .scanbyteme_not_found
       mov      esi,[SRCdata]
       mov      edi,[PTRdata]
       xor      ecx,ecx
       xor      ebx,ebx
       dec      ecx
.scanbyteme_loop:
       inc      ecx
       cmp      ecx,[SRCsize]
       ja       .scanbyteme_not_found
       mov      al,[esi+ecx]
       cmp      al,[edi+ebx]
       jne      .scanbyteme_loop
.scanbyteme_found_byte:
       add      ecx,[RCsize]
       inc      ebx
       cmp      ebx,[PTRsize]
       jne      .scanbyteme_loop
.scanbyteme_found:
       mov      eax,1
       jmp      .scanbytemem_ret
.scanbyteme_not_found:
       xor      eax,eax
.scanbytemem_ret:
       pop      edx ecx ebx edi esi
       ret
;********************************************************************************************
endp
;*************************************************************************************************************** 

;***************************************************************************************************************
proc _memcpy,lpStrA:dword,lpStrB:dword,lpSize:dword
     push    ecx esi edi
     mov     edi,[lpStrA]
     mov     esi,[lpStrB]
     mov     ecx,[lpSize]
        dec     ecx
     JNG           .memcpy_loop_ret  
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
proc _lstrcpy_zero,lpStrA:dword,lpSizeA:dword,lpStrB:dword
     push esi edi ebx ecx edx
     mov     edi,[lpStrA]
     mov     esi,[lpStrB]
     mov     ebx,[lpSizeA]
     mov     ecx,-1
.lstrcpy_loop:
     inc     ecx
     mov     al,[esi+ecx]
     mov     [edi+ecx],al
     or      al,al
     jne     .lstrcpy_loop
     mov     [lpSizeA],ecx
.lstrcpy_loop_zero:
     mov     byte[edi+ecx],al
     inc     ecx
     cmp     ecx,ebx
     jna     .lstrcpy_loop_zero
.lstrcpy_ret:
     mov   eax,[lpSizeA]
     pop   edx ecx ebx edi esi
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
proc _lstrlen,lpStr:dword
     mov        eax,[lpStr]
     dec        eax
.lstrlen_loop:
     inc        eax
     cmp        byte [eax],0
     jne        .lstrlen_loop
     sub        eax,[lpStr]
.lstrlen_ret:
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












 







;***************************************************************************************************************
proc TimerProc,uTimerID,uMsg,dwUser,dw1,dw2
locals
    format_hex                  db '%08X',0
    format_dec                  db '%d',0
    format_stats_add    db '%d/%d',0
    format_stats_sub    db '-%d/%d',0
    format_time         db '%02d:%02d:%02d',0
    format_network              db '%d/%d/%d/%d/%d',0
    BuffTimerProc               rb 500h
endl
iglobal
        CConsoleExecute dd 0x46AA60
        str_cfg_load            rb 500h
        server db 'server',0
endg    
push ebx ecx edx esi edi
;--------------------------------------------------
            push   TimerProc_handler  ;Установим наш обработчик
            push   dword[fs:0]
            mov    dword[fs:0], esp
                        cmp byte [GameLevel],0
                        je @f
                        invoke  SendMessage,[_HWND],WM_SETTEXT,0,GameLevel
                        @@:
                        stdcall GetClientCount
                        xchg esi,eax
                        invoke SendMessage,[hList],LVM_GETITEMCOUNT,0,0
                        cmp esi,eax
                        JNG @f
                        invoke SendMessage,[hList],LVM_INSERTITEM,0,lvi
                        @@:
                        cmp esi,eax
                        JNL @f     
                        invoke SendMessage,[hList],LVM_DELETEITEM,NULL,0                    
                        @@: 
                        xor  esi,esi    
timer_loop:             
               stdcall GetClientByNum,esi
               or       eax,eax
               je   timer_loop_end
               xchg edi,eax
               mov  ebx,[edi+CLIENT.ADDR]
               mov  eax,[ebx+0x4028+0x4]
               mov  [edi+CLIENT.pPING],eax
               mov  eax,[ebx+0x4028+0x5C]
               mov  [edi+CLIENT.pRECIVE],eax
               mov  eax,[ebx+0x4028+0x64]
               mov  [edi+CLIENT.pSEND],eax
               mov  eax,[ebx+0x4028+0x24]
               mov  [edi+CLIENT.pRETRIED],eax
               mov  eax,[ebx+0x4028+0x2C]
               mov  [edi+CLIENT.pDROPPED],eax
               mov  eax,[ebx+0x4028+0x74]
               mov  [edi+CLIENT.pBLOCKED],eax
.client_time:
                        add     byte[edi+CLIENT.wSecond],1
                        cmp  byte[edi+CLIENT.wSecond],60
                        jb      .client_time_end
                        mov     byte[edi+CLIENT.wSecond],0
                        add     byte[edi+CLIENT.wMinute],1
                        cmp     byte[edi+CLIENT.wMinute],60
                        jb      .client_time_end
                        mov     byte[edi+CLIENT.wMinute],0
                        add     byte[edi+CLIENT.wHour],1
                        cmp     byte[edi+CLIENT.wHour],24
                        jb      .client_time_end
                        mov     byte[edi+CLIENT.wHour],0
.client_time_end:
.recive_packet:
                        cmp byte[edi+CLIENT.wMinute],3
                        jb      .recive_packet_kick_end          
                        cmp     [edi+CLIENT.pRECIVE],120
                        jb      .recive_packet_kick_end
                        add     dword[edi+CLIENT.ClientKick],1
                        cmp     dword[edi+CLIENT.ClientKick],3
                        jb      .recive_packet_kick_end
.recive_packet_kick:
                        ; нельзя выбросить серверного клиента !
                        stdcall scanmem,[edi+CLIENT.addr_player_name],server.size,server,server.size,server    ; server's client
                        or  eax,eax
                        jne timer_loop_next
                        stdcall KickedByBattleEyE,[edi+CLIENT.ID],xkicked_by_server
                        ; после того как выбросили игрока, код ниже нельзя выполнить, т.к данные для этого игрока уже удалены!
                        jmp timer_loop_next
.recive_packet_kick_end:
               cmp [PROC_SHOW_HIDE],1
                        je      timer_loop_next
               lea      eax,[edi+CLIENT.StrNAME]
               stdcall  SETITEMTEXT,eax,esi,0
               lea  eax,[edi+CLIENT.StrIP]
               stdcall  SETITEMTEXT,eax,esi,1
               movzx  eax,byte[edi+CLIENT.wHour]
               movzx  ebx,byte[edi+CLIENT.wMinute]
               movzx  ecx,byte[edi+CLIENT.wSecond]
               cinvoke wsprintf,addr BuffTimerProc,addr format_time,eax,ebx,ecx
               stdcall  SETITEMTEXT,addr BuffTimerProc,esi,4
                  cinvoke wsprintf,addr BuffTimerProc,addr format_dec,dword[edi+CLIENT.pPING]
                  stdcall  SETITEMTEXT,addr BuffTimerProc,esi,2
               mov      ecx,[edi+CLIENT.ADDR]
               mov  ecx,[ecx+0x00004160]
             movzx eax,word  [ecx+0x45]
                     movzx ebx,word  [ecx+0x47]
                     sub eax,ebx
                     JNL .client_statictic_add
.client_statictic_sub:
                  movzx eax,word  [ecx+0x45]
                  sub ebx,eax
                  movzx eax,word  [ecx+0x4F]
                     cinvoke  wsprintf,addr BuffTimerProc,addr format_stats_sub,eax,ebx
                     stdcall  SETITEMTEXT,addr BuffTimerProc,esi,3
                     jmp .client_statictic_end
.client_statictic_add:
                     movzx ebx,word  [ecx+0x4F]
                     movzx eax,word  [ecx+0x45]
                     cinvoke  wsprintf,addr BuffTimerProc,addr format_stats_add,eax,ebx
                     stdcall  SETITEMTEXT,addr BuffTimerProc,esi,3
.client_statictic_end:
               ;------------------------------------------------------------[GetMPS_RECIVE]-----------[GetMPS_SEND]-----------[Get_RETRIED]-------------[Get_DROPPED]-------------[Get_Blocked]-------
                  cinvoke wsprintf,addr BuffTimerProc,addr format_network,dword[edi+CLIENT.pRECIVE],dword[edi+CLIENT.pSEND],dword[edi+CLIENT.pRETRIED],dword[edi+CLIENT.pDROPPED],dword[edi+CLIENT.pBLOCKED]
                  stdcall  SETITEMTEXT,addr BuffTimerProc,esi,5
timer_loop_next:
               add esi,1
               jmp timer_loop
timer_loop_end:
            pop  dword[fs:0]                ;Убираем обработчик
            add  esp,4
            pop  edi esi edx ecx ebx
            ret
            ;-----------------------------------------------------
TimerProc_handler:
            mov ecx,[esp+8];pFrame равно тому, что мы поместили в fs:[0]
            mov eax,dword[esp+0Ch];Context
            mov dword[eax+0C4h],ecx;ESP
            mov dword[eax+0B8h],timer_loop_end;ThreadCont
            xor eax,eax ;Продолжить выполнение
            retn 4*4 
endp
;***************************************************************************************************************

;***************************************************************************************************
proc ReNamePlayer,pAddr
        push  ecx esi
     mov   esi,[pAddr]
     xor   ecx,ecx
     dec   esi
     dec   ecx
.rename_player_loop:
     inc   esi
     inc   ecx
     movzx eax,byte [esi]
     cmp  al,0x0
     je  .rename_player_ret
     cmp  al,0x25
     jne  .rename_player_loop
     mov  byte [esi],'_'
     jmp  .rename_player_loop
.rename_player_ret:
     xchg eax,ecx
     pop  esi ecx
    ret
endp
;***************************************************************************************************

;***************************************************************************************************************
proc CreateMenuMouse,HwD
local pt:POINT
;----------------------------------------
id_testers_mp_agroprom                  equ 500h
id_testers_mp_atp                               equ 501h
id_testers_mp_darkvalley                        equ 502h
id_testers_mp_factory                   equ 503h
id_testers_mp_lost_village              equ 504h
id_testers_mp_military_1                        equ 505h
id_testers_mp_pool                              equ 506h
id_testers_mp_railroad                  equ 507h
id_testers_mp_rostok                    equ 508h
id_testers_mp_workshop                  equ 509h
;----------------------------------------
id_dm                                           equ 600h
id_tdm                                          equ 601h
id_ah                                           equ 602h
;----------------------------------------
id_kicked                                               equ 700h
id_banned                                               equ 701h
id_reconnect                                    equ 702h   
id_sendmessage_by_id                    equ 703h
;----------------------------------------
iglobal
        H_MENU          dd 0
        H_MENU_1                dd 0
        H_MENU_2                dd 0
        ;-------------------------------------------------------
        addr_agroprom                   dd      testers_mp_agroprom
        addr_atp                                dd      testers_mp_atp
        addr_darkvalley         dd      testers_mp_darkvalley   
        addr_factory                    dd      testers_mp_factory
        addr_lost_village               dd      testers_mp_lost_village
        addr_military_1         dd      testers_mp_military_1   
        addr_pool                               dd      testers_mp_pool
        addr_railroad                   dd      testers_mp_railroad
        addr_rostok                     dd      testers_mp_rostok
        addr_workshop                   dd      testers_mp_workshop
        ;-------------------------------------------------------
        addr_deathmatch         dd      deathmatch
        addr_teamdeathmatch             dd      teamdeathmatch
        addr_artefacthunt               dd      artefacthunt
        ;-------------------------------------------------------
        kicked                          db 'Kicked',0   
        banned                          db 'Banned',0   
        reconnect                               db 'Reconnect',0  
        sendmessagebyid         db 'SendMessageByID',0          
        ;-------------------------------------------------------
        gametype                                db 'GameType',0
        gamelevel                               db 'GameLevel',0
        ;-------------------------------------------------------
        deathmatch                      db 'deathmatch',0
        teamdeathmatch                  db 'teamdeathmatch',0
        artefacthunt                    db 'artefacthunt',0
        ;-------------------------------------------------------
        testers_mp_agroprom             db 'testers_mp_agroprom',0
        testers_mp_atp                  db 'testers_mp_atp',0
        testers_mp_darkvalley   db 'testers_mp_darkvalley',0
        testers_mp_factory              db 'testers_mp_factory',0
        testers_mp_lost_village db 'testers_mp_lost_village',0
        testers_mp_military_1   db 'testers_mp_military_1',0
        testers_mp_pool         db 'testers_mp_pool',0
        testers_mp_railroad             db 'testers_mp_railroad',0
        testers_mp_rostok               db 'testers_mp_rostok',0
        testers_mp_workshop             db 'testers_mp_workshop',0
        ;-------------------------------------------------------
endg
         cmp [H_MENU],NULL
jne @f
           invoke  CreatePopupMenu
         mov    [H_MENU],eax
         invoke  CreatePopupMenu
         mov    [H_MENU_1],eax
         invoke  CreatePopupMenu
         mov    [H_MENU_2],eax
;-----------------------------------------------------------------------------------------------------------
              
              
              
              
           invoke    AppendMenu,[H_MENU],   MF_STRING ,id_kicked,kicked
           invoke    AppendMenu,[H_MENU],   MF_STRING ,id_banned,banned
           invoke    AppendMenu,[H_MENU],   MF_STRING ,id_reconnect,reconnect
           invoke    AppendMenu,[H_MENU],   MF_STRING ,id_sendmessage_by_id,sendmessagebyid
           ;-------------------------------------------------------------------------;
           invoke    AppendMenu,[H_MENU],    MF_POPUP   ,[H_MENU_1],gametype
           invoke    AppendMenu,[H_MENU],    MF_POPUP   ,[H_MENU_2],gamelevel
           ;-------------------------------------------------------------------------;
           invoke    AppendMenu,[H_MENU_1],        MF_STRING   ,id_dm, deathmatch
           invoke    AppendMenu,[H_MENU_1],        MF_STRING   ,id_tdm,teamdeathmatch
           invoke    AppendMenu,[H_MENU_1],        MF_STRING   ,id_ah, artefacthunt
         ;----------------------------------------------------------------------------------------------------;
         invoke    AppendMenu,[H_MENU_2]   ,   MF_STRING  ,id_testers_mp_agroprom       ,testers_mp_agroprom
         invoke    AppendMenu,[H_MENU_2]   ,   MF_STRING  ,id_testers_mp_atp            ,testers_mp_atp
         invoke    AppendMenu,[H_MENU_2]   ,   MF_STRING  ,id_testers_mp_darkvalley,testers_mp_darkvalley
         invoke    AppendMenu,[H_MENU_2]   ,   MF_STRING  ,id_testers_mp_factory        ,testers_mp_factory
         invoke    AppendMenu,[H_MENU_2]   ,   MF_STRING  ,id_testers_mp_lost_village,testers_mp_lost_village
         invoke    AppendMenu,[H_MENU_2]   ,   MF_STRING  ,id_testers_mp_military_1,testers_mp_military_1
         invoke    AppendMenu,[H_MENU_2]   ,   MF_STRING  ,id_testers_mp_pool           ,testers_mp_pool
         invoke    AppendMenu,[H_MENU_2]   ,   MF_STRING  ,id_testers_mp_railroad       ,testers_mp_railroad
         invoke    AppendMenu,[H_MENU_2]   ,   MF_STRING  ,id_testers_mp_rostok ,testers_mp_rostok
         invoke    AppendMenu,[H_MENU_2]   ,   MF_STRING  ,id_testers_mp_workshop       ,testers_mp_workshop
         ;----------------------------------------------------------------------------------------------------;
@@:
           lea     eax,[pt]
         invoke    GetCursorPos, eax
         invoke    TrackPopupMenu, [H_MENU],TPM_TOPALIGN or TPM_LEFTALIGN,[pt.x],[pt.y],NULL,[HwD],NULL
;-----------------------------------------------------------------------------------------------------------
     ret
endp
;***************************************************************************************************************


;*************************************************************************************************************** 
proc StrToHex lpStr:dword,lsStrOut
;********************************************************************************************
        push    ebx edx ecx edx edi esi
 
        xor     eax,eax
        xor     ecx,ecx
        mov     esi,[lpStr]
        mov     edi,[lsStrOut]
.str2hex_xor:
        xor edx,edx
.str2hex_loop:
        movsx   ebx,byte [esi]
        or      bl,20h
        cmp     bl,'0'
        jb      .str2hex_ret
        cmp     bl,'9'
        ja      .str2hex_1
        sub     bl,'0'
        jmp     .str2hex_calc
.str2hex_1:
        cmp     bl,'a'
        jb      .str2hex_ret
        cmp     bl,'f'
        ja      .str2hex_ret
        sub     bl,('a'-10)
.str2hex_calc:
        inc     esi
        inc     edx
        ;-----------------------------------------------------------
        shl eax,04
        add eax,ebx
        cmp edx,2
        jne .str2hex_loop
        mov byte [edi],al
        inc     ecx     ;Loop Size
        inc     edi
        jmp .str2hex_xor
.str2hex_ret:
        xchg    eax,ecx
        pop     esi edi edx ecx edx ebx
        ret
;********************************************************************************************
endp
;*************************************************************************************************************** 

;*************************************************************************************************************** 
proc HexToStr,_ADDR,_ADDROUT,_SIZE,_SPACE
;********************************************************************************************
       push esi edi ecx

       mov esi,[_ADDR]
       mov edi,[_ADDROUT]
       xor ecx,ecx
.HexToStrConvert:
       cmp ecx,[_SIZE]
       je .HexToStrWriteEnd
       movzx eax,byte [esi]
       aam 10h
       cmp al,10
       sbb al,0x69
       das
       xchg ah,al
       cmp al,10
       sbb al,0x69
       das
.HexToStrWrite:
;----------------------------------------
       mov byte [edi],al
       xchg ah,al
       inc edi
       mov byte [edi],al
       inc edi
;----------------------------------------
       cmp [_SPACE],0
je .spaceNo
       mov byte [edi],' '
       inc edi
.spaceNo:
;----------------------------------------
       inc esi
       inc ecx
       jmp .HexToStrConvert
.HexToStrWriteEnd:
       mov  byte [edi],0
       sub  edi,[_ADDROUT]
       xchg eax,edi
pop ecx edi esi
    ret
;*********************************************************************************************
endp
;*************************************************************************************************************** 

;***************************************************************************************************************
proc GETITEM,rBuff,rD,rR
          mov eax,[rD]
          mov [lvi.iItem],eax
          mov eax,[rR]
          mov [lvi.iSubItem],eax
          mov eax,[rBuff]         
          mov [lvi.pszText],eax
          mov [lvi.cchTextMax],0x50
          invoke SendMessage,[hList],LVM_GETITEM, 0 ,lvi
          mov eax,[rBuff]
          mov [lvi.iItem],0
          mov [lvi.iSubItem],0
          mov [lvi.pszText],0
          ret
endp
;***************************************************************************************************************

;***************************************************************************************************************
proc INSERTCOLUMN,_hList,pszText_1,pszText_2,pszText_3,pszText_4,pszText_5,pszText_6,cx_1,cx_2,cx_3,cx_4,cx_5,cx_6
   COLLORBACKGROUND = 00A6F7F0h         ;0x9F55FB
   invoke  SendMessage,[_hList],LVM_SETEXTENDEDLISTVIEWSTYLE,NULL,LVS_EX_FULLROWSELECT+LVS_EX_GRIDLINES;+LVS_EX_ONECLICKACTIVATE
   invoke  SendMessage,[_hList], LVM_SETTEXTBKCOLOR, 0,COLLORBACKGROUND
;-----------------------------------------------------------------------------------------------------------
         mov [lvc.mask],LVCF_TEXT+LVCF_WIDTH
         mov eax,[pszText_1]
         mov [lvc.pszText],eax
         mov eax,[cx_1]
         mov [lvc.cx],eax            
         invoke SendMessage,[_hList], LVM_INSERTCOLUMN,0,lvc
         or  [lvc.mask],LVCF_FMT
         mov [lvc.fmt],LVCFMT_LEFT
         mov eax,[pszText_2]
         mov [lvc.pszText],eax
         mov eax,[cx_2]
         mov [lvc.cx],eax
         invoke SendMessage,[_hList], LVM_INSERTCOLUMN, 1 ,lvc
         or  [lvc.mask],LVCF_FMT
         mov [lvc.fmt],LVCFMT_LEFT
         mov eax,[pszText_3]
         mov [lvc.pszText],eax
         mov eax,[cx_3]
         mov [lvc.cx],eax
         invoke SendMessage,[_hList], LVM_INSERTCOLUMN, 2 ,lvc
         or  [lvc.mask],LVCF_FMT
         mov [lvc.fmt],LVCFMT_LEFT
         mov eax,[pszText_4]
         mov [lvc.pszText],eax
         mov eax,[cx_4]
         mov [lvc.cx],eax
         invoke SendMessage,[_hList], LVM_INSERTCOLUMN, 3 ,lvc
         or  [lvc.mask],LVCF_FMT
         mov [lvc.fmt],LVCFMT_LEFT
         mov eax,[pszText_5]
         mov [lvc.pszText],eax
         mov eax,[cx_5]
         mov [lvc.cx],eax
         invoke SendMessage,[_hList], LVM_INSERTCOLUMN, 4 ,lvc
         or  [lvc.mask],LVCF_FMT
         mov [lvc.fmt],LVCFMT_LEFT
         mov eax,[pszText_6]
         mov [lvc.pszText],eax
         mov eax,[cx_6]
         mov [lvc.cx],eax
         invoke SendMessage,[_hList], LVM_INSERTCOLUMN, 5 ,lvc
         ret
endp
;***************************************************************************************************************

;***************************************************************************************************************
proc SETITEMTEXT,pszText,iItem,iSubItem
push  ebx edx ecx
     mov [lvi.mask],LVIF_TEXT+LVIF_STATE
     mov eax,[pszText]
     mov [lvi.pszText],eax
     mov eax,[iItem]
     mov [lvi.iItem],eax
     mov eax,[iSubItem]
     mov [lvi.iSubItem],eax
     invoke SendMessage,[hList],LVM_SETITEM,0, lvi
     invoke RtlZeroMemory,lvi,sizeof.LV_ITEM
pop  ecx edx ebx     
     ret
endp
;***************************************************************************************************************

;***************************************************************************************************************
proc GetModuleAddress
iglobal
        xrNetServer                             db 'xrNetServer.dll',0
        xrGame                                  db 'xrGame.dll',0
        xrCore                                  db 'xrCore.dll',0
        xrWS2_32                                        db 'WS2_32.dll',0
        xrWSASendTo                             db 'WSASendTo',0
        xrWSARecvFrom                           db 'WSARecvFrom',0
        xrWSAGetOverlappedResult                db 'WSAGetOverlappedResult',0


        XR_WSASendTo                            dd 0
        XR_WSARecvFrom                          dd 0
        XR_WSAGetOverlappedResult       dd 0     
        
        
        
        xr_3da.exe                              dd 0
        xrNetServer.dll                 dd 0
        xrGame.dll                              dd 0
        xrCore.dll                              dd 0
        ChangeGameType                          dd 0
        ChangeGameLevel                 dd 0
        IPureServerRecieve                      dd 0
        IPureClientSend                 dd 0
        ChatSend                                        dd 0
        KickedClient                            dd 0
        GetObjectByID                           dd 0
        xrCore.msg                              dd 0
        xrFlushLog                              dd 0
        SendToBuff                              dd 0
        SendBroadcast                           dd 0
        DisconnectClient                        dd 0
endg 


        mov [xr_3da.exe],0x00400000
        invoke GetModuleHandle,xrNetServer
        mov [xrNetServer.dll],eax
        mov ecx,eax
        add eax,0x93B0
        mov [IPureServerRecieve],eax
        mov eax,ecx
        add eax,0x3CC0
        mov [IPureClientSend],eax
        mov eax,ecx
        add eax,0x93B0
        mov [IPureServerRecieve],eax
        mov eax,ecx
        add eax,0xA090
        mov [SendToBuff],eax
        mov eax,ecx
        add eax,0xA220
        mov [SendBroadcast],eax
        mov eax,ecx
        add eax,0xA590
        mov [DisconnectClient],eax
        
        
        
        
    
    
    
        invoke GetModuleHandle,xrGame
        mov [xrGame.dll],eax
        mov ecx,eax
        add eax,0x28AE10
        mov [ChangeGameType],eax
        mov eax,ecx
        add eax,0x28AEE0
        mov [ChangeGameLevel],eax 
        mov eax,ecx
        add eax,0x28C2C0 
        mov [ChatSend],eax
        mov eax,ecx
        add eax,0x29C860        ;289FC0
        mov [KickedClient],eax
        mov eax,ecx
        add eax,0x2C1560         
        mov [GetObjectByID],eax
        
        
        
        
        invoke GetModuleHandle,xrCore
        mov [xrCore.dll],eax
        mov ecx,eax
        add eax,0x18640
        mov [xrCore.msg],eax
        mov eax,ecx
        add eax,0x18540 ;Flush Log
        mov [xrFlushLog],eax
        


        invoke GetModuleHandle,xrWS2_32
        xchg    esi,eax
        invoke GetProcAddress,esi,xrWSASendTo
        mov [XR_WSASendTo],eax
        invoke GetProcAddress,esi,xrWSARecvFrom
        mov [XR_WSARecvFrom],eax
        invoke GetProcAddress,esi,xrWSAGetOverlappedResult
        mov [XR_WSAGetOverlappedResult],eax
        
        
    ret
endp
;***************************************************************************************************************

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
proc SendToAll,rData,rSize,rID
   push  esi edi ebx ecx edx
   mov eax,[xrGame.dll]
   add eax,0x4589FC
   mov eax,[eax]
   mov eax,[eax]
   mov ecx,[eax+0x45F4]
   stdcall [SendBroadcast],[rID],[rData],[rSize],0x200
   pop   edx ecx ebx edi esi
   ret
endp
;***************************************************************************************************************

;***************************************************************************************************************
proc SendTo,rData,rSize,rID
   push  esi edi ebx ecx edx   
   mov eax,[xrGame.dll]
   add eax,0x4589FC
   mov eax,[eax]
   mov eax,[eax]
   mov ecx,[eax+0x45F4]
   stdcall [SendToBuff],[rID],[rData],[rSize],0x200,NULL
   pop   edx ecx ebx edi esi
   ret 
endp
;***************************************************************************************************************
        
;***************************************************************************************************************
proc RelocationImage,rModule,rAlock,rGame
 
       push  esi edi ebx ecx edx

       mov   esi,[rModule]
       add   esi,[esi+3Ch]        ;PE
       mov   esi,[esi+0xA0]       ;RelocationTableAdress
       add   esi,[rAlock]         ;ALLOCK ADDR
       mov   ebx,[rGame]          ;Game ALockADDR
       sub   ebx,[rModule]        ;BASE MODULE

       cmp   dword  [esi],00
       je    end_ex
loop_ex:
       mov   edx,[esi+04]
       lea   eax,[esi+04]
       mov   [rModule],eax
       cmp   edx,08
       jb    loop_ex_ex
       add   edx,-08
       shr   edx,1
       je    loop_ex_ex
       xor   ecx,ecx
movzx_ex:
       movzx eax,word  [esi+ecx*2+08]
       test  eax,eax
       je    loop_ex_inc
       and   eax,0xFFF
       add   eax,[esi]
       add   eax,[rAlock]
       add   [eax],ebx
loop_ex_inc:
       inc   ecx
       cmp   ecx,edx
       jb    movzx_ex
       mov   eax,[rModule]
loop_ex_ex:
       add   esi,[eax]
       cmp   dword[esi],00
jne loop_ex

end_ex:
       pop   edx ecx ebx edi esi
       ret
endp
;***************************************************************************************************************


;***************************************************************************************************************
proc checkforupdates
iglobal
        hInternetSession                dd 0
        hURL                                    dd 0
        URL                                     db 'https://raw.githubusercontent.com/darkzzz/xray/master/xray/xray.exe',0
        CurrentDirectoryFile    rb 500h
        UPDATE_OK                               db 'UPDATE_OK',0
        UPDATE_BAG                      db 'UPDATE_BAG',0    
endg    
            invoke InternetOpen,NULL,INTERNET_OPEN_TYPE_PRECONFIG,NULL, NULL,NULL
            mov [hInternetSession],eax
            invoke InternetOpenUrl,[hInternetSession],URL,NULL,NULL,INTERNET_FLAG_RELOAD or INTERNET_FLAG_DONT_CACHE, NULL
            mov [hURL],eax
            test eax,eax
            je .checkforupdates_bag
            invoke  VirtualAlloc,0,0x100000,MEM_COMMIT+MEM_RESERVE,PAGE_EXECUTE_READWRITE
            mov    [rAlock],eax
            mov    esi,eax
@@:
            invoke InternetReadFile,[hURL],esi,0x1024,Buff
            test  eax,eax
              je  @f
            mov   edi,dword[Buff]
            add   esi,edi
            test  edi,edi
              jne @b
@@:
            mov  eax,[rAlock]
            sub  esi,[rAlock]
            mov  [rSize],esi
            invoke CreateFile,CurrentDirectoryFile,GENERIC_READ+GENERIC_WRITE,NULL,NULL,TRUNCATE_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL
              cmp eax,INVALID_HANDLE_VALUE
              je     .checkforupdates_bag
              mov     edi,eax
              mov     eax,[rAlock]
              invoke  WriteFile,edi,eax,[rSize],Buff,NULL
              test    eax,eax
              je      .checkforupdates_bag 
              invoke  CloseHandle,edi
              invoke  VirtualFree,[rAlock],0,MEM_RELEASE
              invoke  InternetCloseHandle,[hURL]
              invoke  InternetCloseHandle,[hInternetSession]
.checkforupdates_ok:
              mov eax,1
              jmp .checkforupdates_ret      
.checkforupdates_bag:
              xor eax,eax
.checkforupdates_ret:
            ret
endp
;***************************************************************************************************************



;***************************************************************************************************************
proc StrToHexEAX lpStr:dword
        push    ebx edx esi
 
        xor     eax,eax
        mov     esi,[lpStr]
.str2hex_loop:
        movsx   ebx,byte [esi]
        or      bl,20h
        cmp     bl,'0'
        jb      .str2hex_ret
        cmp     bl,'9'
        ja      .str2hex_1
        sub     bl,'0'
        jmp     .str2hex_calc
.str2hex_1:
        cmp     bl,'a'
        jb      .str2hex_ret
        cmp     bl,'f'
        ja      .str2hex_ret
        sub     bl,('a'-10)
.str2hex_calc:
        imul    eax,16
        add     eax,ebx
        inc     esi
        jmp     .str2hex_loop
.str2hex_ret:
        pop     esi edx ebx
        ret
endp  
;***************************************************************************************************************


;***************************************************************************************************************
proc StrToDexEAX lpStr:dword
        push    ebx edx esi
        xor     eax,eax
        mov     esi,[lpStr]
.str2dec_loop:
        movsx   ebx,byte [esi]
        sub     bl,'0'
        cmp     bl,10
        jnb     .str2dec_ret
        imul    eax,10
        add     eax,ebx
        inc     esi
        jmp     .str2dec_loop
.str2dec_ret:
        pop     esi edx ebx
        ret
endp
;***************************************************************************************************************



;***************************************************************************************************************
proc HexToStrEAX,lpStr:dword
        push  ecx edi
        mov     edi,[lpStr]
        mov     ecx,8
.hex_to_str:
        rol     eax,4
        push    eax
        and     al,0Fh
        daa
        add     al,0F0h
        adc     al,40h
        stosb
        pop     eax
        loop    .hex_to_str
        mov     al,0
        stosb
        pop edi ecx
        ret
endp 
;***************************************************************************************************************


;***************************************************************************************************************
proc CopyCoLLoRPacket,CopyBuff,CoLLoR
iglobal
        CopyCollorData       db 0x13,0x00,0x24,0x00,0x00,0x00,0
endg
     push    ebx ecx edx esi edi
     xor     eax,eax
     mov     edi,[CopyBuff]
     add     edi,CopyCollorData.size
     cmp     [CoLLoR],0
     je      @f
     invoke  lstrlen,[CoLLoR]
     @@:
     add     edi,eax
     xchg    esi,eax
     invoke  lstrlen,[CopyBuff]
     mov     byte [edi+eax],0
     invoke  RtlMoveMemory,edi,[CopyBuff],eax    ;12345
     invoke  RtlMoveMemory,[CopyBuff],CopyCollorData,CopyCollorData.size
     add     [CopyBuff],CopyCollorData.size
     invoke  RtlMoveMemory,[CopyBuff],[CoLLoR],esi
     invoke  lstrlen,[CopyBuff]
     add     eax,CopyCollorData.size+1
.ret:
     pop  edi esi edx ecx ebx
     ret
endp    
;***************************************************************************************************************
  


;*************************************************************************************************************** 
proc GetObject,pClass:dword,ObjectID:dword
        push  ebx ecx edx esi edi
                mov     edi,[pClass]
                or      edi,edi
                je      .get_object_bag
                mov  edx,[edi+CLIENT.NETADDR]
                mov  edx,[edx+0x108D4]
                mov  eax,[ObjectID]
                stdcall [GetObjectByID]
                or      eax,eax
                jne     .get_object_ret
.get_object_bag:

                xor eax,eax
.get_object_ret:
        pop  edi esi edx ecx ebx
        ret
endp
;***************************************************************************************************************
  
  


  
  