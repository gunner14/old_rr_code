#!/bin/bash
echo ""
echo "----mailer: sending report"
echo ""
#:<<BLOCK
receiver=""
while read LINE
do
	receiver="${receiver},$LINE"
done < $1

/usr/sbin/sendmail -t <<EOF
From:autobuilder<xiang.ye@opi-corp.com>
To:$receiver
Subject:autobuilder report v`date +20%y%m%d`
`cat $2`


======== final report ========
--- failed when do "make" ! ---
`cat $3`
-------------------------------

--- failed when do "test" ! ---
`cat $4`
-------------------------------

------------ pass! ------------
`cat $5`
-------------------------------

EOF
#BLOCK
