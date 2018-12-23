local component = require("component")
local event = require("event")
local term = require("term")
local keyboard = require("keyboard")
local modem = component.modem
local gpu = component.gpu
local guest = ""
local times = 0

modem.open(25)
print("Listening on port 25")
while times < 5 do
  local type, _, address, _, _, message = event.pull(5, "modem")
  modem.broadcast(25, "chatping")
  print("Searching...")
  if message == "chatping" then
    guest = address
    modem.send(guest, 25, "chatping")
    goto chat
  end
  times = times + 1
end

os.exit()
::chat::
term.clear()
gpu.setForeground(0x42f448)
print("We got a connecton! with "..guest)
print("Hold ALT to send message")
gpu.setForeground(0xFFFFFF)
local waiting = true

while waiting == true do
  local type, _, _, _, _, message = event.pull(2, "modem")
  if type == "modem_message" and message ~= "Received!" then
    print("< "..message)
    modem.send(guest, 25, "Received!")
  elseif type == "modem_message" then
    gpu.setForeground(0xf1f442)
    print(message)
    gpu.setForeground(0xFFFFFF)
  end
  if keyboard.isAltDown() == true then
    waiting = false
  end
end

::send::
term.clear()
io.write("Type your message:\n")
message = io.read()
modem.send(guest, 25, message)
goto chat
