#include "Application.hpp"

int main(){
    Application application;

    while(application.shouldWindowClose()){
        application.draw();
    }
	return 0;
}
