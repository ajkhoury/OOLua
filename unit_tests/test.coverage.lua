--OOlua test.coverage
local root = "../"

local coverage_include_dirs = function()
	includedirs
	{
		"include/cppunit",
		"include/gmock",
		"include/lua",
		"include/",
		"/usr/local/include",
		"/usr/include",
		root .. "include",
		root .. "unit_tests/bind_classes",
		root .. "unit_tests/cpp_classes",
		root .. "unit_tests/test_classes"
	}
end


create_package("test.coverage",root,"ConsoleApp")
	configuration {}
	files
	{
		root .. "include/*.h",
		root .. "src/*.cpp",
		root .. "unit_tests/main.cpp",
		root .. "unit_tests/bind_classes/*.h",
		root .. "unit_tests/bind_classes/*.cpp",
		root .. "unit_tests/cpp_classes/*.h",
		root .. "unit_tests/cpp_classes/*.cpp",
		root .. "unit_tests/test_classes/*.h",
		root .. "unit_tests/test_classes/*.cpp",
	}
	coverage_include_dirs()
	unit_test_config(root,"test.coverage")
	coverage()

create_package("test.coverage.using_exceptions",root,"ConsoleApp")
	configuration {}
	files
	{
		root .. "include/*.h",
		root .. "src/*.cpp",
		root .. "unit_tests/main.cpp",
		root .. "unit_tests/bind_classes/*.h",
		root .. "unit_tests/bind_classes/*.cpp",
		root .. "unit_tests/cpp_classes/*.h",
		root .. "unit_tests/cpp_classes/*.cpp",
		root .. "unit_tests/test_classes/*.h",
		root .. "unit_tests/test_classes/*.cpp",
	}
	coverage_include_dirs()
	unit_test_config(root,"test.coverage.using_exceptions")
	coverage()
	defines
	{
		"OOLUA_RUNTIME_CHECKS_ENABLED=1",
		"OOLUA_STD_STRING_IS_INTEGRAL=1",
		"OOLUA_SAFE_ID_COMPARE=1",

		"OOLUA_STORE_LAST_ERROR=0",
		"OOLUA_USE_EXCEPTIONS=1"
	}


create_package("test.coverage.string_is_integral",root,"ConsoleApp")
	configuration {}
	files
	{
    	root .. "src/*.cpp",
    	root .. "include/*.h",
    	root .. "unit_tests/test_classes/common_cppunit_headers.h",
    	root .. "unit_tests/main.cpp",
    	root .. "unit_tests/string_is_integral/string_as_integral.cpp",
    	root .. "unit_tests/string_is_integral/is_string_class.cpp",
	}
	defines
	{
		"OOLUA_RUNTIME_CHECKS_ENABLED=1",
		"OOLUA_STD_STRING_IS_INTEGRAL=1",
		"OOLUA_SAFE_ID_COMPARE=0",
		"OOLUA_STORE_LAST_ERROR=1",
		"OOLUA_USE_EXCEPTIONS=0"
	}
	coverage_include_dirs()
	unit_test_config(root,"test.coverage.string_is_integral")
	coverage()

create_package("test.coverage.string_is_integral.using_exceptions",root,"ConsoleApp")
	configuration {}
	files
	{
    	root .. "src/*.cpp",
    	root .. "include/*.h",
    	root .. "unit_tests/test_classes/common_cppunit_headers.h",
    	root .. "unit_tests/main.cpp",
    	root .. "unit_tests/string_is_integral/string_as_integral.cpp",
    	root .. "unit_tests/string_is_integral/is_string_class.cpp",
	}
	defines
	{
		"OOLUA_RUNTIME_CHECKS_ENABLED=1",
		"OOLUA_STD_STRING_IS_INTEGRAL=1",
		"OOLUA_SAFE_ID_COMPARE=0",
		"OOLUA_STORE_LAST_ERROR=0",
		"OOLUA_USE_EXCEPTIONS=1"
	}
	coverage_include_dirs()
	unit_test_config(root,"test.coverage.string_is_integral.using_exceptions")
	coverage()


