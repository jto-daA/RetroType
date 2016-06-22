# Microsoft Developer Studio Generated NMAKE File, Based on PrimaryCubes3D.dsp
!IF "$(CFG)" == ""
CFG=PrimaryCubes3D - Win32 Debug
!MESSAGE No configuration specified. Defaulting to PrimaryCubes3D - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "PrimaryCubes3D - Win32 Release" && "$(CFG)" != "PrimaryCubes3D - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PrimaryCubes3D.mak" CFG="PrimaryCubes3D - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PrimaryCubes3D - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "PrimaryCubes3D - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "PrimaryCubes3D - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\PrimaryCubes3D.exe"


CLEAN :
	-@erase "$(INTDIR)\company_intro.obj"
	-@erase "$(INTDIR)\dev_intro.obj"
	-@erase "$(INTDIR)\font.obj"
	-@erase "$(INTDIR)\game.obj"
	-@erase "$(INTDIR)\gfx_elements.obj"
	-@erase "$(INTDIR)\gfx_font.obj"
	-@erase "$(INTDIR)\gfx_sprite.obj"
	-@erase "$(INTDIR)\gfx_system.obj"
	-@erase "$(INTDIR)\logic.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\PrimaryCubes3D.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\PrimaryCubes3D.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\PrimaryCubes3D.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\PrimaryCubes3D.pdb" /machine:I386 /out:"$(OUTDIR)\PrimaryCubes3D.exe" 
LINK32_OBJS= \
	"$(INTDIR)\game.obj" \
	"$(INTDIR)\logic.obj" \
	"$(INTDIR)\gfx_font.obj" \
	"$(INTDIR)\gfx_sprite.obj" \
	"$(INTDIR)\gfx_system.obj" \
	"$(INTDIR)\gfx_elements.obj" \
	"$(INTDIR)\company_intro.obj" \
	"$(INTDIR)\dev_intro.obj" \
	"$(INTDIR)\font.obj" \
	"$(INTDIR)\main.obj"

"$(OUTDIR)\PrimaryCubes3D.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "PrimaryCubes3D - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\PrimaryCubes3D.exe" "$(OUTDIR)\PrimaryCubes3D.bsc"


CLEAN :
	-@erase "$(INTDIR)\company_intro.obj"
	-@erase "$(INTDIR)\company_intro.sbr"
	-@erase "$(INTDIR)\dev_intro.obj"
	-@erase "$(INTDIR)\dev_intro.sbr"
	-@erase "$(INTDIR)\font.obj"
	-@erase "$(INTDIR)\font.sbr"
	-@erase "$(INTDIR)\game.obj"
	-@erase "$(INTDIR)\game.sbr"
	-@erase "$(INTDIR)\gfx_elements.obj"
	-@erase "$(INTDIR)\gfx_elements.sbr"
	-@erase "$(INTDIR)\gfx_font.obj"
	-@erase "$(INTDIR)\gfx_font.sbr"
	-@erase "$(INTDIR)\gfx_sprite.obj"
	-@erase "$(INTDIR)\gfx_sprite.sbr"
	-@erase "$(INTDIR)\gfx_system.obj"
	-@erase "$(INTDIR)\gfx_system.sbr"
	-@erase "$(INTDIR)\logic.obj"
	-@erase "$(INTDIR)\logic.sbr"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\main.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\PrimaryCubes3D.bsc"
	-@erase "$(OUTDIR)\PrimaryCubes3D.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\PrimaryCubes3D.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\PrimaryCubes3D.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\game.sbr" \
	"$(INTDIR)\logic.sbr" \
	"$(INTDIR)\gfx_font.sbr" \
	"$(INTDIR)\gfx_sprite.sbr" \
	"$(INTDIR)\gfx_system.sbr" \
	"$(INTDIR)\gfx_elements.sbr" \
	"$(INTDIR)\company_intro.sbr" \
	"$(INTDIR)\dev_intro.sbr" \
	"$(INTDIR)\font.sbr" \
	"$(INTDIR)\main.sbr"

