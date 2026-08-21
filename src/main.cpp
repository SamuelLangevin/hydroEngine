#include "Application.hpp"

int main(){
	Application application;

	while(application.shouldWindowClose()){
    	application.processFrame();
    }

	return 0;
}
