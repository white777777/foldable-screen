# foldable-screen
EPS8266 controlled foldable projector screen 

This project aims to automate handmade projector screen.
It uses Esp8266, 12v electric motor from Vokswagen polo, 2 relays.
At this time it have REST api with endpoits to run movement and configure timings. Also there simple web interface which uses that REST api.  

There are no position sensor, so I save amount of time required to reach upper and lower positions. I use scheduler to simplify work with delays.
