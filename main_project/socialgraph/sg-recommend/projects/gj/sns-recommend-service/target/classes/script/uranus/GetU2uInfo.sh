#/bin/sh
`cat /data/uranus/data/visit_week | awk '{print $1}' > /data/uranus/data/visit.user`
`time java -jar recommender_fat.jar /data/uranus/data/visit.user /data/uranus/data/u2us.dat`
