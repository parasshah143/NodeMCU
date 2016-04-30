wifi.setmode(wifi.STATION)
wifi.sta.config("paras","123456789")
print(wifi.sta.getip())

outpin=3                       -- Select output pin - GPIO0 3v
       gpio.mode(outpin,gpio.OUTPUT)

inpin=6                        -- Select input pin - GPIO12  5v
       gpio.mode(inpin,gpio.INT,gpio.PULLUP)  -- attach interrupt to inpin

         function motion()
           print("Motion Detected!") 
          gpio.write(outpin,gpio.HIGH)  -- Led ON - Motion detected
          tmr.delay(50000)           -- delay time for marking the movement
         gpio.write(outpin,gpio.LOW)   -- Led OFF
      end

          gpio.trig(6,"up",motion)

srv=net.createServer(net.TCP)
  srv:listen(80,
     function(conn)
        conn:on("receive",function(conn,payload) print(payload)
        conn:send("HTTP/1.1 200 OK\n\n")
        conn:send("<META HTTP-EQUIV=\"REFRESH\" CONTENT=\"2\">")
        conn:send("<html><title>PIR Motion Detector Server - ESP8266</title><body>")
        conn:send("<h1>PIR Motion Detector Server - ESP8266</h1><BR>")
        conn:send('Status: ')
            if (stat == "ON") then conn:send('<B><font color=red>Movement Detected!</font></B>')
            elseif (stat == "OFF") then conn:send('<B><font color=green>No Movement</font></B>')
              else                      
                conn:send(stat)
                conn:send('%')
            end
           conn:send("<BR><BR><br>Node.HEAP : <b>" .. node.heap() .. "</b><BR><BR>")
           conn:send("IP ADDR    : <b>".. wifi.sta.getip() .. "</b><BR>")
           conn:send("TMR.NOW    : <b>" .. tmr.now() .. "</b><BR<BR><BR>")
           conn:send("</html></body>")
           conn:on("sent",function(conn) conn:close() end)
      end)
end)

 function motion()
           print("Motion Detection : ON!") 
          stat = "ON" 
          gpio.write(outpin,gpio.HIGH)  -- Led ON - Motion detected
         gpio.trig(6,"down",nomotion)  -- trigger on falling edge
        return stat
     end

  function nomotion()
         print("Motion Not Detection : OFF!") 
         stat = "OFF" 
        gpio.write(outpin,gpio.LOW)   -- Led OFF 
        gpio.trig(6,"up",motion)  -- trigger on rising edge
       return stat
   endwifi.setmode(wifi.STATION)
wifi.sta.config("paras","123456789")
print(wifi.sta.getip())

outpin=3                       -- Select output pin - GPIO0 3v
       gpio.mode(outpin,gpio.OUTPUT)

inpin=6                        -- Select input pin - GPIO12  5v
       gpio.mode(inpin,gpio.INT,gpio.PULLUP)  -- attach interrupt to inpin

         function motion()
           print("Motion Detected!") 
          gpio.write(outpin,gpio.HIGH)  -- Led ON - Motion detected
          tmr.delay(50000)           -- delay time for marking the movement
         gpio.write(outpin,gpio.LOW)   -- Led OFF
      end

          gpio.trig(6,"up",motion)

srv=net.createServer(net.TCP)
  srv:listen(80,
     function(conn)
        conn:on("receive",function(conn,payload) print(payload)
        conn:send("HTTP/1.1 200 OK\n\n")
        conn:send("<META HTTP-EQUIV=\"REFRESH\" CONTENT=\"2\">")
        conn:send("<html><title>PIR Motion Detector Server - ESP8266</title><body>")
        conn:send("<h1>PIR Motion Detector Server - ESP8266</h1><BR>")
        conn:send('Status: ')
            if (stat == "ON") then conn:send('<B><font color=red>Movement Detected!</font></B>')
            elseif (stat == "OFF") then conn:send('<B><font color=green>No Movement</font></B>')
              else                      
                conn:send(stat)
                conn:send('%')
            end
           conn:send("<BR><BR><br>Node.HEAP : <b>" .. node.heap() .. "</b><BR><BR>")
           conn:send("IP ADDR    : <b>".. wifi.sta.getip() .. "</b><BR>")
           conn:send("TMR.NOW    : <b>" .. tmr.now() .. "</b><BR<BR><BR>")
           conn:send("</html></body>")
           conn:on("sent",function(conn) conn:close() end)
      end)
end)

 function motion()
           print("Motion Detection : ON!") 
          stat = "ON" 
          gpio.write(outpin,gpio.HIGH)  -- Led ON - Motion detected
         gpio.trig(6,"down",nomotion)  -- trigger on falling edge
        return stat
     end

  function nomotion()
         print("Motion Not Detection : OFF!") 
         stat = "OFF" 
        gpio.write(outpin,gpio.LOW)   -- Led OFF 
        gpio.trig(6,"up",motion)  -- trigger on rising edge
       return stat
   end