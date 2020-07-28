--[[
  lua/plugins/debugRawIn.lua
  prints raw received lines to standard out
--]]
-- [[
local parent = core.events.rawIn
function core.events.rawIn(sConnectionID, sLine)
  print(sConnectionID .. '<' .. string.gsub(sLine, "\r\n$", ""))
  parent(sConnectionID, sLine)
end
--]]
