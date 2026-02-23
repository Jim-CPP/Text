// Template.cpp

#include "Template.h"

#include <windows.h>

// Global variables
static ListBoxWindow g_listBoxWindow;
static StatusBarWindow g_statusBarWindow;

BOOL ListBoxWindowSelectionChangeFunction( LPCTSTR lpszSelectedItemText )
{
	// Show selected item text on status bar window
	return g_statusBarWindow.SetText( lpszSelectedItemText );

} // End of function ListBoxWindowSelectionChangeFunction

BOOL ListBoxWindowDoubleClickFunction( LPCTSTR lpszSelectedItemText )
{
	// Display selected item text
	MessageBox( NULL, lpszSelectedItemText, INFORMATION_MESSAGE_CAPTION, ( MB_OK | MB_ICONINFORMATION ) );

	return TRUE;

} // End of function ListBoxWindowDoubleClickFunction

BOOL ArgumentFunction( LPCTSTR lpszArgument )
{
	// Add argument to list box window
	g_listBoxWindow.AddString( lpszArgument );

	return TRUE;

} // End of function ArgumentFunction

BOOL DropFunction( LPCTSTR lpszFilePath )
{
	// Add file to list box window
	g_listBoxWindow.AddString( lpszFilePath );

	return TRUE;

} // End of function DropFunction

int ShowAboutMessage( HWND hWndParent )
{
	int nResult = 0;

	MSGBOXPARAMS mbp;

	// Clear message box parameter structure
	ZeroMemory( &mbp, sizeof( mbp ) );

	// Initialise message box parameter structure
	mbp.cbSize		= sizeof( MSGBOXPARAMS );
	mbp.hwndOwner	= hWndParent;
	mbp.hInstance	= NULL; // Note that this must be null to use standard icons
	mbp.lpszText	= ABOUT_MESSAGE_TEXT;
	mbp.lpszCaption	= ABOUT_MESSAGE_CAPTION;
	mbp.dwStyle		= ( MB_OK | MB_USERICON );
	mbp.lpszIcon	= WINDOW_CLASS_CLASS_DEFAULT_ICON_NAME;

	// Show message box
	nResult = MessageBoxIndirect( &mbp );

	return nResult;

} // End of function ShowAboutMessage

