--[[
	lua/plugins/logDebug.lua
	plugin that demonstrates how logging mechanism can be used
--]]
--- plugin that demonstrates how logging mechanism can be used to log all
-- 'core.log' notifications.
-- module: plugins.logDebug
--

-- [[
local Logger = require 'core.logging'

-- change path to wherever suits you and add other options
local oLogger = Logger({ sPathFile = EXEDIR .. 'logs/debug.log',
							 -- setting to nil causes default to be used
							 sFormatLine = nil })

local function logItem(hItem)
	oLogger:logItem(hItem)
end -- logItem

core.oNotificationManager:subscribe('core.log', logItem)
--]]
return true

