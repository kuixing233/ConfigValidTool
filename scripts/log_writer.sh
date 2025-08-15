#!/usr/bin/env bash
set -euo pipefail

# 用法:
#   scripts/log_writer.sh "your message"    # 可选: ENV INTERVAL=1
# 也可用环境变量 MSG 指定消息, INTERVAL 指定间隔秒
#   MSG="HELLO world" INTERVAL=2 scripts/log_writer.sh

msg=${1:-${MSG:-HELLO world}}
interval=${INTERVAL:-2}

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
log_dir="$repo_root/bin"
log_file="$log_dir/log.log"

mkdir -p "$log_dir"

echo "Starting log writer (PID $$) -> $log_file"
trap 'echo "Stopping log writer (PID $$)"; exit 0' INT TERM

while true; do
	printf '%s %s\n' "$(date '+%Y-%m-%d %H:%M:%S')" "$msg" >> "$log_file"
	sleep "$interval"
done 