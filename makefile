#
#	Open Watcom Wolfenstein 3D makefile
#
#
# Possible optimizations for 8088 class processors
#
# -oa   Relax alias checking
# -ob   Try to generate straight line code
# -oe - expand user functions inline (-oe=20 is default, adds lots of code)
# -oh   Enable repeated optimizations
# -oi   generate certain lib funcs inline
# -oi+  Set max inline depth (C++ only, use -oi for C)
# -ok   Flowing of register save into function flow graph
# -ol   loop optimizations
# -ol+  loop optimizations plus unrolling
# -or   Reorder for pipelined (486+ procs); not sure if good to use
# -os   Favor space over time
# -ot   Favor time over space
# -ei   Allocate an "int" for all enum types
# -zp2  Allow compiler to add padding to structs
# -zpw  Use with above; make sure you are warning free!
# -0    8088/8086 class code generation
# -s    disable stack overflow checking

# -zk0u	translate kanji to unicode... wwww
# -zk0	kanji support~
# -zkl		current codepage

#192x144
#wwww will add these
!ifdef __LINUX__
#!ifdef UNIX
to_os_path=\=/
REMOVECOMMAND=rm -f
COPYCOMMAND=cp -f
DIRSEP=/
OBJ=obj
DUMP=cat
!else		#DOS ^^
to_os_path=/=\
REMOVECOMMAND=del
COPYCOMMAND=copy /y
DIRSEP=\
OBJ=obj
DUMP=type
!endif

TARGET_OS = dos

BUILD_ROOT=$+$(%__CWD__)$-
#DATADIR=data$(DIRSEP)


SRC=src


#
# quiet flags
#
WLIBQ=-q
WCLQ=-q
UPXQ=-qqq

#
# compile flags
#
#-zd{f,p} DS floats vs DS pegged to DGROUP
#-zu SS != DGROUP
#-zp{1,2,4,8,16} struct packing align.
#-ei force enums to be type int
#-wo diagnose problems in overlaid code
S_FLAGS=-s -wo
## -zu -zdp
#-sg -st -of+ -zdf -zff -zgf# -k16768# -zt=84
Z_FLAGS=-zk0 -zc -zm -ei -zp16
O_FLAGS=-opnr -oe=24 -oil+ -outback -ohm -okf+
T_FLAGS=-bt=dos -mm -0 -fpi87 -fo=.$(OBJ) -d1

CPPFLAGS=-DTARGET_MSDOS=16 -DMSDOS=1
AFLAGS=$(WCLQ) $(T_FLAGS)
CFLAGS=$(WCLQ) $(T_FLAGS) -wo $(O_FLAGS) $(S_FLAGS) $(Z_FLAGS)
LFLAGS=$(WCLQ) -l=dos -fm=$^&.map $(S_FLAGS)
LIBFLAGS=$(WLIBQ) -b -n

#
# objects
#
TESTOBJS = id_in.$(OBJ) id_mm.$(OBJ) id_pm.$(OBJ) id_ca.$(OBJ) id_sd.$(OBJ) gamepal.$(OBJ) signon.$(OBJ) id_vl.$(OBJ) id_vh.$(OBJ) wl_scale.$(OBJ) id_us_1.$(OBJ) wl_debug.$(OBJ)
WOLFOBJS = $(TESTOBJS) wl_inter.$(OBJ) wl_agent.$(OBJ) wl_draw.$(OBJ) wl_menu.$(OBJ) wl_state.$(OBJ) wl_text.$(OBJ) wl_game.$(OBJ) wl_act1.$(OBJ) wl_act2.$(OBJ) wl_play.$(OBJ) &
wolfass.$(OBJ)#lib
#id_us_a.$(OBJ) id_sd_a.$(OBJ) id_sd_a.$(OBJ)

#
# libraries
#

#
#	Files locations
#
.c : $(SRC)

#.asm : $(SRC)

#.lib : .;$(DOSLIB_CPU)/dos86h;$(DOSLIB_DOS)/dos86h;$(DOSLIB_VGA)/dos86h;$(DOSLIB_8250)/dos86h

.obj : .

