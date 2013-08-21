package com.xiaonei.xnmonitor.mailreport;

import java.io.UnsupportedEncodingException;

import javax.mail.MessagingException;

import org.springframework.beans.factory.annotation.Autowired;

public class ReportTask {
	@Autowired
	private IMDNSReport imDNSReport;

    public void reportMail() throws UnsupportedEncodingException, MessagingException {
        imDNSReport.sendMail();
        System.out.println("Report IMDNS EMAIL ..");
    }

    public void testCron() {
        System.out.println("ReportTask Test....");
    }

    public static void main(String... args) throws UnsupportedEncodingException, MessagingException {
        ReportTask task = new ReportTask();
        task.reportMail();
        System.out.println("Test End ..");
    }
}
