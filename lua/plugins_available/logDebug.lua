--[[
	lua/plugins/logDebug.lua
	plugin that demonstrates how logging mechanism can be used
--]]
-- [[
local Logger = require 'core.logging'

-- change path to wherever suits you and add other options
local oLogger = Logger({ sPathFile = '/var/log/QtSssSircBot/debug.log',
							 -- setting to nil causes default to be used
							 sFormatLine = nil })

local function logItem(hItem)
	oLogger:logItem(hItem)
end -- logItem

core.oNotificationManager:subscribe('core.log', logItem)
--]]
return true

