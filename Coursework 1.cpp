#include <iostream>
#include "DateTime.h"
#include "Items.h"
#include "Headers.h"
#include "ICS0017DataSource.h"

void PrintDataStructure(HEADER_D* pointer)
{
	if (!pointer)
	{
		printf("Structure is empty!");
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

int main()
{
	HEADER_D* p = GetStruct4(1, 50);
	PrintDataStructure(p);
}