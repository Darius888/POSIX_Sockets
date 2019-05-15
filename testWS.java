import service.FormatterService;
import service.FormatterServiceService;

public class testWS{
    public static void main(String[] args){
        FormatterServiceService service = new FormatterServiceService();
	FormatterService port = service.getFormatterServicePort();
        String _text = "Test  message 1 or  24 and why not  53";
        _text = port.formatterText(_text);
        System.out.println("MESSAGE : "+_text);
    }
}
