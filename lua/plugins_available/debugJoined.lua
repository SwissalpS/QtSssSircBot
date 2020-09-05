--[[
  lua/plugins/debugJoined.lua
  prints joined messages to standard out
--]]
--- prints joined messages to standard out.
-- Plugin that demonstrates how core methods can be overriden by plugin and
-- user modules.
--
-- The same functionality of this plugin could be achieved more transparently
-- by using the notification system.
--
-- See the log plugins for examples of that.
-- module: plugins.debugJoined
--

-- [[
local parent = core.events.joined
function core.events.joined(sConnectionID, sNick, sChannel)
  print('J:' .. sConnectionID .. ':' .. sChannel .. ':' .. sNick)
  parent(sConnectionID, sNick, sChannel)
end
--]]

