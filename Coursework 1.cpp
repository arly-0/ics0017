#include <iostream>
#include "DateTime.h"
#include "Items.h"
#include "Headers.h"
#include "ICS0017DataSource.h"
#include <string>

#pragma warning(disable : 4996)

void PrintDataStructure(HEADER_D* pointer)
{
	if (pointer == NULL)
	{
		printf("DS is null and will not be printed\n");
		return;
	}
	HEADER_D* currentHeaderD = pointer;
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
				printf("%d)%s %lu %s \n", ++i, currentItem->pID, currentItem->Code, currentItem->pTime);
				currentItem = currentItem->pNext;
			}
			currentHeaderA = currentHeaderA->pNext;
		}
		currentHeaderD = currentHeaderD->pNext;
	}
}

bool wordIsValid(char* p, int len)
{
	for (int i = 0; i < len; i++)
	{
		int ascii = (int)*(p + i);
		if (!((ascii >= 65 || ascii <= 90) || (ascii >= 97 || ascii <= 122) || ascii == 45)) return true;
	}
	return false;
}

bool idIsValid(char* pNewItemID)
{
	if (strlen(pNewItemID) < 3) return false;
	char* spacePos = strchr(pNewItemID, ' ');
	if (spacePos == 0 || strchr(spacePos + 1, ' ') != 0) return false;
	char first = *pNewItemID;
	char second = *(spacePos + 1);
	if ((int)first < 65 || (int)first > 90 || (int)second < 65 || (int)second > 90) return false;
	int lenFirst = spacePos - pNewItemID;
	int lenSecond = strlen(pNewItemID) - lenFirst - 1;
	if (wordIsValid(pNewItemID, lenFirst) || wordIsValid(spacePos + 1, lenSecond)) return false;
	return true;
}

HEADER_A* FillNewHeaderA(ITEM1*& pNewItem, char*& secondLetter)
{
	HEADER_A* newHeaderA = new HEADER_A;
	newHeaderA->pItems = pNewItem;
	newHeaderA->cBegin = *secondLetter;
	return newHeaderA;
}

HEADER_D* FillNewHeaders(ITEM1*& pNewItem, char*& firstLetter, char*& secondLetter)
{
	HEADER_A* newHeaderA = FillNewHeaderA(pNewItem, secondLetter);
	HEADER_D* newHeaderD = new HEADER_D;
	newHeaderA->pNext = nullptr;
	newHeaderD->pHeaderA = newHeaderA;
	newHeaderD->cBegin = *firstLetter;
	return newHeaderD;
}


