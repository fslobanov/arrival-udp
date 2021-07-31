#!/usr/bin/env bash

pkill -f arrival-server --signal=SIGUSR2
pkill -f arrival-endpoint --signal=SIGUSR2