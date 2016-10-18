CXX     = wcc
LD      = wlink

INCLUDE = $(%WATCOM)\h
SODDEF  = -DSPEAR -DSODPAL

all: wolf3d.exe sod.exe .symbolic

wolf3d.EXE:
  $(CXX) -i=$(INCLUDE) -bt=dos -0 -fpi87 -ohx -j -zp1 -zq -w0 src/ub.c
  $(LD) name WOLF4GW sys op el FILE ub.obj

sod:
  $(CXX) -i=$(INCLUDE) $(SODDEF) -bt=dos -0-fpi87 -ohx -j -zp1 -zq -w0 src/ub.c
  $(LD) name SOD4GW sys op el FILE ub.obj

clean:    .SYMBOLIC
CLEANEXT = sys exe obj err lnk sym lst map lib
  @for %a in ($(CLEANEXT)) do -@rm *.%a
