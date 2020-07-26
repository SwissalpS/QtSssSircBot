--[[
  debugLoggedIn.lua
  prints loggin message to standard out
--]]
-- [[
local parent = core.events.loggedIn
function core.events.loggedIn(sConnectionID, sLine)
  print('Login:' .. sConnectionID .. ':' .. sLine)
  parent(sConnectionID, sLine)
end
--]]
