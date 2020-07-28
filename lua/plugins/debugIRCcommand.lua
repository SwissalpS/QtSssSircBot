--[[
  lua/plugins/debugIRCcommand.lua
  prints received IRC commands to standard out
--]]
-- [[
local parent = core.events.IRCcommand
function core.events.IRCcommand(sConnectionID, sCommand, lParams)
  local sOut = 'I:' .. sConnectionID .. ':' .. sCommand
  for _, s in ipairs(lParams) do sOut = sOut .. ';' .. s end
  print(sOut)
  parent(sConnectionID, sCommand, lParams)
end
--]]
