#include "xml_parser.hpp"

#include "context.hpp"

#include "attribute_template.hpp"
#include "object_template.hpp"
#include "group_template.hpp"

namespace xios
{
   namespace xml
   {
      /// ////////////////////// Définitions ////////////////////// ///

      void CXMLParser::ParseFile(const StdString & filename)
      {
         StdIFStream ifs ( filename.c_str() , StdIFStream::in );
         if ( (ifs.rdstate() & std::ifstream::failbit ) != 0 )
           ERROR("void CXMLParser::ParseFile(const StdString & filename)",
                  <<endl<< "Can not open <"<<filename<<"> file" );

         CXMLParser::ParseStream(ifs, filename);
      }

      void CXMLParser::ParseString(const StdString & xmlContent)
      {
         StdIStringStream iss ( xmlContent /*, StdIStringStream::in*/ );
         CXMLParser::ParseStream(iss,"string");
      }

      void CXMLParser::ParseStream(StdIStream & stream, const string& fluxId)
      {
         if (!stream.good())
            ERROR("CXMLParser::ParseStream(const StdIStream & stream)",
                  << "Bad xml stream !");
         StdOStringStream oss;
         while(!stream.eof() && !stream.fail ()) oss.put(stream.get());
         const StdString xmlcontent( oss.str(), 0, oss.str().size()-1 );
         try
         {
            rapidxml::xml_document<char> doc;
            doc.parse<0>(const_cast<char*>(xmlcontent.c_str()));

            CXMLNode node(doc.first_node());
            THashAttributes attributes;

            if (node.getElementName().compare(CXMLNode::GetRootName()) != 0)
               ERROR("CXMLParser::ParseStream(StdIStream & stream)",
                     << "Root element should be named simulation (actual = \'"
                     << node.getElementName() << "\')!");

            if (node.goToChildElement())
            {
               do
               {
                  CContextGroup* group_context = CContext::getRoot() ;
 
                  attributes = node.getAttributes();

                  if (attributes.end() == attributes.find("id"))
                  {  
                     DEBUG("The context will not be processed because it is not identified (missing id)");
                     continue; 
                  }

                  CContext::setCurrent(attributes["id"]) ;

                  bool hasctxt = CContext::has(attributes["id"]);

                  if(hasctxt)
                  {  
                     DEBUG("The context will not be processed because it exist an other context with the same id" );
                     continue; 
                  }

                  CContext* context = CContext::create(attributes["id"]);
//                  if (!hasctxt)  group_context->addChild(context);
                  context->parse(node);

                  attributes.clear();

               } while (node.goToNextElement());
            }
         }
         catch (rapidxml::parse_error & exc)
         {
            const char* ptr = exc.where<char>() ;
            const char* begin = xmlcontent.c_str() ;
            const char* content=oss.str().c_str() ;
            size_t pos=ptr-begin ;
            int lineNumber = 1 ;
            int columnNumber = 0 ;
            const char* line;
            const char* endLine;
            
            for(const char* i=content;i<content+pos; ++i, ++columnNumber) if (*i=='\n') { lineNumber++ ; line=i ; columnNumber=0 ;}
            for(endLine=content+pos; *endLine!='\n' && *endLine!='\0' ; ++endLine) ;
            string strLine(line,endLine-line) ;
                  
            ERROR("CXMLParser::ParseStream(StdIStream & stream)", << endl
                  << "Error is occuring when parsing XML flux from <"<<fluxId<<"> at character "<< pos<<" line "<<lineNumber<<" column "<< columnNumber<< endl
                  << strLine<<endl
                  << string(columnNumber-1,'x')<<'^'<<endl
                  <<" Error : " << exc.what() )
         }
      }

   }// namespace xml
} // namespace xios
