#include <iostream>
#include "DateTime.h"
#include "Items.h"
#include "Headers.h"
#include "ICS0017DataSource.h"
#include <string>

class CourseWork1
{
private:
	bool wordIsValid(char* p, int len)
	{
		for (int i = 0; i < len; i++)
		{
			int ascii = (int)*(p + i);
			if (!((ascii >= 65 || ascii <= 90) || (ascii >= 97 || ascii <= 122) || ascii == 45)) return true;
		}
		return false;
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
public:
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

		char* firstLetter = pNewItem->pID;
		char* secondLetter = strchr(pNewItem->pID, ' ') + 1;
		HEADER_D* currentHeaderD = pointer;
		HEADER_D* previousHeaderD = nullptr;
		HEADER_A* currentHeaderA = nullptr;
		HEADER_A* previousHeaderA = nullptr;
		ITEM1* currentItem = nullptr;
		while (currentHeaderD)
		{
			if (currentHeaderD->cBegin == *firstLetter) // add item to existing headers
			{
				currentHeaderA = currentHeaderD->pHeaderA;
				// while loop covering HEADER_A, with a letter of the 2nd word
				while (currentHeaderA)
				{
					if (currentHeaderA->cBegin == *secondLetter)
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

					else if ((previousHeaderA != nullptr) && (currentHeaderA->cBegin > *secondLetter && previousHeaderA->cBegin < *secondLetter))
					{
						HEADER_A* newHeaderAPtr = FillNewHeaderA(pNewItem, secondLetter);
						previousHeaderA->pNext = newHeaderAPtr;
						newHeaderAPtr->pNext = currentHeaderA;
						return pointer;
					} // add a non-exisitng header for the item at the start of the list
					else if (previousHeaderA == nullptr && currentHeaderA->cBegin > *secondLetter)
					{
						HEADER_A* newHeaderAPtr = FillNewHeaderA(pNewItem, secondLetter);
						currentHeaderD->pHeaderA = newHeaderAPtr;
						newHeaderAPtr->pNext = currentHeaderA;
						return pointer;
					} // add a non-exisitng header for the item at the end of the list
					else if (currentHeaderA->pNext == nullptr && currentHeaderA->cBegin < *secondLetter)
					{
						HEADER_A* newHeaderAPtr = FillNewHeaderA(pNewItem, secondLetter);
						newHeaderAPtr->pNext = nullptr;
						currentHeaderA->pNext = newHeaderAPtr;
						return pointer;
					}
					previousHeaderA = currentHeaderA;
					currentHeaderA = currentHeaderA->pNext;
				}
			} // add a non-exisitng header for the item in the middle of the list
			else if ((previousHeaderD != nullptr) && (currentHeaderD->cBegin > *firstLetter && previousHeaderD->cBegin < *firstLetter))
			{
				HEADER_D* newHeaderBPtr = FillNewHeaders(pNewItem, firstLetter, secondLetter);
				previousHeaderD->pNext = newHeaderBPtr;
				newHeaderBPtr->pNext = currentHeaderD;
				return pointer;
			} // add a non-exisitng header for the item at the start of the list
			else if (previousHeaderD == nullptr && currentHeaderD->cBegin > *firstLetter)
			{
				HEADER_D* newHeaderBPtr = FillNewHeaders(pNewItem, firstLetter, secondLetter);
				pointer = newHeaderBPtr;
				newHeaderBPtr->pNext = currentHeaderD;
				return pointer;
			} // add a non-exisitng header for the item at the end of the list
			else if (currentHeaderD->pNext == nullptr && currentHeaderD->cBegin < *firstLetter)
			{
				HEADER_D* newHeaderBPtr = FillNewHeaders(pNewItem, firstLetter, secondLetter);
				newHeaderBPtr->pNext = nullptr;
				currentHeaderD->pNext = newHeaderBPtr;
				return pointer;
			}
			previousHeaderD = currentHeaderD;
			currentHeaderD = currentHeaderD->pNext;
		}
	}

