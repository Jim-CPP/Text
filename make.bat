cls

del *.exe

windres Resource.rc -o Resource.o

g++ -mwindows -m64 -static -Wall -Wextra Template.cpp ^
 ^
 ..\Classes\ArgumentListClass.cpp ^
 ..\Classes\DropClass.cpp ^
 ..\Classes\FontClass.cpp ^
 ..\Classes\MenuClass.cpp ^
 ..\Classes\MessageClass.cpp ^
 ..\Classes\MinMaxInfoClass.cpp ^
 ..\Classes\WindowClass.cpp ^
 ..\Classes\WindowClassClass.cpp ^
 ^
 ..\Classes\ListBoxWindowClass.cpp ^
 ..\Classes\StatusBarWindowClass.cpp ^
 ^
 Resource.o -o Template.exe
