--[[
  debugDisconnected.lua
  prints IP to standard out
--]]
-- [[
local parent = core.events.disconnected
function core.events.disconnected(sConnectionID)
  print('d:' .. sConnectionID)
  parent(sConnectionID)
end
--]]
