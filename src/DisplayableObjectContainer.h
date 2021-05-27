#pragma once

// This implements a container of displayable objects.
// BaseEngine subclasses this, because it has a set of displayable objects to display.
// Other classes could also subclass it to store a set of objects in a consistent manner and take advantage of things like the notify system.
// Example: you could use it for a deck of cards if cards were displayable objects.

#include "header.h"
#include <vector>
#include <algorithm>
#include <functional>

#include "DisplayableObject.h"

class DisplayableObjectContainer
{
public:
	// Constructor
	DisplayableObjectContainer( int iInitialSize = 0 )
		: m_bDrawableObjectsChanged(false),
		m_vecDisplayableObjects(iInitialSize),
		m_iCurrentItemIndex(0)
	{
	}

	virtual ~DisplayableObjectContainer()
	{
		destroyOldObjects(true);
	}


public:
	/* Create array to store objects - set the array size to be big enough to use */
	void createObjectArray(int iNumberObjects) 
	{ 
		destroyOldObjects(true);  
		m_vecDisplayableObjects.resize(iNumberObjects); 
		drawableObjectsChanged();
		m_iCurrentItemIndex = 0;
	}

	/* Store an object into an array element */
	void storeObjectInArray(int iIndex, DisplayableObject* pObject) 
	{ 
		drawableObjectsChanged(); 
		m_vecDisplayableObjects[iIndex] = pObject; 
		m_iCurrentItemIndex = 0;
	}

	// Clear all contents, without deleting objects
	void clearContents()
	{
		m_vecDisplayableObjects.clear();
		m_vecDisplayableObjects.resize(0);
		m_iCurrentItemIndex = 0;
	}

	// Get a count of how many things are in container
	int getContentCount() const { return m_vecDisplayableObjects.size();  }

	// Get a count of how many things are in container
	DisplayableObject* getContentItem( int iIndex ) const { return m_vecDisplayableObjects[iIndex]; }

	/*
	Gets the specified displayable object by index.
	Note: You may need to dynamic_cast the resulting pointer to the correct type since you get a base class pointer back.
	*/
	DisplayableObject* getDisplayableObject(int iIndex) const
	{ 
		if (iIndex >= m_vecDisplayableObjects.size()) 
			return nullptr; 
		return m_vecDisplayableObjects[iIndex]; 
	}

	/* Store an object into an array element */
	void appendObjectToArray(DisplayableObject* pObject) 
	{ 
		drawableObjectsChanged(); 
		m_vecDisplayableObjects.push_back(pObject); 
		m_iCurrentItemIndex = 0;
	}

	/* Destroy current contents of the array */
	virtual void destroyOldObjects( bool dDelete )
	{
		drawableObjectsChanged();
		for (int i = 0; i < m_vecDisplayableObjects.size(); i++)
		{
			DisplayableObject* pOb = m_vecDisplayableObjects[i];
			m_vecDisplayableObjects[i] = nullptr;
			if (pOb != nullptr)
				if (dDelete && pOb->deleteOnRemoval())
					delete pOb;
		}
		m_iCurrentItemIndex = 0;
	}

	/* Move the specified displayable object to the end of the list, so it appears on top */
	bool moveToLast(DisplayableObject* pObject)
	{
		m_iCurrentItemIndex = 0;
		drawableObjectsChanged();
		auto it = std::find(m_vecDisplayableObjects.begin(), m_vecDisplayableObjects.end(), pObject);
		if (it != m_vecDisplayableObjects.end())
		{ // Found the pointer
			m_vecDisplayableObjects.erase(it); // Remove from current position
			m_vecDisplayableObjects.push_back(pObject); // Add at the end
			pObject->redrawRectangle(); // Ensure that object is redrawn in its new position, in case we are doing partial updates now
			return true;
		}
		return false; // Not found
	}

	/* Removes an object from the displayable object array - does NOT delete it */
	bool removeDisplayableObject( DisplayableObject* pObject)
	{
		m_iCurrentItemIndex = 0;
		drawableObjectsChanged();
		auto it = std::find(m_vecDisplayableObjects.begin(), m_vecDisplayableObjects.end(), pObject);
		if (it != m_vecDisplayableObjects.end())
		{ // Found the pointer
			m_vecDisplayableObjects.erase(it); // Remove from current position
			return true;
		}
		return false;
	}



