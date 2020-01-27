@rem *********************************************
@rem Used to proxy to call any other script
@rem sets environment variables
@rem always return back to current dir
@rem *********************************************

@echo on
set p_name=cpython-3.8
set p_full_name=python-3.8.1
set work_dir=%cd%
set build_dir=%cd%\tmp\out
set uninstall_dir=%cd%\tmp\uninstall
set pexe=%build_dir%\win32\%p_full_name%.exe
set pexe_uninstall=%uninstall_dir%\%p_full_name%.exe
set install_dir=%cd%\tmp\to_install
set ready_dir=%cd%\tmp\ready
set build_msi=python\%p_name%\Tools\msi
call %1
cd %work_dir%