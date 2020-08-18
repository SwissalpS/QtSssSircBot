--[[
  lua/plugins/commandsCountdown.lua
  provides command to start a countdown
--]]
--do return false end
-- [[
local config = require 'core.config'

-- change these in your user/init.lua
config.CountdownCommandsConnectionIDrxs = { '.*' }
config.CountdownNickRxs = { '^[~&@%+]?(.*)$' }
config.CountdownDMonly = true
config.CountdownCommand2ConnectionIDrxs = { '.*' }
config.Countdown2NickRxs = { '^[&@](.*)$' }

local sendDM = IRC.send_direct_message

local function countdownAction(tData)

  local iDiff = tData.iDeadline - os.time()
  local sOut = ''
  local bRepeat = true
  if 0 >= iDiff then
    -- time has elapsed
    sOut = tData.sMessage
    bRepeat = false
  elseif 5 >= iDiff then
    sOut = tostring(iDiff) .. ' seconds until ' .. tData.sMessage
  elseif (40 >= iDiff) and (0 == (iDiff % 10)) then
    sOut = tostring(iDiff) .. ' seconds until ' .. tData.sMessage
  elseif ((5 * 60) >= iDiff) and (0 == (iDiff % 60)) then
    sOut = tostring(iDiff / 60) .. ' minutes until ' .. tData.sMessage
  elseif 0 == (iDiff % 600) then
    sOut = tostring(iDiff / 60) .. ' minutes until ' .. tData.sMessage
  end

  sendDM(tData.sConnectionID, tData.sChannel, sOut)

  if bRepeat then core.call_later(1000, countdownAction, tData) end

end -- countdownAction


-- takes a string formated: h:m:s or m:s or just s
-- returns 0 on error and negative results
local function stringToSeconds(s)
  if 'string' ~= type(s) then return 0 end
  -- prepend 0:0: to ensure we get enough parts back
  local lParts = system.split_string('0:0:' .. s, ':')
  -- use only 3 last parts
  local iMultiplier = 1
  local iInterval = 0
  local iPart
  for i = #lParts, #lParts - 2, -1 do
    iPart = tonumber(lParts[i]) or 0
    iInterval = iInterval + (iPart * iMultiplier)
    iMultiplier = iMultiplier * 60
  end
  return math.max(0, iInterval)
end -- stringToSeconds


local function countdownCommand(tP)
  local iInterval = stringToSeconds(tP.sMatch)

  local sMessage = string.match(tP.sMessage, '^!countdown%s%S*%s(.*)$')

  local tData = {
    sConnectionID = tP.sConnectionID,
    sChannel = tP.sChannel or tP.sNick,
    iDeadline = os.time() + iInterval,
    sMessage = sMessage
  }

  sendDM(tP.sConnectionID, tP.sNick, 'OK:started countdown')
  core.call_later(1000, countdownAction, tData)

end -- countdownCommand


local function countdownCommand2(tP)
  print('eueueue')
  local mInterval, sConnectionID, sChannel, sMessage =
    tP.sMatch:match('^(%S*)%s(%S*)%s(%S*)%s(.*)$')
  -- support h:m:s format also m:s and s
  local iInterval = stringToSeconds(mInterval)

  local tData = {
    sConnectionID = sConnectionID,
    sChannel = sChannel,
    iDeadline = os.time() + iInterval,
    sMessage = sMessage
  }

  sendDM(tP.sConnectionID, tP.sNick, 'OK:started countdown on '
    .. sConnectionID .. ' ' .. sChannel)
  core.call_later(1000, countdownAction, tData)

end -- countdownCommand2


local function init()
  local oTrigger = core.Trigger({
    lTriggerRxs = { '^!countdown%s(%S*)%s(.*)$' },
    fCallback = countdownCommand,
    lConnectionIDrxs = config.CountdownCommandsConnectionIDrxs,
    lNickRxs = config.CountdownNickRxs,
    bIncludeInHelp = true,
    sDescription = '!countdown <seconds> <message>\n'
      .. '  starts a countdown. Posts messages to channel on which command was\n'
      .. '  issued on.'
  })
  if config.CountdownDMonly then
    oTrigger:makeDMonly()
  else
    oTrigger:makeCMandDM()
  end

  core.oTriggerManager:addTrigger(oTrigger)

  local oTrigger2 = core.Trigger({
    lTriggerRxs = { '^!countdownOn%s(%S*%s%S*%s%S*%s.*)$' },
    fCallback = countdownCommand2,
    lConnectionIDrxs = config.CountdownCommand2ConnectionIDrxs,
    lNickRxs = config.Countdown2NickRxs,
    bIncludeInHelp = true,
    sDescription = '!countdownOn <seconds> <connectionID> <channel or nick> <message>\n'
      .. '  starts a countdown. Posts messages to <channel or nick>\n'
      .. '  on connection <connectionID>'
  })
  oTrigger2:makeDMonly()

  core.oTriggerManager:addTrigger(oTrigger2)
end -- init

core.oNotificationManager:subscribe('core.init.done', init)
--]]