	/*
	Called when the drawable objects array is changed, to avoid using the array after it has changed.
	The problem is that we may use an invalid pointer if things change while an array is being iterated through
	Basically, some functions may need to know if the thing they are looking at has already been processed.
	These functions first set this variable false, then keep checking whether something has set it to true - in which case they abort immediately.
	If you remove a drawable object or change the array contents then call this method to tell the framework that you did so.
	*/
	void drawableObjectsChanged()
	{
		m_bDrawableObjectsChanged = true;
	}



	// Get a count of the number of non-null valid entries in array
	int getNonNullObjectContentCount() const
	{
		int iReturn = 0;
		std::for_each(m_vecDisplayableObjects.begin(), m_vecDisplayableObjects.end(), [&iReturn](DisplayableObject* pdo) { if (pdo != nullptr) ++iReturn; });
		return iReturn;
	}


	/*
	The notify system provides a simple way to tell all objects about something and potentially to get a return value and report on it.
	When notifyAllObjects...() is called it will send the specified signal number to each object, which can then respond with an integer response value.
	The signal code is entirely defined by you - you can use whatever numbers you want.
	What the return code is used for will depend upon which function is used. E.g. it may be ignored, or the sum, maximum or minimum values may be returned to the caller.
	i.e.: Implement the notify function in each drawable object to do something and return a result code, and then call notifyAllObjects to have the function called with a specified parameter value.
	There are variants which not only call all objects, but also sum, get min or get max results codes returned.
	*/

	/* Send a specified notification value to all displayable objects. */
	void notifyAllObjects(int iSignalNumber, int x = 0, int y = 0) const
	{
		std::for_each(m_vecDisplayableObjects.begin(), m_vecDisplayableObjects.end(), [=](DisplayableObject* pdo) { if (pdo == nullptr) return; pdo->virtNotify(iSignalNumber, x, y); });
	}

	/* Send a specified notification value to all displayable objects and count the number which give a non-zero response. */
	int notifyAllObjectsGetCountNonZero(int iSignalNumber, int x = 0, int y = 0) const
	{
		int iReturn = 0;
		std::for_each(m_vecDisplayableObjects.begin(), m_vecDisplayableObjects.end(), [=, &iReturn](DisplayableObject* pdo) { if (pdo == nullptr) return; if (pdo->virtNotify(iSignalNumber, x, y) != 0) iReturn++; });
		return iReturn;
	}

	/* Send a specified notification value to all displayable objects and return the sum of the returned values. */
	int notifyAllObjectsGetSum(int iSignalNumber, int x = 0, int y = 0) const
	{
		int iReturn = 0;
		std::for_each(m_vecDisplayableObjects.begin(), m_vecDisplayableObjects.end(), [=, &iReturn](DisplayableObject* pdo) { if (pdo == nullptr) return; iReturn += pdo->virtNotify(iSignalNumber, x, y); });
		return iReturn;
	}

	/* Send a specified notification value to all displayable objects and return the largest of the returned values. */
	int notifyAllObjectsGetMax(int iSignalNumber, int x = 0, int y = 0) const
	{
		int iReturn = INT_MIN;
		std::for_each(m_vecDisplayableObjects.begin(), m_vecDisplayableObjects.end(), [=, &iReturn](DisplayableObject* pdo) { if (pdo == nullptr) return; int ival = pdo->virtNotify(iSignalNumber, x, y); if (ival > iReturn) iReturn = ival; });
		return iReturn;
	}

	/* Send a specified notification value to all displayable objects and return the smallest of the returned values. */
	int notifyAllObjectsGetMin(int iSignalNumber, int x = 0, int y = 0) const
	{
		int iReturn = INT_MAX;
		std::for_each(m_vecDisplayableObjects.begin(), m_vecDisplayableObjects.end(), [=, &iReturn](DisplayableObject* pdo) { if (pdo == nullptr) return; int ival = pdo->virtNotify(iSignalNumber, x, y); if (ival < iReturn) iReturn = ival; });
		return iReturn;
	}

	/* Send a specified notification value to all displayable objects and return the smallest of the returned values. */
	void setAllObjectsVisible(bool bVisible) const
	{
		for (auto it = m_vecDisplayableObjects.begin(); it != m_vecDisplayableObjects.end(); it++)
			if (*it)
				(*it)->setVisible(bVisible);
	}

