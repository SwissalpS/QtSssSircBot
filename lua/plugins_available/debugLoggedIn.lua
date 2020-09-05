--[[
  lua/plugins/debugLoggedIn.lua
  prints loggin message to standard out
--]]
--- prints loggin message to standard out.
-- Plugin that demonstrates how core methods can be overriden by plugin and
-- user modules.
--
-- The same functionality of this plugin could be achieved more transparently
-- by using the notification system.
--
-- See the log plugins for examples of that.
-- module: plugins.debugLoggedIn
--

-- [[
local parent = core.events.loggedIn
function core.events.loggedIn(sConnectionID, sLine)
  print('Login:' .. sConnectionID .. ':' .. sLine)
  parent(sConnectionID, sLine)
end
--]]

