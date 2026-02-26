// Text.cpp

#include "Text.h"

#include <windows.h>

// Global variables
static Menu g_contextMenu;
static RichEditWindow g_richEditWindow;
static StatusBarWindow g_statusBarWindow;

void RichEditWindowUpdateFunction( BOOL bCanUndo, BOOL bCanRedo )
{
	// Update menu items accordingly
	g_contextMenu.EnableItem( IDM_EDIT_UNDO, bCanUndo );
	g_contextMenu.EnableItem( IDM_EDIT_REDO, bCanRedo );

} // End of function RichEditWindowUpdateFunction

void RichEditWindowSelectionChangedFunction( BOOL bIsTextSelected )
{
	// Update menu items accordingly
	g_contextMenu.EnableItem( IDM_EDIT_CUT,		bIsTextSelected );
	g_contextMenu.EnableItem( IDM_EDIT_COPY,	bIsTextSelected );
	g_contextMenu.EnableItem( IDM_EDIT_DELETE,	bIsTextSelected );

} // End of function RichEditWindowSelectionChangedFunction

BOOL RichEditWindowSelectionChangeFunction( LPCTSTR lpszSelectedItemText )
{
	// Show selected item text on status bar window
	return g_statusBarWindow.SetText( lpszSelectedItemText );

} // End of function RichEditWindowSelectionChangeFunction

BOOL RichEditWindowDoubleClickFunction( LPCTSTR lpszSelectedItemText )
{
	// Display selected item text
	MessageBox( NULL, lpszSelectedItemText, INFORMATION_MESSAGE_CAPTION, ( MB_OK | MB_ICONINFORMATION ) );

	return TRUE;

} // End of function RichEditWindowDoubleClickFunction

BOOL ArgumentFunction( LPCTSTR lpszArgument )
{
	// Display argument
	MessageBox( NULL, lpszArgument, "Argument", ( MB_OK | MB_ICONINFORMATION ) );

	return TRUE;

} // End of function ArgumentFunction

