#!/usr/bin/env bash

set -e

if [[ "$#" -ne 1 ]]; then
    echo "Usage: ./deploy.sh <service-name>"
    exit 1
fi

SERVICE="$1"

HERE="$(cd "$(dirname "$0")" && pwd)"
TMP="$(mktemp -d)"

trap 'rm -rf "$TMP"' EXIT

echo "=== BUILD ==="

"$HERE/build.sh" "$TMP"

BIN="$(find "$TMP" -maxdepth 1 -type f -executable -print -quit)"

if [[ -z "$BIN" ]]; then
    echo "ERROR: executable not found"
    exit 1
fi

INSTALL="$HOME/.local/bin/$SERVICE"

echo
echo "=== INSTALL ==="
echo "$BIN -> $INSTALL"

TMP_INSTALL="$INSTALL.new.$$"

cp "$BIN" "$TMP_INSTALL"
chmod +x "$TMP_INSTALL"
mv -f "$TMP_INSTALL" "$INSTALL"

echo
echo "=== SERVICE ==="

if systemctl --user cat "$SERVICE.service" >/dev/null 2>&1; then
    systemctl --user restart "$SERVICE.service"
    echo "Restarted: $SERVICE"
else
    register-service "$SERVICE" "$INSTALL"
fi
