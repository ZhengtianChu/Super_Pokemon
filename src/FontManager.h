#pragma once
// Contains classes to manage fonts.


// Font object - you should not need to worry about this since the FontManager will do the work for you.
// See FontManager later in this file
class Font
{
public:
	// Constructor - takes a TTF font object and a font name and point size
	// The font will be loaded by the FontManager later - this just stores the details.
	Font(const char* szFontName, int iSize)
		: m_pFont(NULL)
		, m_iFontSize(iSize)
	{
		m_szFontName = new char[ strlen(szFontName)+1 ];
		strcpy( m_szFontName, szFontName );
	}


	// Destructor - releases the SDL font object
	~Font()
	{
		if ( m_pFont != NULL )
		{
			printf( "Free font: %d point %s\n", m_iFontSize, m_szFontName );
			TTF_CloseFont( m_pFont );
		}
		if ( m_szFontName != NULL )
		{
			delete[] m_szFontName;
		}
	}

	// Get the SDL font object for this font - will be used for drawing it.
	TTF_Font* getTTFFont() { return m_pFont; }

	/* Get the name of the font. */
	const char* getName() { return m_szFontName; }
	/* Get the point size of the font */
	int getSize() { return m_iFontSize; }

	// This is used to allow the FontManager to set the m_pFont but nothing else to do so.
	friend class FontManager;

protected:
	// Pointer to the SDL font object
	TTF_Font* m_pFont;
	// Name of the font file
	char* m_szFontName;
	// Point size of the font
	int m_iFontSize;
};


// Manage the fonts - handles loading, maintaining an array of fonts and retrieving them.
class FontManager
{
public:
	/* Constructor just initialises everything then creates a 20 element array for fonts to be stored in.
	The array will be grown in size when needed.
	*/
	FontManager( void );

	// Destructor destroys the array of fonts and each font object in the array.
	~FontManager( void );

	/* Retrieve a font object by name and point size.
	Gets a pointer to a (possibly new) font object. 
	FontManager retains ownership and will destroy it for you!!!!
	If you call this before LoadFonts() then it just stores the details and then loads them all later when you call LoadFonts.
	If you call this after calling LoadFonts() then it will load the font immediately.
	The purpose of caching all of these in the font manager is to avoid having to keep loading files while the program is running.
	*/
	Font* getFont( const char* szFontName, int iFontSize );

	/* Load all of the fonts which have been selected so far. */
	void loadFonts();

protected:
	// Number of fonts stored so far
	int m_iFontCount;
	// Size of the font array.
	int m_iArraySize;
	// Array of font pointers
	Font** m_pArrayFonts;
	// Initially false, but set to true when LoadFonts() has been called to actually load the selected fonts.
	bool m_bInitialised;
};

