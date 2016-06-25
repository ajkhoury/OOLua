--OOLua test unit C++11 scoped enum
local root = "../../"
local name = "scoped_enums"

--scoped enums require a C++11 compiler
--monkey patch configure for os and reset after calling create_package
configure_for_os_old = configure_for_os
function configure_for_os()
	configuration { "linux" }
		defines{ "PLATFORM_CHECKED" , "UNIX_BUILD" }
		buildoptions { "-W -Wall -ansi -pedantic --std=c++0x" }
end

create_package(name,root,"ConsoleApp")

configure_for_os = configure_for_os_old


configuration {}

files
{
    	"**.h",
    	"**.cpp",
    	root .. "src/*.cpp",
    	root .. "include/*.h",
    	root .. "unit_tests/test_classes/common_cppunit_headers.h",
    	root .. "unit_tests/main.cpp",
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
	"OOLUA_STORE_LAST_ERROR=0",
	"OOLUA_USE_EXCEPTIONS=1"
}

unit_test_config(root,name)
