.AUTODEPEND

.PATH.obj = OBJ

#		*Translator Definitions*
CC = bcc +WOLF3D.CFG
TASM = TASM
TLIB = tlib
TLINK = tlink
LIBPATH = C:\Z\BORLANDC\LIB
INCLUDEPATH = C:\Z\BORLANDC\INCLUDE


#		*Implicit Rules*
.c.obj:
  $(CC) -c {$< }

.cpp.obj:
  $(CC) -c {$< }

#		*List Macros*


EXE_dependencies =  \
 obj\c0.obj \
 h_ldiv.obj \
 wolfhack.obj \
 wolf3d.obj \
 signon.obj \
 gamepal.obj \
 whack_a.obj \
 wl_asm.obj \
 wl_text.obj \
 wl_menu.obj \
 wl_inter.obj \
 wl_game.obj \
 wl_play.obj \
 wl_debug.obj \
 wl_draw.obj \
 wl_dr_a.obj \
 wl_scale.obj \
 wl_state.obj \
 wl_agent.obj \
 wl_act1.obj \
 wl_act2.obj \
 id_ca.obj \
 id_in.obj \
 id_mm.obj \
 id_pm.obj \
 id_sd.obj \
 id_sd_a.obj \
 id_us_a.obj \
 id_us_1.obj \
 id_vl.obj \
 id_vh.obj \
 id_vl_a.obj \
 id_vh_a.obj

#		*Explicit Rules*
obj\wolf3d.exe: wolf3d.cfg $(EXE_dependencies)
  $(TLINK) /v/s/c/P-/L$(LIBPATH) @&&|
c0m.obj+
obj\c0.obj+
obj\h_ldiv.obj+
obj\wolfhack.obj+
obj\wolf3d.obj+
obj\signon.obj+
obj\gamepal.obj+
obj\whack_a.obj+
obj\wl_asm.obj+
obj\wl_text.obj+
obj\wl_menu.obj+
obj\wl_inter.obj+
obj\wl_game.obj+
obj\wl_play.obj+
obj\wl_debug.obj+
obj\wl_draw.obj+
obj\wl_dr_a.obj+
obj\wl_scale.obj+
obj\wl_state.obj+
obj\wl_agent.obj+
obj\wl_act1.obj+
obj\wl_act2.obj+
obj\id_ca.obj+
obj\id_in.obj+
obj\id_mm.obj+
obj\id_pm.obj+
obj\id_sd.obj+
obj\id_sd_a.obj+
obj\id_us_a.obj+
obj\id_us_1.obj+
obj\id_vl.obj+
obj\id_vh.obj+
obj\id_vl_a.obj+
obj\id_vh_a.obj
obj\wolf3d,obj\wolf3d
emu.lib+
mathm.lib+
cm.lib
|


#		*Individual File Dependencies*
obj\c0.obj: wolf3d.cfg c0.asm 
	$(TASM) /MX /ZI /O /D__MEDIUM__ C0.ASM,OBJ\C0.OBJ

h_ldiv.obj: wolf3d.cfg h_ldiv.asm 
	$(TASM) /MX /ZI /O H_LDIV.ASM,OBJ\H_LDIV.OBJ /d__MEDIUM__

wolfhack.obj: wolf3d.cfg wolfhack.c 

wolf3d.obj: wolf3d.cfg wolf3d.c 

signon.obj: wolf3d.cfg signon.c 

gamepal.obj: wolf3d.cfg gamepal.c 

whack_a.obj: wolf3d.cfg whack_a.asm 
	$(TASM) /MX /ZI /O WHACK_A.ASM,OBJ\WHACK_A.OBJ

wl_asm.obj: wolf3d.cfg wl_asm.asm 
	$(TASM) /MX /ZI /O WL_ASM.ASM,OBJ\WL_ASM.OBJ

wl_text.obj: wolf3d.cfg wl_text.c 

wl_menu.obj: wolf3d.cfg wl_menu.c 

wl_inter.obj: wolf3d.cfg wl_inter.c 

wl_game.obj: wolf3d.cfg wl_game.c 

wl_play.obj: wolf3d.cfg wl_play.c 

wl_debug.obj: wolf3d.cfg wl_debug.c 

wl_draw.obj: wolf3d.cfg wl_draw.c 

wl_dr_a.obj: wolf3d.cfg wl_dr_a.asm 
	$(TASM) /MX /ZI /O WL_DR_A.ASM,OBJ\WL_DR_A.OBJ

wl_scale.obj: wolf3d.cfg wl_scale.c 

wl_state.obj: wolf3d.cfg wl_state.c 

wl_agent.obj: wolf3d.cfg wl_agent.c 

wl_act1.obj: wolf3d.cfg wl_act1.c 

wl_act2.obj: wolf3d.cfg wl_act2.c 

id_ca.obj: wolf3d.cfg id_ca.c 

id_in.obj: wolf3d.cfg id_in.c 

id_mm.obj: wolf3d.cfg id_mm.c 

id_pm.obj: wolf3d.cfg id_pm.c 

id_sd.obj: wolf3d.cfg id_sd.c 

id_sd_a.obj: wolf3d.cfg id_sd_a.asm 
	$(TASM) /MX /ZI /O ID_SD_A.ASM,OBJ\ID_SD_A.OBJ

id_us_a.obj: wolf3d.cfg id_us_a.asm 
	$(TASM) /MX /ZI /O ID_US_A.ASM,OBJ\ID_US_A.OBJ

id_us_1.obj: wolf3d.cfg id_us_1.c 

id_vl.obj: wolf3d.cfg id_vl.c 

id_vh.obj: wolf3d.cfg id_vh.c 

id_vl_a.obj: wolf3d.cfg id_vl_a.asm 
	$(TASM) /MX /ZI /O ID_VL_A.ASM,OBJ\ID_VL_A.OBJ

id_vh_a.obj: wolf3d.cfg id_vh_a.asm 
	$(TASM) /MX /ZI /O ID_VH_A.ASM,OBJ\ID_VH_A.OBJ

#		*Compiler Configuration File*
wolf3d.cfg: wolf3d.mak
  copy &&|
-mm
-a
-ff-
-v
-G
-O
-Og
-Oe
-Om
-Ov
-Ol
-Ob
-Op
-Oi
-Z
-d
-vi-
-H=WOLF3D.SYM
-Fc
-weas
-wpre
-nOBJ
-I$(INCLUDEPATH)
-L$(LIBPATH)
-P-.C
| wolf3d.cfg


