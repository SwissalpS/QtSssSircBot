local core = require "core"
local command = require "core.command"
local keymap = require "core.keymap"
local console = require "plugins.console"

command.add(nil, {
  ["project:build-documentation"] = function()
    core.log "Building..."
    console.run {
      command = "./doc/SphinxBreatheExhaleDocutils/build.sh",
      file_pattern = "(.*):(%d+):(%d+): (.*)$",
      on_complete = function() core.log "Build complete" end,
    }
  end
})

keymap.add { ["ctrl+b"] = "project:build-documentation" }

