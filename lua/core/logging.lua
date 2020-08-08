--[[
	lua/core/logging.lua
	class to use for logging to files that get rotated
--]]
local Object = require 'core.object'
local config = require 'core.config'

config.LoggerDefaultMaxArchives = 7
config.LoggerDefaultMaxFileSize = 3072
config.LoggerTimeStampFormat = '%Y%m%d_%H%M%S'
config.LoggerLineFormat = '%timeStamp %level %message\n'

local Logger = Object:extend()

function Logger:new(tParams)
	self:reset()
	if 'table' ~= type(tParams) then
		-- using just defaults with random log-file
		return
	end

	if ('number' == type(tParams.iMaxArchives)) and (0 < tParams.iMaxArchives) then
		self.iMaxArchives = tParams.iMaxArchives
	end
	if ('number' == type(tParams.iMaxBytes)) and (0 < tParams.iMaxBytes) then
		self.iMaxBytes = tParams.iMaxBytes
	end
	if 'string' == type(tParams.sPathFile) then
		self.sPathFile = tParams.sPathFile
	end

end -- Logger:new


function Logger:reset()
	self.iMaxArchives = config.LoggerDefaultMaxArchives
	self.iMaxBytes = config.LoggerDefaultMaxFileSize
	self.rFileHandle = nil
	self.sPathFile = core.temp_filename('.log')
end -- Logger:reset


function Logger:currentTimeStamp(iTime)
	return os.date(config.LoggerTimeStampFormat, iTime)
end -- Logger:currentTimeStamp


function Logger:log(sMessage, sLevel)
	sLevel = sLevel or ' '
	local sOut = self:prepareLine(nil, self:currentTimeStamp(), sLevel, sMessage)
	self:write(sOut)
end -- Logger:log


function Logger:logItem(hItem)
	local sMessage = hItem.sText
	if '!' == hItem.sIcon then sMessage = sMessage .. ' at:' .. hItem.sAt end
	local sOut = self:prepareLine(nil, self:currentTimeStamp(hItem.iTime),
	hItem.sIcon, sMessage)
	self:write(sOut)
end -- Logger:logItem


function Logger:openLogFile()
	self.rFileHandle = io.open(self.sPathFile, 'ab')
	if nil ~= self.rFileHandle then return self.rFileHandle end
	core.error('Failed to open log file for writing: ' .. self.sPathFile)
	return nil
end -- Logger:openLogFile


function Logger:openRollover()
	if nil == self.rFileHandle then
		return self:openLogFile()
	end -- if not yet open

	local iSize = self.rFileHandle:seek('end', 0)
	if (self.iSize < self.iMaxBytes) then
		return self.rFileHandle
	end

	-- rollover
	for i = self.iMaxArchives - 1, 1, -1 do
		os.rename(self.sPathFile .. '.' .. tostring(i),
				  self.sPathFile .. '.' .. tostring(i + 1)
	end

	self.rFileHandle:close()
	self.rFileHandle = nil
	local _, sMessage = os.rename(self.sPathFile, self.sPathFile .. '.1')
	if sMessage then
		core.error('Failed to rollover ' .. self.sPathFile .. ' : '
				   .. sMessage)
		return nil
	end -- if fail

	return self:openLogFile()

end -- Logger:openRollover


function Logger:prepareLine(sPattern, sTimeStamp, sLevel, sMessage)
	local sOut = sPattern or config.LoggerLineFormat
	sMessage = sMessage:gsub('%%', '%%%%')
	sOut = sOut:gsub('%%timeStamp', sTimeStamp)
	sOut = sOut:gsub('%%level', sLevel)
	sOut = sOut:gsub('%%message', sMessage)
	return sOut
end -- Logger:prepareLine


function Logger:write(sLine)
	local rH = self.openRollover()
	if nil == rH then return nil end
	rH:write(sLine)
	return true
end -- Logger:write

return Logger