	/* Return the first object which gives the specified result from the notification message */
	DisplayableObject* getBottomObjectForNotifyValue(int iSignalNumber, int iValue, int x, int y) const
	{
		for (int i = 0; i < m_vecDisplayableObjects.size(); i++)
			if (m_vecDisplayableObjects[i])
				if (m_vecDisplayableObjects[i]->virtNotify(iSignalNumber, x, y) == iValue)
					return m_vecDisplayableObjects[i];
		return nullptr;
	}

	/* Return the top (last in array) object which gives the specified result from the notification message */
	DisplayableObject* getTopObjectForNotifyValue(int iSignalNumber, int iValue, int x, int y) const
	{
		for (int i = m_vecDisplayableObjects.size() - 1; i >= 0; i--)
			if (m_vecDisplayableObjects[i])
				if (m_vecDisplayableObjects[i]->virtNotify(iSignalNumber, x, y) == iValue)
					return m_vecDisplayableObjects[i];
		return nullptr;
	}

	// Call function/lambda f for each displayable object pointer - note that pointers could be NULL!
	void applyToAll(std::function<void(DisplayableObject* ob)> f) const
	{
		std::for_each(m_vecDisplayableObjects.begin(), m_vecDisplayableObjects.end(), f);
	}


	/* Shuffle the contents into a random order - useful for a deck of cards */
	void shuffle()
	{
		std::random_shuffle(m_vecDisplayableObjects.begin(), m_vecDisplayableObjects.end());
	}

	//
	// Manual iteration functions, named bookmark
	//
	// These implement a way that you can allow a user to work through the array, and keep a track of where they are
	// e.g. for a deck of cards they could look through it one by one and advance or go back.
	// NOTE: bookmark is cleared if items are removed/added to array!
	// Feel free to implement an iterator instead if you want a more standard way to do this.
	//

	// Get the object for the current bookmark 
	DisplayableObject* bookmarkGetCurrentObject()
	{
		if (m_iCurrentItemIndex >= m_vecDisplayableObjects.size())
			return nullptr;
		if (m_iCurrentItemIndex < 0)
			return nullptr;
		return m_vecDisplayableObjects[m_iCurrentItemIndex];
	}

	/*
	Get the index position for the current bookmark
	Sets bookmark to 0 if it was on an invalid item
	*/
	int bookmarkGetCurrentIndex()
	{
		if (m_iCurrentItemIndex >= m_vecDisplayableObjects.size())
			m_iCurrentItemIndex = 0;
		if (m_iCurrentItemIndex < 0)
			m_iCurrentItemIndex = 0;
		return m_iCurrentItemIndex;
	}

	/*
	Go to next item in the stored array
	*/
	bool bookmarkIncrement()
	{
		if (m_iCurrentItemIndex >= m_vecDisplayableObjects.size()-1)
		{
			m_iCurrentItemIndex = 0;
			return false;
		}

		++m_iCurrentItemIndex;
		if (m_vecDisplayableObjects[m_iCurrentItemIndex] == nullptr)
			return bookmarkIncrement(); // Skip nullptr
		return true; // Found a valid card
	}

	/*
	Go to previous item in the stored array
	*/
	bool bookmarkDecrement()
	{
		if (m_iCurrentItemIndex <= 0)
		{
			m_iCurrentItemIndex = 0;
			return false;
		}

		--m_iCurrentItemIndex;
		if (m_vecDisplayableObjects[m_iCurrentItemIndex] == nullptr)
			return bookmarkDecrement(); // Skip nullptr
		return true; // Found a valid card
	}
	
	/*
	Go to a specified index in the stored array
	*/
	void bookmarkSet(int iIndex) { m_iCurrentItemIndex = iIndex; }

	/* Go to the last index in the stored array */
	bool bookmarkSetToLast()
	{
		m_iCurrentItemIndex = m_vecDisplayableObjects.size() - 1;
		return m_vecDisplayableObjects.size() >= 1; 
	}




protected:
	// Set to 1 if drawable objects changed so any update should abort
	bool m_bDrawableObjectsChanged;

	/* Array of displayable objects - expect it to be modified by sub-class(es) */
	std::vector<DisplayableObject*> m_vecDisplayableObjects;

	// Current index if iterating through the items
	int m_iCurrentItemIndex;
};
