Kinect 2 con Cinder
===================
Guía de uso para kinect 2 con Cinder 9.

Configuración de proyecto
-------------------------

Primero clonar el block [Cinder-KCB2](https://github.com/wieden-kennedy/Cinder-KCB2) a la carpeta blocks de Cinder.

No sirve usar la plantilla del block así que hay que importar el bloque a manita.
- Primero crear un proyecto con Tinderbox, importas los demás bloques que se vaya a utilizar.
- En el folder del proyecto crear carpeta blocks, y luego otra llamada Cinder-KCB2 dentro.
- Copiar las capetas lib y src dentro del bloque [Cinder-KCB2](https://github.com/wieden-kennedy/Cinder-KCB2).

Configurar el proyecto en Visual Studio 2013, en propiedades:
Es lo mjismo para debug y release y sólo probé para x64, creo que no tiene sentido hacerlo para 32bits, pero debe funcionar.
- C/C++->Carpetas adicionales añadir :
```
$(KINECTSDK20_DIR)\inc
..\blocks\Cinder-KCB2\lib
..\blocks\Cinder-KCB2\src
``` 
- Linker->General->Adsitional Library Directories:
```
$(KINECTSDK20_DIR)\lib\$(PlatformTarget)
..\blocks\Cinder-KCB2\lib\$(PlatformTarget)
``` 
- Linker->Input->Additional Dependencies:
```
KCBv2.lib
kinect20.lib
Kinect20.Face.lib
``` 
- Build events->Post-Build Events:
```
xcopy /e /i /y "$(SolutionDir)..\blocks\Cinder-KCB2\lib\$(PlatformTarget)\*.dll" "$(SolutionDir)$(Platform)\$(Configuration)\"
xcopy /e /i /y "$(KINECTSDK20_DIR)Redist\Face\$(PlatformTarget)\Kinect20.Face.dll" "$(SolutionDir)$(Platform)\$(Configuration)\"
xcopy /e /i /y "$(KINECTSDK20_DIR)Redist\Face\$(PlatformTarget)\NuiDatabase" "$(SolutionDir)$(Platform)\$(Configuration)\NuiDatabase"
```





