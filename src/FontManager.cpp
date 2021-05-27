#include "header.h"

#include "FontManager.h"

/*
Grow an array - has to be a basic C-type array of pointers, where the values can just be copied.
Example usage:
	if ( (m_iFontCount+1) > m_iArraySize )
		m_pArrayFonts = GrowArray( m_pArrayFonts, m_iArraySize, 10 );
This is used by the framework code where basic arrays are being used rather than classes.
Note that the size is passed by REFERENCE so will be changed.
A pointer to the new array is returned so will need to be stored.
The old array is deleted.

The main purpose of having this code here is so that you can see an example of a template function which is used.
This is currently only used in the FontManager.cpp file, but used to be used in more places until I rewrote a lot of the code.
*/
template <class T>
T* growArrayOfObjects(T* pOldArray, int& iSize, int iGrowBy)
{
	T* pNewArray = new T[iSize + iGrowBy];
	if (pOldArray != nullptr)
	{
		for (int i = 0; i < iSize; i++)
		{
			pNewArray[i] = pOldArray[i];
		}
		delete[] pOldArray;
	}
	iSize += iGrowBy;
	return pNewArray;
}




// Constructor just initialises everything then creates a 20 element array for fonts to be stored in.
FontManager::FontManager(void)
: m_iFontCount(0)
, m_pArrayFonts(NULL)
, m_iArraySize(0)
, m_bInitialised(false)
{
	// Hard-coded to size 20 initially, uses GrowArray later
	m_iArraySize = 20;
	m_pArrayFonts = new Font*[ m_iArraySize ];
}

// Destructor destroys the array of fonts and each font object.
FontManager::~FontManager(void)
{
	for ( int i = 0; i < m_iFontCount ; i++ )
		delete m_pArrayFonts[i];
	delete [] m_pArrayFonts;
}

/* 
Get pointer to a (possibly new) font object. FontManager retains ownership and will destroy it for you!!!! 
If you call this before LoadFonts() then it just stores the details and then loads them all later when you call LoadFonts.
If you call this after calling LoadFonts() then it will load the font immediately.
The purpose of caching all of these in the font manager is to avoid having to keep loading files while the program is running.
*/
Font* FontManager::getFont(const char* szFontName, int iFontSize)
{
	for ( int i = 0 ; i < m_iFontCount ; i++ )
		if ( (m_pArrayFonts[i]->getSize() == iFontSize)
				&& strcmp(m_pArrayFonts[i]->getName(), szFontName ) == 0 )
			return m_pArrayFonts[i];
	// Otherwise font is a new one so go through the process of loading it

	// If array not big enough then grow it by 10 elements. This is a template function - worth a look at maybe when you get to template functions
	if ( (m_iFontCount+1) > m_iArraySize )
		m_pArrayFonts = growArrayOfObjects( m_pArrayFonts, m_iArraySize, 10 );

	// Array is now big enough
	printf( "Loading font: %d point %s\n", iFontSize, szFontName );
	m_pArrayFonts[m_iFontCount++] = new Font( szFontName,iFontSize );
	if ( m_bInitialised ) // Fonts already loaded, so load this one too
	{
		m_pArrayFonts[m_iFontCount - 1]->m_pFont = TTF_OpenFont( szFontName,iFontSize );
	}

	// Output a warning message if font load failed
	if ( m_pArrayFonts[m_iFontCount - 1]->m_pFont == NULL )
	{
		printf( "Font loading failed.\n" );
	}

	return m_pArrayFonts[m_iFontCount-1];
}

/** Load all of the fonts which have been selected */
void FontManager::loadFonts()
{
	m_bInitialised = true; // So that future ones get loaded

	for ( int i = 0 ; i < m_iFontCount ; i++ )
	{
		Font* pFont = m_pArrayFonts[i];
		if ( pFont->getTTFFont() == NULL )
			pFont->m_pFont = TTF_OpenFont( pFont->getName(), pFont->getSize() );
	}
}


