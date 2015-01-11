@echo off
SET plugin_dir="C:\Program Files (x86)\Qip Infium\Plugins\NowPlaying"
SET build_dir=%1
cd %1
echo ------ Script by AbsoluteZero --------
echo Output directory: %build_dir%
echo Plugin directory: %plugin_dir%
del %plugin_dir%\*.dll /S /F /Q
mkdir %plugin_dir%\plugins
mkdir %plugin_dir%\languages
copy NowPlaying.dll %plugin_dir%
copy plugins\*.dll %plugin_dir%\plugins
copy languages\*.dll %plugin_dir%\languages
@echo on