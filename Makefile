build: tema1.exe

tema1.exe: tema1.obj hash.lib
	link /nologo /out:tema1.exe $**

tema1.obj: tema1.c
	cl /nologo /DDLL_IMPORTS /c $**

hash.lib: hash.dll	

hash.dll:
	link /nologo /dll /out:$@ /implib:hash.lib $**

clean:
	del /Q /F *.obj *.exe