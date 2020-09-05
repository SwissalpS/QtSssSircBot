--[[
  lua/plugins/debugPing.lua
  prints received pings to standard out
--]]
--- prints received pings to standard out.
-- Plugin that demonstrates how core methods can be overriden by plugin and
-- user modules.
--
-- The same functionality of this plugin could be achieved more transparently
-- by using the notification system.
--
-- See the log plugins for examples of that.
-- module: plugins.debugPing
--

-- [[
local parent = core.events.ping
function core.events.ping(sConnectionID, sLine)
  print('PING:' .. sConnectionID .. ':' .. sLine)
  parent(sConnectionID, sLine)
end
--]]

