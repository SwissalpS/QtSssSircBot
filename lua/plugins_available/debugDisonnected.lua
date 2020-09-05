--[[
  lua/plugins/debugDisconnected.lua
  prints ID to standard out
--]]
--- prints ID to standard out.
-- Plugin that demonstrates how core methods can be overriden by plugin and
-- user modules.
--
-- The same functionality of this plugin could be achieved more transparently
-- by using the notification system.
--
-- See the log plugins for examples of that.
-- module: plugins.debugDisconnected
--

-- [[
local parent = core.events.disconnected
function core.events.disconnected(sConnectionID)
  print('d:' .. sConnectionID)
  parent(sConnectionID)
end
--]]

