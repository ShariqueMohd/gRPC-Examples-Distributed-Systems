#include <bits/stdc++.h>

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include "data_conversion.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;

using dataconversion::Data;
using dataconversion::ConvertedValue;
using dataconversion::DataConversion;

class DataConversionClient {

public:
  DataConversionClient(std::shared_ptr<Channel> channel) : stub_(DataConversion::NewStub(channel)) { 
	Data inchTocm;
	inchTocm.set_fromtype("inch"); inchTocm.set_totype("cm"); inchTocm.set_value(1.0); data_list_.push_back(inchTocm);

	Data inchTomm;
	inchTomm.set_fromtype("inch"); inchTomm.set_totype("mm"); inchTomm.set_value(1.0); data_list_.push_back(inchTomm);

	Data cmToinch;
	cmToinch.set_fromtype("cm"); cmToinch.set_totype("inch"); cmToinch.set_value(1.0); data_list_.push_back(cmToinch);

	Data cmTomm;
	cmTomm.set_fromtype("cm"); cmTomm.set_totype("mm"); cmTomm.set_value(1.0); data_list_.push_back(cmTomm);

	Data lbTokg;
	lbTokg.set_fromtype("lb"); lbTokg.set_totype("kg"); lbTokg.set_value(1.0); data_list_.push_back(lbTokg);

	Data kgTolb;
	kgTolb.set_fromtype("kg"); kgTolb.set_totype("lb"); kgTolb.set_value(1.0); data_list_.push_back(kgTolb);
  }

  void GetConvertedValue() {
	for(auto datum: data_list_) {
		ConvertedValue convertedvalue;
		GetOneConvValue(datum, &convertedvalue);	
		std::cout << "Converted Value is = " << convertedvalue.value() << "\n";
	}
  }

private:
  bool GetOneConvValue(const Data& datum, ConvertedValue* convertedvalue) {
	ClientContext context;
	Status status = stub_->GetConvertedValue(&context, datum, convertedvalue);
	if(!status.ok()) {
		std::cout << "GetFeature rpc failed." << std::endl;
		return false;	
	}
	return true;
  } 


  std::unique_ptr<DataConversion::Stub> stub_;
  std::vector<Data> data_list_;

};

int main(int argc, char** argv) {
	DataConversionClient guide(grpc::CreateChannel("localhost:50051",
                          grpc::InsecureChannelCredentials()));
	
	std::cout << "Get converted value\n";	
	guide.GetConvertedValue();
}
