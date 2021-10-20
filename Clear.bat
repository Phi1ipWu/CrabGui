rd /s /q .\Core\lib
rd /s /q .\Core\temp

rd /s /q .\D3D9Renderer\lib
rd /s /q .\D3D9Renderer\temp

rd /s /q .\FreeParser\lib
rd /s /q .\FreeParser\temp

rd /s /q .\Sample\temp
del /s /q .\Output\CrabGui*_d.*

rd /s /q .\ipch

del /s /q *.cbp
del /s /q *.layout
del /s /q *.depend
del /s /q *.filter

del /s /q *.log
del /s /q *.ncb
del /s /q *.sdf
del /s /q /a *.suo
del /s /q *.user
