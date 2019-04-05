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
	public static ObjectOutputStream out;
	public static DataInputStream in;
		
	
	/********************* METHODS ********************/
	
	static int subscribe(String topic) 
	{	
		String subPort = "4000\0";
		try{
			out.writeObject(subPort);
			out.writeObject(topic);
			out.flush();
		}
		catch(Exception e){
			System.out.println("Error in the connection to the broker < server >: < port >");
		}
		byte response = -1;
		try{
			response = in.readByte();
		}
		catch(Exception e){
			System.out.println("Error in the connection to the broker < server >: < port >");
		}
		if(response == 0){
			System.out.println("c> SUBCRIBE OK");
			System.out.println("Subscribe to: " + topic);
			String resTopic = null;
			String resText = null;
			System.out.println("c > MESSAGE FROM "+resTopic+" : "+resText);
		}else if(response == 1){
			System.out.println("c> SUBCRIBE FAIL");
		}
        
		return 0;
	}

	static int unsubscribe(String topic) 
	{
		String subPort = "4000\0";
		try{
			out.writeObject(subPort);
			out.writeObject(topic);
			out.flush();
		}
		catch(Exception e){
			System.out.println("Error in the connection to the broker < server >: < port >");
		}
		byte response = -1;
		try{
			response = in.readByte();
		}
		catch(Exception e){
			System.out.println("Error in the connection to the broker < server >: < port >");
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
			String host = argv[1];
			int port = Integer.parseInt(argv[3]);
			Socket sc = new Socket(host, port);

			out = new ObjectOutputStream(sc.getOutputStream());
			in = new DataInputStream(sc.getInputStream());
			
			shell();
			
			in.close();
			out.close();
			sc.close();
		} catch(Exception e){
			System.out.println("Error in the connection to the broker <server>:<port>");
		}
		System.out.println("+++ FINISHED +++");	
	}
}
