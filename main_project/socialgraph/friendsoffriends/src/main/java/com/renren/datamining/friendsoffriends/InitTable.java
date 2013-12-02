package com.renren.datamining.friendsoffriends;

import java.io.IOException;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.OptionBuilder;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;
import org.apache.commons.cli.PosixParser;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.hbase.HColumnDescriptor;
import org.apache.hadoop.hbase.HTableDescriptor;
import org.apache.hadoop.hbase.client.HBaseAdmin;

public class InitTable {
  private static org.apache.commons.logging.Log logger = org.apache.commons.logging.LogFactory
      .getLog(InitTable.class);
  private final String name;
  private final boolean isOverwrite;

  private InitTable(String name, boolean isOverwrite) {
    this.name = name;
    this.isOverwrite = isOverwrite;
  }

  public void doit() throws IOException {
    HBaseAdmin adm = new HBaseAdmin(new Configuration());
    boolean exists = adm.tableExists(this.name);
    if (exists && !this.isOverwrite) {
      logger.warn("Table " + this.name + " already exists. Exiting...");
      return;
    }
    if (this.isOverwrite) {
      adm.deleteTable(this.name);
    }
    createTable(adm);
    adm.close();
  }

  private void createTable(HBaseAdmin adm) throws IOException {
    HTableDescriptor desc = new HTableDescriptor(this.name);
    desc.addFamily(new HColumnDescriptor("f")); // f for friends
    desc.addFamily(new HColumnDescriptor("r")); // r for rank
    /*
     * TODO needs more adjusts about split. VERY VERY IMPORTENT.
     */
    adm.createTable(desc);
  }

  @SuppressWarnings("static-access")
  private static InitTable parseArgs(String[] args) throws ParseException {
    Options opts = new Options();
    opts.addOption(OptionBuilder.hasArg(false).isRequired(false)
        .withDescription("This help infomation").withLongOpt("help")
        .create('h'));
    opts.addOption(OptionBuilder.hasArg(false).isRequired(false)
        .withDescription("Force re-initialzie table. !!! WILL LOSE DATA !!!")
        .withLongOpt("force").create('f'));
    opts.addOption(OptionBuilder.hasArg(true).isRequired(true)
        .withDescription("The table name.").withLongOpt("table").create('t'));

    PosixParser parser = new PosixParser();
    CommandLine cmd = parser.parse(opts, args);
    String name = cmd.getOptionValue('t');
    boolean isOverwrite = cmd.hasOption('f');
    return new InitTable(name, isOverwrite);
  }

  public static void main(String[] args) throws ParseException, IOException {
    InitTable init = parseArgs(args);
    init.doit();
  }

}
