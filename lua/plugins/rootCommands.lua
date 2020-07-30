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
config.RootCommandsPassword = 'foo'
config.RootCommandsConnectionIDrxs = { '.*' }
config.RootCommandsNickRxs = { '^[&@](.*)$' }
config.RootCommandsRateLimit = { iRequests = 12, iDuring = 3600 }

local sendDM = IRC.send_direct_message

local function rootCommands(tP)
  local sSubCommond = tP.sMatch
  local sPassword = string.match(tP.sMessage, '^&[acdx]%s(%S*)')
  if sPassword ~= config.RootCommandsPassword then
    --sendDM(tP.sConnectionID, tP.sNick, 'KO:password missmatch')
    return
  end
  print(sSubCommond, sPassword)
  if string.match(sSubCommond, '[cd]') then
    local sConnectionID = string.match(tP.sMessage, '^&[cd]%s%S*%s(%S*)')
    if 'd' == sSubCommond then
      sendDM(tP.sConnectionID, tP.sNick, 'OK:disconnecting ' .. sConnectionID)
      IRC.send_quit(sConnectionID, config.CoreQuitMessage or 'GoodBye :D')
      IRC.disconnect(sConnectionID)
    else -- 'c'
      sendDM(tP.sConnectionID, tP.sNick, 'OK:re-connecting ' .. sConnectionID)
      IRC.send_quit(sConnectionID, config.CoreQuitMessageReconnect or 'brb')
      IRC.reconnect(sConnectionID)
    end
  elseif 'a' == sSubCommond then
    sendDM(tP.sConnectionID, tP.sNick, 'OK:re-loading lua')
    core.abort(1)
  else -- 'x'
    sendDM(tP.sConnectionID, tP.sNick, 'OK:terminating ' .. EXEFILE)
    core.quit()
  end
end -- rootCommands

local function init()
  local oTrigger = core.Trigger({
    lTriggerRxs = {
      '^&([cd])%s(%S*)%s(%S*)', '^&([ax])%s(%S*)' },
    fCallback = rootCommands,
    lConnectionIDrxs = config.RootCommandsConnectionIDrxs,
    lNickRxs = config.RootCommandsNickRxs,
    hRateLimit = config.RootCommandsRateLimit,
    bIncludeInHelp = true,
    sDescription = '&(c|d) <password> <connection-ID>\n'
      .. '  c: (re)connect or d: disconnect the connection with ID.\n'
      .. '&(x|a) <password>\n'
      .. '  exit application or restart lua portion.'
  })
  oTrigger:makeDMonly()

  core.oTriggerManager:addTrigger(oTrigger)
end -- init

core.oNotificationManager:subscribe('core.init.done', init)
--]]

