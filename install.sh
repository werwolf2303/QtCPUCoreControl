#!/bin/bash

SCRIPT_PATH="$(readlink -f "$0")"
SCRIPT_DIR="$(dirname "$SCRIPT_PATH")"

if [ -n "$SUDO_USER" ]; then
  ORIGINAL_USER="$SUDO_USER"
else
  ORIGINAL_USER=$(logname 2>/dev/null)
fi

if [ -z "$PKEXEC_UID" ]; then
  exec env SCRIPT_DIR="$SCRIPT_DIR" pkexec "$SCRIPT_PATH" "$@"
  return
fi

if [ -n "$SCRIPT_DIR" ]; then
  cd "$SCRIPT_DIR" || {
    echo "Failed to cd to script directory: $SCRIPT_DIR"
    exit 1
  }
fi

mkdir -p /opt/QtCPUCoreControl

./build.sh

rm -rf /opt/QtCPUCoreControl
cp -r out/ /opt/QtCPUCoreControl

chmod +x /opt/QtCPUCoreControl/QtCPUCoreControl
chmod +x /opt/QtCPUCoreControl/cpu_toggle_helper

cp QtCPUCoreControl.desktop /home/$ORIGINAL_USER/.config/autostart/