HEADER_D* InsertItem(HEADER_D* pointer, char* pNewID)
{
	if (pointer == NULL)
	{
		printf("Data structure is empty, function stopped\n");
		return pointer;
	}
	ITEM1* pNewItem = nullptr;
	if (pNewID == NULL)
	{
		pNewItem = (ITEM1*)GetItem(7, nullptr);
	}
	else
	{
		if (idIsValid(pNewID))
		{
			pNewItem = (ITEM1*)GetItem(1, pNewID);
		}
		else
		{
			printf("Invalid ID format\n");
			return pointer;
		}
	}


	char* firstLetterPtr = pNewItem->pID;

	char* secondLetterPtr = strchr(pNewItem->pID, ' ') + 1;

	HEADER_D* currentHeaderD = pointer;

	HEADER_D* previousHeaderD = nullptr;

	HEADER_A* currentHeaderA = nullptr;

	HEADER_A* previousHeaderA = nullptr;

	ITEM1* currentItem = nullptr;

	while (currentHeaderD)

	{
		if (currentHeaderD->cBegin == *firstLetterPtr) // add item to existing headers

		{

			currentHeaderA = currentHeaderD->pHeaderA;

			// while loop covering HEADER_A, with a letter of the 2nd word

			while (currentHeaderA)

			{

				if (currentHeaderA->cBegin == *secondLetterPtr)

				{

					currentItem = (ITEM1*)currentHeaderA->pItems;

					// while loop covering items

					while (currentItem)

					{

						if (!strcmp(currentItem->pID, pNewItem->pID)) // check if the item with the same ID already exists

						{

							printf("Item with such ID already exists\n");
							return pointer;
						}

						else if (!currentItem->pNext)

						{

							currentItem->pNext = pNewItem;

							pNewItem->pNext = nullptr;


							return pointer;

						}

						currentItem = currentItem->pNext;

					}

				} // add a non-exisitng header for the item in the middle of the list

				else if ((previousHeaderA != nullptr) && (currentHeaderA->cBegin > *secondLetterPtr && previousHeaderA->cBegin < *secondLetterPtr))

				{

					HEADER_A* newHeaderAPtr = FillNewHeaderA(pNewItem, secondLetterPtr);

					previousHeaderA->pNext = newHeaderAPtr;

					newHeaderAPtr->pNext = currentHeaderA;

					return pointer;

				} // add a non-exisitng header for the item at the start of the list

				else if (previousHeaderA == nullptr && currentHeaderA->cBegin > *secondLetterPtr)

				{

					HEADER_A* newHeaderAPtr = FillNewHeaderA(pNewItem, secondLetterPtr);

					currentHeaderD->pHeaderA = newHeaderAPtr;

					newHeaderAPtr->pNext = currentHeaderA;

					return pointer;

				} // add a non-exisitng header for the item at the end of the list

				else if (currentHeaderA->pNext == nullptr && currentHeaderA->cBegin < *secondLetterPtr)

				{

					HEADER_A* newHeaderAPtr = FillNewHeaderA(pNewItem, secondLetterPtr);

					newHeaderAPtr->pNext = nullptr;

					currentHeaderA->pNext = newHeaderAPtr;

					return pointer;

				}

				previousHeaderA = currentHeaderA;

				currentHeaderA = currentHeaderA->pNext;

			}

		} // add a non-exisitng header for the item in the middle of the list

		else if ((previousHeaderD != nullptr) && (currentHeaderD->cBegin > *firstLetterPtr && previousHeaderD->cBegin < *firstLetterPtr))

		{

			HEADER_D* newHeaderBPtr = FillNewHeaders(pNewItem, firstLetterPtr, secondLetterPtr);

			previousHeaderD->pNext = newHeaderBPtr;

			newHeaderBPtr->pNext = currentHeaderD;

			return pointer;

		} // add a non-exisitng header for the item at the start of the list

		else if (previousHeaderD == nullptr && currentHeaderD->cBegin > *firstLetterPtr)

		{

			HEADER_D* newHeaderBPtr = FillNewHeaders(pNewItem, firstLetterPtr, secondLetterPtr);

			pointer = newHeaderBPtr;

			newHeaderBPtr->pNext = currentHeaderD;


			return pointer;

		} // add a non-exisitng header for the item at the end of the list

		else if (currentHeaderD->pNext == nullptr && currentHeaderD->cBegin < *firstLetterPtr)

		{

			HEADER_D* newHeaderBPtr = FillNewHeaders(pNewItem, firstLetterPtr, secondLetterPtr);

			newHeaderBPtr->pNext = nullptr;

			currentHeaderD->pNext = newHeaderBPtr;
			return pointer;

		}

		previousHeaderD = currentHeaderD;

		currentHeaderD = currentHeaderD->pNext;

	}
}
char* createFirstWord(char* pNewItemID)
{
	char* pWordOne = (char*)malloc(1 * sizeof(char));
	char* pSpace = strchr(pNewItemID, ' ');
	if (!pSpace)
	{
		return NULL;
	}

	for (int i = 1; *(pNewItemID + i); i++)
	{
		if ((pNewItemID + i) == pSpace)
		{
			pWordOne = (char*)malloc(i + 1 * sizeof(char));
			if (*pWordOne != NULL)
			{
				*(pWordOne + i) = '\0';
			}
		}
	}

	if (pSpace)
	{
		int i = 0;
		for (i = 0; (pNewItemID + i) != pSpace; *(pWordOne + i) = *(pNewItemID + i), i++);
	}

	return pWordOne;
}

char* createSecondWord(char* pNewItemID)
{
	char* pWordTwo = (char*)malloc(1 * sizeof(char));
	char* pSpace = strchr(pNewItemID, ' ');
	if (!pSpace || !(*(pSpace + 1)))
	{
		return NULL;
	}

	for (int i = 1; *(pSpace + i); i++)
	{
		if (!(*(pSpace + i + 1)))
		{
			pWordTwo = (char*)malloc((i + 1) * sizeof(char));
			if (*pWordTwo != NULL)
			{
				*(pWordTwo + i) = '\0';
			}
		}
	}

	if ((pSpace + 1))
	{
		for (int i = 0; *(pSpace + i + 1); *(pWordTwo + i) = *(pSpace + i + 1), i++);
	}

	return pWordTwo;
}

