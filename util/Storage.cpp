#include "Storage.h"

void Storage::setCurrentPos(uint32_t pos)
{
	if (pos < memCapacity)
		currentReadPos = memCapacity;
}
uint32_t Storage::getCurrentReadPos()
{
	return currentReadPos;
}
uint32_t Storage::getCurrentWritePos()
{
	return currentWritePos;
}

uint32_t Storage::getCapacity()
{
	return memCapacity;
}