LRESULT CALLBACK MainWindowProcedure( HWND hWndMain, UINT uMessage, WPARAM wParam, LPARAM lParam )
{
	LRESULT lr = 0;

	// Select message
	switch( uMessage )
	{
		case WM_CREATE:
		{
			// A create message
			HINSTANCE hInstance;
			Font font;

			// Get instance
			hInstance = ( ( LPCREATESTRUCT )lParam )->hInstance;

			// Get font
			font.Get( DEFAULT_GUI_FONT );

			// Create list box window
			if( g_listBoxWindow.Create( hWndMain, hInstance, LIST_BOX_WINDOW_CLASS_DEFAULT_TEXT ) )
			{
				// Successfully created list box window

				// Set list box window font
				g_listBoxWindow.SetFont( font );

				// Create status bar window
				if( g_statusBarWindow.Create( hWndMain, hInstance, STATUS_BAR_WINDOW_CLASS_DEFAULT_TEXT ) )
				{
					// Successfully created status bar window

					// Set status bar window font
					g_statusBarWindow.SetFont( font );

				} // End of successfully created status bar window

			} // End of successfully created list box window

			// Break out of switch
			break;

		} // End of a create message
		case WM_ACTIVATE:
		{
			// An activate message

			// Focus on list box window
			g_listBoxWindow.SetFocus();

			// Break out of switch
			break;

		} // End of an activate message
		case WM_SIZE:
		{
			// A size message
			int nClientWidth;
			int nClientHeight;
			RECT rcStatusBar;
			int nStatusBarWindowHeight;
			int nListBoxWindowHeight;

			// Store client width and height
			nClientWidth	= ( int )LOWORD( lParam );
			nClientHeight	= ( int )HIWORD( lParam );

			// Size status bar window
			g_statusBarWindow.Size();

			// Get status bar window size
			g_statusBarWindow.GetWindowRect( &rcStatusBar );

			// Calculate window sizes
			nStatusBarWindowHeight	= ( rcStatusBar.bottom - rcStatusBar.top );
			nListBoxWindowHeight	= ( nClientHeight - nStatusBarWindowHeight );

			// Move control windows
			g_listBoxWindow.Move( 0, 0, nClientWidth, nListBoxWindowHeight );

			// Break out of switch
			break;

		} // End of a size message
		case WM_GETMINMAXINFO:
		{
			// A get min max info message
			MinMaxInfo minMaxInfo;

			// Update min max info
			minMaxInfo.Update( lParam );
			// Note that this limits the minimum size of the window

			// Break out of switch
			break;

		} // End of a get min max info message
		case WM_DROPFILES:
		{
			// A drop files message
			//
			// Note that window needs extended style WS_EX_ACCEPTFILES for this to work
			Drop drop;

			// Handle dropped files
			drop.ProcessFiles( wParam, &DropFunction );

			// Break out of switch
			break;

		} // End of a drop files message
		case WM_COMMAND:
		{
			// A command message

			// Select command
			switch( LOWORD( wParam ) )
			{
				case IDM_FILE_EXIT:
				{
					// A file exit command

					// Destroy main window
					DestroyWindow( hWndMain );

					// Break out of switch
					break;

				} // End of a file exit command
				case IDM_HELP_ABOUT:
				{
					// A help about command

					// Show about message
					ShowAboutMessage( hWndMain );

					// Break out of switch
					break;

				} // End of a help about command
				default:
				{
					// Default command

					// See if command message is from list box window
					if( ( HWND )lParam == g_listBoxWindow )
					{
						// Command message is from list box window

						// Handle command message from list box window
						lr = g_listBoxWindow.HandleCommandMessage( hWndMain, wParam, lParam, &ListBoxWindowSelectionChangeFunction, &ListBoxWindowDoubleClickFunction );

					} // End of command message is from list box window
					else
					{
						// Command message is not from list box window

						// Call default procedure
						lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

					} // End of command message is not from list box window

					// Break out of switch
					break;

				} // End of default command

			}; // End of selection for command

			// Break out of switch
			break;

		} // End of a command message
		case WM_SYSCOMMAND:
		{
			// A system command message

			// Select system command
			switch( LOWORD( wParam ) )
			{
				case IDM_HELP_ABOUT:
				{
					// A help about system command

					// Show about message
					ShowAboutMessage( hWndMain );

					// Break out of switch
					break;

				} // End of a help about system command
				default:
				{
					// Default system command

					// Call default procedure
					lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

					// Break out of switch
					break;

				} // End of default system command

			}; // End of selection for system command

			// Break out of switch
			break;

		} // End of a system command message
		case WM_NOTIFY:
		{
			// A notify message
			LPNMHDR lpNmHdr;

			// Get notify message handler
			lpNmHdr = ( LPNMHDR )lParam;

			// Source window is lpNmHdr->hwndFrom

			// Call default procedure
			lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

			// Break out of switch
			break;

		} // End of a notify message
		case WM_CONTEXTMENU:
		{
			// A context menu message
			Menu contextMenu;

			// Load context menu
			contextMenu.Load( MAKEINTRESOURCE( IDR_CONTEXT_MENU ) );

			// Show context menu
			contextMenu.TrackPopupMenu( 0, hWndMain, lParam );

			// Break out of switch
			break;

		} // End of a context menu message
		case WM_CLOSE:
		{
			// A c message

			// Destroy main window
			DestroyWindow( hWndMain );

			// Break out of switch
			break;

		} // End of a close message
		case WM_DESTROY:
		{
			// A destroy message

			// Terminate thread
			PostQuitMessage( 0 );

			// Break out of switch
			break;

		} // End of a destroy message
		default:
		{
			// Default message

			// Call default handler
			lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

			// Break out of switch
			break;

		} // End of default message

	}; // End of selection for message

	return lr;

} // End of function MainWindowProcedure

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPTSTR, int nCmdShow )
{
	Message message;

	WindowClass mainWindowClass;

	// Initialise main window class
	mainWindowClass.Init( MAIN_WINDOW_CLASS_NAME, MainWindowProcedure, hInstance );

	// Register main window class
	if( mainWindowClass.Register() )
	{
		// Successfully registered main window class
		Window mainWindow;

		// Create main window
		if( mainWindow.Create( MAIN_WINDOW_CLASS_NAME, NULL, hInstance, MAIN_WINDOW_TEXT ) )
		{
			// Successfully created main window
			Menu systemMenu;
			ArgumentList argumentList;

			// Get system menu
			systemMenu = mainWindow.GetSystemMenu( FALSE );

			// Add separator item to system menu
			systemMenu.InsertSeparator( MENU_CLASS_SYSTEM_MENU_SEPARATOR_ITEM_POSITION );

			// Add about item to system menu
			systemMenu.InsertItem( MENU_CLASS_SYSTEM_MENU_ABOUT_ITEM_POSITION, MENU_CLASS_SYSTEM_MENU_ABOUT_ITEM_TEXT, IDM_HELP_ABOUT );

			// Get argument list
			if( argumentList.Get() )
			{
				// Successfully got argument list

				// Process arguments
				argumentList.ProcessArguments( &ArgumentFunction );

			} // End of successfully got argument list

			// Show main window
			mainWindow.Show( nCmdShow );

			// Update main window
			mainWindow.Update();

			// Main message loop
			while( message.Get() > 0 )
			{
				// Translate message
				message.Translate();

				// Dispatch message
				message.Dispatch();

			}; // End of main message loop

		} // End of successfully created main window
		else
		{
			// Unable to create main window

			// Display error message
			MessageBox( NULL, WINDOW_CLASS_UNABLE_TO_CREATE_WINDOW_ERROR_MESSAGE, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

		} // End of unable to create main window

	} // End of successfully registered main window class
	else
	{
		// Unable to register main window class

		// Display error message
		MessageBox( NULL, WINDOW_CLASS_CLASS_UNABLE_TO_REGISTER_WINDOW_CLASS_ERROR_MESSAGE, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

	} // End of unable to register main window class

	return message;

} // End of function WinMain
