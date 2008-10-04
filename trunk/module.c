#include <stdio.h>
#include <stdlib.h>
#include "module.h"
#include <dlfcn.h>
#include <pcre.h>

int LoadModules(GListInfo *moduleList){
	
	FILE *moduleConfFile=NULL;
	FILE *module;
	char mode[100];
	char moduleName[100];
	int loadedModules=0;
	module_prototype f;


	fprintf(stderr,"Module Loadding....\n");
	moduleConfFile = fopen("conf/modules.conf","r");
	if(moduleConfFile ==NULL){
		fprintf(stderr,"can't open configuration file\n");

		exit(1);
	}
	fprintf(stderr,"module.conf loaded\n");
	while(fscanf(moduleConfFile,"%s %s",mode,moduleName)==2){
		fprintf(stderr,"command:%s, %s\n",mode,moduleName);
		glist_append(moduleList,(void *) insmod(moduleName,0));
		loadedModules++;
		
	};

	fprintf(stderr, "%d modules loaded\n",loadedModules);
	return loadedModules;
}

module_prototype insmod(const char *module,int insertPoint){
	void *handle=NULL;
	getModuleType funcGetModuleType;
	module_prototype funcModuleMain;
	char *error;
				
	handle = dlopen(module,RTLD_NOW);

	if(!handle){
		fprintf(stderr, "handle error:%s\n", dlerror());
		exit(1);
	}
	funcGetModuleType = dlsym(handle, "getModuleType");
	if((error = dlerror()) != NULL){
		fprintf(stderr, "dlsym1 error : %s\n",error);
		exit(1);
	}

	funcModuleMain = dlsym(handle, "module_main");
	if((error = dlerror()) != NULL){
		fprintf(stderr, "dlsym2 error :%s\n",error);
		exit(1);
	}
	return funcModuleMain;

}
