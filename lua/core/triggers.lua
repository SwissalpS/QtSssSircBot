--[[
  lua/core/triggers.lua
  infrastructure to define and maintain trigger sequences
  Trigger: mandatory params for new instance are:
	lTriggerRxs: minimum one regular expression that matches in message
	fCallback: function that will be called if any in lTriggerRxs match
   Hint: use convenience methods to make direct message only or channel message
	only or mixed mode.
  Manager:
	TODO: add logic that removes stale entries in hRequests
--]]
local Object = require 'core.object'
local config = require 'core.config'
local core = require 'core'

-- n requests within x seconds
config.TriggerDefaultRequestRateLimit = { iRequests = 180, iDuring = 3600 }
-- trigger also reacts to DMs if set to true
config.TriggerDefaultDirectMessagesAlso = false
-- trigger only reacts to DMs if set to true
config.TriggerDefaultDirectMessagesOnly = false
-- if true, description is used in help listings
config.TriggerDefaultIncludeInHelp = false
-- trigger only reacts if connectionID matches any of these
config.TriggerDefaultConnectionIDrxs = { '.*' }
-- trigger only reacts if channel matches any of these
config.TriggerDefaultChannelRxs = { '.*' }
-- trigger only reacts if nick matches any of these
config.TriggerDefaultNickRxs = { '^[~&@%+]?(.*)' }
-- TODO: may be handled differently depending on how it goes
-- trigger only reacts if nick has any of these permission sets
config.TriggerDefaultPermissions = { }

local Trigger = Object:extend()

