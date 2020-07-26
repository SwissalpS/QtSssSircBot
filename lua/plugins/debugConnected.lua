--[[
  debugConnected.lua
  prints IP to standard out
--]]
-- [[
local parent = core.events.connected
function core.events.connected(sConnectionID, sLine)
  print('c:' .. sConnectionID .. ':' .. sLine)
  parent(sConnectionID, sLine)
end
--]]
