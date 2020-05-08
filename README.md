# foldable-screen
EPS8266 controlled foldable projector screen 

This project aims to automate handmade projector screen.
It uses Esp8266, 12v electric motor from Vokswagen polo, 2 relays.
At this time it have REST api with endpoits to run movement and configure timings. Also there simple web interface which uses that REST api.  

There are no position sensor, so I save amount of time required to reach upper and lower positions. I use scheduler to simplify work with delays.

# Plans:
* Add MQTT support
* Add Wifi configuration support from web interface
* Add some Zeroconf protocol support

* Create android app for projector wich will unroll screen when projector startup and roll back when it is off. 

* Mount RGB led strip to screen case and some code to control it

* Improve Web part using React or Angular


