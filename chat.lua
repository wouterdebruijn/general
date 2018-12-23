local component = require("component")
local event = require("event")
local term = require("term")
local keyboard = require("keyboard")
local modem = component.modem
local gpu = component.gpu
local guest = ""

modem.open(25)
print("Listening on 25")
io.write("Do you want to send (1) or receive (2)")
if io.read() == "2" then
  print("Waiting for broadcast from other pc...")
  local _, _, address = event.pull("modem")
  print("Got something from: "..address)
  print("Saying hello...")
  guest = address
  modem.send(guest, 25, "Hello")
  goto chat
else
  print("Sending broadcast every 10 secondes. 5 times")
  print("Press Ctrl+Alt+C to cancel")
  local times = 0
  while times < 5 do
    modem.broadcast(25, "Pinging for chat")
    local message, _, address = event.pull(10)
    if message == "modem_message" then
      guest = address
      goto chat
    end
    times = times + 1
  end
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
  local _, _, _, _, _, message = event.pull("modem")
  print("< "..message)
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
