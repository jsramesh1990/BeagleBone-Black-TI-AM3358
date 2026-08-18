Yes. Since you are making this as a **real BeagleBone Black – TI AM3358 embedded Linux project**, `boot_flow.md` is useful as a separate document.

Your `docs/` can now be:

```text
docs/
├── architecture.md
└── boot_flow.md
```

# `docs/boot_flow.md`

Use this content:

````markdown
# BeagleBone Black TI AM3358 Boot Flow

## 1. Overview

This document describes the complete boot sequence of the
BeagleBone Black based on the TI AM3358 processor.

The boot process starts when power is applied to the board and
ends when the Box Storage application is started by systemd.

```text
Power ON
   |
   v
TI AM3358 Boot ROM
   |
   v
SPL / MLO
   |
   v
U-Boot
   |
   v
Linux Kernel
   |
   v
Device Tree
   |
   v
Root Filesystem
   |
   v
systemd
   |
   v
box-storage.service
   |
   v
Box Storage Application
````

---

# 2. Hardware Boot Components

The main components involved in boot are:

| Component           | Responsibility                      |
| ------------------- | ----------------------------------- |
| TI AM3358 ROM       | Initial hardware boot               |
| MLO / SPL           | Initializes DDR and prepares U-Boot |
| U-Boot              | Loads kernel and Device Tree        |
| Linux Kernel        | Initializes Linux                   |
| Device Tree         | Describes board hardware            |
| RootFS              | Provides Linux userspace            |
| systemd             | Starts userspace services           |
| box-storage.service | Starts application                  |
| boxes               | Box Storage application             |

---

# 3. Power-On

When power is applied:

```text
Power
  |
  v
PMIC / Power Rails
  |
  v
TI AM3358
  |
  v
CPU Reset
```

The processor comes out of reset and begins execution from
the internal Boot ROM.

At this point Linux is not running.

There is no:

```text
systemd
shell
application
filesystem
```

yet.

---

# 4. TI AM3358 Boot ROM

The TI AM3358 contains internal Boot ROM code.

The Boot ROM is responsible for the first stage of the boot process.

Conceptually:

```text
TI AM3358
    |
    v
Boot ROM
    |
    +--> Determine boot mode
    |
    +--> Check boot device
    |
    +--> Load boot image
    |
    v
SPL / MLO
```

The boot device can be selected according to the board's
boot configuration.

For this project, the main deployment targets are:

```text
microSD
eMMC
```

---

# 5. MLO / SPL

MLO is the boot image containing the first-stage loader,
commonly based on U-Boot SPL.

The AM3358 Boot ROM loads MLO into internal memory and
starts execution.

```text
Boot ROM
   |
   v
MLO / SPL
```

SPL has a limited responsibility.

It prepares enough hardware for the next boot stage.

Typical responsibilities include:

```text
SPL
 |
 +--> CPU initialization
 |
 +--> Clock initialization
 |
 +--> DDR initialization
 |
 +--> Basic peripheral initialization
 |
 +--> Load U-Boot
 |
 v
U-Boot
```

---

# 6. DDR Initialization

One important responsibility of SPL is initializing external DDR memory.

Before DDR initialization:

```text
CPU
 |
 +--> Internal memory
```

After DDR initialization:

```text
CPU
 |
 +--> Internal memory
 |
 +--> External DDR
```

This allows larger programs such as U-Boot and Linux to execute
using external RAM.

---

# 7. U-Boot

After SPL completes its work, U-Boot starts.

```text
Boot ROM
   |
   v
MLO / SPL
   |
   v
U-Boot
```

U-Boot is the main bootloader.

Its responsibilities include:

* Initialize additional hardware
* Initialize storage
* Read environment
* Select boot configuration
* Load Linux kernel
* Load Device Tree
* Pass boot arguments
* Start Linux

---

# 8. U-Boot Environment

U-Boot can use environment variables to control the boot process.

Examples include:

```text
bootcmd
bootargs
bootdelay
boot_targets
```

Conceptually:

```text
U-Boot
   |
   v
Environment
   |
   +--> bootcmd
   |
   +--> bootargs
   |
   +--> boot target
```

The actual environment location depends on the board/BSP
configuration.

---

# 9. Kernel Loading

U-Boot loads the Linux kernel into DDR.

Conceptually:

```text
SD/eMMC
   |
   | Linux Kernel
   v
DDR Memory
   |
   v
U-Boot
```

Typical kernel artifact:

```text
Image
```

or another kernel image format depending on the BSP.

---

# 10. Device Tree Loading

U-Boot also loads the Device Tree Blob.

For the BeagleBone Black, a board-specific DTB is used.

Example:

```text
am335x-boneblack.dtb
```

The Device Tree describes hardware such as:

```text
CPU
DDR
UART
I2C
SPI
GPIO
MMC
USB
Ethernet
Audio
Display
```

Conceptually:

```text
U-Boot
 |
 +----> Linux Kernel
 |
 +----> Device Tree
 |
 v
