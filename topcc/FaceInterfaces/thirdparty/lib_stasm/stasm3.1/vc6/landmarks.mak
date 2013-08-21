# Microsoft Developer Studio Generated NMAKE File, Based on landmarks.dsp
!IF "$(CFG)" == ""
CFG=landmarks - Win32 Debug
!MESSAGE No configuration specified. Defaulting to landmarks - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "landmarks - Win32 Release" && "$(CFG)" != "landmarks - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "landmarks.mak" CFG="landmarks - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "landmarks - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "landmarks - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "landmarks - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\landmarks.dll"


CLEAN :
	-@erase "$(INTDIR)\asmsearch.obj"
	-@erase "$(INTDIR)\atface.obj"
	-@erase "$(INTDIR)\err.obj"
	-@erase "$(INTDIR)\ezfont.obj"
	-@erase "$(INTDIR)\find.obj"
	-@erase "$(INTDIR)\follow.obj"
	-@erase "$(INTDIR)\forward.obj"
	-@erase "$(INTDIR)\imfile.obj"
	-@erase "$(INTDIR)\imgiven.obj"
	-@erase "$(INTDIR)\imshape.obj"
	-@erase "$(INTDIR)\imutil.obj"
	-@erase "$(INTDIR)\imwrite.obj"
	-@erase "$(INTDIR)\initasm.obj"
	-@erase "$(INTDIR)\initnet.obj"
	-@erase "$(INTDIR)\jpegutil.obj"
	-@erase "$(INTDIR)\landmarks.obj"
	-@erase "$(INTDIR)\mat.obj"
	-@erase "$(INTDIR)\matvec.obj"
	-@erase "$(INTDIR)\mchol.obj"
	-@erase "$(INTDIR)\mrand.obj"
	-@erase "$(INTDIR)\pngutil.obj"
	-@erase "$(INTDIR)\prof.obj"
	-@erase "$(INTDIR)\readasm.obj"
	-@erase "$(INTDIR)\readconf.obj"
	-@erase "$(INTDIR)\readpng.obj"
	-@erase "$(INTDIR)\regex.obj"
	-@erase "$(INTDIR)\release.obj"
	-@erase "$(INTDIR)\rgbimutil.obj"
	-@erase "$(INTDIR)\rowley.obj"
	-@erase "$(INTDIR)\rowleyhand.obj"
	-@erase "$(INTDIR)\safe_alloc.obj"
	-@erase "$(INTDIR)\search.obj"
	-@erase "$(INTDIR)\shapefile.obj"
	-@erase "$(INTDIR)\shapemodel.obj"
	-@erase "$(INTDIR)\sparsemat.obj"
	-@erase "$(INTDIR)\startshape.obj"
	-@erase "$(INTDIR)\stasm_dll.obj"
	-@erase "$(INTDIR)\tclHash.obj"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\violajones.obj"
	-@erase "$(INTDIR)\vjhand.obj"
	-@erase "$(INTDIR)\wrbmp.obj"
	-@erase "$(OUTDIR)\landmarks.dll"
	-@erase "$(OUTDIR)\landmarks.exp"
	-@erase "$(OUTDIR)\landmarks.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /I "." /I "C:\OpenCV2.1\include\opencv" /I "..\regex" /I "..\gsl" /I "..\gsl\gsl" /I "..\image" /I "..\jpeg" /I "..\mat" /I "..\rowley" /I "..\stasm" /I "..\tasm" /I "..\png" /I "..\png\pnglib" /I "..\png\zlib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LANDMARKS_EXPORTS" /D "HAVE_CONFIG_H" /D "STDC_HEADERS" /D "REGEX_MALLOC" /Fp"$(INTDIR)\landmarks.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\landmarks.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\gsl\lib_ms\mini_gslML.lib ..\gsl\lib_ms\gslcblasML.lib ..\jpeg\lib_ms\libjpeg.lib ..\png\pnglib\libpng.lib ..\png\zlib\zlib.lib C:\OpenCV2.1\lib\cv210.lib C:\OpenCV2.1\lib\cxcore210.lib C:\OpenCV2.1\lib\highgui210.lib setargv.obj /nologo /dll /incremental:no /pdb:"$(OUTDIR)\landmarks.pdb" /machine:I386 /nodefaultlib:"LIBCD" /nodefaultlib:"LIBC" /out:"$(OUTDIR)\landmarks.dll" /implib:"$(OUTDIR)\landmarks.lib" 
