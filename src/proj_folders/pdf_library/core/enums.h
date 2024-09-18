//-*-C++-*-

#ifndef PDF_LIBRARY_CORE_ENUMS_H
#define PDF_LIBRARY_CORE_ENUMS_H

namespace pdf_lib
{
  namespace core
  {
    enum colorspace_type {
      COLORSPACE_UNKNOWN=-1,
      COLORSPACE_RGB=0,
      COLORSPACE_CMYK=1,
      COLORSPACE_GRAY=2,
      COLORSPACE_ICCBASED=3
    };

    colorspace_type to_colorspace_name(std::string name)
    {
      if(name=="COLORSPACE_RGB" or name=="DeviceRGB" or name=="/DeviceRGB")
	return COLORSPACE_RGB;
      else if(name=="COLORSPACE_CMYK" or name=="DeviceCMYK" or name=="/DeviceCMYK")
	return COLORSPACE_CMYK;
      else if(name=="COLORSPACE_GRAY" or name=="DeviceGray" or name=="/DeviceGray")
	return COLORSPACE_GRAY;
      else if(name=="COLORSPACE_ICCBASED" or name=="ICCBased" or name=="/ICCBased")
	return COLORSPACE_ICCBASED;
      else
	return COLORSPACE_UNKNOWN;
    }

    inline std::string to_string(colorspace_type name)
    {
      switch(name)
	{
	case COLORSPACE_UNKNOWN: return "COLORSPACE_UNKNOWN";
	case COLORSPACE_RGB:    return "COLORSPACE_RGB";
	case COLORSPACE_CMYK:   return "COLORSPACE_CMYK";
	case COLORSPACE_GRAY:   return "COLORSPACE_GRAY";
	case COLORSPACE_ICCBASED:   return "COLORSPACE_ICCBASED";
	}

      return "COLORSPACE_UNKNOWN";
    }
    
    enum OPERATORS {
      UNKNOWN,

      b,  B, bS, BS, BDC, BI, BMC, BT, BX,

      c, cm, cs, CS,

      d, d0, d1, Do,

      EI, EMC, ET, EX,
      
      Tc, Td, TD, Tf, Tj, TJ, TL, Tm,
      Ts, Tw, Tz, TS, 

      q , Q,
      
      m, M,
      l, v,  y,  n,
      
      Tr,  

      h, 

      i, ID,
      j, J,

      f,  F, fS,

      k, K, 
      g, gs, G,

      re, rg, RG, ri,
      
      s  , S,
      sc , scn, 
      SC , SCN,
      sh,

      w, W, WS,

      accent, daccent,

      
      // FIXME in QPDF
      // We have encountered pdf's with operator qq [USPTO 6,014,702]
      // I believe qq might refer to the operator q. This is either a bug in
      // the qpdf library or in the pdf-itself!
      // extra operators:
      ETBT, ETq, qq, QBT, Sq, DoBT, Doq      
    };

    typedef OPERATORS operator_type;
    
