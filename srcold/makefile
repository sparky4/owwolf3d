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
DATADIR=data$(DIRSEP)

SRC=wolfsrc


#
# quiet flags
#
WLIBQ=-q
WCLQ=-q
UPXQ=-qqq

#
# compile flags
#
#-bt=dos -0-fpi87 -ohx -j -zp1 -zq -w0
S_FLAGS=-sg -st -of+ -zu -zdf -zff -zgf -k32768#54096#60000
Z_FLAGS=-zk0 -zc -zp8 -zm -zp1
O_FLAGS=-obmilr -oe=24 -out -oh -ei -onac -ol+ -ok##x
T_FLAGS=-bt=dos -mh -0 -fpi87 -fo=.$(OBJ) -d1 -e=64

CPPFLAGS=-DTARGET_MSDOS=16 -DMSDOS=1
AFLAGS=$(WCLQ) $(T_FLAGS)
CFLAGS=$(WCLQ) $(T_FLAGS) -wo $(O_FLAGS) $(S_FLAGS) $(Z_FLAGS)
LFLAGS=$(WCLQ) -l=dos -fm=$^&.mah $(S_FLAGS)
LIBFLAGS=$(WLIBQ) -b -n

INCLUDE = $(%WATCOM)\h
SODDEF  = -DSPEAR -DSODPAL

#
# objects
#
#WOLFOBJS = id_mm.$(OBJ) id_pm.$(OBJ) id_ca.$(OBJ) id_vl.$(OBJ) wl_inter.$(OBJ)
#id_vl_a.$(OBJ)

#
# libraries
#

#
#	Files locations
#
.c : $(SRC)

.asm : $(SRC)

#.lib : .;$(DOSLIB_CPU)/dos86h;$(DOSLIB_DOS)/dos86h;$(DOSLIB_VGA)/dos86h;$(DOSLIB_8250)/dos86h

.obj : .

#
#	Default make rules
#
.c.obj:
	*wcl $(CPPFLAGS) $(CFLAGS) -i=$(INCLUDE) $(extra_$^&_obj_opts) -c $[@

.asm.obj:
	*wcl $(AFLAGS) $(extra_$^&_obj_opts) -c $[@

#CFLAGS is neccessary here
.obj.exe :
	*wcl $(LFLAGS) $(extra_$^&_exe_opts) -fe=$@ $<

.obj.lib :
	*wlib $(LIBFLAGS) $(extra_$^&_lib_opts) $@ $<

#
# List of executables to build
#
EXEC = &
    wolf3d.exe &
    sod.exe

all: $(EXEC) .symbolic

#
# game and bakapi executables
#
#wolf3d.exe:	wl_main.$(OBJ) $(WOLFOBJS)
wold3d.exe:	$(INCLUDE) ub.c
sod.exe:	$(INCLUDE) $(SODDEF) ub.c
#  $(LD) name SOD4GW sys op el FILE ub.obj

#
# Test Executables!
#


#
# executable's objects
#
wl_main.$(OBJ):	$(SRC)/wl_main.c

#
# non executable objects libraries
#

#gfx.lib: $(GFXLIBOBJS)
#	*wlib $(LIBFLAGS) $(extra_$^&_lib_opts) $@ $<

id_pm.$(OBJ):	$(SRC)/id_pm.c
id_mm.$(OBJ):	$(SRC)/id_mm.c
id_ca.$(OBJ):	$(SRC)/id_ca.c
id_vl.$(OBJ):	$(SRC)/id_vl.c
wl_inter.$(OBJ):	$(SRC)/wl_inter.c
wl_menu.$(OBJ):	$(SRC)/wl_menu.c
#id_vl_a.$(OBJ):	$(SRC)/id_vl_a.asm

#
#other~
#
clean: .symbolic
CLEANEXT = sys exe obj err lnk sym lst map lib
  @for %a in ($(CLEANEXT)) do -@rm *.%a

backupconfig: .symbolic
	@$(COPYCOMMAND) .git$(DIRSEP)config git_con.fig
	@$(COPYCOMMAND) .gitmodules git_modu.les
	@$(COPYCOMMAND) .gitignore git_igno.re

comp: .symbolic
	@*upx -9 $(EXEC)

comq: .symbolic
	@*upx -9 $(UPXQ) $(EXEC)

vomitchan: .symbolic
	@$(DUMP) *.err

##
##	External library management~ ^^
##
#git submodule add <repo>
initlibs: .symbolic
	@cp git_con.fig .git/config
	@cp git_modu.les .gitmodules
	@cp git_igno.re .gitignore
