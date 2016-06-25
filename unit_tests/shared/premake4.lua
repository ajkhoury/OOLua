--OOLua test unit shared
local root = "../../"
local name = "shared"
create_package(name,root,"ConsoleApp")


configuration {}

files
{
    	"**.h",
    	"**.cpp",
    	root .. "src/*.cpp",
    	root .. "include/*.h",
    	root .. "unit_tests/test_classes/common_cppunit_headers.h",
    	root .. "unit_tests/main.cpp",
		root .. "unit_tests/cpp_classes/cpp_stub_classes.h",
	  	root .. "unit_tests/bind_classes/expose_stub_classes.h",
		root .. "unit_tests/bind_classes/expose_stub_classes.cpp",

		root .. "unit_tests/cpp_classes/cpp_constructor_mock.h",
		root .. "unit_tests/cpp_classes/cpp_constructor_mock.cpp",
	  	root .. "unit_tests/bind_classes/expose_class_constructors.h",
		root .. "unit_tests/bind_classes/expose_class_constructors.cpp",

		root .. "unit_tests/cpp_classes/cpp_class_ops.h",
	  	root .. "unit_tests/bind_classes/expose_class_ops.h",
		root .. "unit_tests/bind_classes/expose_class_ops.cpp",

		root .. "unit_tests/cpp_classes/cpp_hierarchy.h",
	  	root .. "unit_tests/bind_classes/expose_hierarchy.h",
		root .. "unit_tests/bind_classes/expose_hierarchy.cpp",
}

includedirs
{
	"include/cppunit",
	"include/gmock",
	"include/lua",
	"include/",
	root .. "include/",
	"/usr/local/include",
	"/usr/include",
	root .. "unit_tests/bind_classes/",
	root .. "unit_tests/cpp_classes/",
	root .. "unit_tests/test_classes/"
}

defines
{
	"OOLUA_RUNTIME_CHECKS_ENABLED=1",
	"OOLUA_USE_SHARED_PTR=1",
	"OOLUA_NEW_POINTER_DEFAULT_IS_SHARED_TYPE=0",
	"OOLUA_STORE_LAST_ERROR=1",
	"OOLUA_USE_EXCEPTIONS=0"
}

unit_test_config(root,name)
