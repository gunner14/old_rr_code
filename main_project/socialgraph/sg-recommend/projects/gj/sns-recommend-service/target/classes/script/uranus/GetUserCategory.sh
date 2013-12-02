#!/bin/sh
`cat /data/uranus/data/categoryFile | awk '{for( i=1;i<NF;i++) printf("%s,",$i); print $NF}' > /data/uranus/data/u2c.dat`
