section '.data' data readable writeable
;-----------------------------------------------------------------------
 INFINITE	equ -1
_SYSTEMTIME	SYSTEMTIME
_FILETIME	FILETIME 
node            NOTIFYICONDATA ; »конка в трее
WM_SHELLNOTIFY = WM_USER+5 












struct PROCESSENTRY32 
	dwSize			dd ?
	cntUsage			dd ?
	th32ProcessID		dd ?
	th32DefaultHeapID	dd ?
	th32ModuleID		dd ?
	cntThreads		dd ?
	th32ParentProcessID	dd ?
	pcPriClassBase		dd ?
	dwFlags			dd ?
	szExeFile			rb MAX_PATH
ends
_PROCESSENTRY32 PROCESSENTRY32






struct LISTVIEW_BOX
	hdr		NMHDR
	iItem	dd ?
	iSubItem	dd ?
	uNewState	dd ?
	uOldState	dd ?
	uChanged	dd ?
	ptAction	POINT
	lParam	dd ?
ends
virtual at 0
	_NML  NM_LISTVIEW
end virtual 

 


virtual at 0
	_LV_KEYDOWN  LV_KEYDOWN
end virtual 


struct ACTCTX
	cbSize			dd 0
	dwFlags			dd 0
	lpSource			dd 0
	wLangId			dd 0
	lpAssemblyDirectory	dd 0
	lpResourceName		dd 0
	lpApplicationName	dd 0
	hModule			dd 0
	PACTCTX			dd 0
ends
_ACTCTX ACTCTX



struct NMLVCUSTOMDRAW
	hwndFrom			dd ?
	idFrom			dd ?
	code				dd ?
	dwDrawStage		dd ?
	hdc				dd ?
	left				dd ?
	top				dd ?
	right			dd ?
	bottom			dd ?
	dwItemSpec		dd ?
	uItemState		dd ?
	lItemlParam		dd ?
	clrText			dd ?
	clrTextBk			dd ?
ends
virtual at 0
	nml NMLVCUSTOMDRAW
end virtual 









struct _RTL_CRITICAL_SECTION
    DebugInfo       dd 0
    LockCount       dd 0
    RecursionCount  dd 0
    OwningThread    dd 0
    LockSemaphore   dd 0
    SpinCount       dd _SpinCount
ends 
RTL_CRITICAL_SECTION _RTL_CRITICAL_SECTION    
_SpinCount          dd 0 

























 


 


;------------------------------------------------
INTERNET_OPEN_TYPE_PRECONFIG		= 0
HTTP_QUERY_CONTENT_LENGTH		= 0x5
HTTP_QUERY_FLAG_NUMBER			= 0x20000000
;------------------------------------------------
INTERNET_FLAG_RELOAD			= 0x80000000
INTERNET_FLAG_DONT_CACHE			= 0x04000000
;------------------------------------------------ 
INTERNET_FLAG_PASSIVE			= 0x08000000
INTERNET_SERVICE_FTP			= 1
FTP_TRANSFER_TYPE_BINARY			= 2
;------------------------------------------------ 
LVN_FIRST						= 0-100
LVN_ITEMACTIVATE				= LVN_FIRST - 14
LVM_GETSELECTIONMARK			= LVM_FIRST + 66
LVM_ENSUREVISIBLE				= LVM_FIRST + 19
lvc	LV_COLUMN
lvi	LV_ITEM  



 
                      
                      
                      
                      
                      



hInstance				dd 0
hSnapshot				dd 0
hProcess				dd 0
RemoteThreadBaseAddress	dd 0
rGame				dd 0
rAlock				dd 0
rSize				dd 0
pName				db 'xr_3da.exe',0




















 





























		        
	         





























