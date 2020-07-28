--[[
  lua/plugins/debugJoined.lua
  prints joined messages to standard out
--]]
-- [[
local parent = core.events.joined
function core.events.joined(sConnectionID, sNick, sChannel)
  print('J:' .. sConnectionID .. ':' .. sChannel .. ':' .. sNick)
  parent(sConnectionID, sNick, sChannel)
end
--]]
