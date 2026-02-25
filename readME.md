# Automated Watering System Prototype💧🌱
This project explores the implementation of an automated water system using an ELEGOO Mega 2350 Arduino board.

### Technologies Used 🖥️
- ELEGOO Mega 2350 Arduino board
- [PlatformIO](https://platformio.org/) - [Documentation](https://docs.platformio.org/en/latest/)
- [usbip-win](https://github.com/dorssel/usbipd-win) - [Releases](https://github.com/dorssel/usbipd-win/releases)
- Soil Moisture Sensor (TBD .. [Example](https://www.amazon.ca/Moisture-Humidity-Control-Automatic-Watering/dp/B0CXYVJVLB/ref=asc_df_B0CXYVJVLB?mcid=e687bb0dd22337778f8992a42513d7c5&tag=googleshopc0c-20&linkCode=df0&hvadid=706827341480&hvpos=&hvnetw=g&hvrand=7990515298228629834&hvpone=&hvptwo=&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=9189172&hvtargid=pla-2421863996818&psc=1&hvocijid=7990515298228629834-B0CXYVJVLB-&hvexpln=0&gad_source=1))
- Integrated WSL Server to load the data. (TBD)
- Integrated Dashboard to display data and control. (TBD)

### Configuring usbip-win🔌
We’ll use usbipd-win to "bridge" the USB signal from Windows into your WSL Linux kernel
Attach the Arduino

#### Plugging in the Arduino

This is the "handshake" process. You must do this every time you plug the Arduino in (or after a reboot).

**1: Find the Bus ID** 
```
PowerShell
usbipd list
```
Identify your Arduino (it might say "USB Serial Port" or "CH340" or "Arduino Uno"). 
Note the BUSID (e.g., 2-3).

**2: Bind and Attach**

Still in Windows PowerShell Admin, run these two commands (replace 2-3 with your actual BUSID):

Bind the device (tells Windows to let go):
```
PowerShell
usbipd bind --busid 2-3
```

Attach to WSL:
```
PowerShell
usbipd attach --wsl --busid 2-3
```

### Verify in WSL
Now, go back to your WSL terminal and run:

```Bash
lsusb
```
You should now see your Arduino listed there. 

To see the actual port name PlatformIO will use (usually /dev/ttyACM0 or /dev/ttyUSB0), run:
```Bash
dmesg | tail
```

### Final Permission Fix
By default, Linux might block your user from talking to the serial port. Run this once in WSL to give yourself permission:

```Bash
sudo usermod -a -G dialout $USER
```
**Important: You must restart WSL for this to take effect. Run wsl --shutdown in PowerShell, then reopen your WSL terminal.**

### Auto-Attach
If you get tired of typing the BUSID, you can use usbipd attach --wsl --busid <ID> --auto-attach. This will keep the connection alive even if the Arduino resets during a code upload.

**Example:**

**Powershell**
```
# usbipd attach --wsl -b 2-6 --auto-attach
usbipd: info: Using WSL distribution 'Ubuntu' to attach; the device will be available in all WSL 2 distributions.
usbipd: info: Loading vhci_hcd module.
usbipd: info: Detected networking mode 'nat'.
usbipd: info: Using IP address 192.168.16.1 to reach the host.
usbipd: info: Starting endless attach loop; press Ctrl+C to quit.
WSL Monitoring host 192.168.16.1 for BUSID: 2-6
WSL 2026-02-25 19:27:09 Device 2-6 is available. Attempting to attach...
WSL 2026-02-25 19:27:09 Attach command for device 2-6 succeeded.
WSL 2026-02-25 19:27:17 Device 2-6 is now attached.
```

**WSL**
```
lsusb
Bus 001 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
Bus 001 Device 002: ID 2341:0042 Arduino SA Mega 2560 R3 (CDC ACM)
Bus 002 Device 001: ID 1d6b:0003 Linux Foundation 3.0 root hub
```

### Uploading the project to PlatformIO 

### Building the Project 🛠️

For **Release** build (optimized, no debug symbols)
```
From the /build directory:
cmake -DCMAKE_BUILD_TYPE=Release ..
```

For **Debug** build (optimized, no debug symbols)
```
From the /build directory:
cmake -DCMAKE_BUILD_TYPE=Debug ..
```

### Hard Reset (if you encounter build issues): 🔁
Clean the build environment to ensure CMake actually sees your changes.
```
cd /mnt/c/Development/Projects/C++_BlackJack/build
rm -rf *
cmake ..
make
```

### Running the Project 🚀
```
./BlackJackAI [Train-AI-or-Not] [Play-Manual-or-Not] [Display-GUI]
```

**Arguments:**

- **Train-AI:** ```0``` = Train new model, ```1``` = Load existing model
- **Play-Mode:** ```0``` = Manual play, ```1``` = AI plays
- **Display-GUI:** ```0``` = Console only, ```1``` = GUI display

**Examples:**
```
./BlackjackAI 0 0 0    # Train AI, you play manually, console only
./BlackjackAI 1 1 0    # Load AI, AI plays, console only
./BlackjackAI 0 0 1    # Train AI, you play manually, with GUI
./BlackjackAI 1 1 1    # Load AI, AI plays, with GUI
```
### How the AI works 🧠

#### Q-Learning State Definition: 
Each game state is represented by three values:

- Player's current hand total (e.g., 12-21)
- Dealer's visible card value (e.g., 2-11)
- Whether the player has an Ace that can be counted as 1 (soft hand flag)

#### Actions:
    0 = Stand (Stop drawing)
    1 = Hit (Draw another card)

#### Q-Table: 
A table that stores values for each (state, action) pair. Higher values indicate better decisions for that situation. For example, the AI learns that hitting when you have 12 and the dealer shows a 6 is generally good, so that Q-value is high.

#### Training Process:
The Silent Trainer runs 250,000 simulated hands using epsilon-greedy strategy:

- **Exploitation (80%)**: AI picks the best-known action
- **Exploration (20%)**: AI tries random actions to discover new strategies

#### After Training:
Once trained, the Q-Table is saved to a SQLite database (blackjack_brain.db). When you play, the AI looks up each state in the Q-Table and uses the learned knowledge to make decisions — no randomness, just playing optimally.

### Game Features ✨
- Interactive GUI with card images
- Manual player input (H for hit, S for stand)
- AI decision-making with trained Q-Learning model
- Blackjack detection (instant win on 21)
- Win/Loss/Tie determination
- Play multiple rounds in one session

### Game Screen
![Game Sample](/assets/docs/game_sample.png)