#
#	Default make rules
#
.c.obj:
	*wcl $(CPPFLAGS) $(CFLAGS) $(extra_$^&_obj_opts) -c $[@

.asm.obj:
	*wcl $(AFLAGS) $(extra_$^&_obj_opts) -c $[@

#CFLAGS is neccessary here
.obj.exe :
	*wcl $(LFLAGS) $(extra_$^&_exe_opts)-fe=$@ $<

.obj.lib :
	*wlib $(LIBFLAGS) $(extra_$^&_lib_opts) $@ $<

#
# List of executables to build
#
EXEC = &
#	wolf3dte.exe &
	wolf3d.exe

all: $(EXEC)#sega.exe

#
# game executables
#
wolf3d.exe:	wolf3d.$(OBJ) $(WOLFOBJS) #src/obj/signon.$(OBJ)

#
# Test Executables!
#
wolf3dte.exe:	wolf3dte.$(OBJ) $(WOLFOBJS)# $(TESTOBJS)

#
# executable's objects
#
wolf3d.$(OBJ):	$(SRC)/wolf3d.c
wolf3dte.$(OBJ):	$(SRC)/wolf3dte.c
sega.$(OBJ):	sega.c

#
# non executable objects libraries
#

wolfass.lib: .symbolic
	@if exist src/WOLFASS.LIB	cp src/WOLFASS.LIB ./wolfass.lib

#gfx.lib: $(GFXLIBOBJS)
#	*wlib $(LIBFLAGS) $(extra_$^&_lib_opts) $@ $<

id_pm.$(OBJ):	$(SRC)/id_pm.c
id_mm.$(OBJ):	$(SRC)/id_mm.c
id_ca.$(OBJ):	$(SRC)/id_ca.c
id_vl.$(OBJ):	$(SRC)/id_vl.c
wl_inter.$(OBJ):	$(SRC)/wl_inter.c
wl_menu.$(OBJ):	$(SRC)/wl_menu.c
signon.$(OBJ):	$(SRC)/signon.c
gamepal.$(OBJ):	$(SRC)/gamepal.c
id_in.$(OBJ):	$(SRC)/id_in.c
id_sd.$(OBJ):	$(SRC)/id_sd.c
id_sd_a.$(OBJ):	$(SRC)/id_sd_a.c
id_us_1.$(OBJ):	$(SRC)/id_us_1.c
id_us_a.$(OBJ):	$(SRC)/id_us_a.c
id_vh.$(OBJ):	$(SRC)/id_vh.c
wl_act1.$(OBJ):	$(SRC)/wl_act1.c
wl_act2.$(OBJ):	$(SRC)/wl_act2.c
wl_agent.$(OBJ):	$(SRC)/wl_agent.c
wl_debug.$(OBJ):	$(SRC)/wl_debug.c
wl_draw.$(OBJ):	$(SRC)/wl_draw.c
wl_game.$(OBJ):	$(SRC)/wl_game.c
wl_play.$(OBJ):	$(SRC)/wl_play.c
wl_state.$(OBJ):	$(SRC)/wl_state.c
wl_text.$(OBJ):	$(SRC)/wl_text.c
wl_scale.$(OBJ):	$(SRC)/wl_scale.c
wolfass..$(OBJ):	$(SRC)/wolfass..c
#id_exter.$(OBJ):	$(SRC)/id_exter.c
#id_vl_a.$(OBJ):	$(SRC)/id_vl_a.asm

#
#other~
#
clean: .symbolic
	@if not exist wolf3d/WOLFSRC.1 wmake -h initlibs
	@for %f in ($(EXEC)) do @if exist %f $(REMOVECOMMAND) %f
!ifdef __LINUX__
	@rm *.LIB
	@rm *.EXE
	@if exist src/obj/*.EXE	mv src/obj/*.EXE bcwolf.exe
	#@wmake -h bomb
!endif
	@if exist *.obj $(REMOVECOMMAND) *.obj
	@if exist *.OBJ $(REMOVECOMMAND) *.OBJ
	@if exist *.LIB $(REMOVECOMMAND) *.LIB
	@if exist *.lnk $(REMOVECOMMAND) *.lnk
	@if exist *.LNK $(REMOVECOMMAND) *.LNK
	@if exist *.smp $(REMOVECOMMAND) *.smp
	@if exist *.SMP $(REMOVECOMMAND) *.SMP
	@if exist *.hed $(REMOVECOMMAND) *.hed
	@if exist *.mah $(REMOVECOMMAND) *.mah
	@if exist *.MAH $(REMOVECOMMAND) *.MAH
	@if exist *.err $(REMOVECOMMAND) *.err

bomb: .symbolic
	#@wmake -h clean
	@if exist src/obj/*.*	$(REMOVECOMMAND) src/obj/*.*
!ifdef __LINUX__
	#@if exist src/obj/*.OBJ	$(REMOVECOMMAND) src/obj/*.OBJ
	@if exist src/TC*.SWP	$(REMOVECOMMAND) src/TC*.SWP
!endif

backupconfig: .symbolic
	@$(COPYCOMMAND) .git$(DIRSEP)config git_con.fig
	@$(COPYCOMMAND) .gitmodules git_modu.les
	@$(COPYCOMMAND) .gitignore git_igno.re

comp: .symbolic
	@*upx -9 $(EXEC)

comq: .symbolic
	@*upx -9 $(UPXQ) $(EXEC)

vomitchan: .symbolic
	@if exist *.err $(DUMP) *.err

##
##	External library management~ ^^
##
#git submodule add <repo>
reinitlibs: .symbolic
	@$(REMOVECOMMAND) wolf3d
	@wmake -h initlibs

initlibs: .symbolic
	@cp git_con.fig .git/config
	@cp git_modu.les .gitmodules
	@cp git_igno.re .gitignore
	@git clone https://github.com/id-Software/wolf3d.git
