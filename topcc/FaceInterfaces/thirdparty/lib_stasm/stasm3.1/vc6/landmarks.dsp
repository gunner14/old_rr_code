# Microsoft Developer Studio Project File - Name="landmarks" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=landmarks - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "landmarks.mak".
!MESSAGE 
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

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "landmarks - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LANDMARKS_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "." /I "C:\OpenCV2.1\include\opencv" /I "..\regex" /I "..\gsl" /I "..\gsl\gsl" /I "..\image" /I "..\jpeg" /I "..\mat" /I "..\rowley" /I "..\stasm" /I "..\tasm" /I "..\png" /I "..\png\pnglib" /I "..\png\zlib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LANDMARKS_EXPORTS" /D "HAVE_CONFIG_H" /D "STDC_HEADERS" /D "REGEX_MALLOC" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\gsl\lib_ms\mini_gslML.lib ..\gsl\lib_ms\gslcblasML.lib ..\jpeg\lib_ms\libjpeg.lib ..\png\pnglib\libpng.lib ..\png\zlib\zlib.lib C:\OpenCV2.1\lib\cv210.lib C:\OpenCV2.1\lib\cxcore210.lib C:\OpenCV2.1\lib\highgui210.lib setargv.obj /nologo /dll /machine:I386 /nodefaultlib:"LIBCD" /nodefaultlib:"LIBC"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "landmarks - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LANDMARKS_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "." /I "C:\OpenCV2.1\include\opencv" /I "..\regex" /I "..\gsl" /I "..\gsl\gsl" /I "..\image" /I "..\jpeg" /I "..\mat" /I "..\rowley" /I "..\stasm" /I "..\tasm" /I "..\png" /I "..\png\pnglib" /I "..\png\zlib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LANDMARKS_EXPORTS" /D "HAVE_CONFIG_H" /D "STDC_HEADERS" /D "REGEX_MALLOC" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\gsl\lib_ms\mini_gslML.lib ..\gsl\lib_ms\gslcblasML.lib ..\jpeg\lib_ms\libjpeg.lib ..\png\pnglib\libpng.lib ..\png\zlib\zlib.lib C:\OpenCV2.1\lib\cv210.lib C:\OpenCV2.1\lib\cxcore210.lib C:\OpenCV2.1\lib\highgui210.lib /nologo /dll /debug /machine:I386 /nodefaultlib:"LIBCD" /nodefaultlib:"LIBC" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "landmarks - Win32 Release"
# Name "landmarks - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\stasm\asmsearch.cpp
# End Source File
# Begin Source File

SOURCE=..\stasm\atface.cpp
# End Source File
# Begin Source File

SOURCE=..\stasm\err.cpp
# End Source File
# Begin Source File

SOURCE=..\image\ezfont.c
# End Source File
# Begin Source File

SOURCE=..\rowley\find.cpp
# End Source File
# Begin Source File

SOURCE=..\rowley\follow.cpp
# End Source File
# Begin Source File

SOURCE=..\rowley\forward.cpp
# End Source File
# Begin Source File

SOURCE=..\image\imfile.cpp
# End Source File
# Begin Source File

SOURCE=..\image\imgiven.cpp
# End Source File
# Begin Source File

SOURCE=..\stasm\imshape.cpp
# End Source File
# Begin Source File

SOURCE=..\image\imutil.cpp
# End Source File
# Begin Source File

SOURCE=..\image\imwrite.cpp
# End Source File
# Begin Source File

SOURCE=..\stasm\initasm.cpp
# End Source File
# Begin Source File

SOURCE=..\rowley\initnet.cpp
# End Source File
# Begin Source File

SOURCE=..\image\jpegutil.cpp
# End Source File
# Begin Source File

SOURCE=..\stasm\landmarks.cpp
# End Source File
# Begin Source File

SOURCE=..\mat\mat.cpp
# End Source File
# Begin Source File

SOURCE=..\mat\matvec.cpp
# End Source File
# Begin Source File

SOURCE=..\mat\mchol.cpp
# End Source File
# Begin Source File

SOURCE=..\tasm\mrand.cpp
# End Source File
# Begin Source File

SOURCE=..\image\pngutil.cpp
# End Source File
# Begin Source File

SOURCE=..\stasm\prof.cpp
# End Source File
# Begin Source File

SOURCE=..\stasm\readasm.cpp
# End Source File
# Begin Source File

SOURCE=..\stasm\readconf.cpp
# End Source File
# Begin Source File

SOURCE=..\png\readpng.c
# End Source File
# Begin Source File

SOURCE=..\regex\regex.c
# End Source File
# Begin Source File

SOURCE=..\stasm\release.cpp
# End Source File
# Begin Source File

SOURCE=..\image\rgbimutil.cpp
# End Source File
# Begin Source File

SOURCE=..\stasm\rowley.cpp
# End Source File
# Begin Source File

SOURCE=..\stasm\rowleyhand.cpp
# End Source File
# Begin Source File

SOURCE=..\stasm\safe_alloc.cpp
# End Source File
# Begin Source File

SOURCE=..\rowley\search.cpp
# End Source File
# Begin Source File

SOURCE=..\stasm\shapefile.cpp
# End Source File
# Begin Source File

SOURCE=..\stasm\shapemodel.cpp
# End Source File
# Begin Source File

SOURCE=..\stasm\sparsemat.cpp
# End Source File
# Begin Source File

SOURCE=..\stasm\startshape.cpp
# End Source File
# Begin Source File

SOURCE=..\stasm\stasm_dll.cpp
# End Source File
# Begin Source File

SOURCE=..\rowley\tclHash.c
# End Source File
# Begin Source File

SOURCE=..\stasm\util.cpp
# End Source File
# Begin Source File

SOURCE=..\stasm\violajones.cpp
# End Source File
# Begin Source File

SOURCE=..\stasm\vjhand.cpp
# End Source File
# Begin Source File

SOURCE=..\jpeg\wrbmp.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\stasm\landmarks.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
