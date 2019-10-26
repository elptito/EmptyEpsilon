set PROJECT_DIR=%~dp0
set WSLENV=PROJECT_DIR/p
bash -c "$PROJECT_DIR/build_release.sh"
pause