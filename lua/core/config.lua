--[[
  lua/core/config.lua
  default configurations for the bot, these can be overriden
--]]
local config = {}

-- purge activity history that is older than this in seconds
config.CoreTriggerPurgeOlder = 2 * 3600
config.CoreRepeaterInterval = 20 * 60

return config

