#include <iostream>
#include "DateTime.h"
#include "Items.h"
#include "Headers.h"
#include "ICS0017DataSource.h"
#include <string>

void PrintDataStructure(HEADER_D* pointer)
{
	if (!pointer)
	{
		printf("Print: Pointer to DS cannot be null");
	}
	else
	{
		int i = 0;
		while (pointer)
		{
			while (pointer->pHeaderA)
			{
				ITEM1* item = (ITEM1*)pointer->pHeaderA->pItems;
				while (item)
				{
					printf("%d)%s %lu %s \n", ++i, item->pID, item->Code, item->pTime);
					item = item->pNext;
				}
				pointer->pHeaderA = pointer->pHeaderA->pNext;
			}
			pointer = pointer->pNext;
		}
	}
}

bool wordIsValid(char* p, int len) 
{
	for (int i = 0; i < len; i++) {
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

void createHeaderD(char first, char second, HEADER_D* p, ITEM1* pItem) 
{
	HEADER_A* newHeaderA = new HEADER_A;
	newHeaderA = (HEADER_A*)malloc(sizeof(HEADER_A));
	newHeaderA->cBegin = second;
	newHeaderA->pItems = pItem;
	newHeaderA->pNext = 0;
	HEADER_D* newHeaderD = new HEADER_D;
	newHeaderD = (HEADER_D*)malloc(sizeof(HEADER_D));
	newHeaderD->cBegin = first;
	newHeaderD->pHeaderA = newHeaderA;
	newHeaderD->pNext = 0;
	newHeaderD->pPrior = 0;
	while (p->pNext != 0 && (int)p->pNext->cBegin < (int)first) { p = p->pNext; }
	if (p->pNext == 0) {
		p->pNext = newHeaderD;
	}
	else {
		newHeaderD->pNext = p->pNext;
		p->pNext = newHeaderD;
	}
	return;
}

void createHeaderA(char second, HEADER_A* p, ITEM1* pItem) 
{
	HEADER_A* newHeader;
	newHeader = (HEADER_A*)malloc(sizeof(HEADER_A));
	newHeader->cBegin = second;
	newHeader->pItems = pItem;
	newHeader->pNext = 0;
	while (p->pNext != 0 && (int)p->pNext->cBegin < (int)second) { p = p->pNext; }
	if (p->pNext == 0) {
		p->pNext = newHeader;
	}
	else {
		newHeader->pNext = p->pNext;
		p->pNext = newHeader;
	}
	return;
}
HEADER_D* InsertItem(HEADER_D* pointer, char* pNewItemID = 0) 
{
	if (pNewItemID == 0 || idIsValid(pNewItemID)) {
		ITEM1* pItem = (ITEM1*)GetItem(1, pNewItemID);
		if (pNewItemID == 0) {
			pNewItemID = pItem->pID;
		}
		char first = *pNewItemID;
		char second = *(strchr(pNewItemID, ' ') + 1);
		HEADER_D* searchP = pointer;
		while (searchP->cBegin != first && searchP->pNext != 0) { searchP = searchP->pNext; }
		if (searchP->cBegin != first && searchP->pNext == 0) 
		{
			createHeaderD(first, second, pointer, pItem);
			return pointer;
		}
		else 
		{
			HEADER_A* searchA = searchP->pHeaderA;
			while (searchA->cBegin != second && searchA->pNext != 0) { searchA = searchA->pNext; }
			if (searchA->cBegin != second && searchA->pNext == 0) 
			{
				createHeaderA(second, searchP->pHeaderA, pItem);
				return pointer;
			}
			else 
			{
				ITEM1* searchI = (ITEM1*)searchA->pItems;
				while (strcmp(searchI->pID, pItem->pID) != 0 && searchI->pNext != 0) { searchI = searchI->pNext; }
				if (strcmp(searchI->pID, pItem->pID) != 0 && searchI->pNext == 0) 
				{
					pItem->pNext = (ITEM1*)searchA->pItems;
					searchA->pItems = pItem;
					return pointer;
				}
				else 
				{
					//The security ID structure is invalid.
					throw 1337;
				}
			}
		}
	}
	else 
	{
		//The security ID structure is invalid.
		throw 1337;
	}
}
int main()
{
	HEADER_D* p = GetStruct4(1, 30);
	PrintDataStructure(p);
	char id[] = "Z A";
	char* iD = id;
	HEADER_D* ds = InsertItem(p, iD);
	PrintDataStructure(ds);
}