---
title: "dev board"
github: "https://github.com/minye065/imu-dev-board"
description: "my own devboard"
created_at: "2026-10-28"
total_time: "15h"
---

# October 29, 2026: Found parts and wrote basic BOM
<!-- fabricate:entry 1611 -->

I found parts and wrote the goal and stuff down. This was just basic planning.
Here is the BOM:
![](https://fabricate.hackclub-assets.com/e799f377e8a19a05975de00f1aefbc36ff8909f0be29e2936f7ade8add26e6d9/image-png/image.png)

I used ai to source the parts after giving it the exact needs for each part, it was able to find the names and kicad downloads for them which made me not waste 2hrs trying to find 1 part (has happened before).

I'll redo the BOM at the end by checking the parts.

**Total time spent: 2h**

# October 29, 2026 (entry 2): Made the schematic
<!-- fabricate:entry 1616 -->

![](https://fabricate.hackclub-assets.com/493d73da10b61c23ff3c13a39e5454d2400cdfba52d8de55396c349e12bd1071/image-png/image.png)
Made the schematic!! It took a while due to me being new to this kind of thing and making a lot of mistakes.
![](https://fabricate.hackclub-assets.com/be0da2b0d2e20939887b5d436c5e5590547ee6ab35693e8bd11fecca522acf5c/image-png/image.png)
ERC is clear so i think im good to start on the pcb.
I really don't know how im going to make everything fit.

**Total time spent: 6h**

# October 29, 2026 (entry 3): Tried working on pcb
<!-- fabricate:entry 1631 -->

![](https://fabricate.hackclub-assets.com/3264f4bcd4234c31aaeb8a49fbdb6527fbe38d67f3e397eca51542f057732202/image-png/image.png)
Started to work on the pcb and immediately ran into a problem.
If you can't see it its the the 2 wires there
![](https://fabricate.hackclub-assets.com/3771afbba00963ae0833ef3a22e9fe2f9a53cda0590669eb58fd311b5770f864/image-png/image.png)
They just dont let the third wire go through and there is no space for anything on this design. I might have to move the parts outward but i don't want to do that.

**Total time spent: 2h**

# October 30, 2026: Made the pcb
<!-- fabricate:entry 1687 -->

This was attempt 3 of trying to route this. I tried using a fill box to resolve the many ground thingies but i don't think it did too much.
DRC is clear and everything is wired so thats great.

![](https://fabricate.hackclub-assets.com/b8fa2273a16a620335264f1dd0c578b0989ca741d2ee36c4914713dd2452f6b2/image-png/image.png)

The arrows are just the silkscreen.

![](https://fabricate.hackclub-assets.com/c16520c50b98105e877db9eb76bb344d6e8b1e53d0270675fa5021c2ac632913/image-png/image.png)
This part was the most annoying due to how thick the via's are and how many wires there had to be.

There are also small dashed lines connected to a box
![](https://fabricate.hackclub-assets.com/5785465e8833f3cd730c98e96c907d9ceb4e8b519ac976001fc459a2dca88737/image-png/image.png)

That is the exclude areas.

**Total time spent: 5h**
