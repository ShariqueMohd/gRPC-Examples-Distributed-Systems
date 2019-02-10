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
  DataConversionClient(std::shared_ptr<Channel> channel) : stub_(DataConversion::NewStub(channel)) { }

  void Push_Data(std::string FT, std::string TT, float value) {
	Data inpData;
	inpData.set_fromtype(FT); inpData.set_totype(TT); inpData.set_value(value); 
	data_list_.push_back(inpData);
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
	
	std::ifstream inFile("data.txt");
	
	// Assuming we have correct data.
	std::cout << "Entered\n";
	std::string ft, temp, tt, strValue; 
	float value;		
	
	while(inFile >> ft >> temp >> tt >> strValue) {
		value = std::stof(strValue);	
		guide.Push_Data(ft, tt, value);
	}
	
	inFile.close();	
	std::cout << "Get converted value\n";	
	guide.GetConvertedValue();
}