BOOL DropFunction( LPCTSTR lpszFilePath )
{
	// Display file path
	MessageBox( NULL, lpszFilePath, "Dropped", ( MB_OK | MB_ICONINFORMATION ) );

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

	static BOOL s_bListening = FALSE;

	// Select message
	switch( uMessage )
	{
		case WM_CREATE:
		{
			// A create message
			HINSTANCE hInstance;

			// Get instance
			hInstance = ( ( LPCREATESTRUCT )lParam )->hInstance;

			// Add main window as a clipboard listener
			s_bListening = ::AddClipboardFormatListener( hWndMain );

			// Create rich edit window
			if( g_richEditWindow.Create( hWndMain, hInstance, RICH_EDIT_WINDOW_CLASS_DEFAULT_TEXT ) )
			{
				// Successfully created rich edit window
				Font font;

				// Get rich edit window font
				font.Get( ANSI_FIXED_FONT );

				// Set rich edit window font
				g_richEditWindow.SetFont( font );

				// Set rich edit window to plain text mode
				g_richEditWindow.SetTextMode( RICH_EDIT_WINDOW_CLASS_DEFAULT_PLAIN_TEXT_MODE );

				// Set rich edit window event mask to default
				g_richEditWindow.SetEventMask();

				// Create status bar window
				if( g_statusBarWindow.Create( hWndMain, hInstance, STATUS_BAR_WINDOW_CLASS_DEFAULT_TEXT ) )
				{
					// Successfully created status bar window

					// Get status bar window font
					font.Get( DEFAULT_GUI_FONT );

					// Set status bar window font
					g_statusBarWindow.SetFont( font );

				} // End of successfully created status bar window

			} // End of successfully created rich edit window

			// Break out of switch
			break;

		} // End of a create message
		case WM_ACTIVATE:
		{
			// An activate message

			// Focus on rich edit window
			g_richEditWindow.SetFocus();

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
			int nRichEditWindowHeight;

			// Store client width and height
			nClientWidth	= ( int )LOWORD( lParam );
			nClientHeight	= ( int )HIWORD( lParam );

			// Size status bar window
			g_statusBarWindow.Size();

			// Get status bar window size
			g_statusBarWindow.GetWindowRect( &rcStatusBar );

			// Calculate window sizes
			nStatusBarWindowHeight	= ( rcStatusBar.bottom - rcStatusBar.top );
			nRichEditWindowHeight	= ( nClientHeight - nStatusBarWindowHeight );

			// Move control windows
			g_richEditWindow.Move( 0, 0, nClientWidth, nRichEditWindowHeight );

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
				case IDM_EDIT_UNDO:
				{
					// An edit undo command

					// Undo
					g_richEditWindow.Undo();

					// Break out of switch
					break;

				} // End of an edit undo command
				case IDM_EDIT_REDO:
				{
					// An edit redo command

					// Redo
					g_richEditWindow.Redo();

					// Break out of switch
					break;

				} // End of an edit redo command
				case IDM_EDIT_CUT:
				{
					// An edit cut command

					// Cut
					g_richEditWindow.Cut();

					// Break out of switch
					break;

				} // End of an edit cut command
				case IDM_EDIT_COPY:
				{
					// An edit copy command

					// Copy
					g_richEditWindow.Copy();

					// Break out of switch
					break;

				} // End of an edit copy command
				case IDM_EDIT_PASTE:
				{
					// An edit paste command

					// Paste
					g_richEditWindow.Paste();

					// Break out of switch
					break;

				} // End of an edit paste command
				case IDM_EDIT_DELETE:
				{
					// An edit delete command

					// Delete text
					g_richEditWindow.Delete();

					// Break out of switch
					break;

				} // End of an edit delete command
				case IDM_EDIT_SELECT_ALL:
				{
					// An edit select all command

					// Select all text
					g_richEditWindow.Select();

					// Break out of switch
					break;

				} // End of an edit select all command
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

					// See if command message is from rich edit window
					if( ( HWND )lParam == g_richEditWindow )
					{
						// Command message is from rich edit window

						// Handle command message from rich edit window
						lr = g_richEditWindow.HandleCommandMessage( hWndMain, wParam, lParam, &RichEditWindowUpdateFunction );

					} // End of command message is from rich edit window
					else
					{
						// Command message is not from rich edit window

						// Call default procedure
						lr = ::DefWindowProc( hWndMain, uMessage, wParam, lParam );

					} // End of command message is not from rich edit window

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
					lr = ::DefWindowProc( hWndMain, uMessage, wParam, lParam );

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

			// See if notify message is from rich edit window
			if( lpNmHdr->hwndFrom == g_richEditWindow )
			{
				// Notify message is from rich edit window

				// Handle message from rich edit window
				lr = g_richEditWindow.HandleNotifyMessage( hWndMain, wParam, lParam, &RichEditWindowSelectionChangedFunction );

			} // End of notify message is from rich edit window
			else
			{
				// Notify message is not from rich edit window

				// Call default procedure
				lr = ::DefWindowProc( hWndMain, uMessage, wParam, lParam );

			} // End of notify message is not from rich edit window

			// Call default procedure
			lr = ::DefWindowProc( hWndMain, uMessage, wParam, lParam );

			// Break out of switch
			break;

		} // End of a notify message
		case WM_CLIPBOARDUPDATE:
		{
			// A clipboard update message
			BOOL bDoesClipboardContainText;

			// See if clipboard contains text
			bDoesClipboardContainText = ::IsClipboardFormatAvailable( CF_TEXT );

			// Update paste menu item accordingly
			g_contextMenu.EnableItem( IDM_EDIT_PASTE, bDoesClipboardContainText );

			// Break out of switch
			break;

		} // End of a clipboard update message
		case WM_CONTEXTMENU:
		{
			// A context menu message

			// Show context menu
			g_contextMenu.TrackPopupMenu( 0, hWndMain, lParam );

			// Break out of switch
			break;

		} // End of a context menu message
		case WM_CLOSE:
		{
			// A close message
			File file;

			// Create file
			if( file.Create( TEXT_FILE_NAME ) )
			{
				// Successfully opened file
				DWORD dwRichEditWindowTextLength;

				// Get rich edit window text length
				dwRichEditWindowTextLength = g_richEditWindow.GetTextLength();

				// Ensure that rich edit window contains text
				if( dwRichEditWindowTextLength > 0 )
				{
					// Rich edit window contains text
					DWORD dwBufferLength;

					// Calculate buffer length
					dwBufferLength = ( dwRichEditWindowTextLength + sizeof( char ) );

					// Allocate string memory
					LPTSTR lpszRichEditWindowText = new char[ dwBufferLength ];

					// Get rich edit window text
					if( g_richEditWindow.GetText( lpszRichEditWindowText, dwBufferLength ) )
					{
						// Successfully got rich edit window text

						// Write rich edit window test to file
						file.Write( lpszRichEditWindowText, dwRichEditWindowTextLength );

					} // End of successfully got rich edit window text

					// Free string memory
					delete [] lpszRichEditWindowText;

				} // End of rich edit window contains text

				// Close file
				file.Close();

			} // End of successfully opened file

			// Destroy main window
			DestroyWindow( hWndMain );

			// Break out of switch
			break;

		} // End of a close message
		case WM_DESTROY:
		{
			// A destroy message

			// See if main window is listening to clipboard
			if( s_bListening )
			{
				// Main window is listening to clipboard

				// Remove main window from clipboard listeners
				::RemoveClipboardFormatListener( hWndMain );

				// Reset listening value
				s_bListening = FALSE;

			} // End of main window is listening to clipboard

			// Terminate thread
			PostQuitMessage( 0 );

			// Break out of switch
			break;

		} // End of a destroy message
		default:
		{
			// Default message

			// Call default handler
			lr = ::DefWindowProc( hWndMain, uMessage, wParam, lParam );

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
			File file;

			// Get system menu
			systemMenu = mainWindow.GetSystemMenu( FALSE );

			// Add separator item to system menu
			systemMenu.InsertSeparator( MENU_CLASS_SYSTEM_MENU_SEPARATOR_ITEM_POSITION );

			// Add about item to system menu
			systemMenu.InsertItem( MENU_CLASS_SYSTEM_MENU_ABOUT_ITEM_POSITION, MENU_CLASS_SYSTEM_MENU_ABOUT_ITEM_TEXT, IDM_HELP_ABOUT );

			// Load context menu
			g_contextMenu.Load( MAKEINTRESOURCE( IDR_CONTEXT_MENU ) );

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

			// Open file
			if( file.Open( TEXT_FILE_NAME ) )
			{
				// Successfully opened file
				DWORD dwFileSize;
				DWORD dwBufferLength;
				LPTSTR lpszFileText;

				// Get file size
				dwFileSize = file.GetSize();

				// Calculate buffer length
				dwBufferLength = ( dwFileSize + sizeof( char ) );

				// Allocate string memory
				lpszFileText = new char[ dwBufferLength ];

				// Read file text
				if( file.Read( lpszFileText, dwFileSize ) )
				{
					// Successfully read file text

					// Terminate file text
					lpszFileText[ dwFileSize ] = ( char )NULL;

					// Copy file text to rich edit window
					if( g_richEditWindow.SetText( lpszFileText ) )
					{
						// Successfully copied file text to rich edit window

						// Select text on rich edit window
						g_richEditWindow.Select();

					} // End of successfully copied file text to rich edit window

				} // End of successfully read file text

				// Close file
				file.Close();

				// Free string memory
				delete [] lpszFileText;

			} // End of successfully opened file

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
