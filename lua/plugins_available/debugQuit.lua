--[[
  lua/plugins/debugQuit.lua
  prints received quit messages to standard out
--]]
-- [[
local parent = core.events.quit
function core.events.quit(sConnectionID, sNick, sMessage)
  print('QUIT:' .. sConnectionID .. ':' .. sNick .. ':' .. sMessage)
  parent(sConnectionID, sNick, sMessage)
end
--]]