	HEADER_D* RemoveItem(HEADER_D* pointer, char* pItemID)
	{
		if (pointer == NULL)
		{
			printf("Data structure is empty\n");
			return pointer;
		}

		if (pItemID == NULL)
		{
			printf("ID is null\n");
			return pointer;
		}
		else
		{
			if (idIsValid(pItemID) == false)
			{
				printf("ID is in invalid format.\n");
				return pointer;
			}
		}
		char* firstLetter = pItemID;

		char* secondLetter = strchr(pItemID, ' ') + 1;

		HEADER_D* currentPositionD = pointer;

		HEADER_D* previousPositionD = nullptr;

		HEADER_A* currentPositionA = nullptr;

		HEADER_A* previousPositionA = nullptr;

		ITEM1* currentPositionItem = nullptr;

		ITEM1* previousPositionItem = nullptr;

		char headerDFlag = 0;

		char headerAFlag = 0;

		char itemFlag = 0;

		// while loop covering HEADER_D, with a letter of the 1st word

		while (currentPositionD)
		{
			if (currentPositionD->cBegin == *firstLetter) // header D with the required letter exists
			{
				headerDFlag = 1;
				currentPositionA = currentPositionD->pHeaderA;
				// while loop covering HEADER_A, with a letter of the 2nd word
				while (currentPositionA)
				{
					if (currentPositionA->cBegin == *secondLetter)
					{
						headerAFlag = 1;
						currentPositionItem = (ITEM1*)currentPositionA->pItems;
						// while loop covering items
						while (currentPositionItem)
						{
							if (!strcmp(currentPositionItem->pID, pItemID)) // check if the item with the same ID already exists
							{
								itemFlag = 1;
								if (previousPositionItem != nullptr && currentPositionItem->pNext != nullptr) // item in the middle
								{
									previousPositionItem->pNext = currentPositionItem->pNext;
									delete[] currentPositionItem->pID;
									delete currentPositionItem;
									return pointer;
								}
								else if (previousPositionItem != nullptr && currentPositionItem->pNext == nullptr) // item at the end
								{
									previousPositionItem->pNext = nullptr;
									delete[] currentPositionItem->pID;
									delete currentPositionItem;
									return pointer;
								}
								else if (previousPositionItem == nullptr && currentPositionItem->pNext != nullptr) // item at the start
								{
									currentPositionA->pItems = currentPositionItem->pNext;
									delete[] currentPositionItem->pID;
									delete currentPositionItem;
									return pointer;
								}
								else if (previousPositionItem == nullptr && currentPositionItem->pNext == nullptr) // 1 item in the list
								{
									delete[] currentPositionItem->pID;
									delete currentPositionItem;
									if (previousPositionA != nullptr && currentPositionA->pNext != nullptr) // there are several A headers under 1 D header, header under removal is in the middle
									{
										previousPositionA->pNext = currentPositionA->pNext;
										delete currentPositionA;
										return pointer;
									}
									else if (previousPositionA != nullptr && currentPositionA->pNext == nullptr) // there are several A headers under 1 D header, header under removal is at the end
									{
										previousPositionA->pNext = nullptr;
										delete currentPositionA;
										return pointer;
									}
									else if (previousPositionA == nullptr && currentPositionA->pNext != nullptr) // there are several A headers under 1 D header, header under removal is at the start
									{
										currentPositionD->pHeaderA = currentPositionA->pNext;
										delete currentPositionA;
										return pointer;
									}
									else if (previousPositionA == nullptr && currentPositionA->pNext == nullptr) // there is only 1 A header under 1 D header
									{
										delete currentPositionA;
										if (previousPositionD != nullptr && currentPositionD->pNext != nullptr) // there are several D headers, header under removal is in the middle
										{
											previousPositionD->pNext = currentPositionD->pNext;
											delete currentPositionD;
											return pointer;
										}
										else if (previousPositionD != nullptr && currentPositionD->pNext == nullptr) // there are several D headers, header under removal is at the end
										{
											previousPositionD->pNext = nullptr;
											delete currentPositionD;
											return pointer;
										}
										else if (previousPositionD == nullptr && currentPositionD->pNext != nullptr) // there are several D headers, header under removal is at the start
										{
											pointer = currentPositionD->pNext;
											delete currentPositionD;
											return pointer;
										}
										else if (previousPositionD == nullptr && currentPositionD->pNext == nullptr) // there is only 1 D header, resuting data structure is empty
										{
											delete currentPositionD;
											return nullptr;
										}
									}
								}
							}
							previousPositionItem = currentPositionItem;
							currentPositionItem = currentPositionItem->pNext;
						}
					}
					previousPositionA = currentPositionA;
					currentPositionA = currentPositionA->pNext;
				}
			}
			previousPositionD = currentPositionD;
			currentPositionD = currentPositionD->pNext;
		}
		// header D with the required letter does not exist - require item for removal does not exist
		if (headerDFlag == 0)
		{
			printf("Item with such ID does not exist - Header D\n");
			return pointer;
		}
		else if (headerDFlag == 1 && headerAFlag == 0)
		{
			printf("Item with such ID does not exist - Header A\n");
			return pointer;
		}
		else if (headerDFlag == 1 && headerAFlag == 1 && itemFlag == 0)
		{
			printf("Item with such ID does not exist - ITEM\n");
			return pointer;
		}
	}

	ITEM1* CreateItem1(char* pID = nullptr) { return (ITEM1*)GetItem(1, pID); }

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
};