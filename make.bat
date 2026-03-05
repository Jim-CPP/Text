cls

del *.exe

windres Resource.rc -o Resource.o

g++ -mwindows -m64 -static -Wall -Wextra Text.cpp ^
 ^
 ..\Classes\ArgumentListClass.cpp ^
 ..\Classes\ClipboardClass.cpp ^
 ..\Classes\DropClass.cpp ^
 ..\Classes\FileClass.cpp ^
 ..\Classes\FontClass.cpp ^
 ..\Classes\MenuClass.cpp ^
 ..\Classes\MessageClass.cpp ^
 ..\Classes\MinMaxInfoClass.cpp ^
 ..\Classes\OpenFileNameClass.cpp ^
 ..\Classes\WindowClass.cpp ^
 ..\Classes\WindowClassClass.cpp ^
 ^
 ..\Classes\RichEditWindowClass.cpp ^
 ..\Classes\StatusBarWindowClass.cpp ^
 ..\Classes\ToolBarWindowClass.cpp ^
 ^
 Resource.o -o Text.exe -lcomctl32
