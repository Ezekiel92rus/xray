macro _invoke proc,[arg]
       { 
        common 
        stdcall [proc],arg 
       } 



struc db [data]
       {
         common
         label .data byte
         db data
         .size = $-1-.data
       }



struc rb [data]
       {
         common
         label .data byte
         rb data
         .size = $-.data
       } 














macro proc [args]			; define procedure
 { common
    match name params, args>
    \{ define@proc name,<params \} }

prologue@proc equ prologuedef

macro prologuedef procname,flag,parmbytes,localbytes,reglist
 { local loc
   loc = (localbytes+3) and (not 3)
   parmbase@proc equ ebp+8
   localbase@proc equ ebp-loc
   if parmbytes | localbytes
    mov	edi,edi
    push ebp
    mov ebp,esp
    if localbytes
     sub esp,loc
    end if
   end if
   irps reg, reglist \{ push reg \} }

epilogue@proc equ epiloguedef