LINK32_OBJS= \
	"$(INTDIR)\asmsearch.obj" \
	"$(INTDIR)\atface.obj" \
	"$(INTDIR)\err.obj" \
	"$(INTDIR)\ezfont.obj" \
	"$(INTDIR)\find.obj" \
	"$(INTDIR)\follow.obj" \
	"$(INTDIR)\forward.obj" \
	"$(INTDIR)\imfile.obj" \
	"$(INTDIR)\imgiven.obj" \
	"$(INTDIR)\imshape.obj" \
	"$(INTDIR)\imutil.obj" \
	"$(INTDIR)\imwrite.obj" \
	"$(INTDIR)\initasm.obj" \
	"$(INTDIR)\initnet.obj" \
	"$(INTDIR)\jpegutil.obj" \
	"$(INTDIR)\landmarks.obj" \
	"$(INTDIR)\mat.obj" \
	"$(INTDIR)\matvec.obj" \
	"$(INTDIR)\mchol.obj" \
	"$(INTDIR)\mrand.obj" \
	"$(INTDIR)\pngutil.obj" \
	"$(INTDIR)\prof.obj" \
	"$(INTDIR)\readasm.obj" \
	"$(INTDIR)\readconf.obj" \
	"$(INTDIR)\readpng.obj" \
	"$(INTDIR)\regex.obj" \
	"$(INTDIR)\release.obj" \
	"$(INTDIR)\rgbimutil.obj" \
	"$(INTDIR)\rowley.obj" \
	"$(INTDIR)\rowleyhand.obj" \
	"$(INTDIR)\safe_alloc.obj" \
	"$(INTDIR)\search.obj" \
	"$(INTDIR)\shapefile.obj" \
	"$(INTDIR)\shapemodel.obj" \
	"$(INTDIR)\sparsemat.obj" \
	"$(INTDIR)\startshape.obj" \
	"$(INTDIR)\stasm_dll.obj" \
	"$(INTDIR)\tclHash.obj" \
	"$(INTDIR)\util.obj" \
	"$(INTDIR)\violajones.obj" \
	"$(INTDIR)\vjhand.obj" \
	"$(INTDIR)\wrbmp.obj"

"$(OUTDIR)\landmarks.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "landmarks - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\landmarks.dll"


