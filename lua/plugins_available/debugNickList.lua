--[[
  lua/plugins/debugNickList.lua
  prints received nick list to standard out
--]]
--- prints received nick list to standard out.
-- Plugin that demonstrates how core methods can be overriden by plugin and
-- user modules.
--
-- The same functionality of this plugin could be achieved more transparently
-- by using the notification system.
--
-- See the log plugins for examples of that.
-- module: plugins.debugNickList
--

-- [[
local parent = core.events.nickList
function core.events.nickList(sConnectionID, sChannel, lNicks)
  local sOut = 'nickList:' .. sConnectionID .. ':' .. sChannel
  for _, s in ipairs(lNicks) do sOut = sOut .. ';' .. s end
  print(sOut)
  parent(sConnectionID, sChannel, lNicks)
end
--]]