Start Linux
```

---

# 11. Kernel Command Line

U-Boot passes boot arguments to Linux.

A key argument is normally the root filesystem location.

Conceptually:

```text
bootargs
   |
   +--> console
   |
   +--> root filesystem
   |
   +--> filesystem type
   |
   +--> other kernel parameters
```

Example concept:

```text
console=ttyO0,115200
root=/dev/mmcblk0p2
```

The exact console device and root device depend on the BSP/kernel configuration.

---

# 12. Starting Linux

U-Boot transfers control to the Linux kernel.

```text
U-Boot
   |
   | boot
   v
Linux Kernel
```

At this point U-Boot's main job is finished.

Linux begins executing.

---

# 13. Linux Kernel Initialization

The kernel performs many initialization tasks.

Conceptually:

```text
Linux Kernel
    |
    +--> CPU initialization
    |
    +--> Memory management
    |
    +--> Scheduler
    |
    +--> Interrupt subsystem
    |
    +--> Device model
    |
    +--> Driver initialization
    |
    +--> Filesystem initialization
    |
    +--> Device Tree parsing
    |
    v
Userspace
```

---

# 14. Device Tree Parsing

The kernel receives the Device Tree from U-Boot.

It parses the hardware description.

Example:

```text
Device Tree
     |
     +--> UART
     +--> GPIO
     +--> I2C
     +--> SPI
     +--> MMC
     +--> USB
     +--> Ethernet
```

The kernel uses this information to instantiate
and configure hardware devices and their drivers.

---

# 15. Root Filesystem

After kernel initialization, Linux mounts the root filesystem.

Conceptually:

```text
Linux Kernel
     |
     v
Root filesystem
     |
     +--> /bin
     +--> /etc
     +--> /lib
     +--> /usr
     +--> /var
     +--> /opt
```

For this project:

```text
/opt/box-storage/
    |
    +--> bin/
    |     |
    |     +--> boxes
    |
    +--> config/
          |
          +--> boxes.conf
          +--> logging.conf
```

---

# 16. init / systemd

Once the kernel starts userspace, the init system starts.

For this project, systemd is used.

```text
Linux Kernel
     |
     v
systemd
```

systemd manages:

* Services
* Mounts
* Logging
* Targets
* Dependencies
* Application startup

---

# 17. box-storage.service

The project provides:

```text
deployment/systemd/
└── box-storage.service
```

During deployment it is installed into:

```text
/etc/systemd/system/
└── box-storage.service
```

systemd reads the service definition.

Conceptually:

```text
systemd
   |
   v
box-storage.service
   |
   v
ExecStart
   |
   v
/opt/box-storage/bin/boxes
```

---

# 18. Application Startup

The service starts the Box Storage executable.

```text
box-storage.service
        |
        v
/opt/box-storage/bin/boxes
        |
        v
main()
```

Then:

```text
main()
  |
  v
BoxManager
  |
  +--------+--------+
  |                 |
  v                 v
StaticBox       DynamicBox
```

---

# 19. Configuration Loading

The application reads its configuration.

```text
/opt/box-storage/config/
        |
        +--> boxes.conf
        |
        +--> logging.conf
```

Conceptually:

```text
boxes
 |
 +--> boxes.conf
 |       |
 |       v
 |   Storage configuration
 |
 +--> logging.conf
         |
         v
     Logging configuration
```

---

# 20. Application Runtime

Once initialized:

```text
                    boxes
                      |
                      v
                 BoxManager
                      |
             +--------+--------+
             |                 |
             v                 v
         StaticBox         DynamicBox
             |                 |
             v                 v
          std::array       std::vector
             |                 |
             +--------+--------+
                      |
                      v
                    Entry
                      |
                      v
                     Date
```

The application can then process commands such as:

```bash
./boxes add dynamic 2026-08-18 "Temperature=32C"
```

```bash
./boxes get dynamic 2026-08-18
```

```bash
./boxes range dynamic 2026-08-01 2026-08-18
```

---

# 21. Complete Boot Timeline

The complete sequence is:

```text
1. Power ON
       |
       v
2. TI AM3358 reset
       |
       v
3. Boot ROM executes
       |
       v
4. Boot device selected
       |
       v
5. MLO/SPL loaded
       |
       v
6. SPL initializes DDR
       |
       v
7. SPL loads U-Boot
       |
       v
8. U-Boot starts
       |
       v
9. U-Boot reads environment
       |
       v
10. U-Boot selects boot target
       |
       v
11. Kernel loaded into DDR
       |
       v
12. Device Tree loaded
       |
       v
