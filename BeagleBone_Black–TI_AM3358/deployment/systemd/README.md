Yes. If your service is named **`box-storage.service`**, use that exact name consistently instead of `boxes.service`.

## `deployment/systemd/box-storage.service`

```ini
[Unit]
Description=Box Storage System
Documentation=https://github.com/jsramesh1990/Storing_Fetching_Date_Indexed_Data

# Start after the filesystem is available
After=local-fs.target

# Make sure the service starts during normal multi-user boot
Wants=local-fs.target


[Service]
Type=simple

# Application binary
ExecStart=/opt/box-storage/bin/boxes

# Working directory
WorkingDirectory=/opt/box-storage

# Configuration files
Environment="BOXES_CONFIG=/opt/box-storage/config/boxes.conf"
Environment="BOXES_LOG_CONFIG=/opt/box-storage/config/logging.conf"

# Restart application if it crashes
Restart=on-failure
RestartSec=5

# Logging goes to systemd journal
StandardOutput=journal
StandardError=journal

# Security / runtime settings
NoNewPrivileges=true

# Application user
User=root
Group=root


[Install]
WantedBy=multi-user.target
```

### Directory on BeagleBone Black

After installation:

```text
/opt/box-storage/
├── bin/
│   └── boxes
│
└── config/
    ├── boxes.conf
    └── logging.conf
```

Persistent data:

```text
/var/lib/box-storage/
```

Logs:

```text
journalctl -u box-storage.service
```

---

## Install it on BeagleBone Black

Copy the service:

```bash
sudo cp box-storage.service \
    /etc/systemd/system/
```

Reload systemd:

```bash
sudo systemctl daemon-reload
```

Enable it at boot:

```bash
sudo systemctl enable box-storage.service
```

Start it immediately:

```bash
sudo systemctl start box-storage.service
```

Check status:

```bash
sudo systemctl status box-storage.service
```

View logs:

```bash
sudo journalctl -u box-storage.service
```

Follow logs live:

```bash
sudo journalctl -u box-storage.service -f
```

Stop:

```bash
sudo systemctl stop box-storage.service
```

Disable automatic startup:

```bash
sudo systemctl disable box-storage.service
```

---

## Boot-time flow

On the **BeagleBone Black / TI AM3358**:

```text
Power ON
   ↓
TI AM3358 Boot ROM
   ↓
SPL
   ↓
U-Boot
   ↓
Linux Kernel
   ↓
Device Tree
   ↓
Root Filesystem
   ↓
systemd
   ↓
systemd reads
box-storage.service
   ↓
ExecStart
   ↓
/opt/box-storage/bin/boxes
   ↓
Box Storage Application
   ↓
StaticBox / DynamicBox
```

So the important relationship is:

```text
box-storage.service
        |
        | ExecStart
        ↓
/opt/box-storage/bin/boxes
        |
        ↓
      main()
        |
        ↓
   BoxManager
        |
   +----+----+
   |         |
   ↓         ↓
StaticBox DynamicBox
```

### One important correction

Your current `main.cpp` is a **command-line application** (`add`, `get`, `range`, `demo`). A systemd service normally runs a long-lived process.

So eventually we should change the application architecture to:

```text
box-storage.service
        ↓
      boxes
        ↓
   Application Loop
        ↓
  IPC / CLI / API
        ↓
   BoxManager
```

rather than starting:

```text
systemd → boxes → demo → exit
```

That will make the BeagleBone project much more realistic.