;=========================================================================================================
WEAPONS:
  ;------------------------------------------------------------
   mp_wpn_knife             dd rmp_wpn_knife+1+1
   mp_wpn_pm                dd rmp_wpn_pm+1+1
   mp_wpn_pb                dd rmp_wpn_pb+1+1
   mp_wpn_fort              dd rmp_wpn_fort+1+1
   mp_wpn_walther           dd rmp_wpn_walther+1+1
   mp_wpn_colt1911          dd rmp_wpn_colt1911+1+1
   mp_wpn_sig220            dd rmp_wpn_sig220+1+1
   mp_wpn_usp               dd rmp_wpn_usp+1+1
   mp_wpn_desert_eagle      dd rmp_wpn_desert_eagle+1+1
  ;------------------------------------------------------------
   mp_wpn_bm16              dd rmp_wpn_bm16+1+1
   mp_wpn_wincheaster1300   dd rmp_wpn_wincheaster1300+1+1
   mp_wpn_spas12            dd rmp_wpn_spas12+1+1
   ;------------------------------------------------------------
   mp_wpn_ak74u             dd rmp_wpn_ak74u+1+1
   mp_wpn_mp5               dd rmp_wpn_mp5+1+1
   mp_wpn_ak74              dd rmp_wpn_ak74+1+1
   mp_wpn_l85               dd rmp_wpn_l85+1+1
   mp_wpn_abakan            dd rmp_wpn_abakan+1+1
   mp_wpn_lr300             dd rmp_wpn_lr300+1+1
   mp_wpn_groza             dd rmp_wpn_groza+1+1
   mp_wpn_sig550            dd rmp_wpn_sig550+1+1
   mp_wpn_val               dd rmp_wpn_val+1+1
   mp_wpn_g36               dd rmp_wpn_g36+1+1
   mp_wpn_fn2000            dd rmp_wpn_fn2000+1+1
   ;------------------------------------------------------------
   mp_wpn_vintorez           dd rmp_wpn_vintorez+1+1
   mp_wpn_svd                dd rmp_wpn_svd+1+1
   mp_wpn_svu                dd rmp_wpn_svu+1+1
   mp_wpn_gauss              dd rmp_wpn_gauss+1+1
;------------------------------------------
   mp_wpn_rpg7               dd rmp_wpn_rpg7+1+1
   mp_wpn_rg_6               dd rmp_wpn_rg_6+1+1
;-------------------------------------------
   mp_grenade_rgd5           dd rmp_grenade_rgd5+1+1
   mp_grenade_f1             dd rmp_grenade_f1+1+1
   mp_grenade_gd_05          dd rmp_grenade_gd_05+1+1
   mp_ammo_vog_25            dd rmp_ammo_vog_25+1+1
   mp_ammo_m209              dd rmp_ammo_m209+1+1
   mp_ammo_og_7b             dd rmp_ammo_og_7b+1+1
;-------------------------------------------
   mp_scientific_outfit       dd rmp_scientific_outfit+1+1
   mp_military_stalker_outfit dd rmp_military_stalker_outfit+1+1
   mp_exo_outfit              dd rmp_exo_outfit+1+1
;-------------------------------------------
   mp_device_torch                    dd rmp_device_torch+1+1
   mp_wpn_addon_silencer              dd rmp_wpn_addon_silencer+1+1
   mp_wpn_addon_scope                 dd rmp_wpn_addon_scope+1+1
   mp_wpn_addon_scope_susat           dd rmp_wpn_addon_scope_susat+1+1
   mp_wpn_addon_grenade_launcher      dd rmp_wpn_addon_grenade_launcher+1+1
   mp_wpn_addon_grenade_launcher_m203 dd rmp_wpn_addon_grenade_launcher_m203+1+1
   mp_medkit                          dd rmp_medkit+1+1
   mp_antirad                         dd rmp_antirad+1+1
   mp_detector_advances               dd rmp_detector_advances+1+1
   mp_wpn_binoc                       dd rmp_wpn_binoc+1+1
;-------------------------------------------






;-------------------------------------------
   mp_ammo_9x18_fmj          dd rmp_ammo_9x18_fmj+1+1
   mp_ammo_9x18_pmm          dd rmp_ammo_9x18_pmm+1+1
   mp_ammo_9x19_fmj          dd rmp_ammo_9x19_fmj+1+1
   mp_ammo_9x19_pbp          dd rmp_ammo_9x19_pbp+1+1
   mp_ammo_11_43x23_fmj      dd rmp_ammo_11_43x23_fmj+1+1
   mp_ammo_11_43x23_hydro    dd rmp_ammo_11_43x23_hydro+1+1
