--/**
--[[
	\file oolua_amalgamation.lua
	\brief Lua module for amalgamating the library's headers and source files into
	one header and source file.
--]]
--*/

local copyright = [[
/*
The MIT License

Copyright (c) 2009 - 2015 Liam Devine

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
]]

--[[This allows doxygen to document the functions as you may expect--]]

--function amalgamate(include_dir, src_dir, output_dir);
--Lua module that generates a single header and source file for the library.
--/**
--[[
\addtogroup OOLuaConfig
@{
	\addtogroup OOLuaAmalagate File amalgamation
	@{
		\brief Header and source file amalgamation
		\details The module "oolua_amalgamation" returns a table containing a single function, amalgamate.
		\snippet oolua_amalgamation.lua AmalModuleReturn
		This function concatenates all of the library's header files to produce the file oolua.h, and
		similarly the source files to produce the file oolua.cpp. The two files will contain all the functionality
		of the library and could quickly be integrated into your project

		You can produce the files using either the module with Lua or premake4.
		<p>
		<b>Lua module</b>:
		\code
		lua -e "require('build_scripts.oolua_amalgamation').amalgamate('./include/', './src/', './')"
		\endcode
		<p>
		<b>Premake4</b>:
		<p>
		Generating the amalgamated files with premake4, will create the files oolua.h and oolua.cpp
		that will be located in the directory "amal".
		\code
		premake4 oolua-amalgam
		\endcode


		\fn function amalgamate (include_dir, src_dir, output_dir)
		\param include_dir Directory containing the header files
		\param src_dir Directory containing the source files
		\param output_dir Output directory for the amalgamated files
		\brief Generates an amalgamated header and source file for the library.
		\details Concatenates all the header files from include_dir and separately the source files from
		src_dir, producing the outputs oolua.h  and oolua.cpp respectively. These two files, located in
		output_dir, contain all the functionality of the library.
	@}
@}
--]]
--*/
--function amalgamate(include_dir, src_dir, output_dir);

local headers =
{
	'lua_includes.h'
	,'platform_check.h'
	,'oolua_config.h'
	,'typelist_structs.h'
	,'type_list.h'


	,'dsl_va_args.h'
	,'lvd_type_traits.h'
	,'lvd_types.h'
	,'proxy_test.h'
	,'oolua_string.h'
	,'oolua_traits_fwd.h'
	,'oolua_traits.h'


	,'char_arrays.h'

	,'proxy_userdata.h'
	,'proxy_class.h'
	,'proxy_base_checker.h'
	,'class_from_stack.h'

	,'oolua_error.h'
	,'oolua_exception.h'


	,'oolua_boilerplate.h'
	,'type_converters.h'
	,'oolua_stack_fwd.h'
	,'proxy_tag_info.h'
	,'proxy_storage.h'
	,'push_pointer_internal.h'
	,'proxy_stack_helper.h'
	,'proxy_caller.h'
	,'default_trait_caller.h'

	,'oolua.h'
	,'oolua_check_result.h'
	,'oolua_chunk.h'
	,'oolua_dsl.h'
	,'oolua_dsl_export.h'

	,'oolua_ref.h'
	,'oolua_function.h'

	,'oolua_helpers.h'
	,'oolua_open.h'
	,'oolua_pull.h'
	,'oolua_push.h'

	,'oolua_registration_fwd.h'
	,'proxy_function_dispatch.h'
	,'proxy_tags.h'
	,'oolua_table.h'
	,'proxy_operators.h'
	,'oolua_registration.h'

	,'oolua_script.h'
	,'oolua_stack.h'
	,'oolua_stack_dump.h'

	,'oolua_version.h'

	,'proxy_constructor_param_tester.h'
	,'proxy_constructor.h'

	,'proxy_member_function.h'
	,'proxy_none_member_function.h'
	,'proxy_public_member.h'
	,'stack_get.h'
	,'proxy_function_exports.h'

}

local source =
{
	'class_from_stack.cpp'
	,'oolua.cpp'
	,'oolua_check_result.cpp'
	,'oolua_chunk.cpp'
	,'oolua_error.cpp'
	,'oolua_exception.cpp'
	,'oolua_function.cpp'
	,'oolua_helpers.cpp'
	,'oolua_open.cpp'
	,'oolua_push.cpp'
	,'oolua_pull.cpp'
	,'oolua_ref.cpp'
	,'oolua_registration.cpp'
	,'oolua_script.cpp'
	,'oolua_stack_dump.cpp'
	,'oolua_string.cpp'
	,'oolua_table.cpp'
	,'proxy_storage.cpp'
	,'push_pointer_internal.cpp'
	,'stack_get.cpp'
}

local hcopy = {}
for _,v in ipairs(headers)do
	hcopy[v] = 1
end

local remove_all_oolua_header_includes = function(header_file_data)
	local result = header_file_data:gsub('(\n#%s-include%s-)"(.-)"',
			function(c1,c2)
				if hcopy[c2] then return ''
				else return c1..'"'..c2..'"'
				end
			end)
	return result
end

local do_headers = function(include_dir, output_dir)
	local f = io.open(output_dir .. 'oolua.h', 'w+')
	for _, file in ipairs(headers) do
		local _f, err = io.open(include_dir .. file)
		if err then print(err) end
		f:write(remove_all_oolua_header_includes(_f:read('*a')))
		_f:close()
	end
	f:close()
end

local do_source = function(src_dir, output_dir)
	local f = io.open(output_dir .. 'oolua.cpp', 'w+')
	f:write('#include "oolua.h"\n')
	for _, file in ipairs(source) do
		local _f, err = io.open(src_dir .. file)
		if err then print(err) end
		f:write(remove_all_oolua_header_includes(_f:read('*a')))
		_f:close()
	end
	f:close()
end

local gen_false_header = function(leaf_and_path)
	local f, err = io.open(leaf_and_path, 'w+')
	if err then print(err) end
	f:write('#include "oolua.h"\n')
	f:close()
end

local amalgamate = function(include_dir, src_dir, output_dir)
	do_headers(include_dir, output_dir)
	do_source(src_dir, output_dir)
--	gen_false_header(output_dir .. 'oolua_dsl.h')
--	gen_false_header(output_dir .. 'oolua_dsl_export.h')
end

--/*[AmalModuleReturn]*/
return {
	amalgamate = amalgamate
}
--/*[AmalModuleReturn]*/
