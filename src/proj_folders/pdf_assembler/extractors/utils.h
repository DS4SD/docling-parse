//-*-C++-*-

/***********************************************************
IBM Confidential
OCO Source Materials
IBM Research Zurich Smart Annotator Licensed Internal Code
(C) Copyright IBM Corp. 2017

Authors: Peter W. J. Staar, Michele Dolfi, Christoph Auer,
Matthieu Mottet 
Email: {taa, dol, cau, ttt}@zurich.ibm.com
Date: 18/08/2017
**********************************************************/

#ifndef DOCUMENT_EXTRACTOR_UTILITIES_H
#define DOCUMENT_EXTRACTOR_UTILITIES_H

namespace pdf_lib
{
  class extractor_utilities
  {

  public:

    extractor_utilities();
    ~extractor_utilities();

    static void contract_text(container_lib::container& list);
    
    static std::vector<double> contract_bbox(std::vector<double>& bbox_c,
					     std::vector<double>& bbox_n);
    
    static container_lib::container contract_prov(container_lib::container& prov_c,
						  container_lib::container& prov_n);
    
    static container_lib::container contract_refs(container_lib::container& refs_c,
						  container_lib::container& refs_n);


  private:
    
  };

  void extractor_utilities::contract_text(container_lib::container& list)
  {
    //logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    for(int l=0; l<int(list.get_size())-1; )
      {
	std::string type_c, type_n;
	type_c <= list[l+0]["type"];
	type_n <= list[l+1]["type"];

	std::string text_c, text_n;

	std::vector<double> bbox_c, bbox_n, bbox;

	container_lib::container prov_c, prov_n, prov;
	container_lib::container refs_c, refs_n, refs;
	
	bool updated=false;
	if(type_c==type_n                        and
	   (type_c!="table" or type_c!="figure") and
	   list[l+0].has("text")                 and
	   list[l+1].has("text")                   )
	  {
	    text_c <= list[l+0]["text"];
	    text_n <= list[l+1]["text"];

	    bbox_c <= list[l+0]["bbox"];
	    bbox_n <= list[l+1]["bbox"];

	    prov_c <= list[l+0]["prov"];
	    prov_n <= list[l+1]["prov"];

	    refs_c <= list[l+0]["refs"];
	    refs_n <= list[l+1]["refs"];

	    if((text_c.back()==' '                          or 
		text_c.back()==','                          or 
		('a'<=text_c.back() and text_c.back()<='z') or
		('A'<=text_c.back() and text_c.back()<='Z')   ) and
	       (text_n.front()==' '                         or
                text_n.front()==','                         or
		('a'<=text_n.front() and text_n.front()<='z'))
	       )
	      {
		text_c += " "+text_n;
		list[l+0]["text"] <= text_c;

		bbox = contract_bbox(bbox_c, bbox_n);
		list[l+0]["bbox"] <= bbox;

		prov = contract_prov(prov_c, prov_n);
		list[l+0]["prov"] <= prov;

		refs = contract_refs(refs_c, refs_n);
		list[l+0]["refs"] <= refs;
		
		updated=true;
	      }	    
	    else if(text_c.back()=='-' and
	       ('a'<=text_n.front() and text_n.front()<='z'))
	      {
		text_c.pop_back();

		text_c += text_n;
		list[l+0]["text"] <= text_c;

		bbox = contract_bbox(bbox_c, bbox_n);
		list[l+0]["bbox"] <= bbox;

		prov = contract_prov(prov_c, prov_n);
		list[l+0]["prov"] <= prov;

		refs = contract_refs(refs_c, refs_n);
		list[l+0]["refs"] <= refs;
		
		updated=true;
	      }
	    else
	      {
	      }
	  }

	if(updated)
	  {
	    list.erase(l+1);
	  }
	else
	  {
	    l += 1;
	  }
      }
  }
  
