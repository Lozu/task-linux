#!/bin/sh -e
# SPDX-License-Identifier: GPL-2.0

git format-patch ref-point..task1 --start-number 1 -o patches