;-------------------------------------------
   mp_ammo_5_45x39_fmj       dd rmp_ammo_5_45x39_fmj+1+1
   mp_ammo_5_45x39_ap        dd rmp_ammo_5_45x39_ap+1+1
   mp_ammo_5_56x45_ss190     dd rmp_ammo_5_56x45_ss190+1+1
   mp_ammo_5_56x45_ap        dd rmp_ammo_5_56x45_ap+1+1
   mp_ammo_7_62x54_7h1       dd rmp_ammo_7_62x54_7h1+1+1
   mp_ammo_7_62x54_ap        dd rmp_ammo_7_62x54_ap+1+1
   mp_ammo_9x39_pab9         dd rmp_ammo_9x39_pab9+1+1
   mp_ammo_9x39_ap           dd rmp_ammo_9x39_ap+1+1
   mp_ammo_12x70_buck        dd rmp_ammo_12x70_buck+1+1
   mp_ammo_12x76_zhekan      dd rmp_ammo_12x76_zhekan+1+1
   mp_ammo_12x76_dart        dd rmp_ammo_12x76_dart+1+1
   mp_ammo_gauss             dd rmp_ammo_gauss+1+1





























  ;------------------------------------------------------------
   rmp_wpn_knife                       db 0x0,0x30,'mp_wpn_knife',0
  ;------------------------------------------------------------
   rmp_wpn_pm                          db 0x0,0x35,'mp_wpn_pm',0
   rmp_wpn_pb                          db 0x0,0x34,'mp_wpn_pb',0
   rmp_wpn_fort                        db 0x0,0x2C,'mp_wpn_fort',0
   rmp_wpn_walther                     db 0x0,0x40,'mp_wpn_walther',0
   rmp_wpn_colt1911                    db 0x0,0x38,'mp_wpn_colt1911',0
   rmp_wpn_sig220                      db 0x0,0x29,'mp_wpn_sig220',0
   rmp_wpn_usp                         db 0x0,0x3D,'mp_wpn_usp',0
   rmp_wpn_desert_eagle                db 0x0,0x2A,'mp_wpn_desert_eagle',0
  ;------------------------------------------------------------
   rmp_wpn_bm16                        db 0x0,0x28,'mp_wpn_bm16',0
   rmp_wpn_wincheaster1300             db 0x0,0x41,'mp_wpn_wincheaster1300',0
   rmp_wpn_spas12                      db 0x0,0x3A,'mp_wpn_spas12',0
   ;------------------------------------------------------------
   rmp_wpn_ak74u                       db 0x0,0x26,'mp_wpn_ak74u',0
   rmp_wpn_mp5                         db 0x0,0x33,'mp_wpn_mp5',0
   rmp_wpn_ak74                        db 0x0,0x25,'mp_wpn_ak74',0
   rmp_wpn_l85                         db 0x0,0x31,'mp_wpn_l85',0
   rmp_wpn_abakan                      db 0x0,0x1F,'mp_wpn_abakan',0
   rmp_wpn_lr300                       db 0x0,0x32,'mp_wpn_lr300',0
   rmp_wpn_groza                       db 0x0,0x2F,'mp_wpn_groza',0
   rmp_wpn_sig550                      db 0x0,0x39,'mp_wpn_sig550',0
   rmp_wpn_val                         db 0x0,0x3E,'mp_wpn_val',0
   rmp_wpn_g36                         db 0x0,0x2D,'mp_wpn_g36',0
   rmp_wpn_fn2000                      db 0x0,0x2B,'mp_wpn_fn2000',0
   ;-------------------------------------------
   rmp_wpn_vintorez                    db 0x0,0x3F,'mp_wpn_vintorez',0
   rmp_wpn_svd                         db 0x0,0x3B,'mp_wpn_svd',0
   rmp_wpn_svu                         db 0x0,0x3C,'mp_wpn_svu',0
   rmp_wpn_gauss                       db 0x0,0x2E,'mp_wpn_gauss',0
;------------------------------------------
   rmp_wpn_rpg7                        db 0x0,0x37,'mp_wpn_rpg7',0
   rmp_wpn_rg_6                        db 0x0,0x36,'mp_wpn_rg_6',0
;-------------------------------------------
   rmp_grenade_rgd5                    db 0x0,0x1B,'mp_grenade_rgd5',0
   rmp_grenade_f1                      db 0x0,0x19,'mp_grenade_f1',0
   rmp_grenade_gd_05                   db 0x0,0x1A,'mp_grenade_gd_05',0
   rmp_ammo_vog_25                     db 0x0,0x14,'mp_ammo_vog_25',0
   rmp_ammo_m209                       db 0x0,0x12,'mp_ammo_m209',0
   rmp_ammo_og_7b                      db 0x0,0x13,'mp_ammo_og_7b',0

;-------------------------------------------
   rmp_scientific_outfit               db 0x0,0x1E,'mp_scientific_outfit',0
   rmp_military_stalker_outfit         db 0x0,0x1D,'mp_military_stalker_outfit',0
   rmp_exo_outfit                      db 0x0,0x18,'mp_exo_outfit',0
