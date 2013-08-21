# Microsoft Developer Studio Project File - Name="facelandmark" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=facelandmark - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "facelandmark.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "facelandmark.mak" CFG="facelandmark - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "facelandmark - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "facelandmark - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "facelandmark - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\regex" /I "..\gsl" /I "..\gsl\gsl" /I "..\image" /I "..\jpeg" /I "..\mat" /I "..\rowley" /I "..\stasm" /I "..\tasm" /I "..\png" /I "..\png\pnglib" /I "..\png\zlib" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "HAVE_CONFIG_H" /D "STDC_HEADERS" /D "REGEX_MALLOC" /FR /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "facelandmark - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\regex" /I "..\gsl" /I "..\gsl\gsl" /I "..\image" /I "..\jpeg" /I "..\mat" /I "..\rowley" /I "..\stasm" /I "..\tasm" /I "..\png" /I "..\png\pnglib" /I "..\png\zlib" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "HAVE_CONFIG_H" /D "STDC_HEADERS" /D "REGEX_MALLOC" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "facelandmark - Win32 Release"
# Name "facelandmark - Win32 Debug"
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

SOURCE=.\facelandmark.cpp
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

SOURCE=..\stasm\prof.cpp
# End Source File
# Begin Source File

SOURCE=..\stasm\readasm.cpp
# End Source File
# Begin Source File

SOURCE=..\stasm\readconf.cpp
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

SOURCE=..\rowley\tclHash.c
# End Source File
# Begin Source File

SOURCE=..\stasm\util.cpp
# End Source File
# Begin Source File

SOURCE=..\stasm\vjhand.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\facelandmark.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\gsl\lib_ms\gslcblasML.lib
# End Source File
# Begin Source File

SOURCE=..\gsl\lib_ms\mini_gslML.lib
# End Source File
# End Target
# End Project
