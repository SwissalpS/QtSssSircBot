--[[
  lua/plugins/debugConnected.lua
  prints ID and IP to standard out
--]]
--- prints ID and IP to standard out.
-- Plugin that demonstrates how core methods can be overriden by plugin and
-- user modules.
--
-- The same functionality of this plugin could be achieved more transparently
-- by using the notification system.
--
-- See the log plugins for examples of that.
-- module: plugins.debugConnected
--

-- [[
local parent = core.events.connected
function core.events.connected(sConnectionID, sLine)
  print('c:' .. sConnectionID .. ':' .. sLine)
  parent(sConnectionID, sLine)
end
--]]

