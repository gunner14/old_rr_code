import MySQLdb

conn = MySQLdb.connect(host="stat2.db",user="xndev",passwd="rebornlOM",db="fb",charset="utf8")
cursor = conn.cursor()
cursor.execute("select server_name,server_port,cache_size from memconfig")
table = cursor.fetchall()
#print table
server = {}
result = {}
for row in table:
	if not server.has_key(row[0]+":"+str(row[1])):
		server[row[0]+":"+str(row[1])]=row[2]
		print row[0]+":"+str(row[1])+"="+str(row[2])+"m"
		
count = 0	
for key in server.keys():
	host,port = key.split(":")
	count = count+server[host+":"+str(port)]
	if not result.has_key(host):
		result[host]=server[host+":"+str(port)]
	else:
		result[host]=result[host]+server[host+":"+str(port)]
print "total==============="+str(count)+"m"
print result
