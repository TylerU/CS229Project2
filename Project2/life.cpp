#include "SimOptions.h"
#include "FileParsing.h"


int main(int artc, char* argv[]){
	try{
		SimOptions opts;
		FileParser p("lifeIn1.txt", opts);
	}catch (runtime_error e){
		printf("Error encountered: %s\n", e.what());
	}

	system("pause");
	return 0;
}