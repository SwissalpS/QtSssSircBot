--[[
  lua/plugins/debugDirectMessage.lua
  prints received direct messages to standard out
--]]
-- [[
local parent = core.events.directMessage
function core.events.directMessage(sConnectionID, sNick, sMessage)
  print('DM:' .. sConnectionID .. ':' .. sNick .. ':' .. sMessage)
  parent(sConnectionID, sNick, sMessage)
end
--]]
