#pragma once

#include "stdint.h"
#include "HWInterface.h"
#include "Storage.h"

template<class T>
class EEPROM : public Storage
{
private:
	enum Cmd
	{
		WREN = 0x06,  /*!< Write enable instruction */
		WRDI = 0x04,  /*!< Write disable instruction */
		RDSR = 0x05,  /*!< Read Status Register instruction  */
		WRSR = 0x01,  /*!< Write Status Register instruction */
		WRITE = 0x02, /*!< Write to Memory instruction */
		READ = 0x03,  /*!< Read from Memory instruction */
		RDID = 0x83,  /*!< Read Identification Page */
		WRID = 0x82,  /*!< Write Identification Page */
		RDLS = 0x83,  /*!< Reads the Identification Page lock Status */
		LID = 0x82	  /*!< Locks the Identification page in read-only mode */
	};

	enum StatusFlag
	{
		WIP = 0x01,  /*!< Write In Progress bit */
		WEL = 0x02,  /*!< Write Enable Latch bit */
		BP0 = 0x04,  /*!< Block Protect bit 0 */
		BP1 = 0x08,  /*!< Block Protect bit 1 */
		SRWD = 0x80  /*!< Status Register Write Protect */
	};

	bool sendCmd(Cmd cmd, bool deselectAfterSend = true)
	{
		bool status = false;

		interface.select();
		status = interface.writeByte(cmd) == IF_OK;

		if (deselectAfterSend)
			interface.deselect();

		return status;
	}

	bool isBusy()
	{
		uint8_t status = 0;

		if (!interface.isBusy())
		{
			sendCmd(Cmd::RDSR, false);
			interface.readByte(&status);
			interface.deselect();
		}

		return (interface.isBusy() || (status & StatusFlag::WIP));
	}

	bool waitForFree(uint32_t timeout)
	{
		if (isBusy())
		{
			uint32_t tickstart = HAL_GetTick();

			while ((HAL_GetTick() - tickstart) < timeout)
			{
				if (!isBusy())
					return false;
			}

			if ((0xFFFFFFFFU - tickstart) < timeout)
			{
				while (HAL_GetTick() < (timeout - (0xFFFFFFFFU - tickstart)))
				{
					if (!isBusy())
						return false;
				}
			}
			return true;
		}
	}

	T interface;
	uint32_t pageSize;
	static const uint32_t commTimeout = 500;

public:
	Status init(uint16_t capacity, uint16_t pagesize, void* periphery, IRQn_Type irq, uint8_t irqPriority = 5)
	{
		if (!interface.init(periphery, irq, irqPriority))
		{
			pageSize = pagesize;
			memCapacity = capacity;
			return CMD_OK;
		}
		else
			return IF_ERROR;
	}
	Status write(uint8_t* data, uint16_t size, uint16_t address = 0xFFFF)
	{
		if (address == 0xFFFF)
		{
			address = currentWritePos;
		}
		else
		{
			currentWritePos = address;
		}

		if ((address + size) > memCapacity)
		{
			return SIZE_ERROR;
		}

		uint16_t startPageNo = address / pageSize;
		uint16_t endPageNo = (address + size) / pageSize;
		uint16_t startPageDataSize = ((size + address) > pageSize) ? pageSize - (address % pageSize) : size;
		uint16_t endPageDataSize = (size - startPageDataSize) % pageSize;

		for (uint16_t i = startPageNo; i <= endPageNo; i++)
		{
			uint16_t addressOffset = 0, pageDataSize = 0;
			
			sendCmd(Cmd::WREN);
			sendCmd(Cmd::WRITE, false);

			if (i == startPageNo)
			{
				addressOffset = 0;
				pageDataSize = (startPageNo == endPageNo) ? size : startPageDataSize;
			}
			else if (i == endPageNo)
			{
				addressOffset = size - endPageDataSize;
				pageDataSize = endPageDataSize;
			}
			else
			{
				addressOffset = (i - startPageNo - 1) * pageSize + startPageDataSize;
				pageDataSize = pageSize;
			}

			interface.writeShort(__REV16(address + addressOffset));
			interface.write(data + addressOffset, pageDataSize);
			interface.waitForFree(commTimeout);
			interface.deselect();
			waitForFree(commTimeout);
		}

		sendCmd(Cmd::WRDI);

		currentWritePos += size;

		return CMD_OK;
	}
	Status read(uint8_t* data, uint16_t size, uint16_t address = 0xFFFF)
	{
		if (address == 0xFFFF)
		{
			address = currentReadPos;
		}
		else
		{
			currentReadPos = address;
		}

		if ((address + size) > memCapacity)
		{
			return SIZE_ERROR;
		}

		sendCmd(Cmd::READ, false);

		interface.writeShort(__REV16(address));
		interface.read(data, size);
		interface.deselect();

		currentReadPos += size;
	}

	Status erase()
	{
		for (uint16_t i = 0; i < (memCapacity / pageSize); i++)
		{
			sendCmd(Cmd::WREN);
			sendCmd(Cmd::WRITE, false);

			interface.writeShort(__REV16(i * pageSize));

			for (uint16_t j = 0; j < pageSize / 4; j++)
				interface.writeQuad(0);
			interface.waitForFree(commTimeout);
			interface.deselect();

			waitForFree(commTimeout);
		}

		sendCmd(Cmd::WRDI);
		
		currentWritePos = 0;
		currentReadPos = 0;

		return CMD_OK;
	}

	T& getInterface()
	{
		return interface;
	}
};
