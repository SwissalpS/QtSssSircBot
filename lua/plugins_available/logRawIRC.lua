--[[
	lua/plugins/logRawIRC.lua
	plugin that demonstrates how logging mechanism can be used
--]]
local Logger = require 'core.logging'

-- change path to wherever suits you and add other options
local oLogger = Logger({ sPathFile = EXEDIR .. 'logs/rawIRC.log',
							 sFormatLine = '%timeStamp:%message\n' })

local function logIn(hEvent)
	local sOut = '<' .. hEvent.sConnectionID .. ':'
				 .. hEvent.sLine:gsub('[\n\r]+$', '')
	oLogger:log(sOut)
end -- logIn

local function logOut(hEvent)
	local sOut = '>' .. hEvent.sConnectionID .. ':'
				 .. hEvent.sLine:gsub('[\n\r]+$', '')
	oLogger:log(sOut)
end -- logOut

core.oNotificationManager:subscribe('core.events.rawIn', logIn)
core.oNotificationManager:subscribe('core.events.rawOut', logOut)

