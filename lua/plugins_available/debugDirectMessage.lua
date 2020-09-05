--[[
  lua/plugins/debugDirectMessage.lua
  prints received direct messages to standard out
--]]
--- prints received direct messages to standard out.
-- Plugin that demonstrates how core methods can be overriden by plugin and
-- user modules.
--
-- The same functionality of this plugin could be achieved more transparently
-- by using the notification system.
--
-- See the log plugins for examples of that.
-- module: plugins.debugDirectMessage
--

-- [[
local parent = core.events.directMessage
function core.events.directMessage(sConnectionID, sNick, sMessage)
  print('DM:' .. sConnectionID .. ':' .. sNick .. ':' .. sMessage)
  parent(sConnectionID, sNick, sMessage)
end
--]]

