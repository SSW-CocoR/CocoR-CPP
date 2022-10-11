local base_dir = "/home/mingo/dev/c/A_grammars/CocoR-CPP/src/";
local includes_base = {}
local sq_sources = [==[
Action.cpp
BitArray.cpp
CharClass.cpp
CharSet.cpp
Comment.cpp
DFA.cpp
Generator.cpp
HashTable.cpp
Melted.cpp
Node.cpp
Parser.cpp
ParserGen.cpp
Position.cpp
Scanner.cpp
SortedList.cpp
State.cpp
StringBuilder.cpp
Symbol.cpp
Tab.cpp
Target.cpp
Coco.cpp
]==];

local included = {};
local inc_sys = {};
local inc_sys_count = 0;
local out = io.stdout

function CopyWithInline(prefix, filename)
	if included[filename] then return end
	included[filename] = true
	print('//--Start of', filename);
	--if(filename:match("luac?.c"))
	local inp = io.open(prefix .. filename, "r")
	if not inp then
		for idx in ipairs(includes_base) do
			local sdir = includes_base[idx]
			local fn = prefix .. sdir .. filename
			--print(fn)
			inp = io.open(fn, "r")
			if inp then break end
		end
	end
	if not inp then
		if filename == "fzn_picat_sat_bc.h" then
			print('//--End of', filename);
		end
	else
		assert(inp)
		for line in inp:lines() do
			if line:match('#define LUA_USE_READLINE') then
				out:write("//" .. line .. "\n")
			else
				local inc = line:match('#include%s+(["<].-)[">]')
				if inc  then
					out:write("//" .. line .. "\n")
					if inc:sub(1,1) == '"' or inc:match('[<"]sq') then
						CopyWithInline(prefix, inc:sub(2))
					else
						local fn = inc:sub(2)
						if inc_sys[fn] == null then
							inc_sys_count = inc_sys_count +1
							inc_sys[fn] = inc_sys_count
						end
					end
				else
					out:write(line .. "\n")
				end
			end
		end
		print('//--End of', filename);
	end
end

print([==[
#ifdef WITH_COSMOPOLITAN

STATIC_STACK_SIZE(0x400000);

#endif

#ifndef __COSMOPOLITAN__
//g++ -g -Wall -Wextra -DWITHOUT_WCHAR -fno-rtti -fno-exceptions  cocor-am.cpp -o Coco
#include <stdlib.h> //3
#include <fcntl.h> //7
#include <stddef.h> //5
#include <limits.h> //1
#include <ctype.h> //10
#include <memory.h> //9
#include <string.h> //4
#include <wchar.h> //8
#include <stdio.h> //2
//#include <io.h> //6

#endif
]==])

local prefix = base_dir; local src_files = sq_sources;
for filename in src_files:gmatch('([^\n]+)') do
	CopyWithInline(prefix, filename);
end
--for k, v in pairs(inc_sys) do print("#include <" .. k .. "> //" .. v ) end
