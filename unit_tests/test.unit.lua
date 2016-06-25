--OOLua test.unit
local root = "../"
local name = "test.unit"
create_package(name,root,"ConsoleApp")


configuration {}

files
{
    	root .. "unit_tests/main.cpp",
    	root .. "unit_tests/bind_classes/*.h",
    	root .. "unit_tests/bind_classes/*.cpp",
		root .. "unit_tests/cpp_classes/*.h",
		root .. "unit_tests/cpp_classes/*.cpp",
		root .. "unit_tests/test_classes/*h",
		root .. "unit_tests/test_classes/*.cpp"
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
	root .. "unit_tests/bind_classes",
	root .. "unit_tests/cpp_classes",
	root .. "unit_tests/test_classes",
	--"/usr/local/include/lua52"
}
--flags{ "NoExceptions"}
defines
{
	"OOLUA_USE_PRECOMPILED_HEADER"
}

links
{
	"oolua"
}

configuration{"gmake"}
	if tonumber((_PREMAKE_VERSION):match("(%d%.%d)") or "0") < 4.4 then
		pchheader("unit_tests/test_classes/oolua_tests_pch.h")
	else
		pchheader("test_classes/oolua_tests_pch.h")
	end
configuration{"not gmake","not vs*"}
	pchheader(root .. "unit_tests/test_classes/oolua_tests_pch.h")
configuration{"vs*"}
	pchsource(root .. "unit_tests/test_classes/oolua_tests_pch.cpp")
	buildoptions {"/FI oolua_tests_pch.h"}
	if tonumber((_PREMAKE_VERSION):match("(%d%.%d)") or '0') < 4.4 then
		pchheader(root .. "unit_tests/test_classes/oolua_tests_pch.h")
	else
		pchheader("oolua_tests_pch.h")
	end


unit_test_config(root,name)