bool compareStrings(char* str1, char* str2)
{
	if (strlen(str1) != strlen(str2))
	{
		return false;
	}
	else
	{
		size_t stlen = strlen(str1);
		for (int i = 0; i < stlen; i++)
		{
			if (*(str1 + i) == *(str2 + i))
			{
				if (i + 1 == stlen)
				{
					return true;
				}
				return false;
			}
			else
			{
				return false;
			}
		}
	}
	return false;
}

ITEM1* getPreviousItem(ITEM1* pItem, ITEM1* pItemInit)
{
	ITEM1* pPrevItem = pItemInit;

	while (pPrevItem)
	{
		if (pPrevItem->pNext == pItem)
		{
			return pPrevItem;
		}
		else if (pPrevItem = pItem)
		{
			return NULL;
		}
		pPrevItem = pPrevItem->pNext;
	}
	return pPrevItem;
}

HEADER_D* RemoveItem(HEADER_D* pointer, char* pItemID)
{
	if (!pointer)
	{
		printf("Data structure is empty\n");
		return nullptr;
	}
	if (!pItemID)
	{
		printf("ID is null\n");
		return nullptr;
	}
	if (idIsValid(pItemID) == false)
	{
		throw 2;
	}
	char* pFirstWord = createFirstWord(pItemID);
	char* pSecondWord = createSecondWord(pItemID);
	char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	HEADER_D* structure = pointer;
	while (structure->pHeaderA)
	{
		if (structure->pHeaderA->cBegin == *pFirstWord)
		{
			for (int i = 0; i < 26; i++)
			{
				if (structure->pHeaderA->pItems != NULL && letters[i] == *pSecondWord)
				{
					ITEM1* pItem = (ITEM1*)(structure->pHeaderA->pItems);
					while (pItem != NULL)
					{
						char* pItemFirstWord = createFirstWord(pItem->pID);
						char* pItemSecondWord = createSecondWord(pItem->pID);
						if (compareStrings(pItemFirstWord, pFirstWord) && compareStrings(pItemSecondWord, pSecondWord))
						{
							ITEM1* pPrevItem = (ITEM1*)malloc(sizeof(ITEM1));
							pPrevItem = getPreviousItem(pItem, (ITEM1*)structure->pHeaderA->pItems + i);
							ITEM1* pNextItem = NULL;
							if (pItem->pNext != NULL)
							{
								ITEM1* pNextItem = pItem->pNext;
							}
							else
							{
								ITEM1* pNextItem = NULL;
							}
							if (pPrevItem == NULL)
							{
								delete(pItem);
								return pointer;
							}
							else
							{
								pPrevItem->pNext = pNextItem;
								delete(pItem);
								return pointer;
							}
						}
						pItem = pItem->pNext;
					}
				}
			}
		}
		structure = structure->pNext;
	}
	throw 3;
}

int main()
{
	// Task 1
	HEADER_D* DS = GetStruct4(1, 30);
	PrintDataStructure(DS);

	// TASK 2
	DS = InsertItem(DS, (char*)"Z A");
	DS = InsertItem(DS, (char*)"Z Z");
	DS = InsertItem(DS, (char*)"Z K");
	DS = InsertItem(DS, (char*)"A Z");
	DS = InsertItem(DS, (char*)"A A");
	DS = InsertItem(DS, (char*)"A K");
	DS = InsertItem(DS, (char*)"G Z");
	DS = InsertItem(DS, (char*)"G A");
	DS = InsertItem(DS, (char*)"G K");
	DS = InsertItem(DS, (char*)"M A");
	DS = InsertItem(DS, (char*)"M Ba");
	DS = InsertItem(DS, (char*)"M Bb");
	DS = InsertItem(DS, (char*)"M Z");
	DS = InsertItem(DS, (char*)"M Ba");
	DS = InsertItem(DS, (char*)"Mba");
	PrintDataStructure(DS);
}