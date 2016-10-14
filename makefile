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
#EXMMTESTDIR=16$(DIRSEP)exmmtest$(DIRSEP)
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
S_FLAGS=-sg -st -of+ -zu -zdf -zff -zgf -k32768#54096#60000
Z_FLAGS=-zk0 -zc -zp8 -zm
O_FLAGS=-obmilr -oe=24 -out -oh -ei -onac -ol+ -ok##x
T_FLAGS=-bt=dos -mh -0 -fpi87 -fo=.$(OBJ) -d1 -e=65536

CPPFLAGS=-DTARGET_MSDOS=16 -DMSDOS=1
AFLAGS=$(WCLQ) $(T_FLAGS)
CFLAGS=$(WCLQ) $(T_FLAGS) -wo $(O_FLAGS) $(S_FLAGS) $(Z_FLAGS)
LFLAGS=$(WCLQ) -l=dos -fm=$^&.mah $(S_FLAGS)
LIBFLAGS=$(WLIBQ) -b -n

#
# objects
#

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
	*wcl $(CPPFLAGS) $(CFLAGS) $(extra_$^&_obj_opts) -c $[@

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
    wolf3d.exe


all: $(EXEC)

#
# game and bakapi executables
#
wolf3d.exe:           wl_main.$(OBJ)

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


modex16.$(OBJ):   $(SRCLIB)/modex16.c $(SRCLIB)/modex16.h


#
#other~
#
clean: .symbolic
	@for %f in ($(EXEC)) do @if exist %f $(REMOVECOMMAND) %f
!ifdef __LINUX__
	@rm *.LIB
	@rm *.EXE
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