CLEAN :
	-@erase "$(INTDIR)\asmsearch.obj"
	-@erase "$(INTDIR)\atface.obj"
	-@erase "$(INTDIR)\err.obj"
	-@erase "$(INTDIR)\ezfont.obj"
	-@erase "$(INTDIR)\find.obj"
	-@erase "$(INTDIR)\follow.obj"
	-@erase "$(INTDIR)\forward.obj"
	-@erase "$(INTDIR)\imfile.obj"
	-@erase "$(INTDIR)\imgiven.obj"
	-@erase "$(INTDIR)\imshape.obj"
	-@erase "$(INTDIR)\imutil.obj"
	-@erase "$(INTDIR)\imwrite.obj"
	-@erase "$(INTDIR)\initasm.obj"
	-@erase "$(INTDIR)\initnet.obj"
	-@erase "$(INTDIR)\jpegutil.obj"
	-@erase "$(INTDIR)\landmarks.obj"
	-@erase "$(INTDIR)\mat.obj"
	-@erase "$(INTDIR)\matvec.obj"
	-@erase "$(INTDIR)\mchol.obj"
	-@erase "$(INTDIR)\mrand.obj"
	-@erase "$(INTDIR)\pngutil.obj"
	-@erase "$(INTDIR)\prof.obj"
	-@erase "$(INTDIR)\readasm.obj"
	-@erase "$(INTDIR)\readconf.obj"
	-@erase "$(INTDIR)\readpng.obj"
	-@erase "$(INTDIR)\regex.obj"
	-@erase "$(INTDIR)\release.obj"
	-@erase "$(INTDIR)\rgbimutil.obj"
	-@erase "$(INTDIR)\rowley.obj"
	-@erase "$(INTDIR)\rowleyhand.obj"
	-@erase "$(INTDIR)\safe_alloc.obj"
	-@erase "$(INTDIR)\search.obj"
	-@erase "$(INTDIR)\shapefile.obj"
	-@erase "$(INTDIR)\shapemodel.obj"
	-@erase "$(INTDIR)\sparsemat.obj"
	-@erase "$(INTDIR)\startshape.obj"
	-@erase "$(INTDIR)\stasm_dll.obj"
	-@erase "$(INTDIR)\tclHash.obj"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\violajones.obj"
	-@erase "$(INTDIR)\vjhand.obj"
	-@erase "$(INTDIR)\wrbmp.obj"
	-@erase "$(OUTDIR)\landmarks.dll"
	-@erase "$(OUTDIR)\landmarks.exp"
	-@erase "$(OUTDIR)\landmarks.ilk"
	-@erase "$(OUTDIR)\landmarks.lib"
	-@erase "$(OUTDIR)\landmarks.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "." /I "C:\OpenCV2.1\include\opencv" /I "..\regex" /I "..\gsl" /I "..\gsl\gsl" /I "..\image" /I "..\jpeg" /I "..\mat" /I "..\rowley" /I "..\stasm" /I "..\tasm" /I "..\png" /I "..\png\pnglib" /I "..\png\zlib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LANDMARKS_EXPORTS" /D "HAVE_CONFIG_H" /D "STDC_HEADERS" /D "REGEX_MALLOC" /Fp"$(INTDIR)\landmarks.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\landmarks.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\gsl\lib_ms\mini_gslML.lib ..\gsl\lib_ms\gslcblasML.lib ..\jpeg\lib_ms\libjpeg.lib ..\png\pnglib\libpng.lib ..\png\zlib\zlib.lib C:\OpenCV2.1\lib\cv210.lib C:\OpenCV2.1\lib\cxcore210.lib C:\OpenCV2.1\lib\highgui210.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\landmarks.pdb" /debug /machine:I386 /nodefaultlib:"LIBCD" /nodefaultlib:"LIBC" /out:"$(OUTDIR)\landmarks.dll" /implib:"$(OUTDIR)\landmarks.lib" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\asmsearch.obj" \
	"$(INTDIR)\atface.obj" \
	"$(INTDIR)\err.obj" \
	"$(INTDIR)\ezfont.obj" \
	"$(INTDIR)\find.obj" \
	"$(INTDIR)\follow.obj" \
	"$(INTDIR)\forward.obj" \
	"$(INTDIR)\imfile.obj" \
	"$(INTDIR)\imgiven.obj" \
	"$(INTDIR)\imshape.obj" \
	"$(INTDIR)\imutil.obj" \
	"$(INTDIR)\imwrite.obj" \
	"$(INTDIR)\initasm.obj" \
	"$(INTDIR)\initnet.obj" \
	"$(INTDIR)\jpegutil.obj" \
	"$(INTDIR)\landmarks.obj" \
	"$(INTDIR)\mat.obj" \
	"$(INTDIR)\matvec.obj" \
	"$(INTDIR)\mchol.obj" \
	"$(INTDIR)\mrand.obj" \
	"$(INTDIR)\pngutil.obj" \
	"$(INTDIR)\prof.obj" \
	"$(INTDIR)\readasm.obj" \
	"$(INTDIR)\readconf.obj" \
	"$(INTDIR)\readpng.obj" \
	"$(INTDIR)\regex.obj" \
	"$(INTDIR)\release.obj" \
	"$(INTDIR)\rgbimutil.obj" \
	"$(INTDIR)\rowley.obj" \
	"$(INTDIR)\rowleyhand.obj" \
	"$(INTDIR)\safe_alloc.obj" \
	"$(INTDIR)\search.obj" \
	"$(INTDIR)\shapefile.obj" \
	"$(INTDIR)\shapemodel.obj" \
	"$(INTDIR)\sparsemat.obj" \
	"$(INTDIR)\startshape.obj" \
	"$(INTDIR)\stasm_dll.obj" \
	"$(INTDIR)\tclHash.obj" \
	"$(INTDIR)\util.obj" \
	"$(INTDIR)\violajones.obj" \
	"$(INTDIR)\vjhand.obj" \
	"$(INTDIR)\wrbmp.obj"