--C++11 scoped enums require a C++11 compiler.
--Monkey patch the function tconfigure_for_os and then after call
--to create_package reset it to the previous value.
local create_package_c11 = function(name, root_directory, type)
	configure_for_os_org = configure_for_os

	function configure_for_os()
		configuration { "linux" }
			defines{ "PLATFORM_CHECKED" , "UNIX_BUILD" }
			buildoptions { "-W -Wall -ansi -pedantic --std=c++0x" }
	end
	create_package(name, root_directory, type)

	configure_for_os = configure_for_os_org
	configure_for_os_org = nil
end

create_package_c11("test.coverage.scoped_enum",root,"ConsoleApp")
	configuration {}
	files
	{
    	root .. "unit_tests/scoped_enum/**.h",
    	root .. "unit_tests/scoped_enum/**.cpp",
    	root .. "src/*.cpp",
    	root .. "include/*.h",
    	root .. "unit_tests/test_classes/common_cppunit_headers.h",
    	root .. "unit_tests/main.cpp",
	}
	coverage_include_dirs()
	unit_test_config(root,"test.coverage.scoped_enum")
	coverage()


create_package("test.coverage.shared",root,"ConsoleApp")
	configuration {}
	files
	{
    	root .."unit_tests/shared/**.h",
    	root .."unit_tests/shared/**.cpp",
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
	coverage_include_dirs()
	defines
	{
		"OOLUA_RUNTIME_CHECKS_ENABLED=1",
		"OOLUA_USE_SHARED_PTR=1",
		"OOLUA_NEW_POINTER_DEFAULT_IS_SHARED_TYPE=0",
		"OOLUA_STORE_LAST_ERROR=1",
		"OOLUA_USE_EXCEPTIONS=0"
	}
	unit_test_config(root,"test.coverage.shared")
	coverage()



create_package("test.coverage.shared.using_exceptions",root,"ConsoleApp")
	configuration {}
	files
	{
   	 	root .."unit_tests/shared/**.h",
    	root .."unit_tests/shared/**.cpp",
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
	defines
	{
		"OOLUA_RUNTIME_CHECKS_ENABLED=1",
		"OOLUA_USE_SHARED_PTR=1",
		"OOLUA_NEW_POINTER_DEFAULT_IS_SHARED_TYPE=0",
		"OOLUA_STORE_LAST_ERROR=0",
		"OOLUA_USE_EXCEPTIONS=1"
	}
	coverage_include_dirs()
	unit_test_config(root,"test.coverage.shared.using_exceptions")
	coverage()


create_package("test.coverage.shared_by_default",root,"ConsoleApp")
	configuration {}
	files
	{
    	root .."unit_tests/shared_by_default/**.h",
    	root .."unit_tests/shared_by_default/**.cpp",
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
	}
	defines
	{
		"OOLUA_RUNTIME_CHECKS_ENABLED=1",
		"OOLUA_USE_SHARED_PTR=1",
		"OOLUA_NEW_POINTER_DEFAULT_IS_SHARED_TYPE=1",
		"OOLUA_STORE_LAST_ERROR=1",
		"OOLUA_USE_EXCEPTIONS=0"
	}
	coverage_include_dirs()
	unit_test_config(root,"test.coverage.shared_by_default")
	coverage()


create_package("test.coverage.shared_by_default.using_exceptions",root,"ConsoleApp")
	configuration {}
	files
	{
    	root .."unit_tests/shared_by_default/**.h",
    	root .."unit_tests/shared_by_default/**.cpp",
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
	}
	defines
	{
		"OOLUA_RUNTIME_CHECKS_ENABLED=1",
		"OOLUA_USE_SHARED_PTR=1",
		"OOLUA_NEW_POINTER_DEFAULT_IS_SHARED_TYPE=1",
		"OOLUA_STORE_LAST_ERROR=0",
		"OOLUA_USE_EXCEPTIONS=1"
	}
	coverage_include_dirs()
	unit_test_config(root,"test.coverage.shared_by_default.using_exceptions")
	coverage()
