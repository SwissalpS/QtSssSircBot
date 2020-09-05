--[[
  lua/plugins/debugRawOut.lua
  prints raw sent lines to standard out
--]]
--- prints raw sent lines to standard out.
-- Plugin that demonstrates how core methods can be overriden by plugin and
-- user modules.
--
-- The same functionality of this plugin could be achieved more transparently
-- by using the notification system.
--
-- See the log plugins for examples of that.
-- module: plugins.debugRawOut
--

-- [[
local parent = core.events.rawOut
-- raw output bot has sent
function core.events.rawOut(sConnectionID, sLine)
  print(sConnectionID .. '>' .. string.gsub(sLine, "\r\n$", ""))
  parent(sConnectionID, sLine)
end
--]]

