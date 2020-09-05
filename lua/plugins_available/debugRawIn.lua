--[[
  lua/plugins/debugRawIn.lua
  prints raw received lines to standard out
--]]
--- prints raw received lines to standard out.
-- Plugin that demonstrates how core methods can be overriden by plugin and
-- user modules.
--
-- The same functionality of this plugin could be achieved more transparently
-- by using the notification system.
--
-- See the log plugins for examples of that.
-- module: plugins.debugRawIn
--

-- [[
local parent = core.events.rawIn
function core.events.rawIn(sConnectionID, sLine)
  print(sConnectionID .. '<' .. string.gsub(sLine, "\r\n$", ""))
  parent(sConnectionID, sLine)
end
--]]

