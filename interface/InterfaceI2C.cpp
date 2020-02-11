#include "InterfaceI2C.h"

InterfaceI2C::InterfaceI2C()
{

}

bool InterfaceI2C::init(void* const instance, IRQn_Type irq, uint8_t irqPriority)
{

}

bool InterfaceI2C::deinit(void* const instance)
{

}

int32_t InterfaceI2C::read(uint8_t* data, uint32_t size, bool unblocking)
{

}

int32_t InterfaceI2C::write(uint8_t* data, uint32_t size, bool unblocking)
{

}

bool InterfaceI2C::isBusy()
{

}

bool InterfaceI2C::waitForFree(uint32_t timeout)
{

}

void InterfaceI2C::handler()
{

}

void InterfaceI2C::errorHandler()
{

}
