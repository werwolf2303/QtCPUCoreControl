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

killall QtCPUCoreControl

rm /home/$ORIGINAL_USER/.config/autostart/QtCPUCoreControl.desktop

rm -rf /opt/QtCPUCoreControl