"$(OUTDIR)\landmarks.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("landmarks.dep")
!INCLUDE "landmarks.dep"
!ELSE 
!MESSAGE Warning: cannot find "landmarks.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "landmarks - Win32 Release" || "$(CFG)" == "landmarks - Win32 Debug"
SOURCE=..\stasm\asmsearch.cpp

"$(INTDIR)\asmsearch.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\stasm\atface.cpp

"$(INTDIR)\atface.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\stasm\err.cpp

"$(INTDIR)\err.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\image\ezfont.c

"$(INTDIR)\ezfont.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\rowley\find.cpp

"$(INTDIR)\find.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\rowley\follow.cpp

"$(INTDIR)\follow.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\rowley\forward.cpp

"$(INTDIR)\forward.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\image\imfile.cpp

"$(INTDIR)\imfile.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\image\imgiven.cpp

"$(INTDIR)\imgiven.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\stasm\imshape.cpp

"$(INTDIR)\imshape.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\image\imutil.cpp

"$(INTDIR)\imutil.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\image\imwrite.cpp

"$(INTDIR)\imwrite.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\stasm\initasm.cpp

"$(INTDIR)\initasm.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\rowley\initnet.cpp

"$(INTDIR)\initnet.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\image\jpegutil.cpp

"$(INTDIR)\jpegutil.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\stasm\landmarks.cpp

"$(INTDIR)\landmarks.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\mat\mat.cpp

"$(INTDIR)\mat.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\mat\matvec.cpp

"$(INTDIR)\matvec.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\mat\mchol.cpp

"$(INTDIR)\mchol.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\tasm\mrand.cpp

"$(INTDIR)\mrand.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\image\pngutil.cpp

"$(INTDIR)\pngutil.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\stasm\prof.cpp

"$(INTDIR)\prof.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\stasm\readasm.cpp

"$(INTDIR)\readasm.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\stasm\readconf.cpp

"$(INTDIR)\readconf.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\png\readpng.c

"$(INTDIR)\readpng.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\regex\regex.c

"$(INTDIR)\regex.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\stasm\release.cpp

"$(INTDIR)\release.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\image\rgbimutil.cpp

"$(INTDIR)\rgbimutil.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\stasm\rowley.cpp

"$(INTDIR)\rowley.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\stasm\rowleyhand.cpp

"$(INTDIR)\rowleyhand.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\stasm\safe_alloc.cpp

"$(INTDIR)\safe_alloc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\rowley\search.cpp

"$(INTDIR)\search.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\stasm\shapefile.cpp

"$(INTDIR)\shapefile.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\stasm\shapemodel.cpp

"$(INTDIR)\shapemodel.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\stasm\sparsemat.cpp

"$(INTDIR)\sparsemat.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\stasm\startshape.cpp

"$(INTDIR)\startshape.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\stasm\stasm_dll.cpp

"$(INTDIR)\stasm_dll.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\rowley\tclHash.c

"$(INTDIR)\tclHash.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\stasm\util.cpp

"$(INTDIR)\util.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\stasm\violajones.cpp

"$(INTDIR)\violajones.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\stasm\vjhand.cpp

"$(INTDIR)\vjhand.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\jpeg\wrbmp.c

"$(INTDIR)\wrbmp.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

