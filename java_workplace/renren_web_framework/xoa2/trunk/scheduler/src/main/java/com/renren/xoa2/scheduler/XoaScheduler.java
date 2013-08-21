package com.renren.xoa2.scheduler;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.net.URLDecoder;
import java.util.Enumeration;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.conf.Configured;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.Tool;
import org.apache.hadoop.util.ToolRunner;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.renren.xoa2.scheduler.configure.XoaJobConfigure;
import com.renren.xoa2.scheduler.configure.XoaConfiguration;
import com.renren.xoa2.server.Bootstrap;

/**
 * 对Xoa的服务进行分布式调度的任务，依附于 hadoop 的 mapreduce 模块(Not mapred) <br><br>
 * 1. 由于Map数量不易控制，暂时使用无意义文件作为Input<br>
 * 2. 使用Reduce作为实际服务运行载体，传递相关的服务配置信息
 * @author Yuan Liu (yuan.liu1@renren-inc.com)
 * @date   2012-4-1 (This is a good day for coding, Have fun!)
 */
public class XoaScheduler extends Configured implements Tool {
    private static Logger logger = LoggerFactory.getLogger(XoaScheduler.class);
    
    /**
     * Mapper 进行一些环境准备的工作<br><br>
     * 1. 检查ZK相关环境<br>
     * 2. 进行一致性验证(服务的启动场景)<br>
     */
    static class XoaMapper extends Mapper<Object, Text, IntWritable, Text> {
        @Override
        public void run(Mapper<Object,Text,IntWritable,Text>.Context context) 
                throws IOException ,InterruptedException {
            // TODO：准备工作
        }
    }
    
    /**
     * 在 Reduce 中包装 XoaService 的服务进程
     */
    static public class XoaReducer extends Reducer<IntWritable, Text, IntWritable, Text> {
        /**
         * 将Jar包中的配置文件抽出
         * 
         * @param confStream
         * @return
         * @throws IOException 
         */
        String getConfFile(InputStream confStream) throws IOException {
            File file = File.createTempFile("xoa", ".xml");
            
            FileWriter writer = new FileWriter(file);
            int byteRead = confStream.read(); 
            while (byteRead != -1) {
                writer.append((char)byteRead);
                byteRead = confStream.read(); 
            }
            
            writer.close();
            
            return file.getAbsolutePath();
        }
        
        @Override
        public void run(Reducer<IntWritable,Text,IntWritable,Text>.Context context)
                throws IOException ,InterruptedException {
            Configuration conf = context.getConfiguration();
            Bootstrap bootStrap = new Bootstrap();
            
            // 获取Xoa服务的配置文件
            InputStream input = this.getClass().getClassLoader()
                    .getResourceAsStream(conf.getStrings(XoaConfiguration.SERVICE_CONFIG_PATH)[0]);
            
            bootStrap.startServices(getConfFile(input)); 
            
            long serviceTime = 
                    Long.parseLong(conf.getStrings(XoaConfiguration.SERVICE_TIME)[0]) * 1000;
            long serviceIdleTime = 
                    Long.parseLong(conf.getStrings(XoaConfiguration.SERVICE_IDLE_TIMEOUT)[0]) * 1000;
            
            if (serviceTime <=0 || serviceIdleTime <= 0) {
                throw new RuntimeException("Invalid service-time or service-idle-time in conf!");
            }
            
            long curTime = 0;
            long endTime = serviceTime + serviceIdleTime;
            
            // 等待 service-time 时间后取消可见性
            try {
                while (curTime < serviceTime) {
                    Thread.currentThread().join(Bootstrap.MONITOR_TIME_INTERVAL);
                    
                    curTime += Bootstrap.MONITOR_TIME_INTERVAL;
                    
                    if (bootStrap.getInvalidServicNumber() > 0) {
                        throw new RuntimeException("Inner service is crashed!");
                    }
                }
            } catch (InterruptedException e) {
                bootStrap.haltAll();
                throw e;
            }
            
            bootStrap.disableServices();
            
            // 等待 service-idle-time 后任务结束
            try {
                while (curTime < endTime) {
                    Thread.currentThread().join(Bootstrap.MONITOR_TIME_INTERVAL);
                    
                    curTime += Bootstrap.MONITOR_TIME_INTERVAL;
                    
                    if (bootStrap.getInvalidServicNumber() > 0) {
                        throw new RuntimeException("Inner service is crashed!");
                    }
                }
            } catch (InterruptedException e) {
                bootStrap.haltAll();
                throw e;
            }
            
            bootStrap.haltAll();
        }
    }
    
    /**
     * 获取对应的最上层的Jar包，防止 one-jar 丢类的情况
     * 
     * @param my_class
     * @return
     */
    private static String findContainingJar(Class my_class) {
        ClassLoader loader = my_class.getClassLoader();
        String class_file = my_class.getName().replaceAll("\\.", "/") + ".class";
        try {
          for(Enumeration itr = loader.getResources(class_file);
              itr.hasMoreElements();) {
            URL url = (URL) itr.nextElement();

            if ("jar".equals(url.getProtocol())) {
              String toReturn = url.getPath();
              if (toReturn.startsWith("file:")) {
                toReturn = toReturn.substring("file:".length());
              }
              toReturn = URLDecoder.decode(toReturn, "UTF-8");
              
              toReturn = toReturn.replaceAll("!.*$", "");
              
              // FIXME：Dirty Hack，后续根据规约设定进行修改
              int idx = toReturn.indexOf("lib/xoa2-scheduler");
              if (idx != -1) {
                  toReturn = toReturn.substring(0, idx);
              }
              
              return toReturn;
            }

          }
        } catch (IOException e) {
          throw new RuntimeException(e);
        }
        return null;
      }
    
    @Override
    public int run(String[] args) throws Exception {
        if (args.length <= 2) {
            System.err.println("args: jobconf.xml inputPath outputPath");
            System.exit(-1);
        }
        
        XoaJobConfigure xoaConf = XoaConfiguration.getJobConfigure(args[0]);
        if (xoaConf == null || !xoaConf.isValid()) {
            logger.error("Invalid configure file [%s]", args[0]);            
            System.exit(-1);
        }
        
        Configuration conf = getConf();

        conf.setInt(XoaConfiguration.SERVICE_TIME, xoaConf.getServiceTime());
        conf.setInt(XoaConfiguration.SERVICE_IDLE_TIMEOUT, xoaConf.getServiceIdleTime());
        conf.setStrings(XoaConfiguration.SERVICE_CONFIG_PATH, xoaConf.getServiceConf());

        Job job = new Job(conf);
        job.setJobName("XoaTest");

        job.setJarByClass(XoaScheduler.class);
        
        job.setMapperClass(XoaMapper.class);
        job.setReducerClass(XoaReducer.class);
        job.setOutputKeyClass(NullWritable.class);
               
        job.setNumReduceTasks(xoaConf.getTaskNumber());
        job.setSpeculativeExecution(false);
        
        FileInputFormat.setInputPaths(job, new Path(args[1]));
        FileOutputFormat.setOutputPath(job, new Path(args[2]));
        
        return job.waitForCompletion(true) ? 0:1;
    }    

    public static void main(String[] args) throws Exception {
        int ret = ToolRunner.run(new XoaScheduler(), args);
        System.exit(ret);
    }
}