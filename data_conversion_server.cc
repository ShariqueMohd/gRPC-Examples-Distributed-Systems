#include <bits/stdc++.h>

#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>

#include "data_conversion.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;

using dataconversion::Data;
using dataconversion::ConvertedValue;
using dataconversion::DataConversion;

using std::chrono::system_clock;

class DataConversionImpl final : public DataConversion::Service {
  public:
	explicit DataConversionImpl() {
		Data obj;
		obj.set_fromtype("inch");
		obj.set_totype("cm");
		obj.set_value(1.0);
		data_list_.push_back(obj);
	}
	
	Status GetConvertedValue(ServerContext* context, const Data* data, ConvertedValue *convertedvalue) override {
		const std::string source = data->fromtype(), target = data->totype(); 		
		convertedvalue->set_convunit(target);
		if(source == "inch") {
			std::cout << "From inch to ";
			if(target == "cm") {
				std::cout << "cm\n";
				convertedvalue->set_value(data->value() * 2.54);			
			}
			else if(target == "mm") {
				std::cout << "mm\n";
				convertedvalue->set_value(data->value() * 25.4);			
			}
		}
		else if(source == "cm") {
			std::cout << "From cm to ";
			if(target == "inch") {
				std::cout << "inch\n";
				convertedvalue->set_value(data->value() / 2.54);
			}
			else if(target == "mm") {
				std::cout << "mm\n";
				convertedvalue->set_value(data->value() * 10.0);			
			}		
		}
		else if(source == "lb") {
			std::cout << "From lb to ";
			if(target == "kg") {
				std::cout << "kg\n";
				convertedvalue->set_value(data->value() * 0.453);
			}		
		}
		else if(source == "kg") {
			std::cout << "From kg to ";
			if(target == "lb") {
				std::cout << "lb\n";
				convertedvalue->set_value(data->value() / 0.453);			
			}		
		}
		return Status::OK;
	}

  private:
	std::vector<Data> data_list_;
};


void RunServer() {
	std::string server_address("0.0.0.0:50051");
	DataConversionImpl service;
	
	ServerBuilder builder;
  	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  	builder.RegisterService(&service);
  	std::unique_ptr<Server> server(builder.BuildAndStart());
  	std::cout << "Server listening on " << server_address << std::endl;
  	server->Wait();
}

int main(int argc, char** argv) {
	RunServer();
	return 0;
}


