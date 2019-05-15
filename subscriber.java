import java.io.*;
import gnu.getopt.Getopt;
import java.lang.*;
import java.net.*;
import java.util.*;
import service.FormatterService;
import service.FormatterServiceService;

class subscriber{
	
	/********************* TYPES **********************/
	
	
	/******************* ATTRIBUTES *******************/
	
	private static String _server   = null;
	private static short _port = -1;
	public static DataOutputStream out;
	public static DataInputStream in;
	public static Socket sc;
	public static ServerSocket serverSock;
	public static int sockPort;
	public static String subPort;
	
	/********************* METHODS ********************/

	/* When the subscritpion is OK, a thread is created to open a socket that listens to the broker */
	static class SubThread extends Thread{
		private static ServerSocket _serverSock;
		private static DataInputStream _in;
		private static BufferedReader _br;
		private static String _topic;

		public SubThread(String topic, ServerSocket serverSocket){ /* Constructor */
			_topic = topic;
			_serverSock = serverSocket;
		}

		public void run(){
			String _text;
			/* initialize the web service that consists of formatting the text */
			FormatterServiceService service = new FormatterServiceService();
			FormatterService port = service.getFormatterServicePort();
			try{
				while(true){				
					Socket clientSocket = _serverSock.accept(); /* connection to the broker */
					_in = new DataInputStream(clientSocket.getInputStream());
					_br = new BufferedReader(new InputStreamReader(_in));
					while((_text = _br.readLine()) != null) { /* receive the message from the broker */
						String xml10pattern = "[^"+ "\u0009\r\n"+ "\u0020-\uD7FF"+ "\uE000-\uFFFD"+"\ud800\udc00-\udbff\udfff"+ "]";
						String text_temp = _text.replaceAll(xml10pattern, "");
						_text = port.formatterText(text_temp); /* use of the web service */
						System.out.println("MESSAGE FROM "+_topic+" : "+_text);
						System.out.print("c> ");
					}
				}
			}
			catch(Exception e){
				System.out.println("NETWORK ERROR");
				System.out.print("c> ");
			}
			try{
				serverSock.close();
			}
			catch(Exception e){
				System.out.println("NETWORK ERROR");
				System.out.print("c> ");
			}
		}
	}

	static int subscribe(String topic){
		try{
			sc = new Socket(_server, _port); /* connection to the broker */
			out = new DataOutputStream(sc.getOutputStream());
			in = new DataInputStream(sc.getInputStream());
			
			out.write("SUBSCRIBE\0".getBytes()); /* identify the operation */
			out.flush();

			out.write((topic+"\0").getBytes()); /* send the topic to subscribe */
			out.flush();

			serverSock = new ServerSocket(0); /* create the new socket with a valid free port number */
			sockPort = serverSock.getLocalPort(); /* get the port number to send to the broker */
			subPort = Integer.toString(sockPort)+"\0";
			out.write(subPort.getBytes()); /* send the port number where the susbscriber receives the messages */
			out.flush();

			/* check if the operation is successful or not */
			if(in.readByte() == 0){
				System.out.println("c> SUBCRIBE OK");
				/* a thread is created to listen to the broker  */
				Thread thread = new SubThread(topic, serverSock);
				thread.start();
			} else if(in.readByte() == -1){
				System.out.println("c> SUBCRIBE FAIL");
			}
			/* close the connection */
			in.close();
			out.close();
			sc.close();
		}
		catch(Exception e){
			System.out.println("Error in the connection to the broker "+_server+" : "+_port);
		}
		return 0;
	}

	static int unsubscribe(String topic){
		try{
			sc = new Socket(_server, _port); /* connection to the broker */
			out = new DataOutputStream(sc.getOutputStream());
			in = new DataInputStream(sc.getInputStream());
			
			out.write("UNSUBSCRIBE\0".getBytes()); /* identify the operation */
			out.flush();

			out.write((topic+"\0").getBytes()); /* send the topic to unsubscribe */
			out.flush();

			if(in.readByte() == 1){
				System.out.println("c> TOPIC NOT SUBSCRIBED");
			}else if(in.readByte() == 0){
				System.out.println("c> UNSUBCRIBE OK");
			}else if(in.readByte() == 2){
				System.out.println("c> UNSUBCRIBE FAIL");
			}
			/* close the connection */
			in.close();
			out.close();
			sc.close();
		}
		catch(Exception e){
			System.out.println("Error in the connection to the broker "+_server+" : "+_port);
		}
		return 0;
	}	
	
	/**
	 * @brief Command interpreter for the suscriptor. It calls the protocol functions.
	 */
	static void shell() 
	{
		boolean exit = false;
		String input;
		String [] line;
		BufferedReader in = new BufferedReader(new InputStreamReader(System.in));

		while (!exit) {
			try {
				System.out.print("c> ");
				input = in.readLine();
				line = input.split("\\s");

				if (line.length > 0) {
					/*********** SUBSCRIBE *************/
					if (line[0].equals("SUBSCRIBE")) {
						if  (line.length == 2) {
							subscribe(line[1]); // topic = line[1]
						} else {
							System.out.println("Syntax error. Usage: SUBSCRIBE <topic>");
						}
					} 
					
					/********** UNSUBSCRIBE ************/
					else if (line[0].equals("UNSUBSCRIBE")) {
						if  (line.length == 2) {
							unsubscribe(line[1]); // topic = line[1]
						} else {
							System.out.println("Syntax error. Usage: UNSUBSCRIBE <topic>");
						}
                    } 
                    
                    /************** QUIT **************/
                    else if (line[0].equals("QUIT")){
						if (line.length == 1) {
							exit = true;
						} else {
							System.out.println("Syntax error. Use: QUIT");
						}
					} 
					
					/************* UNKNOWN ************/
					else {						
						System.out.println("Error: command '" + line[0] + "' not valid.");
					}
				}				
			} catch (java.io.IOException e) {
				System.out.println("Exception: " + e);
				e.printStackTrace();
			}
		}
	}
	
	/**
	 * @brief Prints program usage
	 */
	static void usage() 
	{
		System.out.println("Usage: java -cp . subscriber -s <server> -p <port>");
	}
	
	/**
	 * @brief Parses program execution arguments 
	 */ 
	static boolean parseArguments(String [] argv) 
	{
		Getopt g = new Getopt("subscriber", argv, "ds:p:");

		int c;
		String arg;

		while ((c = g.getopt()) != -1) {
			switch(c) {
				//case 'd':
				//	_debug = true;
				//	break;
				case 's':
					_server = g.getOptarg();
					break;
				case 'p':
					arg = g.getOptarg();
					_port = Short.parseShort(arg);
					break;
				case '?':
					System.out.print("getopt() returned " + c + "\n");
					break; // getopt() already printed an error
				default:
					System.out.print("getopt() returned " + c + "\n");
			}
		}
		
		if (_server == null)
			return false;
		
		if ((_port < 1024) || (_port > 65535)) {
			System.out.println("Error: Port must be in the range 1024 <= port <= 65535");
			return false;
		}

		return true;
	}
	
	
	
	/********************* MAIN **********************/
	
	public static void main(String[] argv) 
	{
		if(!parseArguments(argv)) {
			usage();
			return;
		}
		
		try{
			_server = argv[1]; /* IP of the broker */
			_port = Short.parseShort(argv[3]); /* port of the broker */
			
			shell();
			
		} catch(Exception e){
			System.out.println("Error in the connection to the broker "+_server+":"+_port);
		}
		System.out.println("+++ FINISHED +++");
	}
}
