--[[
	lua/user/switchNick.lua
	user module that provides commands to easily change nick.
	Probably just confusing when using as a bot too. The idea was, use bot
	as proxy for a moment for manual posting to channels as some user.
--]]
-- [[
local function nickSwitch(tP)
	local sPassword, sConnectionID, sNewNick = tP.sMatch:match('(%S*)%s(%S*)%s(%S*)')
	if sPassword ~= config.RootCommandsPassword then return end
	IRC.send_nick_change_request(sConnectionID, sNewNick)
	sendDM(tP.sConnectionID, tP.sNick, 'OK:sent nick change request')
end -- nickSwitch


local function init()
	local oTrigger = core.Trigger({
		lTriggerRxs = { '^&n%s(%S*%s%S*%s%S*)' },
		fCallback = nickSwitch,
		lConnectionIDrxs = config.RootCommandsConnectionIDrxs,
		lNickRxs = config.RootCommandsNickRxs,
		hRateLimit = config.RootCommandsRateLimit,
		bIncludeInHelp = false,
		sDescription = '&n <password> <connection-ID> <new-nick>\n'
	})
	oTrigger:makeDMonly()

	core.oTriggerManager:addTrigger(oTrigger)
end -- init


-- user modules would not need this mechanism, but it's good practice so we
-- don't forget to use it when writting plugins
core.oNotificationManager:subscribe('core.init.done', init)
--]]
return true

