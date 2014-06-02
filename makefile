CC =        icc
LD =        link386
OBJ1=       xscheme.obj xsdmem.obj xsimage.obj xsio.obj xsobj.obj \
            xsprint.obj xsread.obj xssym.obj xsfun1.obj xsfun2.obj \
            xsmath.obj os.obj
OBJ2=       xsinit.obj xscom.obj xsint.obj
OBJ3=       xsftab.obj

CFLAGS=     /C /W3 /Gs /O+ /G4
LDFLAGS=    /PACKCODE /PACKDATA

.c.obj:
    $(CC) $(CFLAGS) $<

xscheme.exe:$(OBJ1) $(OBJ2) xsftab.obj
    $(LD) $(LDFLAGS) @xscheme.lnk

$(OBJ1):    xscheme.h
$(OBJ2):    xscheme.h xsbcode.h
$(OBJ3):    xscheme.h osdefs.h osptrs.h
