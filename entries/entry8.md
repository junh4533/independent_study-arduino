## Entry 8: Connecting to the wifi part 2

AT+CIPMODE=0
AT+CIPMUX=1
AT+CIPSTART=0,"TCP","api.thingspeak.com",80
AT+CIPSEND=0,110
GET https://api.thingspeak.com/apps/thinghttp/send_request?api_key=GHMJY16ZJ5XHOMYC HTTP/1.1

https://www.yahoo.com/news/weather/united-states/michigan/brooklyn-2369943