;-------------------------------------------

   rmp_device_torch                    db 0x0,0x17,'mp_device_torch',0
   rmp_wpn_addon_silencer              db 0x0,0x24,'mp_wpn_addon_silencer',0
   rmp_wpn_addon_scope                 db 0x0,0x22,'mp_wpn_addon_scope',0
   rmp_wpn_addon_scope_susat           db 0x0,0x23,'mp_wpn_addon_scope_susat',0
   rmp_wpn_addon_grenade_launcher      db 0x0,0x20,'mp_wpn_addon_grenade_launcher',0
   rmp_wpn_addon_grenade_launcher_m203 db 0x0,0x21,'mp_wpn_addon_grenade_launcher_m203',0
   rmp_medkit                          db 0x0,0x1C,'mp_medkit',0
   rmp_antirad                         db 0x0,0x15,'mp_antirad',0
   rmp_detector_advances               db 0x0,0x16,'mp_detector_advances',0
   rmp_wpn_binoc                       db 0x0,0x27,'mp_wpn_binoc',0
;-------------------------------------------









;-------------------------------------------
   rmp_ammo_9x18_fmj                   db 0x0,0x0C,'mp_ammo_9x18_fmj',0
   rmp_ammo_9x18_pmm                   db 0x0,0x0B,'mp_ammo_9x18_pmm',0
   rmp_ammo_9x19_fmj                   db 0x0,0x0D,'mp_ammo_9x19_fmj',0
   rmp_ammo_9x19_pbp                   db 0x0,0x0E,'mp_ammo_9x19_pbp',0
   rmp_ammo_11_43x23_fmj               db 0x0,0x11,'mp_ammo_11_43x23_fmj',0
   rmp_ammo_11_43x23_hydro             db 0x0,0x00,'mp_ammo_11_43x23_hydro',0
;-------------------------------------------
   rmp_ammo_5_45x39_fmj                db 0x0,0x01,'mp_ammo_5_45x39_fmj',0
   rmp_ammo_5_45x39_ap                 db 0x0,0x02,'mp_ammo_5_45x39_ap',0
   rmp_ammo_5_56x45_ss190              db 0x0,0x04,'mp_ammo_5_56x45_ss190',0
   rmp_ammo_5_56x45_ap                 db 0x0,0x03,'mp_ammo_5_56x45_ap',0
   rmp_ammo_7_62x54_7h1                db 0x0,0x06,'mp_ammo_7_62x54_7h1',0
   rmp_ammo_7_62x54_ap                 db 0x0,0x05,'mp_ammo_7_62x54_ap',0
   rmp_ammo_9x39_pab9                  db 0x0,0x08,'mp_ammo_9x39_pab9',0
   rmp_ammo_9x39_ap                    db 0x0,0x07,'mp_ammo_9x39_ap',0
   rmp_ammo_12x70_buck                 db 0x0,0x10,'mp_ammo_12x70_buck',0
   rmp_ammo_12x76_zhekan               db 0x0,0x0F,'mp_ammo_12x76_zhekan',0
   rmp_ammo_12x76_dart                 db 0x0,0x09,'mp_ammo_12x76_dart',0
   rmp_ammo_gauss                      db 0x0,0x0A,'mp_ammo_gauss',0
;========================================================================================================= 













