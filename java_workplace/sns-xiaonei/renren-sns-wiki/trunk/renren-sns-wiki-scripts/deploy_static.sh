#!/bin/bash

echo "============= deploy static file to online static server ======="
rsync --include='core' -ztrvlC --ignore-existing /data/web/web-wiki/static/ xnimg@10.3.17.37:/data/static
echo "============= finish ==========================================="