  std::vector<double> extractor_utilities::contract_bbox(std::vector<double>& bbox_c,
							 std::vector<double>& bbox_n)
  {
    assert(bbox_c.size()==4);
    assert(bbox_n.size()==4);
    std::vector<double> bbox = bbox_c;
    
    {
      bbox[0] = std::min(bbox_c[0], bbox_n[0]);
      bbox[1] = std::min(bbox_c[1], bbox_n[1]);
      bbox[2] = std::max(bbox_c[2], bbox_n[2]);
      bbox[3] = std::max(bbox_c[3], bbox_n[3]);
    }

    return bbox;
  }

  container_lib::container extractor_utilities::contract_prov(container_lib::container& prov_c,
							      container_lib::container& prov_n)
  {
    assert(prov_c.get_size()>0);

    if(prov_n.get_size()>1)
      {
	for(int l=0; l<prov_n.get_size(); l++)
	  {
	    container_lib::container tmp;
	    tmp.resize(1);
	    tmp[0] = prov_n[l];

	    prov_c = contract_prov(prov_c, tmp);
	  }

	return prov_c;
      }
    else if(prov_n.get_size()==1)
      {
	container_lib::container prov;
	prov = prov_c;
	
	int M = prov_c.get_size()-1;
	int N = prov_n.get_size()-1;
	
	int page_c, page_n;
	{
	  page_c <= prov_c[M]["page"];
	  page_n <= prov_n[N]["page"];
	}
	
	if(page_c==page_n)
	  {
	    std::vector<double> bbox_c, bbox_n;
	    {
	      bbox_c <= prov_c[M]["bbox"];
	      bbox_n <= prov_n[N]["bbox"];
	      
	      bbox_c = contract_bbox(bbox_c, bbox_n);		
	    }
	    
	    std::vector<int> span_c, span_n;
	    {
	      span_c <= prov_c[M]["span"];
	      span_n <= prov_n[N]["span"];
	      
	      span_c[1] += span_n[1];
	    }
	    
	    {
	      prov[M]["bbox"] <= bbox_c;
	      prov[M]["span"] <= span_c;
	    }
	    
	    /*
	      {
	      IO::writer<IO::JSON_CONTAINER> writer;
	      std::cout << "\n----------------------------------\n";
	      std::cout << "\n" << writer.to_string(prov_c) << "\n";
	      std::cout << "\n" << writer.to_string(prov_n) << "\n";
	      std::cout << "\n" << writer.to_string(prov)   << "\n";
	      std::cout << "\n----------------------------------\n";
	      }
	    */
	  }
	else
	  {
	    std::vector<int> span_c, span_n;
	    {
	      span_c <= prov_c[M]["span"];
	      span_n <= prov_n[N]["span"];
	      
	      span_n[0] += span_c[1];
	      span_n[1] += span_c[1];
	      
	      prov_n[N]["span"] <= span_n;
	    }
	    
	    int M = prov.get_size();
	    prov.resize(M+1);
	    
	    prov[M] = prov_n[0];
	    
	    /*
	      {
	      IO::writer<IO::JSON_CONTAINER> writer;
	      std::cout << "\n----------------------------------\n";
	      std::cout << "\n" << writer.to_string(prov_c) << "\n";
	      std::cout << "\n" << writer.to_string(prov_n) << "\n";
	      std::cout << "\n" << writer.to_string(prov)   << "\n";
	      std::cout << "\n----------------------------------\n";
	      }
	    */
	  }

	return prov;
      }
      else
	{
	  logging_lib::warn("pdf-assembler") << "WARNING: provenance does not have the right properties";
	  return prov_c;
	}
  }

  container_lib::container extractor_utilities::contract_refs(container_lib::container& refs_c,
							      container_lib::container& refs_n)
  {
    container_lib::container result;
    result = refs_c;
    
    for(int i=0; i<refs_n.get_size(); i++)
      {
	int N = result.get_size();
	result.resize(N+1);

	result[N] = refs_n[i];
      }

    return result;
  }

  
}

#endif
