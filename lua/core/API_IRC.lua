--[[
	lua/core/API_IRC.lua
	symbols in IRC-API, mainly for lite to autocomplete correctly
--]]
return { "abort", -- (iNumber) 0 < iNumber means reload lua in iNumber milliseconds.
						-- 0 >= iNumber means shutdown lua session. Probably application terminating.
	"add_connection", -- (sID, sJSON) returns nil or true if sJSON passes
						-- JSON syntax test. Uses same format as connection
						-- config passed as command line argument or set
						-- in Settings.ini as sIRCconfigPath

	"connection_ids", -- () returns list of all connection IDs
	"disconnect", -- (sID) disconnects socket of sID
	"exit", -- (sID, iCode) terminate application with exit code iCode
	"poll_event", -- () handled by LuaController. Returns nil or event list
	"reconnect", -- (sID) reconnects socket of sID using cached parameters (disconnects if connected)
	"reload_connections", -- () reads connection config files and reconnects all
	"send_channel_message", -- (sID, sChannel, sMessage)
	"send_direct_message", -- (sID, sNick, sMessage)
	"send_join", -- (sID, sChannel)
	"send_line", -- (sID, sLine)
	"send_nick_change_request", -- (sID, sNewNick)
	"send_part", -- (sID, sChannels, sMessage = '') sChannels is a comma list
	"send_quit" -- (sID, sMessage = '')
}

