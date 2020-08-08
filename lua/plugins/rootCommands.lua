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
  local sSubCommand = tP.sMatch
  local sPassword = string.match(tP.sMessage, '^&[acdx]%s(%S*)')
  if sPassword ~= config.RootCommandsPassword then
    --sendDM(tP.sConnectionID, tP.sNick, 'KO:password missmatch')
    return
  end
  if string.match(sSubCommand, '[cd]') then
    local sConnectionID = string.match(tP.sMessage, '^&[cd]%s%S*%s(%S*)')
    if 'd' == sSubCommand then
      sendDM(tP.sConnectionID, tP.sNick, 'OK:disconnecting ' .. sConnectionID)
      IRC.send_quit(sConnectionID, config.CoreQuitMessage or 'GoodBye :D')
      IRC.disconnect(sConnectionID)
    else -- 'c'
      sendDM(tP.sConnectionID, tP.sNick, 'OK:re-connecting ' .. sConnectionID)
      IRC.send_quit(sConnectionID, config.CoreQuitMessageReconnect or 'brb')
      IRC.reconnect(sConnectionID)
    end
  elseif 'a' == sSubCommand then
    sendDM(tP.sConnectionID, tP.sNick, 'OK:re-loading lua')
    core.abort(1)
  elseif 'r' == sSubCommand then
    sendDM(tP.sConnectionID, tP.sNick, 'OK:re-loading connections')
    IRC.reload_connections()
  else -- 'x'
    sendDM(tP.sConnectionID, tP.sNick, 'OK:terminating ' .. EXEFILE)
    core.quit()
  end
end -- rootCommands

local function rootCommands2(tP)
  local sPassword, sConnectionID, sMessage = tP.sMatch:match('^(%S*)%s(%S*)%s(.*)$')
  if sPassword ~= config.RootCommandsPassword then
    --sendDM(tP.sConnectionID, tP.sNick, 'KO:password missmatch')
    return
  end
  IRC.send_line(sConnectionID, sMessage)
  sendDM(tP.sConnectionID, tP.sNick, 'OK:sent line')
end -- rootCommands2

local function init()
  local oTrigger = core.Trigger({
    lTriggerRxs = {
      '^&([cd])%s(%S*)%s(%S*)', '^&([arx])%s(%S*)' },
    fCallback = rootCommands,
    lConnectionIDrxs = config.RootCommandsConnectionIDrxs,
    lNickRxs = config.RootCommandsNickRxs,
    hRateLimit = config.RootCommandsRateLimit,
    bIncludeInHelp = true,
    sDescription = '&(c|d) <password> <connection-ID>\n'
      .. '  c: (re)connect or d: disconnect the connection with ID.\n'
      .. '&(x|a|r) <password>\n'
      .. '  e(x)it application, rest(a)rt lu(a) portion or (r)eload connections.'
  })
  oTrigger:makeDMonly()

  core.oTriggerManager:addTrigger(oTrigger)

  local oTrigger2 = core.Trigger({
    lTriggerRxs = { '^&s%s(%S*%s%S*%s.*)$' },
    fCallback = rootCommands2,
    lConnectionIDrxs = config.RootCommandsConnectionIDrxs,
    lNickRxs = config.RootCommandsNickRxs,
    hRateLimit = config.RootCommandsRateLimit,
    bIncludeInHelp = true,
    -- TODO: implement privs authentication system so don't need to pass password each time
    sDescription = '&s <password> <connection-ID> <raw IRC line to send>\n'
  })
  oTrigger2:makeDMonly()

  core.oTriggerManager:addTrigger(oTrigger2)
end -- init

core.oNotificationManager:subscribe('core.init.done', init)
--]]