    inline std::string to_string(operator_type ot)
    {
      switch(ot)
	{
	case UNKNOWN : return "UNKNOWN";

	case b       : return "b";
	case B       : return "B";
	case bS      : return "b*";
	case BS      : return "B*";

	case BDC     : return "BDC";
	case BI      : return "BDC";
	case BMC     : return "BDC";
	case BT      : return "BT";
	case BX      : return "BX";

	case c       : return "c" ;
	case cm      : return "cm";
	case cs      : return "cs";	  
	case CS      : return "CS";

	case d       : return "d";
	case d0      : return "d0";
	case d1      : return "d1";
	case Do      : return "Do";

	case EI      : return "EI";
	case EMC     : return "EMC";
	case ET      : return "ET";
	case EX      : return "EX";

	case Tc      : return "Tc";
	case Td      : return "Td";
	case TD      : return "TD";
	case Tf      : return "Tf";
	case Tj      : return "Tj";
	case TJ      : return "TJ";
	case TL      : return "TL";
	case Tm      : return "Tm";
	case Ts      : return "Ts";
	case Tw      : return "Tw";
	case Tz      : return "Tz";
	case TS      : return "T*";

	case q       : return "q" ;
	case Q       : return "Q" ;

	case m       : return "m" ;
	case M       : return "M" ;

	case l       : return "l" ;

	case v       : return "v" ;
	case y       : return "y" ;
	case n       : return "n" ;

	case Tr      : return "Tr";

	case f       : return "f";
	case F       : return "F";
	case fS      : return "f*";

	case g       : return "g";
	case G       : return "G";
	case gs      : return "gs";

	case h       : return "h";

	case i       : return "i";
	case ID      : return "ID";

	case j       : return "j";
	case J       : return "J";

	case k       : return "k";
	case K       : return "K";

	case re      : return "re";
	case rg      : return "rg";
	case RG      : return "RG";
	case ri      : return "ri";

	case s       : return "s";
	case S       : return "S";

	case scn     : return "scn";
	case SCN     : return "SCN";

	case sc      : return "sc";	  
	case SC      : return "SC";

	case sh      : return "sh";

	case w       : return "w";
	case W       : return "W";
	case WS      : return "W*";

	case accent  : return "'";
	case daccent : return "\"";	  

	  // FIXME in QPDF
	  // extra operators, to fix some erroneous operator-parsing in QPF	  
	case ETBT    : return "ETBT";
 	case ETq     : return "ETq";
	case qq      : return "qq";
	case QBT     : return "QBT";
	case Sq      : return "Sq";
	case DoBT    : return "DoBT";
	case Doq    : return "Doq";
	  
	default      : return "UNKNOWN";
	}
    }

    operator_type to_operator_type(std::string op_string)
    {
      static std::map<std::string, core::operator_type> map = {
	{   "b", core::b   }, {   "B", core::B  }, {  "b*", core::bS }, { "B*", core::BS},
	{ "BDC", core::BDC }, {  "BI", core::BI }, { "BMC", core::BMC}, { "BT", core::BT}, { "BX", core::BX},
	
	{ "c", core::c }, { "cm", core::cm}, { "cs", core::cs }, { "CS", core::CS}, 
	
	{ "d", core::d }, { "d0", core::d0}, { "d1", core::d1 },
	
	{"Do", core::Do}, 
	
	{ "EI", core::EI }, { "EMC", core::EMC}, {"ET", core::ET}, {"EX", core::EX}, 

	{"Tc", core::Tc}, {"Td", core::Td}, {"TD", core::TD}, {"Tf", core::Tf},
	{"Tj", core::Tj}, {"TJ", core::TJ}, {"TL", core::TL}, {"Tm", core::Tm},
	{"Ts", core::Ts}, {"Tw", core::Tw}, {"Tz", core::Tz}, {"T*", core::TS},

	{ "q", core::q }, { "Q", core::Q }, 

	{ "m", core::m }, { "l", core::l }, 

	{ "v", core::v }, { "y", core::y }, { "n", core::n }, 
	{"Tr", core::Tr}, { "h", core::h }, 

	{"f" , core::f }, { "F", core::F }, {"f*", core::fS},  	

	{"g" , core::g }, { "G", core::G }, {"gs", core::gs}, 

	{"h", core::h}, 
	{"i", core::i}, {"ID", core::ID}, 

	{"j", core::j}, {"J", core::J}, 

	{"k" , core::k }, { "K", core::K }, 

	{"m" , core::m }, { "M", core::M }, 

	{"re", core::re}, {"rg", core::rg}, {"RG", core::RG}, {"ri", core::ri}, 
	
	{ "s" , core::s  }, {   "S", core::S   },
	{"sc" , core::sc }, {  "SC", core::SC  }, 
	{"scn", core::scn}, { "SCN", core::SCN },
	{ "sh", core::sh },

	{ "w", core::w }, { "W", core::W }, { "W*", core::WS }, 

	{ "'", core::accent }, { "\"", core::daccent },

	// FIXME in QPDF
	// extra operators, to fix some erroneous operator-parsing in QPF
	{ "ETBT", core::ETBT },
 	{ "ETq" , core::ETq  },
	{ "qq"  , core::qq   },
	{ "QBT" , core::QBT  },
	{ "Sq"  , core::Sq   },
	{ "DoBT", core::DoBT },
	{ "Doq" , core::Doq }
      };
      
      return map.count(op_string)==0? UNKNOWN : map.at(op_string);
    }

    bool is_operator_type(std::string op_string)
    {
      return (to_operator_type(op_string)!=UNKNOWN);
    }    
  }

}

#endif
