WOLFOBJS = gamepal3.o id_ca.o id_in.o id_pm.o id_sd.o id_us_1.o id_vh.o id_vl.o signon.o wl_act1.o wl_act2.o wl_agent.o wl_debug.o wl_draw.o wl_game.o wl_inter.o wolfdx.o wl_menu.o wl_play.o wl_state.o wl_text.o
#id_us_a.o
#
#	Files locations
#
.c : .

.asm : .

#.lib : .;$(DOSLIB_CPU)/dos86h;$(DOSLIB_DOS)/dos86h;$(DOSLIB_VGA)/dos86h;$(DOSLIB_8250)/dos86h

.obj : .

#
#	Default make rules
#
.c.obj:
	*wcc  -w4 -e25 -j -zp1 -zq -od -d2 -0 -bt=dos -mh $(extra_$^&_obj_opts) -c $[@

.asm.obj:
	*wasm -zcm=tasm -mh -0 -w4 -e25 -zq $(extra_$^&_obj_opts) -c $[@

id_us_a.o: id_us_a.asm# .AUTODEPEND

gamepal3.o: gamepal3.c# .AUTODEPEND
id_ca.o: id_ca.c# .AUTODEPEND
id_in.o: id_in.c# .AUTODEPEND
id_pm.o: id_pm.c# .AUTODEPEND
id_sd.o: id_sd.c# .AUTODEPEND
id_us_1.o: id_us_1.c# .AUTODEPEND
id_vh.o: id_vh.c# .AUTODEPEND
id_vl.o: id_vl.c# .AUTODEPEND
signon.o: signon.c# .AUTODEPEND
wl_act1.o: wl_act1.c# .AUTODEPEND
wl_act2.o: wl_act2.c# .AUTODEPEND
wl_agent.o: wl_agent.c# .AUTODEPEND
wl_debug.o: wl_debug.c# .AUTODEPEND
wl_draw.o: wl_draw.c# .AUTODEPEND
wl_game.o: wl_game.c# .AUTODEPEND
wl_inter.o: wl_inter.c# .AUTODEPEND
wolfdx.o: wolfdx.c# .AUTODEPEND
wl_menu.o: wl_menu.c# .AUTODEPEND
wl_play.o: wl_play.c# .AUTODEPEND
wl_state.o: wl_state.c# .AUTODEPEND
wl_text.o: wl_text.c# .AUTODEPEND

all: wolfdx.exe
wolfdx.exe: $(WOLFOBJS)
	@%write wolfdx.lk1 FIL id_us_a.o,gamepal3d.o,id_ca.o,id_in.o,id_pm.o,id_sd.o,id_us_1.o,id_vh.o,id_vl.o,signon.o,wl_act1.o,wl_act2.o,wl_agent.o,wl_debug.o,wl_draw.o,wl_game.o,wl_inter.o,wl_main.o,wl_menu.o,wl_play.o,wl_state.o,wl_text.o
	@%append wolf.lk1
	*wlink name wolfdx d all SYS dos4g op inc op m op maxe=25 op q op symf
	@wolfdx.lk1

