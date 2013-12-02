#!/bin/sh
min_goods_id=`date +%y%m%d%H%M%S0000000 -d "10 days ago"`
`mongoexport --host db.jie.d.xiaonei.com --port 62166 -d sns_online -u gj -p dec2011online4nb -c Goods -q "{avenueSts:{\\$gt:-1},goodsId:{\\$gt:$min_goods_id}}" -f "goodsId,ownerId,categoryId,price,shareCount,shareCount,likeCount,link,tagList" --csv | sed '1d;\$d' | sed 's/, /:/g' | sed 's/[" \[\]//g' | sed 's/\]//g' > /data/uranus/data/goods.dat`
