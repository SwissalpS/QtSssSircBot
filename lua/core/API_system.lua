--[[
	lua/core/API_system.lua
	symbols in system-API, mainly for lite to autocomplete correctly
--]]
return { "absolute_path", -- (sPath) return absolute-/ realpath
	"chdir", -- (sPath) change current working directory
	"delayed_callback", -- (sID, iDuration) use core.call_later(iInterval, fCallback, mData)
		-- iInterval is in milliseconds
		-- fCallback is the function you want called when interval expires
		-- mData the data to pass to callback. This can be of any type a lua table can hold
	"exec", -- (command) runs system command in background process
	"get_file_info", -- (sPath) returns details about file/dir (type, size, change)
	"get_rand", -- () returns a pseudo-random number
	"init_rand", -- () initialize random number generator with current time
	"list_dir", -- (sPath) returns list of items in sPath
	"split_string" -- (sHaystack, sNeedle) split string into list
}

