//-*-C++-*-

#ifndef QPDF_UTILS_H
#define QPDF_UTILS_H

#include <sstream>
#include <iostream>
#include <iomanip>

#include <nlohmann/json.hpp>

#include <qpdf/QPDF.hh>

namespace pdflib
{
  // FIXME: add a begin time to cap the max time spent in this routine
  nlohmann::json to_json(QPDFObjectHandle obj, std::set<std::string> prev_objs={},
                         int level=0, int max_level=32)
  {
    nlohmann::json result;

    if(obj.isDictionary() or obj.isArray())
      {
        std::string this_obj = obj.unparse();

        if(not utf8::is_valid(this_obj.begin(), this_obj.end()))
          {
            utf8::replace_invalid(this_obj.begin(), this_obj.end(),
                                  std::back_inserter(this_obj));
            LOG_S(WARNING) << "fixed already encountered object: " << this_obj;
          }

        if(prev_objs.count(this_obj)==1)
          {
            result = "[ref to previous object: '"+this_obj+"']";
            return result;
          }
        else
          {
            prev_objs.insert(this_obj);
          }
      }

    if(level<max_level)
      {
        const static std::set<std::string> keys_to_be_skipped = {"/Parent", "/P", "/Annots", "/B"};

        if(obj.isDictionary())
          {
            /*
              LOG_S(INFO) << "detected dict: ";
              for(auto key : obj.getKeys())
              {
              LOG_S(INFO) << " -> key: " << key;
              }
            */

            for(auto key : obj.getKeys())
              {
                //LOG_S(INFO) << "key: " << key;

                if(keys_to_be_skipped.count(key)==1)
                  {
                    result[key] = "[skipping " + key + "]";
                  }
                else
                  {
                    result[key] = to_json(obj.getKey(key), prev_objs, level+1);
                  }
              }
          }
        else if(obj.isArray())
          {
            //LOG_S(INFO) << "array: " << obj.getArrayNItems();
            for(int l=0; l<obj.getArrayNItems(); l++)
              {
                QPDFObjectHandle new_obj = obj.getArrayItem(l);

                auto item = to_json(new_obj, prev_objs, level+1);
                result.push_back(item);
              }
          }
        else if(obj.isStream())
          {
	    /*
	    std::string val = "";
	    
	    try
	      {
		// Convert raw data to std::string
		std::shared_ptr<Buffer> ptr = obj.getStreamData(qpdf_dl_all);
		val = std::string(reinterpret_cast<const char*>(ptr->getBuffer()), ptr->getSize());
	      }
	    catch(const std::exception& exc)
	      {
		LOG_S(ERROR) << "could not stream data, " << exc.what();
		val = obj.unparse()+" [stream]";
	      }
	    */
	    
	    std::string val = obj.unparse()+" [stream]";
	    
            if(utf8::is_valid(val.begin(), val.end()))
              {
                result = val;
              }
            else
              {
                LOG_S(WARNING) << "val is not utf8: " << val;
              }
          }
        else if(obj.isName())
          {
            std::string val = obj.getName();

            if(utf8::is_valid(val.begin(), val.end()))
              {
                result = val;
              }
            else
              {
                LOG_S(WARNING) << "val is not utf8: " << val;

                std::string tmp;
                utf8::replace_invalid(val.begin(), val.end(),
                                      std::back_inserter(tmp));

                LOG_S(WARNING) << " --> " << tmp;

                result = tmp;
              }
          }
        else if(obj.isString())
          {
            std::string val = obj.getUTF8Value();

            if(utf8::is_valid(val.begin(), val.end()))
              {
                result = val;
              }
            else
              {
                LOG_S(WARNING) << "val is not utf8: " << val;

                utf8::replace_invalid(val.begin(), val.end(),
                                      std::back_inserter(val));

                LOG_S(WARNING) << " --> " << val;

                result = val;
              }
          }
        else if(obj.isInteger())
          {
            int val = obj.getIntValue();
            result = val;
          }
        else if(obj.isReal())
          {
            double val = obj.getNumericValue();
            result = val;
          }
        else if(obj.isBool())
          {
            bool val = obj.getBoolValue();
            result = val;
          }
        else
          {
            std::string val = obj.unparse() + " ["+obj.getTypeName()+"]";
            LOG_S(INFO) << "unidentified value: " << val;

            if(utf8::is_valid(val.begin(), val.end()))
              {
                result = val;
              }
            else
              {
                LOG_S(WARNING) << "val is not utf8: " << val;

                utf8::replace_invalid(val.begin(), val.end(),
                                      std::back_inserter(val));

                LOG_S(WARNING) << " --> " << val;

                result = val;
              }
          }
      }
    else
      {
        LOG_S(WARNING) << __FUNCTION__ << "\t level=" << level << "\t" << obj.unparse();
        result = "[exceeding recursion]";
      }

    return result;
  }

