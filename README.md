# FreeShopNX
Nintendo Switch CDN title installer, based on Rei's BogInstaller (evolution of eNXhop)
\
\
\
Screenshots:

<img src="https://user-images.githubusercontent.com/18319133/43985398-37045f4a-9cd5-11e8-8cc0-0e16777dee86.png" width="50%"><img src="https://user-images.githubusercontent.com/18319133/43985416-51bc5e46-9cd5-11e8-8eb3-962dff665cc7.png" width="50%">
<img src="https://user-images.githubusercontent.com/18319133/43985627-c2179100-9cd6-11e8-8f0e-edbf64e012b6.png" width="50%"><img src="https://user-images.githubusercontent.com/18319133/43985646-e2ba4696-9cd6-11e8-8226-0b21be6a5350.png" width="50%">
\
\
\
Requirements:
* Sig Patches
* `sdmc:/switch/FreeShopNX/FreeShopNX.nro`
* `sdmc:/switch/FreeShopNX/FreeShopNX.txt`
* `sdmc:/switch/FreeShopNX/info.json`
* `sdmc:/switch/FreeShopNX/config.conf`
\
\
\
`FreeShopNX.txt` contains the Rights ID, Title Key, and Title Name of any titles you wish to install, verticle pipe separated. Example below:

```
01001de0050120000000000000000003|XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX|Quest of Dungeons
01002b30028f60000000000000000004|XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX|Celeste
01000000000100000000000000000003|XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX|Super Mario Odyssey
01007ef00011e0000000000000000000|XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX|The Legend of Zelda: Breath of the Wild

```
Rights IDs are the 16 character Title ID followed by the Master Key revision the game uses padded with leading zeros to 16 characters. Rights IDs can be obtained from the games CNMT or by using hactool on the game's NCA.

No Title Keys or completed `FreeShopNX.txt` file will be provided from this repo. 
\
\
\
`info.json` contains information aboout the games to display on the game info screen. Minimal example game:
```
{
    "01000320000cc0000000000000000000": {
        "titleid": "01000320000cc000",
        "release_date_string": "Mar 03, 2017",
        "title": "1-2-Switch™",
        "category": [
            "Party"
        ],
        "number_of_players": "up to 2 players",
        "intro": "Lock eyes with your opponent and discover the many functions of the Joy-Con in 1-2-Switch!",
        "description": "This is the first Nintendo video game in which players compete in fun activities while staring into each other’s eyes instead of at the TV screen. From Wild West gunslinging to copying each other’s dance moves, the games in 1-2-Switch make creative use of a broad variety of Nintendo Switch features to liven up parties with anyone, anywhere, at any time.",
        "languages": [
            "Japanese",
            "English",
            "Spanish",
            "French",
            "German",
            "Italian",
            "Dutch",
            "Russian"
        ],
        "size": 1496317952
    }
}
```
This file can be auto-generated using the python3 script here: https://gitlab.com/pandanopants/gameinfo
\
\
\
`config.conf` contains the URLs for auto-updating the `FreeShopNX.txt` and `info.json` file. Layout is as follows:
```
title_info_url=http://...
title_key_url=http://...
```
`title_info_url` for the info.json file and `title_key_url` for the FreeShopNX.txt file. Note that the Switch's libCurl implementation does not currently support HTTPS URLs, so only HTTP addresses can be downloaded from at the moment. 
\
\
\
Credits to Adubbz for Tik and Cert installation with TinFoil, XorTroll for the eNXhop base and UI, Reisyukaku for on-system title installation via BogInstaller, AmiiboUGC for the icon, Panda for the scraped JSON info, yellows8 for the HBMenu sysfont code and theme support, and megasharer for acquisition of difficult to find resources.

Note: You'll probably get banned. 
