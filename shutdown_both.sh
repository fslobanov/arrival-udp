#!/usr/bin/env bash
script_directory=$(dirname $(readlink -f $0))
cd ${script_directory}
pkill -f arrival-server --signal=SIGUSR2
pkill -f arrival-endpoint --signal=SIGUSR2