13. Kernel command line prepared
       |
       v
14. U-Boot starts Linux
       |
       v
15. Linux kernel initializes
       |
       v
16. Device Tree parsed
       |
       v
17. Drivers initialized
       |
       v
18. RootFS mounted
       |
       v
19. systemd starts
       |
       v
20. box-storage.service starts
       |
       v
21. boxes executable starts
       |
       v
22. main()
       |
       v
23. BoxManager
       |
       +----------+----------+
       |                     |
       v                     v
   StaticBox             DynamicBox
       |                     |
       +----------+----------+
                  |
                  v
              Application
              Ready
```

---

# 22. SD Card Boot Architecture

The SD card contains the bootloader and Linux filesystem.

```text
                 SD CARD
                    |
       +------------+------------+
       |                         |
       v                         v
  BOOT PARTITION             ROOTFS
       |                         |
       |                         +--> /bin
       |                         +--> /etc
       |                         +--> /lib
       |                         +--> /usr
       |                         +--> /opt
       |
       +--> MLO
       +--> u-boot.img
       +--> Image
       +--> am335x-boneblack.dtb
```

---

# 23. eMMC Boot Architecture

The same general boot chain can be used when booting from eMMC.

```text
                 eMMC
                   |
       +-----------+-----------+
       |                       |
       v                       v
    BOOT                    ROOTFS
       |                       |
       v                       v
     MLO                    Linux FS
       |
       v
    U-Boot
       |
       v
    Kernel
       |
       v
    Device Tree
       |
       v
    RootFS
```

The exact partitioning and boot-device configuration depend on
the board's boot mode and BSP.

---

# 24. Failure Points During Boot

Each stage can fail independently.

```text
Power
 |
 X --> Hardware / power problem
 |
 v
Boot ROM
 |
 X --> Boot-device problem
 |
 v
MLO / SPL
 |
 X --> DDR / SPL problem
 |
 v
U-Boot
 |
 X --> Storage / environment problem
 |
 v
Kernel
 |
 X --> Kernel / Device Tree problem
 |
 v
RootFS
 |
 X --> Filesystem / mount problem
 |
 v
systemd
 |
 X --> Service dependency problem
 |
 v
box-storage.service
 |
 X --> Application startup problem
 |
 v
boxes
 |
 X --> Application bug
 |
 v
Output
```

---

# 25. Debugging Boot Problems

Use the serial console on the BeagleBone Black.

The boot log allows the engineer to determine which stage failed.

Conceptually:

```text
Power ON
   |
   v
Serial Console
   |
   +--> ROM/SPL messages
   |
   +--> U-Boot messages
   |
   +--> Kernel messages
   |
   +--> systemd messages
   |
   +--> Application messages
```

Once Linux is running:

```bash
dmesg
```

can be used to inspect kernel messages.

For systemd:

```bash
systemctl status box-storage.service
```

For application logs:

```bash
journalctl -u box-storage.service
```

---

# 26. Project Boot-to-Application Flow

The final relationship between the board boot process and this project is:

```text
                   BeagleBone Black
                    TI AM3358
                         |
                         v
                     Boot ROM
                         |
                         v
                       MLO
                         |
                         v
                      U-Boot
                         |
              +----------+----------+
              |                     |
              v                     v
           Kernel               Device Tree
              |                     |
              +----------+----------+
                         |
                         v
                       Linux
                         |
                         v
                       RootFS
                         |
                         v
                      systemd
                         |
                         v
              box-storage.service
                         |
                         v
                  boxes executable
                         |
                         v
                       main()
                         |
                         v
                    BoxManager
                         |
              +----------+----------+
              |                     |
              v                     v
          StaticBox             DynamicBox
              |                     |
              +----------+----------+
                         |
                         v
                  Date Indexed Data
                         |
                         v
                      OUTPUT
```

---

# 27. Summary

The BeagleBone Black boot process can be understood as:

```text
ROM
 ↓
SPL/MLO
 ↓
U-Boot
 ↓
Kernel + Device Tree
 ↓
RootFS
 ↓
systemd
 ↓
Application
```

The Box Storage project enters the boot process only after
Linux userspace is available.

The application itself is started by:

```text
systemd
   ↓
box-storage.service
   ↓
/opt/box-storage/bin/boxes
```

Therefore:

**Bootloader gets Linux running.**

**Linux gets userspace running.**

**systemd gets the Box Storage application running.**

**BoxManager/StaticBox/DynamicBox perform the actual project functionality.**

````

### Updated `docs/`

Your documentation structure is now:

```text
docs/
├── architecture.md
└── boot_flow.md
````

This separation is useful because `architecture.md` answers **"what are the components and how are they connected?"**, while `boot_flow.md` answers **"how does the AM3358 go from power-on all the way to our `boxes` application?"**

