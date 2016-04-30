cfg={}
     cfg.ssid="myssid"
     cfg.pwd="taralshah"
wifi.setmode(wifi.SOFTAP)
wifi.ap.config(cfg)

gpio.mode(1, gpio.INPUT, gpio.PULLUP)
gpio.mode(2, gpio.INPUT, gpio.PULLUP)

srv=net.createServer(net.TCP)
srv:listen(80,function(conn)

local n0 = 0
n0 = gpio.read(1)
print(n0)
conn:send("<!DOCTYPE html><html><head><meta http-equiv=\"refresh\" content=\"3\"></head><body><p><b>Read pin</b><br>GPIO0 status "..n0.."</p></body></html>")

local n1=0
n1 = gpio.read(2)
print(n1)n0 = gpio.read(1)
print(n0)
conn:send("<!DOCTYPE html><html><head><meta http-equiv=\"refresh\" content=\"3\"></head><body><p><b>Read pin</b><br>GPIO0 status "..n0.."</p></body></html>")

conn:send("<!DOCTYPE html><html><head><meta http-equiv=\"refresh\" content=\"3\"></head><body><p><b>Read pin</b><br>GPIO0 status "..n1.."</p></body></html>")

conn:on("sent",function(conn) conn:close() end)
end
)