  void print_obj(QPDFObjectHandle obj, int level = 0)
  {
    if(obj.isDictionary())
      {
        for(auto key : obj.getKeys())
          {
            std::cout << std::setw(level * 4) << " " << key << ": ";
            if(key=="/Parent")
              {
                std::cout << " [skipping] \n";
                continue;
              }

            if(obj.getKey(key).isStream())
              {
                std::cout << "Stream" << std::endl;
                print_obj(obj.getKey(key).getDict(), level + 1);
              }
            else if(obj.getKey(key).isDictionary())
              {
                std::cout << std::endl;
                print_obj(obj.getKey(key), level + 1);
              }
            else
              {
                print_obj(obj.getKey(key), 0);
              }
            /*
              else if(obj.getKey(key).isInteger())
              {
              obj.getKey(key).getObj(), level + 1);
              }
              else
              {
              std::cout << "[" << obj.getKey(key).getTypeName() << "] --> "
              << obj.getKey(key).unparse() << std::endl;
              }
            */
          }
      }
    else if(obj.isArray())
      {
        std::cout << std::setw(level * 4) << "[\n";
        for(int l=0; l<obj.getArrayNItems(); l++)
          {
            QPDFObjectHandle new_obj = obj.getArrayItem(l);
            //std::cout << std::setw(level * 4) << "[" << new_obj.getTypeName() << "]\t";

            print_obj(new_obj, level+1);
          }
        std::cout << std::setw(level * 4) << "]\n";
      }
    else
      {
        //std::cout << std::setw(level * 4);
        for(int l=0; l<level*4; l++)
          std::cout << " ";

        if(obj.isName())
          {
            std::cout << obj.getName() << std::endl;
          }
        else if(obj.isString())
          {
            std::cout << obj.getUTF8Value() << std::endl;
          }
        else if(obj.isInteger())
          {
            std::cout << obj.getIntValue() << std::endl;
          }
        else
          {
            std::cout << "[" << obj.getTypeName() << "] --> "
                      << obj.unparse() << std::endl;
          }
      }

  }

  // FIXME: add a begin time to cap the max time spent in this routine
  nlohmann::json extract_annots_in_json(QPDFObjectHandle obj,
                                        std::set<std::string> prev_objs={},
                                        int level=0, int max_level=16)
  {
    LOG_S(INFO) << __FUNCTION__;

    /*
    if(obj.isDictionary())
      {
        for(auto key : obj.getKeys())
          {
            LOG_S(INFO) << "key: " << key;
          }
      }
    */
    
    nlohmann::json result;

    if(level==0 and obj.isDictionary() and
       obj.hasKey("/Annot"))
      {
        result = to_json(obj.getKey("/Annot"), prev_objs, level, max_level);
      }
    else if(level==0 and obj.isDictionary() and
            obj.hasKey("/Annots"))
      {
        result = to_json(obj.getKey("/Annots"), prev_objs, level, max_level);
      }

    return result;
  }

  /*** Top level Annotations ***/

  nlohmann::json extract_acroform_in_json(QPDF& pdf_obj,
					  QPDFObjectHandle& root)
  {
    nlohmann::json result = nlohmann::json::value_t::null;
    
    if(root.hasKey("/AcroForm"))
      {
	LOG_S(INFO) << "/AcroForm detected!";
	
	try
	  {
	    result = to_json(root.getKey("/AcroForm"), {}, 0, 16);
	  }
	catch(const std::exception& exc)
	  {
	    LOG_S(ERROR) << "encountered exception: " << exc.what(); 
	  }
      }
    else
      {
	LOG_S(INFO) << "no /AcroForm detected ...";
      }

    return result;
  }

  nlohmann::json extract_metadata_in_json(QPDF& pdf_obj,
					  QPDFObjectHandle& root)
  {
    nlohmann::json result = nlohmann::json::value_t::null;

    if(root.hasKey("/Metadata"))
      {
	LOG_S(INFO) << "/Metadata detected!";
	
	try
	  {
	    QPDFObjectHandle metadata = root.getKey("/Metadata");

	    if(metadata.isStream())
	      {
		std::shared_ptr<Buffer> ptr = metadata.getStreamData(qpdf_dl_all);
		
		// Convert raw data to std::string
		std::string content(reinterpret_cast<const char*>(ptr->getBuffer()), ptr->getSize());
		result = content;
	      }
	    else
	      {
		LOG_S(ERROR) << "metadata is not a stream"; 
	      }
	  }
	catch(const std::exception& exc)
	  {
	    LOG_S(ERROR) << "encountered exception: " << exc.what(); 
	  }
      }
    else
      {
	LOG_S(INFO) << "no /Metadata detected ...";
      }
    
    return result;
  }