function Trigger:new(tParams)
	self:reset()
	if 'table' ~= type(tParams) then
		core.log('error creating trigger without params')
		return
	end

	if 'function' ~= type(tParams.fCallback) then
		core.log('error creating trigger without callback')
		return
	end
	self.fCallback = tParams.fCallback

	if ('table' ~= type(tParams.lTriggerRxs)) or (0 == #tParams.lTriggerRxs) then
		core.log_quiet('error creating trigger without trigger regexes')
		return
	end
	self.lTriggerRxs = tParams.lTriggerRxs

	if nil ~= tParams.bDirectMessagesAlso then self.bDirectMessagesAlso = tParams.bDirectMessagesAlso end
	if nil ~= tParams.bDirectMessagesOnly then self.bDirectMessagesOnly = tParams.bDirectMessagesOnly end
	if nil ~= tParams.bIncludeInHelp then self.bIncludeInHelp = tParams.bIncludeInHelp end
	if 'table' == type(tParams.lConnectionIDrxs) then self.lConnectionIDrxs = tParams.lConnectionIDrxs end
	if 'table' == type(tParams.lChannelRxs) then self.lChannelRxs = tParams.lChannelRxs end
	if 'table' == type(tParams.lNickRxs) then self.lNickRxs = tParams.lNickRxs end
	if 'table' == type(tParams.lPermissions) then self.lPermissions = tParams.lPermissions end
	if 'table' == type(tParams.hRateLimit) then self.hRateLimit = tParams.hRateLimit end
	if 'string' == type(tParams.sDescription) then self.sDescription = tParams.sDescription end

end -- Trigger:new


function Trigger:reset()
	self.bDirectMessagesAlso = config.TriggerDefaultDirectMessagesAlso
	self.bDirectMessagesOnly = config.TriggerDefaultDirectMessagesOnly
	self.bIncludeInHelp = config.TriggerDefaultIncludeInHelp
	self.fCallback = nil
	self.hRateLimit = config.TriggerDefaultRequestRateLimit
	self.lConnectionIDrxs = config.TriggerDefaultConnectionIDrxs
	self.lChannelRxs = config.TriggerDefaultChannelRxs
	self.lNickRxs = config.TriggerDefaultNickRxs
	self.lPermissions = config.TriggerDefaultPermissions
	self.lTriggerRxs = {}
	self.sDescription = ''
end -- Trigger:reset


function Trigger:makeCMonly()
	self.bDirectMessagesAlso = false
	self.bDirectMessagesOnly = false
end -- Trigger:makeCMonly


function Trigger:makeCMandDM()
	self.bDirectMessagesAlso = true
	self.bDirectMessagesOnly = false
end -- Trigger:makeCMandDM


function Trigger:makeDMonly()
	self.bDirectMessagesAlso = true
	self.bDirectMessagesOnly = true
end -- Trigger:makeDMonly

--------------------------------------------------------------------------

local Manager = Object:extend()

function Manager:new()
	self:reset()
end -- Manager:new


function Manager:reset()
	self.hRequests = {}
	self.lTriggers = {}
end -- Manager:reset


function Manager:addTrigger(oTrigger)
	-- TODO: check that valid trigger object
	table.insert(self.lTriggers, oTrigger)

end -- Manager:addTrigger

function Manager:hasChannelMatch(sChannel, lChannelRxs)
	for _, sRegX in ipairs(lChannelRxs) do
		if string.match(sChannel, sRegX) then return true end
	end
	return false
end -- Manager:hasChannelMatch

function Manager:hasConnectionIDmatch(sConnectionID, lConnectionIDrxs)
	for _, sRegX in ipairs(lConnectionIDrxs) do
		if string.match(sConnectionID, sRegX) then return true end
	end
	return false
end -- Manager:hasConnectionIDmatch


function Manager:hasNickMatch(sNick, lNickRxs)
	for _, sRegX in ipairs(lNickRxs) do
		if string.match(sNick, sRegX) then return true end
	end
	return false
end -- Manager:hasNickMatch


function Manager:hasPermissions(sConnectionID, sNick, lPermissions)
	-- TODO: actually implement an auth system
	return true
end -- Manager:hasPermissions


function Manager:hasTriggerMatch(sMessage, lTriggerRxs)
	local sMatch
	for _, sRegX in ipairs(lTriggerRxs) do
		sMatch = string.match(sMessage, sRegX)
		if sMatch then return sMatch end
	end
	return false
end -- Manager:hasTriggerMatch


function Manager:isInRequestLimit(sNick, hRateLimit)
	local sN = core.cleanNick(sNick)
	local tNick = self.hRequests[sN]
	if nil == tNick then return true end
	if #tNick < hRateLimit.iRequests then return true end
	local iNow = os.time()
	-- no need to check any before iLast
	local iLast = iNow - hRateLimit.iDuring
	local iCount = 0
	for i = #tNick, 1, -1 do
		if tNick[i] < iLast then break end
		iCount = iCount + 1
	end

	return iCount < hRateLimit.iRequests

end -- Manager:isInRequestLimit


function Manager:recordRequest(sNick)
	local sN = core.cleanNick(sNick)
	if nil == self.hRequests[sN] then self.hRequests[sN] = {} end
	table.insert(self.hRequests[sN], os.time())
end -- Manager:recordRequest


function Manager:checkTrigger(oTrigger, sConnectionID, sChannel, sNick, sMessage)
	local isDM = nil == sChannel
	if isDM and (not oTrigger.bDirectMessagesAlso) then return end
	if (not isDM) and oTrigger.bDirectMessagesOnly then return end
	if not self:hasConnectionIDmatch(sConnectionID, oTrigger.lConnectionIDrxs) then return end
	if not self:hasNickMatch(sNick, oTrigger.lNickRxs) then return end
	if not isDM then
		if not self:hasChannelMatch(sChannel, oTrigger.lChannelRxs) then return end
	end
	if not self:hasPermissions(sConnectionID, sNick, oTrigger.lPermissions) then return end

	local sMatch = self:hasTriggerMatch(sMessage, oTrigger.lTriggerRxs)
	if false ~= sMatch then

	  -- drop request if user is over limit
		if not self:isInRequestLimit(sNick, oTrigger.hRateLimit) then return end

		-- keep track of requests
		self:recordRequest(sNick)

		core.try(oTrigger.fCallback, {
			sChannel = sChannel,
			sConnectionID = sConnectionID,
			sMatch = sMatch,
			sMessage = sMessage,
			sNick = sNick
		})

	end -- if found a trigger match
end -- Manager:checkTrigger


function Manager:checkTriggers(sConnectionID, sChannel, sNick, sMessage)
	for _, oTrigger in ipairs(self.lTriggers) do
		self:checkTrigger(oTrigger, sConnectionID, sChannel, sNick, sMessage)
	end -- loop all triggers
end -- Manager:checkTriggers

return { Trigger, Manager }

