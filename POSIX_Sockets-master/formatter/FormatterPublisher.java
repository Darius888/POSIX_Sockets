package formatter;

import javax.xml.ws.Endpoint;

public class FormatterPublisher{
    public static void main(String[] args){
        final String url = "http://localhost:8888/rs";
        System.out.println("Publishing FormatterService at endpoint "+url);
        Endpoint.publish(url, new FormatterService());
    }
}
