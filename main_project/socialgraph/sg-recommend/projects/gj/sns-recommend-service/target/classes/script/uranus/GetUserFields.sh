#!/bin/sh
`cat /data/uranus/data/coped  | awk '{printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",$2,$16,$17,$21,$18,$19,$20,$12,$13,$4,$5,$10,$11,$8,$9,$6,$7,$14,$15) }' | sort  | uniq -u > /data/uranus/data/users.dat`
