
package service;

import javax.xml.bind.JAXBElement;
import javax.xml.bind.annotation.XmlElementDecl;
import javax.xml.bind.annotation.XmlRegistry;
import javax.xml.namespace.QName;


/**
 * This object contains factory methods for each 
 * Java content interface and Java element interface 
 * generated in the service package. 
 * <p>An ObjectFactory allows you to programatically 
 * construct new instances of the Java representation 
 * for XML content. The Java representation of XML 
 * content can consist of schema derived interfaces 
 * and classes representing the binding of schema 
 * type definitions, element declarations and model 
 * groups.  Factory methods for each of these are 
 * provided in this class.
 * 
 */
@XmlRegistry
public class ObjectFactory {

    private final static QName _FormatterTextResponse_QNAME = new QName("http://formatter/", "formatterTextResponse");
    private final static QName _FormatterText_QNAME = new QName("http://formatter/", "formatterText");

    /**
     * Create a new ObjectFactory that can be used to create new instances of schema derived classes for package: service
     * 
     */
    public ObjectFactory() {
    }

    /**
     * Create an instance of {@link FormatterTextResponse }
     * 
     */
    public FormatterTextResponse createFormatterTextResponse() {
        return new FormatterTextResponse();
    }

    /**
     * Create an instance of {@link FormatterText }
     * 
     */
    public FormatterText createFormatterText() {
        return new FormatterText();
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link FormatterTextResponse }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "http://formatter/", name = "formatterTextResponse")
    public JAXBElement<FormatterTextResponse> createFormatterTextResponse(FormatterTextResponse value) {
        return new JAXBElement<FormatterTextResponse>(_FormatterTextResponse_QNAME, FormatterTextResponse.class, null, value);
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link FormatterText }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "http://formatter/", name = "formatterText")
    public JAXBElement<FormatterText> createFormatterText(FormatterText value) {
        return new JAXBElement<FormatterText>(_FormatterText_QNAME, FormatterText.class, null, value);
    }

}
