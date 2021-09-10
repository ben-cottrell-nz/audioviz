set RTAUDIO_PATH=C:\Users\ben\Downloads\rtaudio-5.1.0
set MINGW_PATH=C:\msys64\mingw64\bin
copy "%RTAUDIO_PATH%\build\librtaudio.dll" build-win\
copy "%MINGW_PATH%\zlib1.dll" build-win\
copy "%MINGW_PATH%\libzstd.dll" build-win\
copy "%MINGW_PATH%\libstdc++-6.dll" build-win\
copy "%MINGW_PATH%\libgcc_s_seh-1.dll" build-win\
copy "%MINGW_PATH%\libwinpthread-1.dll" build-win\
 

