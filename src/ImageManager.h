#pragma once

#include <map>
#include <string>
#include <memory>

#include "SimpleImage.h"

/*
The ImageManager manages loaded images. The main aim is that it avoid re-loading images which are already loaded, sharing them between objects.
Images are loaded as RawImageData objects.
SimpleImage and ImageRenderer are front ends to the image data, specifying which pixels to draw from that image.
Uses smart pointers and maps to maintain the list, so may be worth looking at for that reason?
*/
class ImageManager
{
	// Store the data for the images, by URL loaded from
	std::map<std::string, std::weak_ptr<RawImageData> > images;
	// These images are kept permanently loaded
	std::map<std::string, std::shared_ptr<RawImageData> > permanentlyLoadedImages;

public:
	// Singleton implementation, if desired, use static get() to get the singleton object.
	static ImageManager* get()
	{
		if (g_pManager == nullptr)
			g_pManager = new ImageManager();
		return g_pManager;
	}

	// Load an image into memory, and return a SimpleImage object which uses it
	// imageURL : the URL of the file to load
	// loadPermanently : keep the image in memory even when you stop using it in this image
	static SimpleImage loadImage(std::string imageURL, bool loadPermanently = false)
	{
		return ImageManager::get()->getImagebyURL(imageURL, true/*Load*/, loadPermanently/*Keep it loaded?*/);
	}

	/*
	Destroy the image manager object - note that this will destroy its maps which it contains
	Which will destroy the objects contained.
	*/
	static void destroyImageManager()
	{
		if (g_pManager == nullptr)
			return;
		delete g_pManager;
		g_pManager = nullptr;
	}

private:
	/* Singleton ImageManager object pointer. */
	static ImageManager* g_pManager;

public:

	// Constructor - just initialise the variables
	ImageManager()
	{
	}


	// Retrieve an image by its URL. If already loaded then it gets the existing one
	std::shared_ptr<RawImageData> getImagePtrbyURL(std::string imageURL, bool loadIfNotLoaded = false, bool loadPermanently = false)
	{
		//std::cout << "\t\t+++ Find image: " << imageURL << std::endl;

		auto iter = images.find(imageURL);
		if (iter != images.end())
		{
			auto weak = iter->second; // Pull out pointer rather than key
			if (weak.expired())
			{
				// Remove the expired image from the list
				images.erase(iter);
				// And then carry on, to load it again
			}
			else
			{
				//std::cout << "\t\t+++ Find image: " << imageURL << " image found" << std::endl;

				auto shared = weak.lock(); // Convert to a shared pointer, which we are returning
				return shared; // Iterator points at the shared pointer
			}
		}

		// Otherwise either load it or return a null pointer
		if (loadIfNotLoaded)
		{
			std::cout << "\t\t+++ Load image: " << imageURL << std::endl;

			auto newSharedPtr = loadAsNew(imageURL); // Load image from file
			if (newSharedPtr != nullptr)
			{
				std::weak_ptr<RawImageData> weak = newSharedPtr; // Shared to weak pointer
				images.insert(std::pair<std::string, std::weak_ptr<RawImageData>>(imageURL, weak));
				if (loadPermanently) // Storing shared pointer keeps it alive while array exists - forever.
					permanentlyLoadedImages.insert(std::pair<std::string, std::shared_ptr<RawImageData>>(imageURL, newSharedPtr));
			}
			else
			{
				std::cout << "ImageManager::getImagePtrbyURL() : Failed to load image " << imageURL << std::endl;
			}
			return newSharedPtr; // Return the loading attempt result, whether successful or not
		}
		return nullptr; // failed
	}


	// Create a new ImageData object to represent the image and allow manipulation and rendering
	SimpleImage getImagebyURL(std::string imageURL, bool loadIfNotLoaded = false, bool loadPermanently = false)
	{
		return SimpleImage(getImagePtrbyURL(imageURL, loadIfNotLoaded, loadPermanently));
	}



	/*
	Specify whether an image should be permanently loaded or not.
	Keeps a shared pointer to the image data, which will keep it alive
	imageURL : the url of the image file
	makePermanent : true to keep it loaded, false to unload it as soon as nothing is actively using it
	*/
	bool setPermanentlyLoaded(std::string imageURL, bool makePermanent = true)
	{
		if (makePermanent == false)
		{	// Take item out of permanentlyLoadedImages
			auto iter = permanentlyLoadedImages.find(imageURL);
			if (iter != permanentlyLoadedImages.end())
			{
				permanentlyLoadedImages.erase(iter);
				return true;
			}
			return false; // Can't remove because not in there
		}
		else
		{	// Find item in normal array and add to permanently loaded images
			// If already in permanentlyLoadedImages then nothing to do.
			auto iter = permanentlyLoadedImages.find(imageURL);
			if (iter != permanentlyLoadedImages.end())
				return true;
			// Otherwise look for the image...
			auto iter2 = images.find(imageURL);
			if (iter2 != images.end())
			{
				permanentlyLoadedImages.insert(std::pair<std::string, std::shared_ptr<RawImageData>>(iter2->first, iter2->second.lock()));
				return true;
			}
			return false;
		}
	}

	// Load the image data into a NEW RawImageData object and return the shared pointer to it
	std::shared_ptr<RawImageData> loadAsNew(std::string imageURL)
	{
		int cropLeft = 0, cropRight = 0, cropTop = 0, cropBottom = 0;
		RawImageData* pData = new RawImageData(imageURL);
		if (pData->loadImage(imageURL, 0, cropTop, cropBottom, cropLeft, cropRight))
		{
			return std::shared_ptr<RawImageData>(pData);
		}
		printf( "Failed to load image %s\n", imageURL.c_str() );
		delete pData; // Failed so no longer needed
		return nullptr;
	}
};

