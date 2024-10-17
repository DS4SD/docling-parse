//-*-C++-*-

#ifndef QPDF_STREAM_DECODER_H
#define QPDF_STREAM_DECODER_H

#include <exception>

namespace pdflib
{

  class qpdf_stream_decoder:
    public QPDFObjectHandle::ParserCallbacks
  {
  public:

    qpdf_stream_decoder(std::vector<qpdf_instruction>& stream_);
    ~qpdf_stream_decoder();

    void print();

    void decode(QPDFObjectHandle& content);

    // methods used by the QPDFObjectHandle::ParserCallbacks
    ///*virtual*/ void handleObject(QPDFObjectHandle obj) override;

    void handleObject(QPDFObjectHandle obj, size_t offset, size_t len) override;
    
    void contentSize(size_t len) override;

    void handleEOF() override;
    
  private:

    std::vector<qpdf_instruction>& stream;
  };

  qpdf_stream_decoder::qpdf_stream_decoder(std::vector<qpdf_instruction>& stream_):
    QPDFObjectHandle::ParserCallbacks(),
    stream(stream_)
  {}

  qpdf_stream_decoder::~qpdf_stream_decoder()
  {}

  void qpdf_stream_decoder::print()
  {
    for(auto row:stream)
      {
        LOG_S(INFO) << std::setw(12) << row.key << " | " << row.val;
      }
  }

  void qpdf_stream_decoder::decode(QPDFObjectHandle& content)
  {
    LOG_S(INFO) << "start decoding content-stream: " << content.getTypeName() << " -> " << content.unparse();

    stream.clear();

    try
      {
        QPDFObjectHandle::parseContentStream(content, this);
      }
    catch(std::exception& e)
      {
        LOG_S(ERROR) << "QPDF encountered error (" << e.what() << ") during decoding";
      }

    LOG_S(WARNING) << "finished decoding content-stream!";
  }

  /*
  void qpdf_stream_decoder::handleObject(QPDFObjectHandle obj)
  {
    LOG_S(INFO) << __FUNCTION__;

    qpdf_instruction row;
    {
      row.key = obj.getTypeName();
      row.val = obj.unparse();
      row.obj = obj;

      //LOG_S(INFO) << std::setw(12) << row.key << " | " << row.val;
    }

    stream.push_back(row);
  }
  */

  void qpdf_stream_decoder::handleObject(QPDFObjectHandle obj, size_t offset, size_t len)
  {
    //LOG_S(INFO) << __FUNCTION__ << "\t offset: " << offset << ", len: " << len;

    qpdf_instruction row;
    {
      row.key = obj.getTypeName();
      row.val = obj.unparse();
      row.obj = obj;

      //LOG_S(INFO) << std::setw(12) << row.key << " | " << row.val;
    }

    stream.push_back(row);
  }

  void qpdf_stream_decoder::contentSize(size_t len)
  {
    //LOG_S(INFO) << __FUNCTION__ << ": " << len;
  }

  void qpdf_stream_decoder::handleEOF()
  {
    //LOG_S(INFO) << __FUNCTION__;
  }

}

#endif
