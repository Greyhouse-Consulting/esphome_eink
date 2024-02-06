# LilyGo T5 based ESPHome e-ink for Home Assistant

A project to show the temperature and forcast using the lily go T5 plus 4.7 inch e-ink display

Goal is to make a low power display with weather forcast. In this case the [SMHI](https://www.home-assistant.io/integrations/smhi/) weather servie in home assistant is beeing used. 



![Case image](https://github.com/Greyhouse-Consulting/esphome_eink/blob/main/case.JPG "The case")

Case used in this project : https://www.thingiverse.com/thing:5589557


TODO

1. Move battery indicator to top right
2. Reduce size fontsize of precipitation and round not just trunc
3. Remove DynamicJsonDocument and replace with JsonDocument

This project have been heavily inspired by the following repos

https://github.com/kotope/esphome_eink_dashboard

https://gist.github.com/geekuillaume/f412c634637039f17a5bef2e588f5a33