// Text.h

#pragma once

#include <windows.h>

#include "..\Common\Ascii.h"
#include "..\Common\Common.h"

#include "..\Classes\ArgumentListClass.h"
#include "..\Classes\DropClass.h"
#include "..\Classes\FileClass.h"
#include "..\Classes\FontClass.h"
#include "..\Classes\MenuClass.h"
#include "..\Classes\MessageClass.h"
#include "..\Classes\MinMaxInfoClass.h"
#include "..\Classes\WindowClass.h"
#include "..\Classes\WindowClassClass.h"

#include "..\Classes\RichEditWindowClass.h"
#include "..\Classes\StatusBarWindowClass.h"

#include "Resource.h"

#define MAIN_WINDOW_CLASS_NAME													"Main Text Window Class"

#define MAIN_WINDOW_TEXT														"Text"

#define ABOUT_MESSAGE_TEXT														"Text\r\n"							\
																				"\r\n"								\
																				"Written by Jim Smith\r\n"			\
																				"\r\n"								\
																				"February 2026"

#define TEXT_FILE_NAME															"Text.txt"
