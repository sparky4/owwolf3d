LIBOBJS = obj\ID_VL_A.OBJ obj\ID_VH_A.OBJ obj\ID_SD_A.OBJ obj\ID_US_A.OBJ obj\WL_DR_A.OBJ obj\WHACK_A.OBJ obj\H_LDIV.OBJ obj\WL_ASM.OBJ
#obj\C0.OBJ

#
# ASM compiler
#
ASMC = tasm
ASMO = /MX /Z1 /O

#
# LIB maker, uses response file
#
LIBC = tlib

.asm.obj:
	$(ASMC) $(ASMO) $<

all: wolfass.lib

wolfass.lib: wolfass.lbr $(LIBOBJS)
	$(LIBC) wolfass.lib @wolfass.lbr
#@modex.lbr

#$(LIBOBJS): $(LIBOBJS)
