#include "oolua.h"
#include "expose_stub_classes.h"
#include <tr1/memory>
int main()
{
	return OOLUA::shared_return<std::tr1::shared_ptr<Stub1>* >::out;
}
