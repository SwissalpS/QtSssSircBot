--[[
  debugRawOut.lua
  prints raw sent lines to standard out
--]]
-- [[
local parent = core.events.rawOut
-- raw output bot has sent
function core.events.rawOut(sConnectionID, sLine)
  print(sConnectionID .. '>' .. string.gsub(sLine, "\r\n$", ""))
  parent(sConnectionID, sLine)
end
--]]