"$(OUTDIR)\PrimaryCubes3D.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib alld.lib alleg.lib allp.lib /nologo /subsystem:windows /profile /debug /machine:I386 /out:"$(OUTDIR)\PrimaryCubes3D.exe" 
LINK32_OBJS= \
	"$(INTDIR)\game.obj" \
	"$(INTDIR)\logic.obj" \
	"$(INTDIR)\gfx_font.obj" \
	"$(INTDIR)\gfx_sprite.obj" \
	"$(INTDIR)\gfx_system.obj" \
	"$(INTDIR)\gfx_elements.obj" \
	"$(INTDIR)\company_intro.obj" \
	"$(INTDIR)\dev_intro.obj" \
	"$(INTDIR)\font.obj" \
	"$(INTDIR)\main.obj"

"$(OUTDIR)\PrimaryCubes3D.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("PrimaryCubes3D.dep")
!INCLUDE "PrimaryCubes3D.dep"
!ELSE 
!MESSAGE Warning: cannot find "PrimaryCubes3D.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "PrimaryCubes3D - Win32 Release" || "$(CFG)" == "PrimaryCubes3D - Win32 Debug"
SOURCE=.\game.cpp

!IF  "$(CFG)" == "PrimaryCubes3D - Win32 Release"


"$(INTDIR)\game.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PrimaryCubes3D - Win32 Debug"


"$(INTDIR)\game.obj"	"$(INTDIR)\game.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\logic.cpp

!IF  "$(CFG)" == "PrimaryCubes3D - Win32 Release"


"$(INTDIR)\logic.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PrimaryCubes3D - Win32 Debug"


"$(INTDIR)\logic.obj"	"$(INTDIR)\logic.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\gfx_font.cpp

!IF  "$(CFG)" == "PrimaryCubes3D - Win32 Release"


"$(INTDIR)\gfx_font.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PrimaryCubes3D - Win32 Debug"


"$(INTDIR)\gfx_font.obj"	"$(INTDIR)\gfx_font.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\gfx_sprite.cpp

!IF  "$(CFG)" == "PrimaryCubes3D - Win32 Release"


"$(INTDIR)\gfx_sprite.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PrimaryCubes3D - Win32 Debug"


"$(INTDIR)\gfx_sprite.obj"	"$(INTDIR)\gfx_sprite.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\gfx_system.cpp

!IF  "$(CFG)" == "PrimaryCubes3D - Win32 Release"


"$(INTDIR)\gfx_system.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PrimaryCubes3D - Win32 Debug"


"$(INTDIR)\gfx_system.obj"	"$(INTDIR)\gfx_system.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\gfx_elements.cpp

!IF  "$(CFG)" == "PrimaryCubes3D - Win32 Release"


"$(INTDIR)\gfx_elements.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PrimaryCubes3D - Win32 Debug"


"$(INTDIR)\gfx_elements.obj"	"$(INTDIR)\gfx_elements.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\company_intro.cpp

!IF  "$(CFG)" == "PrimaryCubes3D - Win32 Release"


"$(INTDIR)\company_intro.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PrimaryCubes3D - Win32 Debug"


"$(INTDIR)\company_intro.obj"	"$(INTDIR)\company_intro.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\dev_intro.cpp

!IF  "$(CFG)" == "PrimaryCubes3D - Win32 Release"


"$(INTDIR)\dev_intro.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PrimaryCubes3D - Win32 Debug"


"$(INTDIR)\dev_intro.obj"	"$(INTDIR)\dev_intro.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\font.cpp

!IF  "$(CFG)" == "PrimaryCubes3D - Win32 Release"


"$(INTDIR)\font.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PrimaryCubes3D - Win32 Debug"


"$(INTDIR)\font.obj"	"$(INTDIR)\font.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\main.cpp

!IF  "$(CFG)" == "PrimaryCubes3D - Win32 Release"


"$(INTDIR)\main.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PrimaryCubes3D - Win32 Debug"


"$(INTDIR)\main.obj"	"$(INTDIR)\main.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

