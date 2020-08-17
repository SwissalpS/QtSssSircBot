--[[
  lua/user/SwissalpS.mix.lua
  scan messages and repost some on #SwissalpS.mix on freenode
  also keep a log of all those messages
--]]
-- [[
local Logger = require 'core.logging'
local oLogger = Logger({ sPathFile = EXEDIR .. 'logs/SssS_mix.log',
							 sFormatLine = '%timeStamp:%message\n' })

local sPostToChannel = '#SwissalpS.mix'
local sPostToCID = 'fn'

local function repost(tD)
  local sFrom = tD.sFromNick .. ':'
  if 'pandorabot:' == sFrom then sFrom = '' end

  local sOut = tD.sConnectionID .. ':' .. tD.sChannel .. ':' .. sFrom .. tD.sMessage
  oLogger:log(sOut)
  IRC.send_channel_message(sPostToCID, sPostToChannel, sOut)

  end -- repost


local function onMessage(tD)

  if 'fn' ~= tD.sConnectionID then return end
  if sPostToChannel == tD.sChannel then return end

  -- repost mentions/what SwissalpS posts
  local sLowerCaseMessage = tD.sMessage:lower()
  local mMatch = sLowerCaseMessage:match('swissalps')
  if mMatch then repost(tD) return end

  if 'pandorabot' == tD.sFromNick then
    mMatch = sLowerCaseMessage:match('^<admin>')
    local mMatch2 = sLowerCaseMessage:match('^<BuckarooBanzai>')
    if mMatch or mMatch2 then return repost(tD) end
  elseif 'BuckarooBanzai' == tD.sFromNick then
    return repost(tD)
  end

  -- we don't want all messages from all channels
  if '#pandorabox' == tD.sChannel then return end

  repost(tD)

end -- onMessage

core.oNotificationManager:subscribe('core.events.channelMessage', onMessage)

return true
--]]

