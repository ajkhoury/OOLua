#include "expose_public_instances.h"
#include "expose_stub_classes.h"
#include "expose_enum.h"
#include "oolua_dsl_export.h"

/*[PublicMembersExport]*/
OOLUA_EXPORT_FUNCTIONS(Public_variables
								, set_an_int
								, set_int_ptr
								, set_dummy_instance
								, set_dummy_ref
								, set_m_int
								, set_int
								, set_dummy_instance_none_ptr
								, set_enum_instance_none_ptr)

OOLUA_EXPORT_FUNCTIONS_CONST(Public_variables
							, get_an_int
							, get_int_ptr
							, get_dummy_instance
							, get_dummy_ref
							, get_dummy_instance_none_ptr
							, get_int
							, get_m_int)
/*[PublicMembersExport]*/
