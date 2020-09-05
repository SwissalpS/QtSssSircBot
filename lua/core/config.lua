--[[
  lua/core/config.lua
  default configurations for the bot, these can be overriden
--]]
---
-- default configurations for the bot, these can be overriden.
-- your user module or plugins may add and override settings in config.
-- module: core.config

local config = {}

--- purge activity history that is older than this in seconds
config.CoreTriggerPurgeOlder = 2 * 3600
--- How often to run/check cleanup jobs like activity history in trigger system.
config.CoreRepeaterInterval = 20 * 60

return config

