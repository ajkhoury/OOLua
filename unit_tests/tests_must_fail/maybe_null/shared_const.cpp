#include "oolua.h"
#include "expose_stub_classes.h"
#include <tr1/memory>

/*uses default config which does not support shared pointers*/
int main()
{
	return OOLUA::maybe_null<std::tr1::shared_ptr<Stub1 const> >::out;
}
