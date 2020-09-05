--[[
  lua/plugins/debugQuit.lua
  prints received quit messages to standard out
--]]
--- prints received quit messages to standard out.
-- Plugin that demonstrates how core methods can be overriden by plugin and
-- user modules.
--
-- The same functionality of this plugin could be achieved more transparently
-- by using the notification system.
--
-- See the log plugins for examples of that.
-- module: plugins.debugQuit
--

-- [[
local parent = core.events.quit
function core.events.quit(sConnectionID, sNick, sMessage)
  print('QUIT:' .. sConnectionID .. ':' .. sNick .. ':' .. sMessage)
  parent(sConnectionID, sNick, sMessage)
end
--]]

