--[[
  lua/plugins/debugChannelMessage.lua
  prints received channel messages to standard out
--]]
--- prints received channel messages to standard out.
-- Plugin that demonstrates how core methods can be overriden by plugin and
-- user modules.
--
-- The same functionality of this plugin could be achieved more transparently
-- by using the notification system.
--
-- See the log plugins for examples of that.
-- module: plugins.debugChannelMessage
--

-- [[
local parent = core.events.channelMessage
function core.events.channelMessage(sConnectionID, sChannel, sFromNick, sMessage)
  print('CM:' .. sConnectionID .. ':' .. sChannel .. ':' .. sFromNick .. ':' .. sMessage)
  parent(sConnectionID, sChannel, sFromNick, sMessage)
end
--]]