pWEAPONS:
;-------------------------------------------
pmp_ammo_11_43x23_hydro             dd rmp_ammo_11_43x23_hydro+1+1
pmp_ammo_5_45x39_fmj                dd rmp_ammo_5_45x39_fmj+1+1
pmp_ammo_5_45x39_ap                 dd rmp_ammo_5_45x39_ap+1+1
pmp_ammo_5_56x45_ap                 dd rmp_ammo_5_56x45_ap+1+1
pmp_ammo_5_56x45_ss190              dd rmp_ammo_5_56x45_ss190+1+1
pmp_ammo_7_62x54_ap                 dd rmp_ammo_7_62x54_ap+1+1
pmp_ammo_7_62x54_7h1                dd rmp_ammo_7_62x54_7h1+1+1
pmp_ammo_9x39_ap                    dd rmp_ammo_9x39_ap+1+1
pmp_ammo_9x39_pab9                  dd rmp_ammo_9x39_pab9+1+1
pmp_ammo_12x76_dart                 dd rmp_ammo_12x76_dart+1+1
pmp_ammo_gauss                      dd rmp_ammo_gauss+1+1
pmp_ammo_9x18_pmm                   dd rmp_ammo_9x18_pmm+1+1
pmp_ammo_9x18_fmj                   dd rmp_ammo_9x18_fmj+1+1
pmp_ammo_9x19_fmj                   dd rmp_ammo_9x19_fmj+1+1
pmp_ammo_9x19_pbp                   dd rmp_ammo_9x19_pbp+1+1
pmp_ammo_12x76_zhekan               dd rmp_ammo_12x76_zhekan+1+1
pmp_ammo_12x70_buck                 dd rmp_ammo_12x70_buck+1+1
pmp_ammo_11_43x23_fmj               dd rmp_ammo_11_43x23_fmj+1+1
;-------------------------------------------
pmp_ammo_m209                       dd rmp_ammo_m209+1+1  
pmp_ammo_og_7b                      dd rmp_ammo_og_7b+1+1
pmp_ammo_vog_25                     dd rmp_ammo_vog_25+1+1
pmp_antirad                         dd rmp_antirad+1+1
pmp_detector_advances               dd rmp_detector_advances+1+1
pmp_device_torch                    dd rmp_device_torch+1+1
pmp_exo_outfit                      dd rmp_exo_outfit+1+1
pmp_grenade_f1                      dd rmp_grenade_f1+1+1
pmp_grenade_gd_05                   dd rmp_grenade_gd_05+1+1
pmp_grenade_rgd5                    dd rmp_grenade_rgd5+1+1
pmp_medkit                          dd rmp_medkit+1+1
pmp_military_stalker_outfit         dd rmp_military_stalker_outfit+1+1
pmp_scientific_outfit               dd rmp_scientific_outfit+1+1
pmp_wpn_abakan                      dd rmp_wpn_abakan+1+1
;-------------------------------------------
pmp_wpn_addon_grenade_launcher      dd rmp_wpn_addon_grenade_launcher+1+1
pmp_wpn_addon_grenade_launcher_m203 dd rmp_wpn_addon_grenade_launcher_m203+1+1
pmp_wpn_addon_scope                 dd rmp_wpn_addon_scope+1+1
pmp_wpn_addon_scope_susat           dd rmp_wpn_addon_scope_susat+1+1
pmp_wpn_addon_silencer              dd rmp_wpn_addon_silencer+1+1
pmp_wpn_ak74                        dd rmp_wpn_ak74+1+1
pmp_wpn_ak74u                       dd rmp_wpn_ak74u+1+1
pmp_wpn_binoc                       dd rmp_wpn_binoc+1+1
pmp_wpn_bm16                        dd rmp_wpn_bm16+1+1
pmp_wpn_sig220                      dd rmp_wpn_sig220+1+1
pmp_wpn_desert_eagle                dd rmp_wpn_desert_eagle+1+1
pmp_wpn_fn2000                      dd rmp_wpn_fn2000+1+1
pmp_wpn_fort                        dd rmp_wpn_fort+1+1
pmp_wpn_g36                         dd rmp_wpn_g36+1+1
pmp_wpn_gauss                       dd rmp_wpn_gauss+1+1
pmp_wpn_groza                       dd rmp_wpn_groza+1+1
;-------------------------------------------
pmp_wpn_knife                       dd rmp_wpn_knife+1+1
pmp_wpn_l85                         dd rmp_wpn_l85+1+1
pmp_wpn_lr300                       dd rmp_wpn_lr300+1+1
pmp_wpn_mp5                         dd rmp_wpn_mp5+1+1
pmp_wpn_pb                          dd rmp_wpn_pb+1+1
pmp_wpn_pm                          dd rmp_wpn_pm+1+1
pmp_wpn_rg_6                        dd rmp_wpn_rg_6+1+1
pmp_wpn_rpg7                        dd rmp_wpn_rpg7+1+1
pmp_wpn_colt1911                    dd rmp_wpn_colt1911+1+1
pmp_wpn_sig550                      dd rmp_wpn_sig550+1+1
pmp_wpn_spas12                      dd rmp_wpn_spas12+1+1
pmp_wpn_svd                         dd rmp_wpn_svd+1+1
pmp_wpn_svu                         dd rmp_wpn_svu+1+1
pmp_wpn_usp                         dd rmp_wpn_usp+1+1
pmp_wpn_val                         dd rmp_wpn_val+1+1
pmp_wpn_vintorez                    dd rmp_wpn_vintorez+1+1
pmp_wpn_wincheaster1300             dd rmp_wpn_wincheaster1300+1+1
pmp_wpn_walther                     dd rmp_wpn_walther+1+1
;-------------------------------------------

































 