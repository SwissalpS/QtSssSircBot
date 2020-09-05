--[[
	lua/core/logging.lua
	class to use for logging to files that get rotated
--]]
--- Class to use for logging to files that get rotated.
-- Usage; local oLog = Logger(tParams) oLog:log('test')
-- module: core.logging

local Object = require 'core.object'
local config = require 'core.config'

--- How many archives to make (7)
config.LoggerDefaultMaxArchives = 7
--- Maximum file size (3145728)
config.LoggerDefaultMaxFileSize = 3145728
--- Time stamp format ('%Y%m%d_%H%M%S')
config.LoggerFormatTimeStamp = '%Y%m%d_%H%M%S'
--- Line format ('%timeStamp %level %message\n')
config.LoggerFormatLine = '%timeStamp %level %message\n'

--- Class to manage a log file and writing to it.
local Logger = Object:extend()

--- initial paramaters for Logger.
-- Valid fields in table tParams are:
--    - int: iMaxArchives defaults to config.LoggerDefaultMaxArchives
--    - int: iMaxBytes defaults to config.LoggerDefaultMaxFileSize
--    - string: sFormatLine defaults to config.LoggerFormatLine
--    - string: sFormatTimeStamp defaults to config.LoggerFormatTimeStamp
--    - string: sPathFile defaults to core.temp_filename('.log')
-- table: tParams

--- Create a Logger instance.
-- 'public function'
-- Can be used like this; ``local oLog = Logger(tParams)``
-- ?tParams: tParams control parameters
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
	if 'string' == type(tParams.sFormatLine) then
		self.sFormatLine = tParams.sFormatLine
	end
	if 'string' == type(tParams.sFormatTimeStamp) then
		self.sFormatTimeStamp = tParams.sFormatTimeStamp
	end
	if 'string' == type(tParams.sPathFile) then
		self.sPathFile = tParams.sPathFile
	end

end -- Logger:new


--- reset to default values.
-- 'protected function'
-- Caution is advised.
function Logger:reset()
	self.iMaxArchives = config.LoggerDefaultMaxArchives
	self.iMaxBytes = config.LoggerDefaultMaxFileSize
	self.rFileHandle = nil
	self.sFormatLine = config.LoggerFormatLine
	self.sFormatTimeStamp = config.LoggerFormatTimeStamp
	self.sPathFile = core.temp_filename('.log')
end -- Logger:reset


--- get current systems time stamp formated.
-- 'public function'
-- ?int: iTime UNIX time stamp
-- treturn: string formated time stamp
function Logger:currentTimeStamp(iTime)
	return os.date(self.sFormatTimeStamp, iTime)
end -- Logger:currentTimeStamp


--- log a message.
-- 'public function'
-- Append a log message to log file.
-- string: sMessage the message
-- ?string: sLevel one character to indicate seriousness.
-- Can be longer, should not be.
function Logger:log(sMessage, sLevel)
	sLevel = sLevel or ' '
	local sOut = self:prepareLine(nil, self:currentTimeStamp(), sLevel, sMessage)
	self:write(sOut)
end -- Logger:log


--- log a message.
-- 'public function'
-- Same as Logger:log but details are passed in hItem hash-table. calls to
-- core.log(), core.error() and core.log_quiet() all post a notification
-- with an hItem table to 'core.log'.
function Logger:logItem(hItem)
	local sMessage = hItem.sText
	if '!' == hItem.sIcon then sMessage = sMessage .. ' at:' .. hItem.sAt end
	local sOut = self:prepareLine(nil, self:currentTimeStamp(hItem.iTime),
	hItem.sIcon, sMessage)
	self:write(sOut)
end -- Logger:logItem


--- open file for apending.
-- 'protected function'
-- treturn: ?nil|number nil on fail and file handle resource id on success
function Logger:openLogFile()
	self.rFileHandle = io.open(self.sPathFile, 'ab')
	if nil ~= self.rFileHandle then return self.rFileHandle end
	core.error('Failed to open log file for writing: ' .. self.sPathFile)
	return nil
end -- Logger:openLogFile


--- open file for apending, checking size.
-- 'protected function'
-- If size limit is reached, file is closed and archived rolling over any
-- existing archives up to set number of archives.
-- treturn: ?nil|number nil on fail and file handle resource id on success
function Logger:openRollover()
	-- if not yet open, try to open
	self.rFileHandle = self.rFileHandle or self:openLogFile()
	-- if still not open, there is a problem
	-- we don't post log messages here in case of causing recurences
	if nil == self.rFileHandle then return nil end

	-- check file size
	local iSize = self.rFileHandle:seek('end', 0)
	if (iSize < self.iMaxBytes) then
		return self.rFileHandle
	end

	-- rollover
	for i = self.iMaxArchives - 1, 1, -1 do
		os.rename(self.sPathFile .. '.' .. tostring(i),
				  self.sPathFile .. '.' .. tostring(i + 1))
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


--- prepare line for log.
-- 'protected function'
-- string: sPattern the pattern to use to compose the line
-- string: sTimeStamp the formated time stamp
-- string: sLevel the level indicator
-- string: sMessage the actual message.
-- treturn: line formated according to object's paramaters.
function Logger:prepareLine(sPattern, sTimeStamp, sLevel, sMessage)
	sLevel = sLevel or ''
	local sOut = sPattern or self.sFormatLine
	sMessage = sMessage:gsub('%%', '%%%%')
	sOut = sOut:gsub('%%timeStamp', sTimeStamp)
	sOut = sOut:gsub('%%level', sLevel)
	sOut = sOut:gsub('%%message', sMessage)
	return sOut
end -- Logger:prepareLine


--- Append line to log file.
-- 'protected function'
-- string: sLine the line to add. Strictly speaking it's a 'string to append'
-- as it is only a line if it includes cr and or lf. That being said, it can be
-- multiple lines.
function Logger:write(sLine)
	local rH = self:openRollover()
	if nil == rH then return nil end
	rH:write(sLine)
	return true
end -- Logger:write

return Logger

