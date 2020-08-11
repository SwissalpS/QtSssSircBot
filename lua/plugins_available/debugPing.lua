--[[
  lua/plugins/debugPing.lua
  prints received pings to standard out
--]]
-- [[
local parent = core.events.ping
function core.events.ping(sConnectionID, sLine)
  print('PING:' .. sConnectionID .. ':' .. sLine)
  parent(sConnectionID, sLine)
end
--]]
