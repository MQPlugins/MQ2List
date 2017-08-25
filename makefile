!include "../global.mak"

ALL : "$(OUTDIR)\MQ2List.dll"

CLEAN :
	-@erase "$(INTDIR)\MQ2List.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\MQ2List.dll"
	-@erase "$(OUTDIR)\MQ2List.exp"
	-@erase "$(OUTDIR)\MQ2List.lib"
	-@erase "$(OUTDIR)\MQ2List.pdb"


LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib $(DETLIB) ..\Release\MQ2Main.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\MQ2List.pdb" /debug /machine:I386 /out:"$(OUTDIR)\MQ2List.dll" /implib:"$(OUTDIR)\MQ2List.lib" /OPT:NOICF /OPT:NOREF 
LINK32_OBJS= \
	"$(INTDIR)\MQ2List.obj" \
	"$(OUTDIR)\MQ2Main.lib"

"$(OUTDIR)\MQ2List.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) $(LINK32_FLAGS) $(LINK32_OBJS)


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("MQ2List.dep")
!INCLUDE "MQ2List.dep"
!ELSE 
!MESSAGE Warning: cannot find "MQ2List.dep"
!ENDIF 
!ENDIF 


SOURCE=.\MQ2List.cpp

"$(INTDIR)\MQ2List.obj" : $(SOURCE) "$(INTDIR)"

