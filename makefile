cc = "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Tools\MSVC\14.29.30133\bin\Hostx64\x64\cl.exe"
cflags = /I./cryptocpp/incl /I"C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Tools\MSVC\14.29.30133\include" /I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.18362.0\shared" /I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.18362.0\ucrt" /I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.18362.0\um" /I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.18362.0\winrt"  
ldflags = /LIBPATH:./cryptocpp/lib /LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Tools\MSVC\14.29.30133\lib\x64"  /LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.18362.0\ucrt\x64" /LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.18362.0\um\x64" /LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Tools\MSVC\14.29.30133\lib\x64"
libs = cryptlib.lib


dll:
	$(cc) /DEXPORTFILERAPI $(cflags) CalculateHash.cc /c /Fo:CalculateHash.obj
	$(cc) /DEXPORTFILERAPI $(cflags) Classification.cc /c /Fo:Classification.obj
	$(cc) /DEXPORTFILERAPI $(cflags) /std:c++17 Utilities.cc /c /Fo:Utilities.obj
	$(cc) /LD CalculateHash.obj Classification.obj Utilities.obj /link $(ldflags) $(libs) /OUT:FileClassificator.dll /IMPLIB:FileClassificator.lib 
	-del CalculateHash.obj Classification.obj Utilities.obj FileClassificator.exp

test:
	$(cc) $(cflags) /std:c++17 test.cc /c /Fo:test.obj
	$(cc) test.obj /link $(ldflags) /LIBPATH:./ $(libs)  FileClassificator.lib /OUT:test.exe
	-del test.obj
