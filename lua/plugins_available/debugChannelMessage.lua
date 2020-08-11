--[[
  lua/plugins/debugChannelMessage.lua
  prints received channel messages to standard out
--]]
-- [[
local parent = core.events.channelMessage
function core.events.channelMessage(sConnectionID, sChannel, sFromNick, sMessage)
  print('CM:' .. sConnectionID .. ':' .. sChannel .. ':' .. sFromNick .. ':' .. sMessage)
  parent(sConnectionID, sChannel, sFromNick, sMessage)
end
--]]
