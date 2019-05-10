import java.io.*;
import gnu.getopt.Getopt;
import java.lang.*;
import java.net.*;
import java.util.*;


class suscriptor {
	
	/********************* TYPES **********************/
	
	
	/******************* ATTRIBUTES *******************/
	
	private static String _server   = null;
	private static short _port = -1;
	public static DataOutputStream out;
	public static DataInputStream in;
	public static Socket sc;	
	
	/********************* METHODS ********************/

	static int subscribe(String topic){
		String subPort = "4000\0";
		try{
			out.write("SUBSCRIBE\0".getBytes());
			out.flush();
			out.write((topic+"\0").getBytes());
			out.flush();
			out.write(subPort.getBytes());
			out.flush();
		}
		catch(Exception e){
			System.out.println("Error in the connection to the broker < server >: < port >");
			return -1;
		}
		byte response;
		try{
			response = in.readByte();
		}
		catch(Exception e){
			System.out.println("Error in the connection to the broker < server >: < port >");
			return -1;
		}
		if(response == 1){
			System.out.println("c> SUBCRIBE OK");
			System.out.println("Subscribe to: " + topic);
		} else{
			System.out.println("c> SUBCRIBE FAIL");
		}
		byte[] topicSub = new byte[1160];
        try{
			in.read(topicSub);
			String s = new String(topicSub);
			String [] lines = s.split(":");
			System.out.println("c> MESSAGE FROM "+lines[0]+" : "+lines[1]);
		}
		catch(Exception e){
			System.out.println("Error in the connection to the broker < server >: < port >");
			return -1;
		}
		return 0;
	}

	static int unsubscribe(String topic){
		String subPort = "4000\0";
		try{
			out.write("UNSUBSCRIBE\0".getBytes());
			out.flush();
			out.write((topic+"\0").getBytes());
			out.flush();
		}
		catch(Exception e){
			System.out.println("Error in the connection to the broker < server >: < port >");
			return -1;
		}
		byte response;
		try{
			response = in.readByte();
		}
		catch(Exception e){
			System.out.println("Error in the connection to the broker < server >: < port >");
			return -1;
		}
		
		if(response == 1){
			System.out.println("c> TOPIC NOT SUBSCRIBED");
		}else if(response == 0){
			System.out.println("c> UNSUBCRIBE OK");
			System.out.println("Unsubscribe from: " + topic);
		}else if(response == 2){
			System.out.println("c> UNSUBCRIBE FAIL");
		}
        
		return 0;
	}

	static int quit(){
		try{
			out.write("QUIT\0".getBytes());
			out.flush();
		}
		catch(Exception e){
			System.out.println("Error in the connection to the broker < server >: < port >");
			return -1;
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
							quit();
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
		System.out.println("Usage: java -cp . suscriptor -s <server> -p <port>");
	}
	
	/**
	 * @brief Parses program execution arguments 
	 */ 
	static boolean parseArguments(String [] argv) 
	{
		Getopt g = new Getopt("suscriptor", argv, "ds:p:");

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
			_server = argv[1];
			_port = Short.parseShort(argv[3]);
			sc = new Socket(_server, _port);

			out = new DataOutputStream(sc.getOutputStream());
			in = new DataInputStream(sc.getInputStream());
			
			shell();
			
			in.close();
			out.close();
			sc.close();
		} catch(Exception e){
			System.out.println("Error in the connection to the broker "+_server+":"+_port);
		}
		System.out.println("+++ FINISHED +++");	
	}
}
