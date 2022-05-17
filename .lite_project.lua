local config = require "core.config"
local command = require "core.command"
local keymap = require "core.keymap"
local run = require "plugins.run"

config.run.project = "mingw32-make && bin\\main.exe"
command.add(nil, {
  ["run:project"] = run.build "project",
})

keymap.add {
  ["shift+f5"] = "run:project",
}