  nlohmann::json extract_language_in_json(QPDF& pdf_obj,
					  QPDFObjectHandle& root)
  {
    nlohmann::json result = nlohmann::json::value_t::null;

    if(root.hasKey("/Lang"))
      {
	LOG_S(INFO) << "/Lang detected!";	
	
        std::string lang = root.getKey("/Lang").getUTF8Value();
	result = lang;
      }
    else
      {
	LOG_S(INFO) << "no /Lang detected ...";	
      }
    
    return result;
  }
  
  /*** Table of Contents ***/

  nlohmann::json extract_toc_entry_in_json(QPDF& pdf_obj, QPDFObjectHandle& node, int level)
  {
    //LOG_S(INFO) << __FUNCTION__;
    
    nlohmann::json toc_entry;

    //for(auto key : node.getKeys())
    //{
    //LOG_S(INFO) << " -> key: " << key;
    //}
    
    // Extract title
    if(node.hasKey("/Title"))
      {
        toc_entry["title"] = node.getKey("/Title").getUTF8Value();
        toc_entry["level"] = level;
      }

    // Extract title
    if(node.hasKey("/A"))
      {
        toc_entry["link"] = to_json(node.getKey("/A"), {}, 0, 8);
      }
    
    // Extract destination
    if(node.hasKey("/Dest"))
      {
	LOG_S(INFO) << "found a destination!";
	
        // Depending on the type of destination, extract its value
	auto dest = node.getKey("/Dest");
        toc_entry["destination"] = to_json(dest, {}, 0, 8);

	/*
        if(dest.isString())
          {
            toc_entry["destination"] = dest.getUTF8Value();
          }
        else if(dest.isName())
          {
            toc_entry["destination"] = dest.getName();
          }
        else if(dest.isArray())
          {
            auto array = dest.getArrayAsVector();
            std::string result;

            // Extract page reference
            if(!array.empty() and array[0].isIndirect())
              {
                //QPDFObjectHandle page_ref = array[0];

                //int page_number = pdf_obj.getPageNumberForObject(page_ref);
                //toc_entry["page_number"] = page_number;

                //toc_entry["destination"] = dest.unparse();
              }
          }
        else
          {
            // Placeholder for complex cases
            //toc_entry["destination"] = "Complex destination";
          }
	*/
      }
    else
      {
        //toc_entry["destination"] = "No destination";
      }

    // Extract children
    if (node.hasKey("/First"))
      {
        QPDFObjectHandle first = node.getKey("/First");

        while (first.isDictionary())
          {
	    auto child = extract_toc_entry_in_json(pdf_obj, first, level+1);
            toc_entry["children"].push_back(child);
	    
            if(first.hasKey("/Next"))
              {
                first = first.getKey("/Next");
              }
            else
              {
                break;
              }
          }
      }

    return toc_entry;
  }

  nlohmann::json extract_toc_in_json(QPDF& pdf_obj, QPDFObjectHandle& root)
  {
    LOG_S(INFO) << __FUNCTION__;
    
    nlohmann::json toc = nlohmann::json::value_t::null;

    if(root.hasKey("/Outlines"))
      {
	LOG_S(INFO) << "/Outlines (=table-of-contents) detected!";
	
        QPDFObjectHandle outlines = root.getKey("/Outlines");

        if (outlines.hasKey("/First"))
          {
            QPDFObjectHandle first = outlines.getKey("/First");

            toc = nlohmann::json::array({});

            while (first.isDictionary())
              {
		int level=0;
                toc.push_back(extract_toc_entry_in_json(pdf_obj, first, level));

                if (first.hasKey("/Next"))
                  {
                    first = first.getKey("/Next");
                  }
                else
                  {
                    break;
                  }
              }
          }
      }
    else
      {
        LOG_S(WARNING) << "no /Outlines (=table-of-contents) detected ...";
      }

    return toc;
  }
  
  nlohmann::json extract_document_annotations_in_json(QPDF& pdf_obj,
						      QPDFObjectHandle& root)
  {
    LOG_S(INFO) << __FUNCTION__;
    
    nlohmann::json annots = nlohmann::json::object({});

    annots["form"] = extract_acroform_in_json(pdf_obj, root);
    
    annots["meta_xml"] = extract_metadata_in_json(pdf_obj, root);

    annots["language"] = extract_language_in_json(pdf_obj, root);

    annots["table_of_contents"] = extract_toc_in_json(pdf_obj, root);

    //LOG_S(INFO) << "annotations: " << annots.dump(2);
    
    return annots;
  }

}

#endif
