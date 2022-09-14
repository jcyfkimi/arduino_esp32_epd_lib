# arduino_esp32_epd_lib

## What is this project all about?

This is a e-ink display driver library for those epd panels that comes with low price market price tag, aka Electronic Shelf Labels(ESL), which is based on waveshare's E-Paper ESP32 Driver Board sample code, and developed with Arduino. 

## EPD Support List

1. Original EPDs which supported by Waveshare
2. SES VUSION 2.6 BWR GL420 2.66 inch epd, tested with BE2266ES0550ET7AMY01125 and TC026SC1C3-S5(AE2266ES0550EZ7B22009TY)
3. Unknow 3.71 inch epd, FPC labeled as "FPC-2303", which compactable with gooddisplay GDEY037Z03


## Connections
| ESP32 Pin |  EPD |  Description  |
| --------- | -----|  -----------  |
|    13     |  SCK |  Serial Clock |
|    14     |  SDA |  Serial Data  |
|    15     |  CS  |  Chip Select  |
|    25     | BUSY |  Busy Signal  |
|    26     |  RST |  Reset        |
|    27     |  DC  |  Data/Command |



## Change History
20220913: Repo initial, with waveshare base sample code imported.

20220913: Adding support for SES VUSION 2.6 BWR GL420 2.66 inch epd, tested with BE2266ES0550ET7AMY01125 and TC026SC1C3-S5(AE2266ES0550EZ7B22009TY)

20220914: Adding support for unknow 3.71 inch epd, FPC labeled as "FPC-2303", which compactable with gooddisplay GDEY037Z03