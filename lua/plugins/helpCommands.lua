--[[
  lua/plugins/helpCommands.lua
  provides commands to display help
--]]
--do return false end
-- [[
local core = require 'core'
local config = require 'core.config'

-- change these in your user/init.lua

local sendDM = IRC.send_direct_message
local oTM = core.oTriggerManager

local function checkTrigger(oTrigger, sConnectionID, sChannel, sNick, bIncludeAll)

  if not oTrigger.bIncludeInHelp then return false end
  if nil == oTrigger.sDescription then return false end
  if 0 == #oTrigger.sDescription then return false end

  if bIncludeAll then return true end

	if not oTM:hasConnectionIDmatch(sConnectionID, oTrigger.lConnectionIDrxs) then return false end
	if not oTM:hasNickMatch(sNick, oTrigger.lNickRxs) then return false end
	if not oTM:hasPermissions(sConnectionID, sNick, oTrigger.lPermissions) then return false end

  return true

end -- checkTrigger


local function helpCommands(tP)
  local bIncludeAll = 'all' == tP.sMatch
  local sOut = ''

	for _, oTrigger in ipairs(oTM.lTriggers) do
		if (checkTrigger(oTrigger, tP.sConnectionID, tP.sChannel, tP.sNick, bIncludeAll)) then
		  sOut = sOut .. oTrigger.sDescription .. '\n'
		end
	end -- loop all triggers

  sendDM(tP.sConnectionID, tP.sNick, sOut)

end -- helpCommands

local function init()
  local oTrigger = core.Trigger({
    lTriggerRxs = {
      '^!help%s(%S*)$', '^!(help)$' },
    fCallback = helpCommands,
    bIncludeInHelp = true,
    sDescription = '!help[ all]\n'
      .. '  lists help texts to commands available to user.\n'
      .. 'with option "all", lists all commands that are visible to help system.'
  })
  oTrigger:makeDMonly()

  core.oTriggerManager:addTrigger(oTrigger)
end -- init

-- not important for this plugin
core.oNotificationManager:subscribe('core.init.done', init)
--]]

