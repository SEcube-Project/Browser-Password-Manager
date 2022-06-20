@echo off

if not exist ".\WINOUT" mkdir ".\WINOUT" || goto :error
cd .\WINOUT || goto :error

CL /c /D_WIN32_HATE ..\\..\\"Host Password Manager"\sources\L0\L0.cpp || goto :error
CL /c /D_WIN32_HATE ..\\..\\"Host Password Manager"\sources\L0\L0_Base\L0_base.cpp || goto :error
CL /c /D_WIN32_HATE ..\\..\\"Host Password Manager"\sources\L0\L0_commodities.cpp || goto :error
CL /c /D_WIN32_HATE ..\\..\\"Host Password Manager"\sources\L0\L0_communication.cpp || goto :error
CL /c /D_WIN32_HATE ..\\..\\"Host Password Manager"\sources\L0\L0_provision.cpp || goto :error
CL /c /D_WIN32_HATE ..\\..\\"Host Password Manager"\sources\L1\Crypto_Libraries\aes256.cpp || goto :error
CL /c /D_WIN32_HATE ..\\..\\"Host Password Manager"\sources\L1\Crypto_Libraries\pbkdf2.c || goto :error
CL /c /D_WIN32_HATE ..\\..\\"Host Password Manager"\sources\L1\Crypto_Libraries\sha256.c || goto :error
CL /c /D_WIN32_HATE ..\\..\\"Host Password Manager"\sources\L1\L1.cpp || goto :error
CL /c /D_WIN32_HATE ..\\..\\"Host Password Manager"\sources\L1\L1_Base\L1_base.cpp || goto :error
CL /c /D_WIN32_HATE ..\\..\\"Host Password Manager"\sources\L1\L1_login_logout.cpp || goto :error
CL /c /D_WIN32_HATE ..\\..\\"Host Password Manager"\sources\L1\L1_security.cpp || goto :error
CL /c /D_WIN32_HATE ..\\..\\"Host Password Manager"\sources\L1\L1_sekey.cpp || goto :error
CL /c /D_WIN32_HATE ..\\..\\"Host Password Manager"\sources\L1\L1_sepass.cpp || goto :error
CL /c /D_WIN32_HATE ..\host_wrapper.cpp || goto :error
CL /c /D_WIN32_HATE ..\L0_wrapper.cpp  || goto :error
CL /c /D_WIN32_HATE ..\L1_wrapper.cpp || goto :error

link -nologo -dll -out:..\\high_quality_engineering_lib.dll *.obj || goto :error

cd .. || goto :error
rmdir /s /q ".\WINOUT" || goto :error
del ".\\high_quality_engineering_lib.exp" || goto :error
del ".\\high_quality_engineering_lib.lib" || goto :error

python -m pip install -r requirements.txt || goto :error
python -m pip install pyarmor || goto :error
python -m pip install pyinstaller || goto :error
python -m pip install tinyaes || goto :error
python -m pyarmor.pyarmor pack --name BPMMiddleware --clean -e "--add-data 'high_quality_engineering_lib.dll;.' --add-data 'cert.pem;.' --add-data 'key.pem;.' --onefile " app.py || goto :error

MOVE dist\BPMMiddleware.exe . || goto :error
rmdir /s /q build || goto :error
rmdir /s /q dist || goto :error

exit /b 0

:error
echo Failed with error #%errorlevel%.
exit /b %errorlevel%
