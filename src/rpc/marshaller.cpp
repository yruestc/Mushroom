/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Mushroom
 *    > Created Time:  2017-04-30 09:18:05
**/

#include "marshaller.hpp"

namespace Mushroom {

Marshaller::Marshaller(Buffer &input, Buffer &output)
:input_(input), output_(output) { }

Marshaller::~Marshaller() { }

void Marshaller::Read(const void *str, uint32_t len)
{
	output_.Read((const char *)str, len);
}

void Marshaller::Write(void *str, uint32_t len)
{
	input_.Write((char *)str, len);
}

void Marshaller::Unget(uint32_t size)
{
	input_.Unget(size);
}

bool Marshaller::HasCompleteArgs()
{
	if (input_.size() < 4)
		return false;
	uint32_t packet_size;
	*this >> packet_size;
	if (input_.size() >= packet_size) {
		return true;
	} else {
		Unget(4);
		return false;
	}
}

} // namespace Mushroom
