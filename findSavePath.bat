@echo off
main.exe %1 %2 %3
"C:\Program Files\R\R-4.1.1\bin\Rscript.exe" make_map.R %2 %3