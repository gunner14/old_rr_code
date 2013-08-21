package lwb.test.servlet;

import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class WriteTestServlet extends BaseTestServlet {

	private static final long serialVersionUID = 1L;

	static final String s = " Nowadays we use general purpose applications or libraries to communicate with each other. For example, we often use an HTTP client library to retrieve information from a web server and to invoke a remote procedure call via web services." + 
		"However, a general purpose  or  sometimes does . It is like we don't use a general purpose , e-mail messages, and near-realtime messages such as financial information and multiplayer game data. What's required is a highly optimized protocol implementation which is dedicated to a special purpose. For example, you might want to implement an HTTP server which is optimized for AJAX-based chat application, media streaming, or large file transfer. You could even want to design and implement a whole new protocol which is precisely tailored to your need." + 
		"Another  case is when you have to deal with a legacy proprietary protocol to ensure the interoperability with an old . What matters in this case is how quickly we can implement that protocol while not sacrificing the stability and performance of the resulting application. " +
		"However, a general purpose  or  sometimes does . It is like we don't use a general purpose , e-mail messages, and near-realtime messages such as financial information and multiplayer game data. What's required is a highly optimized protocol implementation which is dedicated to a special purpose. For example, you might want to implement an HTTP server which is optimized for AJAX-based chat application, media streaming, or large file transfer. You could even want to design and implement a whole new protocol which is precisely tailored to your need." + 
		"However, a general purpose  or  sometimes does . It is like we don't use a general purpose , e-mail messages, and near-realtime messages such as financial information and multiplayer game data. What's required is a highly optimized protocol implementation which is dedicated to a special purpose. For example, you might want to implement an HTTP server which is optimized for AJAX-based chat application, media streaming, or large file transfer. You could even want to design and implement a whole new protocol which is precisely tailored to your need." + 
		"However, a general purpose  or  sometimes does . It is like we don't use a general purpose , e-mail messages, and near-realtime messages such as financial information and multiplayer game data. What's required is a highly optimized protocol implementation which is dedicated to a special purpose. For example, you might want to implement an HTTP server which is optimized for AJAX-based chat application, media streaming, or large file transfer. You could even want to design and implement a whole new protocol which is precisely tailored to your need." + 
		"However, a general purpose  or  sometimes does . It is like we don't use a general purpose , e-mail messages, and near-realtime messages such as financial information and multiplayer game data. What's required is a highly optimized protocol implementation which is dedicated to a special purpose. For example, you might want to implement an HTTP server which is optimized for AJAX-based chat application, media streaming, or large file transfer. You could even want to design and implement a whole new protocol which is precisely tailored to your need." + 
		"However, a general purpose  or  sometimes does . It is like we don't use a general purpose , e-mail messages, and near-realtime messages such as financial information and multiplayer game data. What's required is a highly optimized protocol implementation which is dedicated to a special purpose. For example, you might want to implement an HTTP server which is optimized for AJAX-based chat application, media streaming, or large file transfer. You could even want to design and implement a whole new protocol which is precisely tailored to your need." + 
		"Another  case is when you have to sdeal with a legacy proprietary protocol to ensure the interoperability with an old . What matters in this case is how quickly we can implement that protocol while not sacrificing the stability and performance of the resulting application. ";

	
	@Override
	protected void doGet(HttpServletRequest req, HttpServletResponse response)
			throws ServletException, IOException {
		response.setContentType("text/plain; charset=UTF-8");
		PrintWriter out = response.getWriter();
		out.append(s);
		out.flush();
	}
	
	@Override
	protected void doPost(HttpServletRequest req, HttpServletResponse resp)
			throws ServletException, IOException {
		doGet(req, resp);
	}
	
	public static void main(String[] args) {
		System.out.println(s.length());
	}
	
}
