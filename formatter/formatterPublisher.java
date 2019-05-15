package formatter;

import javax.xml.ws.Endpoint;

public class formatterPublisher{
    public static void main(String[] args){
        final String url = "http://localhost:8888/formatterService";
        System.out.println("Publishing formatterService at endpoint "+url);
        Endpoint.publish(url, new formatterService());
    }
}
