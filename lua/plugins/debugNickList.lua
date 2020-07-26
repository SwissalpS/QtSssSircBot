--[[
  debugNickList.lua
  prints received nick list to standard out
--]]
-- [[
local parent = core.events.nickList
function core.events.nickList(sConnectionID, sChannel, lNicks)
  local sOut = 'nickList:' .. sConnectionID .. ':' .. sChannel
  for _, s in ipairs(lNicks) do sOut = sOut .. ';' .. s end
  print(sOut)
  parent(sConnectionID, sChannel, lNicks)
end
--]]
