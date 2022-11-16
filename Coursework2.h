#include <iostream>
#include "DateTime.h"
#include "Items.h"
#include "Headers.h"
#include "ICS0017DataSource.h"
#include<iostream>
#include<fstream>
#include "Coursework1.h"
#include <iomanip>

class DataStructure : CourseWork1
{
private:
	HEADER_D* pointer = new HEADER_D[26]();
	bool CheckItem(char* pID)
	{
		HEADER_D* pHeaderD = pointer;
		HEADER_A* pA;
		ITEM1* pItem;

		while (pHeaderD)
		{
			pA = pHeaderD->pHeaderA;
			while (pA)
			{
				if (pA->pItems)
				{
					pItem = (ITEM1*)pA->pItems;
					while (pItem)
					{
						if (strcmp(pItem->pID, pID) == 0)
						{
							return true;
						}
						pItem = pItem->pNext;
					}
				}
				pA = pA->pNext;
			}
			pHeaderD = pHeaderD->pNext;
		}
		return false;
	}
public:
	DataStructure() {}

	DataStructure(int itemCount) // create data structure filled with GetStruct function
	{
		pointer = GetStruct4(1, itemCount);
	}

	DataStructure(char* pFilename) // create data structure filled with a file
	{
		std::fstream File;
		File.open(pFilename, std::fstream::in | std::fstream::binary | std::fstream::app);
		std::string buf;
		ITEM1* newItem = NULL;
		try
		{
			if (File.good())
			{
				while (getline(File, buf, ';'))
				{
					if (buf == "\n")
					{
						break;
					}
					newItem = new ITEM1;
					char* newID = new char[buf.length() + 1];
					strcpy_s(newID, buf.length() + 1, buf.c_str());
					newItem->pID = newID;
					getline(File, buf, ';');
					newItem->Code = stoul(buf);
					getline(File, buf, ';');
					char* newTime = new char[buf.length() + 1];
					strcpy_s(newTime, buf.length() + 1, buf.c_str());
					newItem->pTime = newTime;
					getline(File, buf, '\n');
					*this += newItem;
				}
			}
			else
			{
				throw std::exception("Could not open file");
			}
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}

	DataStructure(const DataStructure& Original) // create data structure copy
	{
		if (Original.pointer == NULL)
		{
			printf("DS is null and will not be printed\n");
			return;
		}
		HEADER_D* currentHeaderD = Original.pointer;
		HEADER_A* currentHeaderA = nullptr;
		ITEM1* currentItem = nullptr;
		int i = 0;
		while (currentHeaderD)
		{
			currentHeaderA = currentHeaderD->pHeaderA;
			while (currentHeaderA)
			{
				currentItem = (ITEM1*)currentHeaderA->pItems;
				while (currentItem)
				{
					ITEM1* pNewItem = new ITEM1;
					pNewItem->Code = currentItem->Code;
					pNewItem->pID = currentItem->pID;
					pNewItem->pTime = currentItem->pTime;
					pNewItem->pNext = 0;
					*this += pNewItem;
					currentItem = currentItem->pNext;
				}
				currentHeaderA = currentHeaderA->pNext;
			}
			currentHeaderD = currentHeaderD->pNext;
		}
	};

	~DataStructure(void)  // remove datastructure
	{
		HEADER_D* pHeaderD = pointer;
		HEADER_D* pHeaderDprev = NULL;
		while (pHeaderD)
		{
			HEADER_A* pA = pHeaderD->pHeaderA;
			HEADER_A* pAprev = NULL;
			while (pA)
			{
				ITEM1* pItem = (ITEM1*)pA->pItems;
				ITEM1* pItemprev = NULL;
				while (pItem)
				{
					if (pItemprev != NULL)
					{
						delete pItemprev;
					}

					pItemprev = pItem;
					if (!pItem->pNext)
					{
						delete pItem;
						break;
					}
					else
					{
						pItem = pItem->pNext;
					}
				}
				if (pAprev != NULL)
				{
					delete pAprev;
				}
				pAprev = pA;
				if (!pA->pNext)
				{
					delete pA;
					break;
				}
				else
				{
					pA = pA->pNext;
				}
			}
			if (pHeaderDprev != NULL)
			{
				delete pHeaderDprev;
			}
			pHeaderDprev = pHeaderD;
			if (!pHeaderD->pNext)
			{
				delete pHeaderD;
				break;
			}
			else
			{
				pHeaderD = pHeaderD->pNext;
			}
		}
	};

	int GetItemsNumber(void) // return the current number of items in the data structure
	{
		if (!pointer) {

			std::cout << "Data structure does not exist.\n" << std::endl;
			return 0;
		}
		HEADER_D* currentHeaderD = pointer;
		HEADER_A* currentHeaderA = nullptr;
		ITEM1* currentItem = nullptr;
		int itemCount = 0;
		// while loop covering HEADER_D, with a letter of the 1st word
		while (currentHeaderD)
		{
			currentHeaderA = currentHeaderD->pHeaderA;
			// while loop covering HEADER_A, with a letter of the 2nd word
			while (currentHeaderA)
			{
				currentItem = (ITEM1*)currentHeaderA->pItems;
				// while loop covering items
				while (currentItem)
				{
					itemCount += 1;
					currentItem = currentItem->pNext;
				}
				currentHeaderA = currentHeaderA->pNext;
			}
			currentHeaderD = currentHeaderD->pNext;
		}
		return itemCount;
	};

	ITEM1* GetItem(char* pID)  // return pointer to an item with the specified ID
	{
		if (!pointer) {
			std::cout << "Data structure does not exist.\n" << std::endl;
			return 0;
		}
		if (!pID) // ID does not exist

		{
			std::cout << "Passed ID for removal is empty, aborting extraction of an item.\n" << std::endl;
			return 0;
		}

		else // ID exists, check it

		{
			if (CourseWork1::idIsValid(pID) == false)

			{
				std::cout << "Incorrect item ID is passed, aborting extraction of an item.\n" << std::endl;
				return 0;
			}
		}
		HEADER_D* currentHeaderD = pointer;
		HEADER_A* currentHeaderA = nullptr;
		ITEM1* currentItem = nullptr;
		// while loop covering HEADER_D, with a letter of the 1st word
		while (currentHeaderD)
		{
			currentHeaderA = currentHeaderD->pHeaderA;
			// while loop covering HEADER_A, with a letter of the 2nd word
			while (currentHeaderA)
			{
				currentItem = (ITEM1*)currentHeaderA->pItems;
				// while loop covering items
				while (currentItem)
				{
					if (!strcmp(currentItem->pID, pID)) // item with the specified ID exists
					{
						std::cout << "Item retrieved: " << currentItem->pID << " " << currentItem->Code << " " << currentItem->pTime << "\n";
						return currentItem;
					}
					currentItem = currentItem->pNext;
				}
				currentHeaderA = currentHeaderA->pNext;
			}
			currentHeaderD = currentHeaderD->pNext;
		}
		std::cout << "Item with the requested ID does not exist in the data structure. ID: " << pID << std::endl;
		return 0;
	};

	void operator+=(ITEM1* newItemPtr) // add a new item into the data structure
	{
		this->CourseWork1::InsertItem(pointer, newItemPtr->pID);
		return;
	};

	void operator-=(char* pID) // remove an item with the specified ID
	{
		this->CourseWork1::RemoveItem(pointer, pID);
		return;
	};

	DataStructure& operator=(const DataStructure& Right) // assignment
	{
		if (Right.pointer == pointer) return *this;
		if (Right.pointer == nullptr)
		{
			std::cout << "Passed data structure is empty, creating empty structure.\n" << std::endl;
			DataStructure* emptyDS = new DataStructure;
			return *emptyDS;
		}

		HEADER_D* currentHeaderD = pointer;
		HEADER_D* bufferHeaderD = nullptr;
		HEADER_A* currentHeaderA = nullptr;
		HEADER_A* bufferHeaderA = nullptr;
		ITEM1* currentItem = nullptr;
		ITEM1* bufferItem = nullptr;

		// remove items if the initial data structure is not empty
		// while loop covering HEADER_D, with a letter of the 1st word
		while (currentHeaderD)
		{
			bufferHeaderD = currentHeaderD->pNext;
			currentHeaderA = currentHeaderD->pHeaderA;
			// while loop covering HEADER_A, with a letter of the 2nd word
			while (currentHeaderA)
			{
				bufferHeaderA = currentHeaderA->pNext;
				currentItem = (ITEM1*)currentHeaderA->pItems;
				// while loop covering items
				while (currentItem)
				{
					bufferItem = currentItem->pNext;
					*this -= currentItem->pID;
					currentItem = bufferItem;
				}
				currentHeaderA = bufferHeaderA;
			}
			currentHeaderD = bufferHeaderD;
		}
		currentHeaderD = Right.pointer;
		
		// copy items
		// while loop covering HEADER_D, with a letter of the 1st word
		while (currentHeaderD)
		{
			currentHeaderA = currentHeaderD->pHeaderA;
			// while loop covering HEADER_A, with a letter of the 2nd word
			while (currentHeaderA)
			{
				currentItem = (ITEM1*)currentHeaderA->pItems;
				// while loop covering items
				while (currentItem)
				{
					char* duplicateID = new char[strlen(currentItem->pID) + 1];
					strcpy_s(duplicateID, strlen(currentItem->pID) + 1, currentItem->pID);
					char* duplicateTime = new char[strlen(currentItem->pTime) + 1];
					strcpy_s(duplicateTime, strlen(currentItem->pTime) + 1, currentItem->pTime);
					ITEM1* duplicateItem = new ITEM1;
					duplicateItem->pID = duplicateID;
					duplicateItem->Code = currentItem->Code;
					duplicateItem->pTime = duplicateTime;
					duplicateItem->pNext = nullptr;
					*this += duplicateItem;
					currentItem = currentItem->pNext;
				}
				currentHeaderA = currentHeaderA->pNext;
			}
			currentHeaderD = currentHeaderD->pNext;
		}
		return *this;
	};

	int operator==(DataStructure& Other) // comparison, returns 0 for no equality, 1 for equality
	{
		if (GetItemsNumber() == Other.GetItemsNumber())
		{
			HEADER_D* pHeaderD = pointer;
			HEADER_A* pA;
			ITEM1* pItem;
			while (pHeaderD)
			{
				pA = pHeaderD->pHeaderA;
				while (pA)
				{
					if (pA->pItems)
					{
						pItem = (ITEM1*)pA->pItems;
						while (pItem)
						{
							if (!Other.CheckItem(pItem->pID))
							{
								return 0;
							}
							pItem = pItem->pNext;
						}
					}
					pA = pA->pNext;
				}
				pHeaderD = pHeaderD->pNext;
			}
			return 1;
		}
		else
		{
			return 0;
		}
	};

	void Write(char* pFilename) // serialize data structure and write it  into the specified file
	{
		std::ofstream File;
		File.open(pFilename, std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);
		if (File.good())
		{
			HEADER_D* pHeaderD = pointer;
			while (pHeaderD)
			{
				HEADER_A* pA = pHeaderD->pHeaderA;
				while (pA)
				{
					ITEM1* pItem = (ITEM1*)pA->pItems;
					while (pItem)
					{
						File << pItem->pID << ";" << pItem->Code << ";" << pItem->pTime << ";\n";
						pItem = pItem->pNext;
					}
					pA = pA->pNext;
				}
				pHeaderD = pHeaderD->pNext;
			}
		}
		else
		{
			std::cout << "Could not open file\n";
		}
		File.close();
	};

	friend std::ostream& operator<<(std::ostream& ostr, const DataStructure& str) // prints all the items into the command prompt
	{
		try
		{
			if (str.pointer == NULL)
			{
				throw std::exception("DS is null and will not be printed\n");
			}
			else 
			{
				HEADER_D* currentHeaderD = str.pointer;
				HEADER_A* currentHeaderA = nullptr;
				ITEM1* currentItem = nullptr;
				int i = 0;
				while (currentHeaderD)
				{
					currentHeaderA = currentHeaderD->pHeaderA;
					while (currentHeaderA)
					{
						currentItem = (ITEM1*)currentHeaderA->pItems;
						while (currentItem)
						{
							ostr << ++i << ") " << currentItem->pID << " " << currentItem->Code << " " << currentItem->pTime << "\n";
							currentItem = currentItem->pNext;
						}
						currentHeaderA = currentHeaderA->pNext;
					}
					currentHeaderD = currentHeaderD->pNext;
				}
				return ostr;
			}
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	};
};

