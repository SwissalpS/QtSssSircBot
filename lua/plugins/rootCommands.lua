--[[
  lua/plugins/rootCommands.lua
  provides commands to restart connections/lua and quit
  possibly other maintenance duties will be added here too
--]]
--do return false end
-- [[
local core = require 'core'
local config = require 'core.config'

-- change these in your user/init.lua
config.RootCommandsPassword = 'root'
config.RootCommandsConnectionIDrxs = { '^unset$' }
config.RootCommandsNickRxs = { '^[~&@%+]?(tom)$', '^[~&@%+]?(rose)$' }
config.RootCommandsRateLimit = { iRequests = 12, iDuring = 3600 }

local sendDM = IRC.send_direct_message

local function rootCommands(tP)
  local sSubCommond = tP.sMatch
  local sPassword = string.match(tP.sMessage, '^&[aceqr]%s(%S*)')
  print(sSubCommond, sPassword)
  if string.match(sSubCommond, '[cqr]') then
	local sConnectionID = string.match(tP.sMessage, '^&[cqr]%s%S*%s(%S*)')
	if 'c' == sSubCommond then
	  sendDM(tP.sConnectionID, tP.sNick, 'OK:re-connecting ' .. sConnectionID)
	  IRC.reconnect(sConnectionID)
	elseif 'q' == sSubCommond then
	  sendDM(tP.sConnectionID, tP.sNick, 'OK:quiting and disconnecting ' .. sConnectionID)
	  IRC.quit(sConnectionID)
	else -- 'r'
	  sendDM(tP.sConnectionID, tP.sNick, 'OK:re-connecting ' .. sConnectionID)
	  IRC.reconnect(sConnectionID)
	end
  elseif 'a' == sSubCommond then
	sendDM(tP.sConnectionID, tP.sNick, 'OK:re-loading lua')
	IRC.abort()
  else -- 'e'
	sendDM(tP.sConnectionID, tP.sNick, 'OK:terminating QtSssSircBot')
	IRC.exit(0)
  end
end -- rootCommands

local oTrigger = core.Trigger({ lTriggerRxs = { '^&([qrc])%s(%S*)%s(%S*)', '^&([ae])%s(%S*)' },
							  fCallback = rootCommands,
							  lConnectionIDrxs = config.RootCommandsConnectionIDrxs,
							  lNickRxs = config.RootCommandsNickRxs,
							  hRateLimit = config.RootCommandsRateLimit,
							  bIncludeInHelp = true,
							  sDescription = '&(q|r|c) <password> <connection-ID>\n'
							  .. '  quit, reconnect or connect the connection with ID.\n'
							  .. '&(e|a) <password>\n'
							  .. '  exit application or restart lua portion.'
							  })
oTrigger:makeDMonly()

core.oTriggerManager:addTrigger(oTrigger)
--]]
