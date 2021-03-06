package formatter;

import javax.jws.WebService;
import javax.jws.WebMethod;
import com.dataaccess.webservicesserver.*;
import java.math.*;

@WebService
    public class FormatterService {

        @WebMethod
            public String formatterText(String text){
                System.out.println("String sent "+text);
                /* call the NumberConversion web service */
                NumberConversion service= new NumberConversion();
                NumberConversionSoapType port = service.getNumberConversionSoap();
                String s=new String("Test  message 1 example 24");
                String [] lines = text.split(" ");
                String st = "";
                /* convert Integer to characters thanks to the web service used */
                for(int i = 0; i < lines.length; i++){
                    try{
                        Integer tmp = Integer.parseInt(lines[i]);
                        if(tmp instanceof Integer){
                            BigInteger bi=new BigInteger(lines[i]);
                            lines[i] = port.numberToWords(bi);
                        }
                    } catch(Exception e){
                        
                    }
                    st = st + lines[i]+" ";
                }
                /* replace the 2 spaces by 1 */
                System.out.println("String 1 "+st);
                String [] line = st.split("  ");
                String st2 = "";
                for(int i = 0; i < line.length; i++){
                    st2 = st2+line[i]+" ";
                }
                /* send the formatted text */
                System.out.println("String 2 "+st2);
                return st2;
            }
    }

