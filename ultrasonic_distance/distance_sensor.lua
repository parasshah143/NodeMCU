cfg={}
     cfg.ssid="sensor"
     cfg.pwd="parasshah"
wifi.setmode(wifi.SOFTAP)
wifi.ap.config(cfg)

gpio.mode(1, gpio.INPUT, gpio.PULLUP) --eco
gpio.mode(2, gpio.OUTPUT) --trigger

srv=net.createServer(net.TCP)
srv:listen(80,function(conn)

local n = 0
gpio.write(2, gpio.HIGH)
tmr.delay(10)
gpio.write(2, gpio.LOW)
local i = 0
local j = 0
print("i value is")
while(gpio.read(1) == 1) do
i = i+1
--tmr.delay(1)
tmr.wdclr()  
end

print(i)
print("j is")
print(j)
conn:send("<!DOCTYPE html><html><body><p><b>Read pin</b><br>GPIO status "..n.."</p><p><b>LOW is</b><br>LOW is "..i.."</p><p><b>HIGH is</b><br>HIGH is "..j.."</p></body></html>")
conn:on("sent",function(conn) conn:close() end)
end)
