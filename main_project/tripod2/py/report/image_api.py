import os.path
import db_api

def PlotImage(biz, ops, begin_time, end_time, items):
    dat_path = 'cron/'
    file_name = biz
    for op in ops:
      file_name += op
    file_name += begin_time
    file_name += end_time
    dat_fn = file_name +'.dat' 
    dat_fn_path = dat_path + dat_fn
    cdm_fn = file_name +'.gcf'
    cmd_fn_path = dat_path + cdm_fn
    pic_fn = file_name+'.jpg'
    pic_path = 'static/images/'
    pic_fn_path = pic_path + pic_fn;
    # write data file
    try:
        if os.path.exists(dat_fn_path):
            os.remove(dat_fn_path)
        f = open(dat_fn_path, 'w')
        for item in items:
            line = '%s-%s-%s-%s' % (item['time'].year, item['time'].month, item['time'].day, item['time'].hour)
            for op in ops:
                line = line + '\t' + ( '%s' % (item[op]) ) 
            line = line + '\n' 
            f.write(line)
        f.close()
    except Exception, e:
        return None
    
    try:
        if os.path.exists(cmd_fn_path):
            os.remove(cmd_fn_path)
    except Exception, e:
        return None

    seq = []
    seq.append("#!/usr/bin/gnuplot\n")
    seq.append("set xdata time\n")
    seq.append("set timefmt \"%Y-%m-%d-%H\"\n")
    seq.append("set format x \"%m/%d/%H\"\n")
    seq.append("set xrange [\"" + begin_time + "\":\"" + end_time + "\"]\n")
    seq.append("set xlabel \"mm/dd/HH\"\n")
#    seq.append("set ylabel \"10k\"\n")
    seq.append("set style fill solid 0.05\n")
    seq.append("set terminal jpeg medium size 1000, 432\n")
    seq.append("set output \"" + pic_fn_path + "\"\n")
    seq.append("set boxwidth 70000\n")
    seq.append("set grid front\n")
    seq.append("set key horizontal bmargin\n")
#    seq.append("set title \"" + biz + "\"  from " + begin_time + " to " + end_time + "\"\n")
    seq.append("plot ")
    row_num = 2
    for op in ops:
        seq.append("\"" + dat_fn_path + "\" using  1:($")
        seq.append(str(row_num))
        print row_num
        seq.append(") title \"")
        seq.append(op)
        seq.append("\" with linespoints lw 2 ps 0.6 ")
        seq.append(",")
        row_num += 1
#   seq.append("\"" + dat_fn_path + "\" using  1:($5) title \"remove\" with linespoints lw 2 ps 0.6\n")
    seq.pop()
    seq.append("\n")
    print seq
    try:
        f = open(cmd_fn_path, "w")
        for s in seq:
            print s
            f.write(s)
        f.close()
    except Exception, e:
        return None    
    os.system("gnuplot " + cmd_fn_path)
    if os.path.exists(pic_fn_path):
        return pic_fn_path
    return None


if __name__ == '__main__':
    data =  db_api.conndb.ReadData('blog_list',['get','miss'], '2012-06-15-00','2012-07-15-22')
#    print data
    image = PlotImage('blog_list',['get','miss'],'2012-06-15-00','2012-07-15-23',data)
