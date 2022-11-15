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
	HEADER_D* pointer = new HEADER_D [26]();
	bool CheckItem(char* pID)
	{
		HEADER_D* pB = pointer;
		HEADER_A* pA;
		ITEM1* pItem;

		while (pB)
		{
			pA = pB->pHeaderA;
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
			pB = pB->pNext;
		}
		return false;
	}
public:
	DataStructure(){}

	DataStructure(int itemCount) // create data structure filled with GetStruct function
	{
		pointer = GetStruct4(1, itemCount);
	}

	DataStructure(char* pFilename) // create data structure filled with a file
	{
		if (!pFilename) // filename is empty
		{
			std::cout << "Filename does not exist." << std::endl;
			return;
		}
		FILE* pFile = fopen(pFilename, "rb");
		char* pData;
		long lSize;
		if (pFile) {
			fseek(pFile, 0, SEEK_END);
			lSize = ftell(pFile);
			rewind(pFile);
			pData = (char*)malloc(lSize);
			int n = fread(pData, 1, lSize, pFile);
			fclose(pFile);
		}
		else 
		{
			std::cout << "Error reading file" << std::endl;
			return;
		}
		char* cursor;
		int totalItems = 0;
		memcpy(&totalItems, cursor = pData, sizeof(int));
		cursor += sizeof(int);
		for (int i = 0; i < totalItems; i++) {
			int pIdSize = 0;
			int pTimeSize = 0;
			char* pID;
			unsigned long int code = 0;
			char* pTime;
			memcpy(&pIdSize, cursor, sizeof(int));
			pID = (char*)malloc(pIdSize * sizeof(char));
			memcpy(pID, cursor += sizeof(int), pIdSize);
			memcpy(&code, cursor += pIdSize, sizeof(unsigned long int));
			memcpy(&pTimeSize, cursor += pTimeSize, sizeof(char));
			pTime = (char*)malloc(pTimeSize * sizeof(char));
			memcpy(pTime, cursor += sizeof(int), pTimeSize);
			cursor += sizeof(int);
			ITEM1* p = CourseWork1::CreateItem1(pID);
			p->pTime = pTime;
			p->Code = code;
			p->pID = pID;
			*this += p;
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
		HEADER_D* pB = pointer;
		HEADER_D* pBprev = NULL;

		while (pB)
		{
			HEADER_A* pA = pB->pHeaderA;
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
			if (pBprev != NULL)
			{
				delete pBprev;
			}

			pBprev = pB;
			if (!pB->pNext)
			{
				delete pB;
				break;
			}
			else
			{
				pB = pB->pNext;
			}
		}
	};

	int GetItemsNumber(void) // return the current number of items in the data structure
	{
		if (!pointer) {

			std::cout << "Data structure does not exist." << std::endl;

			return 0;

		}


		HEADER_D* currentPositionBPtr = pointer;

		HEADER_A* currentPositionAPtr = nullptr;

		ITEM1* currentPositionItemPtr = nullptr;

		int itemCount = 0;


		// while loop covering HEADER_B, with a letter of the 1st word

		while (currentPositionBPtr)

		{

			currentPositionAPtr = currentPositionBPtr->pHeaderA;

			// while loop covering HEADER_A, with a letter of the 2nd word

			while (currentPositionAPtr)

			{

				currentPositionItemPtr = (ITEM1*)currentPositionAPtr->pItems;

				// while loop covering items

				while (currentPositionItemPtr)

				{

					itemCount += 1;

					currentPositionItemPtr = currentPositionItemPtr->pNext;

				}

				currentPositionAPtr = currentPositionAPtr->pNext;

			}

			currentPositionBPtr = currentPositionBPtr->pNext;

		}

		return itemCount;

	}; 

	ITEM1* GetItem(char* pID)  // return pointer to an item with the specified ID
	{
		if (!pointer) {
			std::cout << "Data structure does not exist. (DataStructure::GetItem)" << std::endl;
			return 0;
		}


		if (!pID) // ID does not exist

		{
			std::cout << "Passed ID for removal is empty, aborting extraction of an item. (DataStructure::GetItem)" << std::endl;
			return 0;
		}

		else // ID exists, check it

		{

			if (CourseWork1::idIsValid(pID) == false)

			{
				std::cout << "Incorrect item ID is passed, aborting extraction of an item. (DataStructure::GetItem)" << std::endl;

				return 0;
			}

		}


		HEADER_D* currentPositionBPtr = pointer;

		HEADER_A* currentPositionAPtr = nullptr;

		ITEM1* currentPositionItemPtr = nullptr;


		// while loop covering HEADER_B, with a letter of the 1st word

		while (currentPositionBPtr)

		{

			currentPositionAPtr = currentPositionBPtr->pHeaderA;

			// while loop covering HEADER_A, with a letter of the 2nd word

			while (currentPositionAPtr)

			{

				currentPositionItemPtr = (ITEM1*)currentPositionAPtr->pItems;

				// while loop covering items

				while (currentPositionItemPtr)

				{

					if (!strcmp(currentPositionItemPtr->pID, pID)) // item with the specified ID exists

					{
						std::cout << "Item retrieved: " << currentPositionItemPtr->pID << " " << currentPositionItemPtr->Code << " " << currentPositionItemPtr->pTime << "\n";
						return currentPositionItemPtr;

					}

					currentPositionItemPtr = currentPositionItemPtr->pNext;

				}

				currentPositionAPtr = currentPositionAPtr->pNext;

			}

			currentPositionBPtr = currentPositionBPtr->pNext;

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
		DataStructure::~DataStructure();
		DataStructure::DataStructure(Right);
		return *this;
	}; 

	int operator==(DataStructure& Other) // comparison, returns 0 for no equality, 1 for equality
	{
		if (GetItemsNumber() == Other.GetItemsNumber())
		{
			HEADER_D* pB = pointer;
			HEADER_A* pA;
			ITEM1* pItem;

			while (pB)
			{
				pA = pB->pHeaderA;
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
				pB = pB->pNext;
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
			HEADER_D* pB = pointer;
			while (pB)
			{
				HEADER_A* pA = pB->pHeaderA;

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

				pB = pB->pNext;
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
		HEADER_D* pB = str.pointer;
		const char separator = ' ';
		const int nameWidth = 30;
		const int numWidth = 20;

		int counter = 1;
		while (pB)
		{
			HEADER_A* pA = pB->pHeaderA;

			while (pA)
			{
				ITEM1* res = (ITEM1*)pA->pItems;
				while (res)
				{
					if (counter < 10) { std::cout << "0"; }
					std::cout << counter << ") " << std::left << std::setw(nameWidth) << std::setfill(separator) << res->pID << std::setw(numWidth) << std::setfill(separator) << res->Code << std::setw(numWidth) << std::setfill(separator) << res->pTime << std::endl;
					counter++;

					res = res->pNext;
				}
				pA = pA->pNext;
			}

			pB = pB->pNext;
		}
		return std::cout;
	}; 
};

