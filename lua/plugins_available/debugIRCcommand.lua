--[[
  lua/plugins/debugIRCcommand.lua
  prints received IRC commands to standard out
--]]
--- prints received IRC commands to standard out.
-- Plugin that demonstrates how core methods can be overriden by plugin and
-- user modules.
--
-- The same functionality of this plugin could be achieved more transparently
-- by using the notification system.
--
-- See the log plugins for examples of that.
-- module: plugins.debugIRCcommand
--

-- [[
local parent = core.events.IRCcommand
function core.events.IRCcommand(sConnectionID, sCommand, lParams)
  local sOut = 'I:' .. sConnectionID .. ':' .. sCommand
  for _, s in ipairs(lParams) do sOut = sOut .. ';' .. s end
  print(sOut)
  parent(sConnectionID, sCommand, lParams)
end
--]]

